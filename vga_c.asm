	.386p
	model flat
	ifndef	??version
	?debug	macro
	endm
	endif
	?debug	S "vga_c.c"
	?debug	T "vga_c.c"
_TEXT	segment dword public use32 'CODE'
_TEXT	ends
_DATA	segment dword public use32 'DATA'
_DATA	ends
_BSS	segment dword public use32 'BSS'
_BSS	ends
$$BSYMS	segment byte public use32 'DEBSYM'
$$BSYMS	ends
$$BTYPES	segment byte public use32 'DEBTYP'
$$BTYPES	ends
$$BNAMES	segment byte public use32 'DEBNAM'
$$BNAMES	ends
$$BROWSE	segment byte public use32 'DEBSYM'
$$BROWSE	ends
$$BROWFILE	segment byte public use32 'DEBSYM'
$$BROWFILE	ends
DGROUP	group	_BSS,_DATA
_DATA	segment dword public use32 'DATA'
	align	4
_Video	label	dword
	dd	40960
_DATA	ends
_TEXT	segment dword public use32 'CODE'
_Bal	proc	near
?live1@0:
	?debug L 36
	push      ebp
	mov       ebp,esp
	push      ebx
	push      esi
	push      edi
	?debug L 39
@1:
	mov	 dx,03DAH
	?debug L 40
@2:
wait1:
	?debug L 41
	in	 al,dx
	?debug L 42
	test	 al,8
	?debug L 43
	je        @2
	?debug L 45
@3:
	pop       edi
	pop       esi
	pop       ebx
	pop       ebp
	ret 
_Bal	endp
_TEXT	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	db	1
	db	0
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	db	2
	db	0
	db	0
	db	0
	dw	51
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch1
	dd	?patch2
	dd	?patch3
	df	_Bal
	dw	0
	dw	4096
	dw	0
	dw	1
	dw	0
	dd	?patch4
	db	4
	db	95
	db	66
	db	97
	db	108
?patch1	equ	@3-_Bal+5
?patch2	equ	0
?patch3	equ	@3-_Bal
	dw	2
	dw	6
	dw	8
	dw	531
	dw	7
	dw	65524
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
_ModeVGA	proc	near
?live1@112:
	?debug L 48
	push      ebp
	mov       ebp,esp
	?debug L 51
@4:
	push	 ds
	?debug L 52
	mov	 ax,13h
	?debug L 53
	int	 10h
	?debug L 54
	pop	 ds
	?debug L 60
@5:
	pop       ebp
	ret 
_ModeVGA	endp
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	55
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch5
	dd	?patch6
	dd	?patch7
	df	_ModeVGA
	dw	0
	dw	4098
	dw	0
	dw	2
	dw	0
	dd	?patch8
	db	8
	db	95
	db	77
	db	111
	db	100
	db	101
	db	86
	db	71
	db	65
?patch5	equ	@5-_ModeVGA+2
?patch6	equ	0
?patch7	equ	@5-_ModeVGA
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
_ModeTxt	proc	near
?live1@208:
	?debug L 62
	push      ebp
	mov       ebp,esp
	?debug L 65
@6:
	push	 ds
	?debug L 66
	mov	 ax,3
	?debug L 67
	int	 10h
	?debug L 68
	pop	 ds
	?debug L 74
@7:
	pop       ebp
	ret 
_ModeTxt	endp
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	55
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch9
	dd	?patch10
	dd	?patch11
	df	_ModeTxt
	dw	0
	dw	4100
	dw	0
	dw	3
	dw	0
	dd	?patch12
	db	8
	db	95
	db	77
	db	111
	db	100
	db	101
	db	84
	db	120
	db	116
?patch9	equ	@7-_ModeTxt+2
?patch10	equ	0
?patch11	equ	@7-_ModeTxt
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
_Draw	proc	near
?live1@304:
	?debug L 86
	push      ebp
	mov       ebp,esp
	push      esi
	push      edi
	?debug L 88
