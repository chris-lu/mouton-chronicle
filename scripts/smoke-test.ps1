<#
.SYNOPSIS
  Unattended check that the built game starts and survives in DOSBox-X.

.DESCRIPTION
  Prepares build\watcom\run\ (EXE + data\ as Mouton\), starts DOSBox-X with a
  generated configuration, presses ENTER to leave the text banner, then takes
  a screenshot of the emulator window every few seconds into run\shots\.
  At the end it scans the emulator log for CPU exceptions and DOS memory
  errors and prints PASS or FAIL (exit code 0 / 1).

  With -ClickMenu it also clicks "Options", "OK" and "Equipes" once the menu is
  up (DOSBox-X seamless mouse mode; the window must stay unobstructed).

  Settings come from .env (DOSBOX). Build first with scripts\build.cmd.

.EXAMPLE
  powershell -File scripts\smoke-test.ps1
  powershell -File scripts\smoke-test.ps1 -Seconds 60 -ClickMenu
#>
param(
  [int]$Seconds = 45,          # total run time after the banner
  [int]$Every = 5,             # seconds between screenshots
  [switch]$ClickMenu,          # click through Options / OK / Equipes
  [string]$Program = 'mouton'  # mouton, credits, greeting, credits2, rotpal
)
$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Drawing
Add-Type -AssemblyName System.Windows.Forms
Add-Type @"
using System; using System.Runtime.InteropServices;
public class MCWin {
  [StructLayout(LayoutKind.Sequential)] public struct RECT { public int L, T, R, B; }
  [StructLayout(LayoutKind.Sequential)] public struct POINT { public int X, Y; }
  [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out RECT r);
  [DllImport("user32.dll")] public static extern bool ClientToScreen(IntPtr h, ref POINT p);
  [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr h);
  [DllImport("user32.dll")] public static extern bool SetCursorPos(int x, int y);
  [DllImport("user32.dll")] public static extern void mouse_event(uint f, uint dx, uint dy, uint data, UIntPtr extra);
  [DllImport("user32.dll")] public static extern bool PrintWindow(IntPtr h, IntPtr hdc, uint flags);
}
"@

# --- settings ---------------------------------------------------------------
$Root = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$Env:DOSBOX = $null
$envFile = Join-Path $Root '.env'
$cfg = @{ DOSBOX = 'C:\DOSBox-X\dosbox-x.exe' }
if (Test-Path $envFile) {
  foreach ($line in Get-Content $envFile) {
    if ($line -match '^\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(.*?)\s*$' -and -not $line.StartsWith('#')) {
      $cfg[$Matches[1]] = $Matches[2].Trim('"')
    }
  }
}
$dosbox = $cfg['DOSBOX']
if (-not (Test-Path $dosbox)) { Write-Host "DOSBox-X not found at $dosbox (set DOSBOX in .env)"; exit 1 }

$out  = Join-Path $Root 'build\watcom\out'
$run  = Join-Path $Root 'build\watcom\run'
$shots = Join-Path $run 'shots'
$exe = Join-Path $out "$Program.exe"
if (-not (Test-Path $exe)) { Write-Host "$exe not found - run scripts\build.cmd first"; exit 1 }

# --- run folder --------------------------------------------------------------
New-Item -ItemType Directory -Force $run | Out-Null
Remove-Item -Recurse -Force $shots -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force $shots | Out-Null
Copy-Item $exe (Join-Path $run "$($Program.ToUpper()).EXE") -Force
Copy-Item (Join-Path $Root 'data') (Join-Path $run 'Mouton') -Recurse -Force

# --- generated configuration -------------------------------------------------
$conf = Join-Path $run 'smoke.conf'
$lines = @()
foreach ($l in Get-Content (Join-Path $Root 'build\watcom\dosbox.conf')) {
  if ($l -match '^\s*(output|autolock|mouse_emulation)\s*=') { continue }
  $lines += $l
  if ($l -match '^\s*\[sdl\]') { $lines += 'output=surface'; $lines += 'autolock=false'; $lines += 'mouse_emulation=integration' }
}
$lines += @('', '[autoexec]', ('mount c "' + $run + '"'), 'c:', "$($Program.ToUpper()).EXE")
$lines | Set-Content $conf -Encoding ASCII

