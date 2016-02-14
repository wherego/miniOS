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
  * ���GDT��8�ֽڵ���Ϣ,��64λ
  *		�λ�ַ��32bits�� �ν��ޣ�20bits�� ���ԣ�12bits
  *
  * ar�ĸ�4λ��GD00
  *		G:0,byte; 1,page,4KB(1G bit)
  *		D:0,16λģʽ�� 1,32λģʽ
  *
  * ar�ĵ�8λ��
  *		0000 0000(0x00)��δʹ�õļ�¼��
  *		1001 0010(0x92)��ϵͳר�ã��ɶ�д�ĶΡ�����ִ��
  *		1001 1010(0x9a)��ϵͳר�ã���ִ�еĶΡ��ɶ�����д
  *		1111 0010(0xf2)��Ӧ�ó����ã��ɶ�д�ĶΡ�����ִ��
  *		1111 1010(0xfa)��Ӧ�ó����ã���ִ�еĶΡ��ɶ�����д
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
* ���IDT��8�ֽڵ���Ϣ
*/
typedef struct {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
}gate_descriptor;



/* ��������-----------------------------------------------*/
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);

void gdtidt_init(void);
void set_segdesc(segment_descriptor *sd, uint32_t limit, int base, int ar);
void set_gatedesc(gate_descriptor *gd, int offset, int selector, int ar);

#endif //!__DSCTBL_H