@8:
	cmp       word ptr [ebp+12],0
	je        @9
	?debug L 90
	push	 ds
	?debug L 91
	les	 di,_Video
	?debug L 92
	lds	 si,dword ptr [ebp+8]
	?debug L 93
	mov	 cx,16000
	?debug L 94
	mov	 ax,word ptr [ebp+12]
	?debug L 95
	add	 si,ax
	?debug L 96
	shr	 ax,2
	?debug L 97
	mov	 dx,ax
	?debug L 98
	sub	 cx,ax
	?debug L 99
	db	 66h
	?debug L 100
	rep movsw	
	?debug L 101
	mov	 cx,dx
	?debug L 102
	xor	 ax,ax
	?debug L 103
	db	 66h
	?debug L 104
	rep stosw	
	?debug L 105
	pop	 ds
	?debug L 106
	jmp       short @10
	?debug L 109
@9:
	push	 ds
	?debug L 110
	les	 di,_Video
	?debug L 111
	lds	 si,dword ptr [ebp+8]
	?debug L 112
	mov	 cx,16000
	?debug L 113
	db	 66h
	?debug L 114
	rep movsw	
	?debug L 115
	pop	 ds
	?debug L 118
@10:
@11:
	pop       edi
	pop       esi
	pop       ebp
	ret 
_Draw	endp
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	52
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch13
	dd	?patch14
	dd	?patch15
	df	_Draw
	dw	0
	dw	4102
	dw	0
	dw	4
	dw	0
	dd	?patch16
	db	5
	db	95
	db	68
	db	114
	db	97
	db	119
	dw	18
	dw	512
	dw	8
	dw	0
	dw	1056
	dw	0
	dw	5
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	86
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	4
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	17
	dw	0
	dw	6
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	86
	db	1
	db	88
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	14
	dw	0
?patch13	equ	@11-_Draw+4
?patch14	equ	0
?patch15	equ	@11-_Draw
	dw	2
	dw	6
	dw	8
	dw	531
	dw	6
	dw	65528
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
_Cpy	proc	near
?live1@736:
	?debug L 120
	push      ebp
	mov       ebp,esp
	push      esi
	push      edi
	?debug L 123
@12:
	push	 ds
	?debug L 124
	les	 di,dword ptr [ebp+12]
	?debug L 125
	lds	 si,dword ptr [ebp+8]
	?debug L 126
	mov	 cx,16000
	?debug L 127
	db	 66h
	?debug L 128
	rep movsw	
	?debug L 129
	pop	 ds
	?debug L 131
@13:
	pop       edi
	pop       esi
	pop       ebp
	ret 
_Cpy	endp
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	51
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch17
	dd	?patch18
	dd	?patch19
	df	_Cpy
	dw	0
	dw	4104
	dw	0
	dw	7
	dw	0
	dd	?patch20
	db	4
	db	95
	db	67
	db	112
	db	121
	dw	18
	dw	512
	dw	8
	dw	0
	dw	1056
	dw	0
	dw	8
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	120
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	27
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	1056
	dw	0
	dw	9
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	120
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	37
	dw	0
?patch17	equ	@13-_Cpy+4
?patch18	equ	0
?patch19	equ	@13-_Cpy
	dw	2
	dw	6
	dw	8
	dw	531
	dw	6
	dw	65528
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
_Clr	proc	near
?live1@880:
	?debug L 133
	push      ebp
	mov       ebp,esp
	push      edi
	?debug L 136
@14:
	push	 ds
	?debug L 137
	les	 di,dword ptr [ebp+8]
	?debug L 138
	xor	 ax,ax
	?debug L 139
	mov	 cx,32000
	?debug L 140
	rep stosw	
	?debug L 141
	pop	 ds
	?debug L 143
@15:
	pop       edi
	pop       ebp
	ret 
_Clr	endp
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	51
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch21
	dd	?patch22
	dd	?patch23
	df	_Clr
	dw	0
	dw	4106
	dw	0
	dw	10
	dw	0
	dd	?patch24
	db	4
	db	95
	db	67
	db	108
	db	114
	dw	18
	dw	512
	dw	8
	dw	0
	dw	1056
	dw	0
	dw	11
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	133
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	47
	dw	0
