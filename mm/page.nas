; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 
;							page.nas
;						2016-02-16 	by zlq
;
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

[FORMAT "WCOFF"]					; 制作目标文件的模式
[INSTRSET "i486p"]					; 486の命令まで使いたいという記述
[BITS 32]							; 制作32位模式用的机械语言
[FILE "page.nas"]					; 源文件名信息

		global _memtest_sub

; 以下是实际的函数
[SECTION .text]						; 目标文件中写了这些之后再写程序

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
			