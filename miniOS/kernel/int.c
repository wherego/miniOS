#include <miniOS/int.h>
#include <asm/sys_io.h>

void pic_init(void)
{
	//IMR:interrupt mask register
	io_out8(PIC0_IMR, 0xff);				/* 中断屏蔽寄存器，禁止所有中断		*/
	io_out8(PIC1_IMR, 0xff);				/* 中断屏蔽寄存器，禁止所有中断		*/

	//ICW:initial control word
	io_out8(PIC0_ICW1, 0x11);				/* 边沿触发模式						*/
	io_out8(PIC0_ICW2, 0x20);				/* IRQ0-7由INT20-27接收				*/
	io_out8(PIC0_ICW3, 1 << 2);				/* PIC1由IRQ2链接					*/
	io_out8(PIC0_ICW4, 0x01);				/* 无缓冲区模式						*/

	io_out8(PIC1_ICW1, 0x11);				/* 边沿触发模式						*/
	io_out8(PIC1_ICW2, 0x28);				/* IRQ8-15由INT28-2f接收				*/
	io_out8(PIC1_ICW3, 2);					/* PIC1由IRQ2链接					*/
	io_out8(PIC1_ICW4, 0x01);				/* 无缓冲区模式						*/

	io_out8(PIC0_IMR, 0xfb);				/* 11111011 PIC1以外全部禁止			*/
	io_out8(PIC1_IMR, 0xff);				/* 11111111 禁止所有中断				*/

	return;
}