?patch21	equ	@15-_Clr+3
?patch22	equ	0
?patch23	equ	@15-_Clr
	dw	2
	dw	6
	dw	8
	dw	531
	dw	2
	dw	65532
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
_Put	proc	near
?live1@1008:
	?debug L 146
	push      ebp
	mov       ebp,esp
	add       esp,-12
	push      ebx
	push      esi
	push      edi
	mov       eax,dword ptr [ebp+12]
	?debug L 149
?live1@1024: ; EAX = y
@16:
	xor       edx,edx
	mov       dl,byte ptr [ebp+20]
	mov       dword ptr [ebp-8],edx
	?debug L 150
	movzx     ecx,word ptr [ebp+16]
	mov       dword ptr [ebp-12],ecx
	?debug L 151
	xor       edx,edx
	mov       dl,al
	shl       edx,8
	and       eax,255
	shl       eax,6
	add       edx,eax
	mov       dword ptr [ebp-4],edx
	?debug L 153
?live1@1072: ; 
	push	 ds
	?debug L 154
	les	 di,dword ptr [ebp+24]
	?debug L 155
	lds	 si,dword ptr [ebp+28]
	?debug L 156
	mov	 cx,dword ptr [ebp-4]
	?debug L 157
	add	 cx,word ptr [ebp+8]
	?debug L 158
	add	 di,cx
	?debug L 159
	mov	 ax,di
	?debug L 160
	mov	 bx,dword ptr [ebp-8]
	?debug L 161
@17:
psprh:
	?debug L 162
	mov	 cx,dword ptr [ebp-12]
	?debug L 163
@18:
psprl:
	?debug L 164
	mov	 dl,ds:[si]
	?debug L 165
	or	 dl,dl
	?debug L 166
	je        short @19
	?debug L 167
	mov	 es:[di],dl
	?debug L 168
@19:
psprl2:
	?debug L 169
	inc	 si
	?debug L 170
	inc	 di
	?debug L 171
	dec	 cx
	?debug L 172
	or	 cx,cx
	?debug L 173
	jne       @18
	?debug L 175
	add	 ax,320
	?debug L 176
	mov	 di,ax
	?debug L 177
	dec	 bx
	?debug L 178
	or	 bx,bx
	?debug L 179
	jne       @17
	?debug L 180
	pop	 ds
	?debug L 182
@20:
	pop       edi
	pop       esi
	pop       ebx
	mov       esp,ebp
	pop       ebp
	ret 
_Put	endp
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	51
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch25
	dd	?patch26
	dd	?patch27
	df	_Put
	dw	0
	dw	4108
	dw	0
	dw	12
	dw	0
	dd	?patch28
	db	4
	db	95
	db	80
	db	117
	db	116
	dw	18
	dw	512
	dw	8
	dw	0
	dw	33
	dw	0
	dw	13
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	146
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	57
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	32
	dw	0
	dw	14
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	146
	db	1
	db	254
	dw	151
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	67
	dw	0
	dw	?patch29
	dw	529
	dw	?patch30
	dd	?live1@1024-_Put
	dd	?live1@1072-?live1@1024
	dw	1
?patch30	equ	1
?patch29	equ	14
	dw	18
	dw	512
	dw	16
	dw	0
	dw	33
	dw	0
	dw	15
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	146
	db	1
	db	254
	dw	150
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	82
	dw	0
	dw	18
	dw	512
	dw	20
	dw	0
	dw	32
	dw	0
	dw	16
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	146
	db	1
	db	254
	dw	149
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	97
	dw	0
	dw	18
	dw	512
	dw	24
	dw	0
	dw	1056
	dw	0
	dw	17
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	146
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	112
	dw	0
	dw	18
	dw	512
	dw	28
	dw	0
	dw	1056
	dw	0
	dw	18
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	146
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	122
	dw	0
	dw	18
	dw	512
	dw	65524
	dw	65535
	dw	117
	dw	0
	dw	19
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	148
	db	1
	db	254
	dw	150
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	132
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	117
	dw	0
	dw	20
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	148
	db	1
	db	254
	dw	149
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	147
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	117
	dw	0
	dw	21
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	148
	db	1
	db	254
	dw	151
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	162
	dw	0
