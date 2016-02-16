; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 
;							head.nas
;						2016-01-30 	by zlq
;
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
MAINADR	EQU		0x00280000			; main加载地址
DSKCAC	EQU		0x00100000			; 磁盘缓存的位置
DSKCAC0	EQU		0x00008000			; 磁盘缓存的位置（实模式）

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>@{
; 有关BOOT_INFO
CYLS	EQU		0x0ff0				; 设定启动区
LEDS	EQU		0x0ff1
VMODE	EQU		0x0ff2				; 颜色的位数
SCRNX	EQU		0x0ff4				; 分辨率x，screen x
SCRNY	EQU		0x0ff6				; 分辨率y，screen y
VRAM	EQU		0x0ff8				; 图像缓冲区的起始地址
	
		org		0xc200				; 这个程序将要装到内存的什么地方?

; 设置画面模式，并保存到内存中
		mov		al,0x13				; VGA显卡，320*200*8位色彩
		mov		ah,0x00
		int		0x10
		mov		byte [VMODE],8		; 记录画面
		mov		word [SCRNX],320
		mov		word [SCRNY],200
		mov		dword [VRAM],0x000a0000

; 用BIOS取得键盘上各种LED指示灯的状态

		mov		ah,0x02
		int		0x16 				; keyboard BIOS
		mov		[LEDS],AL
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>}


;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; CPU从实模式切换到保护模式
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 计算机启动在实模式下，为了让操作系统的内核工作在保护模式下，需要从实模式切换到
; 保护模式，切换步骤如下：
;		1、PIC关闭所有中断，关闭CPU中断；
;		2、使能A20地址线，突破1MB的寻址空间；
;		3、设定临时的GDT
;		4、使用长跳转指令跳到保护模式；
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; PIC关闭一切中断
;	根据AT兼容机的规格，如果要初始化PIC
;	必须在CLI之前进行，否则有事会挂起
;	随后进行PIC的初始化
		MOV		AL,0xff
		OUT		0x21,AL				; PIC0关闭一切中断
		NOP							; 如果连续执行OUT命令，有些机种会无法正运行
		OUT		0xa1,AL				; PIC1关闭一切中断

		CLI							; 禁止CPU的中断

; 为了让CPU能够访问1MB以上的内存，设定A20GATE，
		CALL	waitkbdout
		MOV		AL,0xd1
		OUT		0x64,AL
		CALL	waitkbdout
		MOV		AL,0xdf				; enable A20
		OUT		0x60,AL
		CALL	waitkbdout

; 切换到保护模式
[INSTRSET "i486p"]					; “想要使用486指令”的叙述

		LGDT	[GDTR0]				; 临时设定的GDT，真正的GDT设定在C语言代码中
		MOV		EAX,CR0
		AND		EAX,0x7fffffff		; 设bit31为0（为了禁止分页）
		OR		EAX,0x00000001		; 设bit0为1（为了切换到保护模式）
		MOV		CR0,EAX
		JMP		pipelineflush

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
pipelineflush:
		; 进入保护模式之后，段寄存的意义改变了，除了CS（CS如果也改变，会造成
		; 混乱）之外的所有段寄存器值有0x0000改为0x0008，相当于gdt+1
		MOV		AX,1*8				;  可读写的段 32bit
		MOV		DS,AX
		MOV		ES,AX
		MOV		FS,AX
		MOV		GS,AX
		MOV		SS,AX

; main的传送

		MOV		ESI,main			; 传送源
		MOV		EDI,MAINADR			; 传送目的地
		MOV		ECX,512*1024/4
		CALL	memcpy

; 磁盘数据最终转送到他本来的位置去

; 首先从启动扇区开始

		MOV		ESI,0x7c00			; 传送源
		MOV		EDI,DSKCAC			; 传送目的地
		MOV		ECX,512/4
		CALL	memcpy

; 所有剩下的

		MOV		ESI,DSKCAC0+512		; 传送源
		MOV		EDI,DSKCAC+512		; 传送目的地
		MOV		ECX,0
		MOV		CL,BYTE [CYLS]
		IMUL	ECX,512*18*2/4		; 从柱面数变换为字节数/4
		SUB		ECX,512/4			; 减去IPL
		CALL	memcpy
		;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.
		;	这部分是在调用memcpy函数，C语言形式：
		;	memcpy(main,	MAINADR,	512*1024/4);
		;	memcpy(0x7c00,	DSKCAC,	512/4);
		;	memcpy(DSKCAC0+512,	DSKCAC+512,	cyls*512*18*2/4-512/4);
		;
		;	memcpy(传送源地址,	转速目的地,	传送数据的大小);
		;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

; 必须由head来完成的工作，至此全部完毕
;	以后就交由main来完成

; main的启动

		MOV		EBX,MAINADR
		MOV		ECX,[EBX+16]
		ADD		ECX,3				; ECX += 3;
		SHR		ECX,2				; ECX /= 4;
		JZ		skip				; 没有要传送的东西时
		MOV		ESI,[EBX+20]		; 传送源
		ADD		ESI,EBX
		MOV		EDI,[EBX+12]		; 传送目的地
		CALL	memcpy
skip:
		MOV		ESP,[EBX+12]		; 栈初始值
		JMP		DWORD 2*8:0x0000001b

		
waitkbdout:
		IN		 AL,0x64
		AND		 AL,0x02
		JNZ		waitkbdout			; AND的结果如果不是0，就跳到waitkbdout
		RET

memcpy:
		MOV		EAX,[ESI]
		ADD		ESI,4
		MOV		[EDI],EAX
		ADD		EDI,4
		SUB		ECX,1
		JNZ		memcpy				; 减去运算的结果如果不是0，就跳到memcpy
		RET
; memcpyはアドレスサイズプリフィクスを入れ忘れなければ、ストリング命令でも書ける

		ALIGNB	16
GDT0:
		RESB	8					; NULL selector
		DW		0xffff,0x0000,0x9200,0x00cf	; 可以读写的段（semgent）32bit
		DW		0xffff,0x0000,0x9a28,0x0047	; 可以执行的段（segment）32bit（backpack用）

		DW		0
GDTR0:
		DW		8*3-1
		DD		GDT0

		ALIGNB	16
main:
