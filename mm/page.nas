; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 
;							page.nas
;						2016-02-16 	by zlq
;
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

[FORMAT "WCOFF"]					; ����Ŀ���ļ���ģʽ
[INSTRSET "i486p"]					; 486������ޤ�ʹ�������Ȥ���ӛ��
[BITS 32]							; ����32λģʽ�õĻ�е����
[FILE "page.nas"]					; Դ�ļ�����Ϣ

		global _memtest_sub

; ������ʵ�ʵĺ���
[SECTION .text]						; Ŀ���ļ���д����Щ֮����д����

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
			