?patch25	equ	@20-_Put+7
?patch26	equ	0
?patch27	equ	@20-_Put
	dw	2
	dw	6
	dw	8
	dw	531
	dw	7
	dw	65512
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
_GetBlk1	proc	near
?live1@1520:
	?debug L 232
	push      ebp
	mov       ebp,esp
	push      ecx
	push      ebx
	push      esi
	mov       ecx,dword ptr [ebp+28]
	mov       esi,dword ptr [ebp+16]
	mov       eax,dword ptr [ebp+12]
	?debug L 238
?live1@1536: ; ECX = buf, ESI = l, EAX = y
@21:
	movsx     edx,ax
	shl       edx,8
	movsx     eax,ax
	shl       eax,6
	add       edx,eax
	movsx     eax,word ptr [ebp+8]
	add       edx,eax
	add       dword ptr [ebp+24],edx
	?debug L 240
?live1@1552: ; ECX = buf, ESI = l
	mov       byte ptr [ebp-1],0
	jmp       short @23
	?debug L 242
@22:
	mov       eax,dword ptr [ebp+24]
	?debug L 243
?live1@1584: ; EAX = s1, ECX = buf, ESI = l
	xor       edx,edx
	cmp       si,dx
	jbe       short @25
@24:
	mov       bl,byte ptr [eax]
	mov       byte ptr [ecx],bl
	inc       eax
	inc       ecx
	inc       edx
	cmp       si,dx
	ja        short @24
	?debug L 244
?live1@1600: ; ECX = buf, ESI = l
@25:
	add       dword ptr [ebp+24],320
	inc       byte ptr [ebp-1]
@23:
	xor       eax,eax
	mov       al,byte ptr [ebp-1]
	movzx     edx,word ptr [ebp+20]
	cmp       eax,edx
	jl        short @22
	?debug L 246
?live1@1616: ; 
@28:
	pop       esi
	pop       ebx
	pop       ecx
	pop       ebp
	ret 
_GetBlk1	endp
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	55
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch31
	dd	?patch32
	dd	?patch33
	df	_GetBlk1
	dw	0
	dw	4110
	dw	0
	dw	22
	dw	0
	dd	?patch34
	db	8
	db	95
	db	71
	db	101
	db	116
	db	66
	db	108
	db	107
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	17
	dw	0
	dw	23
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	232
	db	1
	db	254
	dw	238
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	177
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	17
	dw	0
	dw	24
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	232
	db	1
	db	254
	dw	238
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	192
	dw	0
	dw	?patch35
	dw	529
	dw	?patch36
	dd	?live1@1536-_GetBlk1
	dd	?live1@1552-?live1@1536
	dw	9
?patch36	equ	1
?patch35	equ	14
	dw	18
	dw	512
	dw	16
	dw	0
	dw	33
	dw	0
	dw	25
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	232
	db	1
	db	254
	dw	243
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	207
	dw	0
	dw	?patch37
	dw	529
	dw	?patch38
	dd	?live1@1536-_GetBlk1
	dd	?live1@1616-?live1@1536
	dw	15
?patch38	equ	1
?patch37	equ	14
	dw	18
	dw	512
	dw	20
	dw	0
	dw	33
	dw	0
	dw	26
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	232
	db	1
	db	254
	dw	240
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	222
	dw	0
	dw	18
	dw	512
	dw	24
	dw	0
	dw	1056
	dw	0
	dw	27
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	232
	db	1
	db	254
	dw	238
	db	4
	db	2
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	237
	dw	0
	dw	18
	dw	512
	dw	28
	dw	0
	dw	1056
	dw	0
	dw	28
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	232
	db	1
	db	254
	dw	243
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	254
	dw	0
	dw	?patch39
	dw	529
	dw	?patch40
	dd	?live1@1536-_GetBlk1
	dd	?live1@1616-?live1@1536
	dw	18