$log = Join-Path $shots 'dosbox-log.txt'
# array form with the path quoted by hand: PowerShell 5 does not quote array elements itself,
# and a single string with an embedded quoted path fails when the path contains spaces
$p = Start-Process -FilePath $dosbox -ArgumentList @('-nomenu', '-fastlaunch', '-conf', ('"' + $conf + '"')) -PassThru `
       -RedirectStandardError $log -RedirectStandardOutput (Join-Path $shots 'dosbox-out.txt')

function Get-Win { $w = Get-Process -Name 'dosbox-x' -ErrorAction SilentlyContinue | Where-Object { $_.MainWindowHandle -ne 0 } | Select-Object -First 1; if ($w) { $w.MainWindowHandle } else { [IntPtr]::Zero } }
function Save-Shot($name) {
  $h = Get-Win; if ($h -eq [IntPtr]::Zero) { Write-Host "  $name : no window (emulator gone?)"; return $false }
  $r = New-Object MCWin+RECT; [MCWin]::GetWindowRect($h, [ref]$r) | Out-Null
  $w = $r.R - $r.L; $hh = $r.B - $r.T; if ($w -le 0) { return $false }
  $bmp = New-Object System.Drawing.Bitmap $w, $hh; $g = [System.Drawing.Graphics]::FromImage($bmp); $hdc = $g.GetHdc()
  [MCWin]::PrintWindow($h, $hdc, 2) | Out-Null; $g.ReleaseHdc($hdc)
  $bmp.Save((Join-Path $shots "$name.png"), [System.Drawing.Imaging.ImageFormat]::Png); $g.Dispose(); $bmp.Dispose()
  Write-Host "  $name.png"; return $true
}
function Send-Key($k) { $h = Get-Win; if ($h -eq [IntPtr]::Zero) { return }; [MCWin]::SetForegroundWindow($h) | Out-Null; Start-Sleep -Milliseconds 200; [System.Windows.Forms.SendKeys]::SendWait($k) }
function Click-Guest($gx, $gy) {   # 320x200 guest coordinates; the window shows the guest at 2x
  $h = Get-Win; if ($h -eq [IntPtr]::Zero) { return }
  [MCWin]::SetForegroundWindow($h) | Out-Null; Start-Sleep -Milliseconds 300
  $pt = New-Object MCWin+POINT; $pt.X = $gx * 2; $pt.Y = $gy * 2; [MCWin]::ClientToScreen($h, [ref]$pt) | Out-Null
  [MCWin]::SetCursorPos($pt.X, $pt.Y) | Out-Null; Start-Sleep -Milliseconds 400
  [MCWin]::SetCursorPos($pt.X + 1, $pt.Y) | Out-Null; Start-Sleep -Milliseconds 400
  [MCWin]::mouse_event(2, 0, 0, 0, [UIntPtr]::Zero); Start-Sleep -Milliseconds 120; [MCWin]::mouse_event(4, 0, 0, 0, [UIntPtr]::Zero)
}

# --- scenario -----------------------------------------------------------------
Write-Host "smoke test: $Program for $Seconds s, screenshots in $shots"
Start-Sleep -Seconds 6
Save-Shot '00-banner' | Out-Null
Send-Key '{ENTER}'                       # leaves the text banner (getch)
$alive = $true; $t = 0; $n = 1
while ($t -lt $Seconds) {
  Start-Sleep -Seconds $Every; $t += $Every
  if (-not (Save-Shot ('{0:00}-t{1}s' -f $n, $t))) { $alive = $false; break }
  $n++
  if ($ClickMenu -and $t -eq 30) {       # the intro is over by then; menu items: Options, OK, Equipes
    Click-Guest 170 87;  Start-Sleep -Seconds 3; Save-Shot ('{0:00}-options' -f $n) | Out-Null; $n++
    Click-Guest 175 162; Start-Sleep -Seconds 3; Save-Shot ('{0:00}-back' -f $n)    | Out-Null; $n++
    Click-Guest 170 107; Start-Sleep -Seconds 3; Save-Shot ('{0:00}-equipes' -f $n) | Out-Null; $n++
  }
}
Get-Process -Name 'dosbox-x' -ErrorAction SilentlyContinue | Stop-Process -Force
Start-Sleep -Seconds 1

# --- verdict ------------------------------------------------------------------
$text = if (Test-Path $log) { Get-Content $log -Raw } else { '' }
$problems = @()
if (-not $alive) { $problems += 'the emulator exited before the end of the run' }
$exc = ([regex]::Matches($text, 'Illegal Unhandled Interrupt Called 6')).Count
if ($exc -gt 0) { $problems += "$exc invalid-opcode exceptions (INT 6)" }
if ($text -match 'Corrupt MCB chain') { $problems += 'DOS memory chain corrupted' }
if ($text -match 'Triple Fault') { $problems += 'triple fault (CPU reset)' }
if ($text -notmatch 'surface consider=640x400') { $problems += 'the game never switched to graphics mode (banner key not delivered?)' }
if ($problems.Count -eq 0) { Write-Host 'PASS'; exit 0 }
Write-Host ('FAIL: ' + ($problems -join '; ')); exit 1
