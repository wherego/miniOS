; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 
;							page.nas
;						2016-02-16 	by zlq
;
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

[FORMAT "WCOFF"]					; 崙恬朕炎猟周議庁塀
[INSTRSET "i486p"]					; 486の凋綜まで聞いたいという��峰
[BITS 32]							; 崙恬32了庁塀喘議字亠囂冱
[FILE "page.nas"]					; 坿猟周兆佚連

		global _memtest_sub

; 參和頁糞縞議痕方
[SECTION .text]						; 朕炎猟周嶄亟阻宸乂岻朔壅亟殻會

_memtest_sub:
		push edi
		push esi
		push ebx
		mov esi,0xaa55aa55
		mov edi,0x55aa55aa
		mov eax,[esp+12+4]
		
mts_loop:
		mov ebx,eax
		add ebx,0xffc
		mov edx,[ebx]
		mov [ebx],esi
		xor dword [ebx],0xffffffff
		cmp edi,[ebx]
		jne mts_fin
		xor dword [ebx],0xffffffff
		cmp esi,[ebx]
		jne mts_fin
		mov [ebx],edx
		add eax,0x1000
		cmp eax,[esp+12+8]
		
		jne mts_loop
		pop ebx
		pop esi
		pop edi
		ret
		
mts_fin:
		mov [ebx],edx
		pop ebx
		pop esi
		pop edi
		ret
			