?patch40	equ	1
?patch39	equ	14
	dw	16
	dw	2
	dw	1056
	dw	0
	dw	17
	dw	29
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	236
	db	1
	db	254
	dw	242
	db	1
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	269
	dw	0
	dw	?patch41
	dw	529
	dw	?patch42
	dd	?live1@1584-_GetBlk1
	dd	?live1@1600-?live1@1584
	dw	17
?patch42	equ	1
?patch41	equ	14
	dw	16
	dw	2
	dw	33
	dw	0
	dw	11
	dw	30
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	235
	db	1
	db	254
	dw	243
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	285
	dw	0
	dw	18
	dw	512
	dw	65535
	dw	65535
	dw	32
	dw	0
	dw	31
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	234
	db	1
	db	254
	dw	240
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	300
	dw	0
?patch31	equ	@28-_GetBlk1+5
?patch32	equ	0
?patch33	equ	@28-_GetBlk1
	dw	2
	dw	6
	dw	8
	dw	531
	dw	5
	dw	65524
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
_Blka	proc	near
?live1@1632:
	?debug L 330
	push      ebp
	mov       ebp,esp
	push      ebx
	push      esi
	push      edi
	?debug L 333
@29:
	push	 ds
	?debug L 335
	les	 di,dword ptr [ebp+36]
	?debug L 336
	lds	 si,dword ptr [ebp+32]
	?debug L 338
	mov	 ax,word ptr [ebp+12]
	?debug L 339
	mov	 bx,320
	?debug L 340
	mul	 bx
	?debug L 341
	add	 ax,word ptr [ebp+8]
	?debug L 342
	add	 si,ax
	?debug L 344
	mov	 ax,word ptr [ebp+28]
	?debug L 345
	mov	 bx,320
	?debug L 346
	mul	 bx
	?debug L 347
	add	 ax,word ptr [ebp+24]
	?debug L 348
	add	 di,ax
	?debug L 350
	mov	 ax,si
	?debug L 351
	mov	 bx,di
	?debug L 353
	mov	 dh,byte ptr [ebp+20]
	?debug L 355
@30:
debut:
	?debug L 356
	mov	 cx,word ptr [ebp+16]
	?debug L 357
@31:
copie:
	?debug L 358
	mov	 dl,ds:[si]
	?debug L 359
	or	 dl,dl
	?debug L 360
	je        short @32
	?debug L 361
	mov	 es:[di],dl
	?debug L 362
@32:
suite:
	?debug L 363
	inc	 si
	?debug L 364
	inc	 di
	?debug L 365
	dec	 cx
	?debug L 366
	or	 cx,cx
	?debug L 367
	jne       @31
	?debug L 369
	add	 ax,320
	?debug L 370
	mov	 si,ax
	?debug L 371
	add	 bx,320
	?debug L 372
	mov	 di,bx
	?debug L 374
	dec	 dh
	?debug L 375
	or	 dh,dh
	?debug L 376
	jne       @30
	?debug L 377
	pop	 ds
	?debug L 380
@33:
	pop       edi
	pop       esi
	pop       ebx
	pop       ebp
	ret 
