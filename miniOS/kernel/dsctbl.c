/**
  *************************************************************************
  *
  *							dsctbl.c
  *						2016-02-11	by zlq
  *
  *------------------------------------------------------------------------
  * @brief 本文件包含了关于GDT和IDT的相关函数
  *
  *************************************************************************
  */
#include <miniOS/dsctbl.h>
#include <miniOS/int.h>

void gdtidt_init(void)
{
	segment_descriptor *gdt = (segment_descriptor *)ADR_GDT;
	gate_descriptor *idt = (gate_descriptor *)ADR_IDT;
	int i;

	//GDT的初始化
	for (i = 0; i < 8192; i++) {
		set_segdesc(gdt + i, 0, 0, 0);
	}
	set_segdesc(gdt + 1, 0xffffffff, 0x00000000, 0x4092);
	set_segdesc(gdt + 2, 0x0007ffff, 0x00280000, 0x409a);
	load_gdtr(0xffff, 0x00270000);

	//IDT的初始化
	for (i = 0; i < 256; i++) {
		set_gatedesc(idt + i, 0, 0, 0);
	}
    set_gatedesc(idt + 0x21,(int) asm_inthandler21, 2<<3, AR_INTGATE32);
    set_gatedesc(idt + 0x2c, (int) asm_inthandler2c, 2<<3, AR_INTGATE32);
	load_idtr(0x7ff, 0x0026f800);

	return;
}


void set_segdesc(segment_descriptor *sd, uint32_t limit, int base, int ar)
{
	if (limit > 0xfffff) {
		ar |= 0x8000;							/* G_bit = 1					*/
		limit /= 0x1000;
	}

	sd->limit_low = limit & 0xffff;				/* 获取limit低16位（2个字节）		*/
	sd->base_low = base & 0xffff;				/* 获取base低16位内容			*/
	sd->base_mid = (base >> 16) & 0xff;			/* 获取base中8位的内容			*/
	sd->access_right = ar & 0xff;				/* access right低8位				*/
	sd->limit_high = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
	sd->base_high = (base >> 24) & 0xff;

	return;
}


void set_gatedesc(gate_descriptor *gd, int offset, int selector, int ar)
{
	gd->offset_low = offset & 0xffff;
	gd->selector = selector;
	gd->dw_count = (ar >> 8) & 0xff;
	gd->access_right = ar & 0xff;
	gd->offset_high = (offset >> 16) & 0xffff;

	return;
}
