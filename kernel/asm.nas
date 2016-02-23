; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 
;							asm.nas
;						2016-02-16 	by zlq
;
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

[FORMAT "WCOFF"]					; 制作目标文件的模式
[INSTRSET "i486p"]					; 486の命令まで使いたいという記述
[BITS 32]							; 制作32位模式用的机械语言
[FILE "asm.nas"]					; 源文件名信息

		global _load_cr0,_store_cr0
		global _asm_inthandler21,_asm_inthandler2c,_asm_inthandler27
        
        extern _inthandler21,_inthandler2c,_inthandler27

; 以下是实际的函数
[SECTION .text]						; 目标文件中写了这些之后再写程序

_load_cr0:
		mov eax,cr0
		ret
		
_store_cr0:
		mov eax,[esp+4]
		mov cr0,eax
		ret


_asm_inthandler21:
        push es
        push ds
        pushad
        mov eax,esp
        push eax
        mov ax,ss
        mov ds,ax
        mov es,ax
        call _inthandler21
        pop eax
        popad
        pop ds
        pop es
        iretd
        
_asm_inthandler2c:
        push es
        push ds
        pushad
        mov eax,esp
        push eax
        mov ax,ss
        mov ds,ax
        mov es,ax
        call _inthandler2c
        pop eax
        popad
        pop ds
        pop es
        iretd
        
_asm_inthandler27:
        push es
        push ds
        pushad
        mov eax,esp
        push eax
        mov ax,ss
        mov ds,ax
        mov es,ax
        call _inthandler27
        pop eax
        popad
        pop ds
        pop es
        iretd