_Blka	endp
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	52
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch43
	dd	?patch44
	dd	?patch45
	df	_Blka
	dw	0
	dw	4112
	dw	0
	dw	32
	dw	0
	dd	?patch46
	db	5
	db	95
	db	66
	db	108
	db	107
	db	97
	dw	18
	dw	512
	dw	8
	dw	0
	dw	33
	dw	0
	dw	33
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	330
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	315
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	33
	dw	0
	dw	34
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	330
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	325
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	33
	dw	0
	dw	35
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	330
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	335
	dw	0
	dw	18
	dw	512
	dw	20
	dw	0
	dw	32
	dw	0
	dw	36
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	330
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	345
	dw	0
	dw	18
	dw	512
	dw	24
	dw	0
	dw	33
	dw	0
	dw	37
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	330
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	355
	dw	0
	dw	18
	dw	512
	dw	28
	dw	0
	dw	33
	dw	0
	dw	38
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	330
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	365
	dw	0
	dw	18
	dw	512
	dw	32
	dw	0
	dw	1056
	dw	0
	dw	39
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	330
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	375
	dw	0
	dw	18
	dw	512
	dw	36
	dw	0
	dw	1056
	dw	0
	dw	40
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	330
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	385
	dw	0
?patch43	equ	@33-_Blka+5
?patch44	equ	0
?patch45	equ	@33-_Blka
	dw	2
	dw	6
	dw	8
	dw	531
	dw	7
	dw	65524
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
_TEXT	ends
	public	_ModeVGA
	public	_ModeTxt
	public	_GetBlk1
	public	_Video
	public	_Bal
	public	_Draw
	public	_Cpy
	public	_Clr
	public	_Put
	public	_Blka
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	22
	dw	514
	df	_Video
	dw	0
	dw	1056
	dw	0
	dw	41
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	8
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	395
	dw	0
	dw	16
	dw	4
	dw	117
	dw	0
	dw	0
	dw	42
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	3
	dw	44
	db	3
	db	254
	dw	290
	db	8
	db	23
	db	255
	db	7
	dw	326
	db	3
	db	7
	db	1
	db	1
	db	1
	db	1
	db	1
	db	3
	db	9
	db	1
	db	97
	db	7
	db	8
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	405
	dw	0
	dw	16
	dw	4
	dw	18
	dw	0
	dw	0
	dw	43
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	3
	dw	49
	db	3
	db	254
	dw	284
	db	12
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	439
	dw	0
	dw	16
	dw	4
	dw	33
	dw	0
	dw	0
	dw	44
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	7
	dw	62
	db	7
	db	254
	dw	338
	db	2
	db	13
	db	2
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	455
	dw	0
	dw	16
	dw	4
	dw	18
	dw	0
	dw	0
	dw	45
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	12
	dw	20
	db	11
	db	67
	db	1
	db	1
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	473
	dw	0
	dw	16
	dw	4
	dw	17
	dw	0
	dw	0
	dw	46
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	12
	dw	24
	db	11
	db	59
	db	6
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	488
	dw	0
	dw	16
	dw	4
	dw	17
	dw	0
	dw	0
	dw	47
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	12
	dw	25
	db	11
	db	60
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	502
	dw	0
	dw	16
	dw	4
	dw	17
	dw	0
	dw	0
	dw	48
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	12
	dw	26
	db	11
	db	61
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	515
	dw	0
	dw	16
	dw	4
	dw	17
	dw	0
	dw	0
	dw	49
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	12
	dw	27
	db	11
	db	62
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	528
	dw	0
	dw	16
	dw	4
	dw	116
	dw	0
	dw	0
	dw	50
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	12
	dw	28
	db	11
	db	63
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	541
	dw	0
	dw	16
	dw	4
	dw	116
	dw	0
	dw	0
	dw	51
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	12
	dw	29
	db	11
	db	64
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	554
	dw	0
	dw	16
	dw	4
	dw	18
	dw	0
	dw	0
	dw	52
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	12
	dw	30
	db	11
	db	66
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	567
	dw	0
	dw	16
	dw	4
	dw	32
	dw	0
	dw	0
	dw	53
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	2
	dw	50
	db	2
	db	65
	db	5
	db	5
	db	5
	db	4
	db	4
	db	4
	db	4
	db	62
	db	1
	db	255
	db	1
	dw	146
	db	88
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	580
	dw	0
	dw	16
	dw	4
	dw	33
	dw	0
	dw	0
	dw	54
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	2
	dw	51
	db	2
	db	88
	db	8
	db	4
	db	4
	db	4
	db	4
	db	4
	db	4
	db	4
	db	4
	db	5
	db	5
	db	5
	db	5
	db	5
	db	5
	db	1
	db	255
	db	1
	dw	232
	db	3
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	607
	dw	0
	dw	16
	dw	4
	dw	34
	dw	0
	dw	0
	dw	55
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	2
	dw	52
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	641
	dw	0
	dw	16
	dw	4
	dw	32
	dw	0
	dw	0
	dw	56
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	2
	dw	53
	db	2
	db	65
	db	5
	db	14
	db	4
	db	4
	db	4
	db	4
	db	4
	db	4
	db	4
	db	4
	db	4
	db	4
	db	4
	db	5
	db	5
	db	5
	db	5
	db	5
	db	255
	db	1
	dw	86
	db	34
	db	13
	db	99
	db	4
	db	94
	db	0
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	651
	dw	0
	dw	16
	dw	4
	dw	33
	dw	0
	dw	0
	dw	57
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	2
	dw	54
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	691
	dw	0
	dw	16
	dw	4
	dw	34
	dw	0
	dw	0
	dw	58
	dw	0
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	2
	dw	55
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	701
	dw	0
	dw	?patch47
	dw	1
	db	2
	db	0
	db	8
	db	24
	db	6
	db	66
	db	67
	db	52
	db	46
	db	48
	db	48
?patch47	equ	13
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	330
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
?patch46	equ	711
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	232
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
?patch34	equ	721
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	146
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
?patch28	equ	731
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	133
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
?patch24	equ	741
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	120
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
?patch20	equ	751
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	86
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
?patch16	equ	761
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	62
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
?patch12	equ	771
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	48
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
?patch8	equ	781
$$BSYMS	ends
$$BROWSE	segment byte public use32 'DEBSYM'
	dw	0
	dw	0
	dw	0
	db	1
	dw	36
	db	0
$$BROWSE	ends
$$BSYMS	segment byte public use32 'DEBSYM'
?patch4	equ	791
$$BSYMS	ends
$$BROWFILE	segment byte public use32 'DEBSYM'
	dw	59
	dw	0
	dw	60
	dw	0
	dw	61
	dw	0
	dw	62
	dw	0
	dw	63
	dw	0
	dw	64
	dw	0
	dw	65
	dw	0
	dw	66
	dw	0
	dw	67
	dw	0
	dw	68
	dw	0
	dw	69
	dw	0
	dw	70
	dw	0
$$BROWFILE	ends
$$BTYPES	segment byte public use32 'DEBTYP'
	db        2,0,0,0,14,0,8,0,3,0,0,0,0,0,0,0
	db        1,16,0,0,4,0,1,2,0,0,14,0,8,0,3,0
	db        0,0,0,0,0,0,3,16,0,0,4,0,1,2,0,0
	db        14,0,8,0,3,0,0,0,0,0,0,0,5,16,0,0
	db        4,0,1,2,0,0,14,0,8,0,3,0,0,0,0,0
	db        2,0,7,16,0,0,12,0,1,2,2,0,32,4,0,0
	db        17,0,0,0,14,0,8,0,3,0,0,0,0,0,2,0
	db        9,16,0,0,12,0,1,2,2,0,32,4,0,0,32,4
	db        0,0,14,0,8,0,3,0,0,0,0,0,1,0,11,16
	db        0,0,8,0,1,2,1,0,32,4,0,0,14,0,8,0
	db        3,0,0,0,0,0,6,0,13,16,0,0,28,0,1,2
	db        6,0,33,0,0,0,32,0,0,0,33,0,0,0,32,0
	db        0,0,32,4,0,0,32,4,0,0,14,0,8,0,3,0
	db        0,0,0,0,6,0,15,16,0,0,28,0,1,2,6,0
	db        17,0,0,0,17,0,0,0,33,0,0,0,33,0,0,0
	db        32,4,0,0,32,4,0,0,14,0,8,0,3,0,0,0
	db        0,0,8,0,17,16,0,0,36,0,1,2,8,0,33,0
	db        0,0,33,0,0,0,33,0,0,0,32,0,0,0,33,0
	db        0,0,33,0,0,0,32,4,0,0,32,4,0,0
