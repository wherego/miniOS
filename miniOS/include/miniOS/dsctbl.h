/**
  *************************************************************************
  *
  *							dsctbl.h
  *						2016-02-11	by zlq
  *
  *------------------------------------------------------------------------
  *
  *************************************************************************
  */
#ifndef __DSCTBL_H
#define __DSCTBL_H

#include "stdint.h"

#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff
#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff
#define ADR_BOTPAK		0x00280000
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_INTGATE32	0x008e



/**
  * 存放GDT的8字节的信息,共64位
  *		段基址：32bits； 段界限：20bits； 属性：12bits
  *
  * ar的高4位：GD00
  *		G:0,byte; 1,page,4KB(1G bit)
  *		D:0,16位模式； 1,32位模式
  *
  * ar的低8位：
  *		0000 0000(0x00)：未使用的记录表
  *		1001 0010(0x92)：系统专用，可读写的段。不可执行
  *		1001 1010(0x9a)：系统专用，可执行的段。可读不可写
  *		1111 0010(0xf2)：应用程序用，可读写的段。不可执行
  *		1111 1010(0xfa)：应用程序用，可执行的段。可读不可写
  *
  ***************************************************************************************************************
  * 63			56|55	52|51			48|47			40|39			32|31				16|15				0
  *--------------------------------------------------------------------------------------------------------------
  *				  |		  | 			  |				  |				  |					  |
  *	base 24:31	  | flags | limit 16:19   | access byte   | base 16:23	  |		base 0:15	  |		limit 0:15
  *				  |		  |		          |			      |				  |					  |
  ***************************************************************************************************************
  */
typedef struct {
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
}segment_descriptor;

/**
* 存放IDT的8字节的信息
*/
typedef struct {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
}gate_descriptor;



/* 函数声明-----------------------------------------------*/
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);

void gdtidt_init(void);
void set_segdesc(segment_descriptor *sd, uint32_t limit, int base, int ar);
void set_gatedesc(gate_descriptor *gd, int offset, int selector, int ar);

#endif //!__DSCTBL_H

