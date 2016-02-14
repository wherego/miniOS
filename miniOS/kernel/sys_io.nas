; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 
;							sys_io.nas
;						2016-01-30 	by zlq
;
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

[FORMAT "WCOFF"]					; 制作目?文件的模式
[INSTRSET "i486p"]					; 486の命令まで使いたいという記述
[BITS 32]							; 制作32位模式用的机械?言
[FILE "sys_io.nas"]					; 源文件名信息

		global _io_hlt,_io_cli,_io_sti
		global _io_in8,_io_in16,_io_in32
		global _io_out8,_io_out16,_io_out32
		global _io_load_eflags,_io_store_eflags
		global _load_gdtr,_load_idtr

; 以下是实际的函数
[SECTION .text]						; 目标文件中写了这些之后再写程序

_io_hlt:							; void io_hlt(void);
		hlt
		ret
	
_io_cli:							; void io_cli(void);
		cli
		ret		
	
_io_sti:							; void io_sti(void);
		sti
		ret

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_io_in8:							; int io_in8(int port);
		mov edx,[esp+4]
		mov eax,0
		in al,dx
		ret
		
_io_in16:
		mov edx,[esp+4]
		mov eax,0
		in ax,dx
		ret
		
_io_in32:
		mov edx,[esp+4]
		in eax,dx
		ret
		
		
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_io_out8:
		mov edx,[esp+4]
		mov al,[esp+8]
		out dx,al
		ret
		
_io_out16:
		mov edx,[esp+4]
		mov eax,[esp+8]
		out dx,ax
		ret
		
_io_out32:
		mov edx,[esp+4]
		mov eax,[esp+8]
		out dx,eax
		ret

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_io_load_eflags:					; int io_load_eflags(void);
		pushfd						; 指push eflags
		pop eax
		ret
		
_io_store_eflags:					; void io_store_eflags(int flags);
		mov eax,[esp+4]
		push eax
		popfd
		ret
		
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_load_gdtr:							; void load_gdtr(int limit,int addr);
		mov ax,[esp+4]
		mov [esp+6],ax
		lgdt [esp+6]
		ret

_load_idtr:							; void load_idtr(int limit,int addr);
		mov ax,[esp+4]
		mov [esp+6],ax
		lidt [esp+6]
		ret
		