$$BTYPES	ends
$$BNAMES	segment byte public use32 'DEBNAM'
	db	3,'Bal'
	db	7,'ModeVGA'
	db	7,'ModeTxt'
	db	4,'Draw'
	db	3,'src'
	db	3,'Mod'
	db	3,'Cpy'
	db	3,'src'
	db	3,'dst'
	db	3,'Clr'
	db	3,'dst'
	db	3,'Put'
	db	1,'x'
	db	1,'y'
	db	1,'l'
	db	1,'h'
	db	3,'scr'
	db	3,'spr'
	db	2,'l2'
	db	2,'h2'
	db	1,'t'
	db	7,'GetBlk1'
	db	1,'x'
	db	1,'y'
	db	1,'l'
	db	1,'h'
	db	3,'scr'
	db	3,'buf'
	db	2,'s1'
	db	1,'i'
	db	1,'j'
	db	4,'Blka'
	db	2,'xs'
	db	2,'ys'
	db	1,'l'
	db	1,'h'
	db	2,'xd'
	db	2,'yd'
	db	3,'src'
	db	3,'dst'
	db	5,'Video'
	db	6,'size_t'
	db	6,'fpos_t'
	db	7,'wchar_t'
	db	6,'time_t'
	db	5,'dev_t'
	db	5,'ino_t'
	db	6,'mode_t'
	db	7,'nlink_t'
	db	5,'uid_t'
	db	5,'gid_t'
	db	5,'off_t'
	db	4,'byte'
	db	4,'word'
	db	5,'dword'
	db	5,'bytef'
	db	5,'wordf'
	db	6,'dwordf'
	db	7,'vga_c.c'
	db	5,'VGA.H'
	db	32,'C:\PROGRA~2\BC45\INCLUDE\stdio.h'
	db	32,'C:\PROGRA~2\BC45\INCLUDE\_defs.h'
	db	33,'C:\PROGRA~2\BC45\INCLUDE\_nfile.h'
	db	32,'C:\PROGRA~2\BC45\INCLUDE\_null.h'
	db	33,'C:\PROGRA~2\BC45\INCLUDE\stdlib.h'
	db	30,'C:\PROGRA~2\BC45\INCLUDE\dos.h'
	db	29,'C:\PROGRA~2\BC45\INCLUDE\io.h'
	db	32,'C:\PROGRA~2\BC45\INCLUDE\fcntl.h'
	db	35,'C:\PROGRA~2\BC45\INCLUDE\sys\stat.h'
	db	36,'C:\PROGRA~2\BC45\INCLUDE\sys/types.h'
$$BNAMES	ends
	?debug	D "C:\PROGRA~2\BC45\INCLUDE\sys/types.h" 7965 9856
	?debug	D "C:\PROGRA~2\BC45\INCLUDE\sys\stat.h" 7965 9856
	?debug	D "C:\PROGRA~2\BC45\INCLUDE\fcntl.h" 7965 9856
	?debug	D "C:\PROGRA~2\BC45\INCLUDE\io.h" 7965 9856
	?debug	D "C:\PROGRA~2\BC45\INCLUDE\dos.h" 7965 9856
	?debug	D "C:\PROGRA~2\BC45\INCLUDE\stdlib.h" 7965 9856
	?debug	D "C:\PROGRA~2\BC45\INCLUDE\_null.h" 7965 9856
	?debug	D "C:\PROGRA~2\BC45\INCLUDE\_nfile.h" 7965 9856
	?debug	D "C:\PROGRA~2\BC45\INCLUDE\_defs.h" 7965 9856
	?debug	D "C:\PROGRA~2\BC45\INCLUDE\stdio.h" 7965 9856
	?debug	D "VGA.H" 10322 29477
	?debug	D "vga_c.c" 10322 29493
	end
