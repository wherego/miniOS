;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; haribote-ipl
; TAB=4
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 2015-10-29
;								计算机的启动顺序
;
;	1、上电，系统RESET，CPU复位，内存清零，CS:FFFFH  IP:0x00指向FFFF:0x00H，
;		即ROM-BIOS的入口；
;
;	2、BIOS入口只有一条指令：JMP START，调到BIOS的硬件自检程序，自检完成后
;		将这些部件初始化；
;
;	3、BIOS在内存中建立两个数据区：中断向量表、BIOS数据区；
;
;	4、BIOS根据指定的顺序，检查引导设备的第一个扇区（即主引导记录）
;		加载在内存地址0x7c00
;
;	5、主引导记录将操作系统加载到内存，并把操作权交给操作系统。
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;
; 							主引导记录（Master boot record：MBR）
;
;	1、第1-446字节：调用操作系统机械码（MBR）
;
;	2、第447-510：分区表（Partition table）,共64字节
;
;	3、第511-512字节：主引导记录签名（0x55和0xaa）
;
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;
;								0x7c00的来由
;
;			00000H	+---------------------------------------
;					|	Interrupts vectors	(1024B)
;			00400H	+---------------------------------------
;					|	BIOS data area		(256B)
;			00500H	+---------------------------------------
;					|
;					|	........OS load area
;					|	0x270000~0x27ffff设为GDT
;					|
;			0x7C00H	+----------------------------------------
;					|	Boot sector		(512B)
;			0x7E00H	+----------------------------------------
;					|	Boot data/stack		(512B)
;			0x8000H	+----------------------------------------
;					|
;					|	........
;					|
;		   0xa0000H	+----------------------------------------
;					|	显示内存区
;		   0xc0000H	+----------------------------------------
;					|	BIOS中断处理程序区
;		  0x100000H	+----------------------------------------
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
BOOTSEG		EQU		0x7c00			; bootsector的的装载地址，bootsector共512B，占用到0x7dff
DISKADR		EQU		0x0820
SYSSEG		EQU		0xc200			; miniOS.sys的加载地址
CYLS		EQU		10

		org 0x7c00
		
		jmp start
		DB		0x90
		DB		"HARIBOTE"			; 启动区的名称可以是任意字符串（8字节）
		DW		512					; 每个扇区的大小（必须是512字节）
		DB		1					; 簇的大小（必须是一个扇区）
		DW		1					; FAT的起始位置（一般从第一个扇区开始）
		DB		2					; FAT的个数（必须是2）
		DW		224					; 根目录的大小（一般设成224项）
		DW		2880				; 该磁盘的大小（必须是2880扇区）
		DB		0xf0				; 磁盘的种类（必须是0xf0）
		DW		9					; FAT的长度（必须是9扇区）
		DW		18					; 1个磁道有几个扇区（必须是18）
		DW		2					; 磁头数（必须是2）
		DD		0					; 不使用分区，必须是0
		DD		2880				; 重写一次磁盘大小
		DB		0,0,0x29			; 意义不明，固定
		DD		0xffffffff			; （可能是）卷标号码
		DB		"HARIBOTEOS "		; 磁盘的名称（11字节）
		DB		"FAT12   "			; 磁盘的格式（8字节）
		RESB	18					; 先空出18字节
		; 1+8+2+1+2+1+2+2+1+2+2+2+4+4+3+4+11+8(60)+18 = 78
		
start:
		mov ax,0					; 初始化寄存器
		mov ss,ax
		mov sp,BOOTSEG				; SS:SP = 0x00:0x7c00 = 0x7c00
		mov ds,ax
		
		
		; 使用[int 0x13]读取磁盘内容
		; 出口参数： CF＝0——操作成功， AH＝00H， AL＝传输的扇区数，否则， AH＝状态代码，参见功能号01H 中的说明
		mov ax,DISKADR
		mov es,ax					; ES:BX = 0x0820:0x0000 = 0x8200
		mov ch,0					; 柱面0	
		mov dh,0					; 磁头0	
		mov cl,2					; 扇区2
		
readloop:
		mov si,0
retry:
		mov ah,0x02					; int 0x13读磁盘入口参数
		mov al,1					; 要读取的扇区数
		mov bx,0					; es:bx 缓冲区地址	
		mov dl,0x00					; A驱动器，00-7FH:软盘 80H-0FFH:磁盘
		int 0x13
		
		jnc next					;  没有错时跳转到next
		add si,1					; si+1
		cmp si,5					; 
		jae error					; 如果si == 5，跳转到error
		mov ah,0x00					;
		mov dl,0x00					;
		int 0x13
		jmp retry
		
next:
		mov ax,es					;  @{内存地址后移0x200
		add ax,0x0020
		mov es,ax					; }
		
		add cl,1
		cmp cl,18
		jbe readloop
		
		mov cl,1
		add dh,1
		cmp dh,2
		jb readloop					; jump if below
		mov dh,0
		add ch,1
		cmp ch,CYLS
		jb readloop
		
; 読み終わったのでharibote.sysを実行だ！

		mov		[0x0ff0],ch			; IPLがどこまで読んだのかをメモ
		jmp		SYSSEG								
		
		mov si,msgl
		
putloop:
		; 使用 [int 0x10] 中断显示字符串
		; ah 0x0e入口参数
		; al 需要显示的字符串
		; bl 前景色
		mov al,[si]					; 将第一个字符赋给al
		add si,1					; 字符串位移加1
		
		cmp al,0					; 
		je  fin
		
		mov ah,0x0e
		mov bx,15
		int 0x10
		jmp putloop

fin:
		hlt
		jmp fin
		
error:
		mov si,errmsg
		jmp putloop
		

		
msgl:
		db 0x0a,0x0a
		db "system loading ..."
		db 0x0a
		db 0

errmsg:
		dw 0x0a0a
		db "load error"
		db 0x0a
		db 0
		
		
		resb 0x7dfe-$
		
		dw 0xaa55