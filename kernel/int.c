/**
  *************************************************************************
  *
  *							int.c
  *						2016-02-15	by zlq
  *
  *------------------------------------------------------------------------
  * @brief 本文件包含了关于中断的相关函数
  * 
  * @attention
  *     处理中断过程中注意要清理中断标志
  *
  *************************************************************************
  */
#include <miniOS/int.h>
#include <asm/io.h>
#include <miniOS/window.h>
#include "stdio.h"

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

/**
  * @brief  键盘IRQ1中断，int 0x21
  *
  * @param[in] esp
  * @return None
  */
void inthandler21(int *esp)
{
    uint8_t data,s[4];
    
    io_out8(PIC0_OCW2,0x61);                /* 清除int 0x21的中断标志             */
    data = io_in8(0x60);
    
    sprintf(s,"%02x", data);
    draw_rectangle(COL8_008484, 0, 16, 16, 16);
    print_string(0, 16, COL8_FFFFFF, s);
    
    return;
}

/**
  * @brief  int 0x27
  *
  * @param[in] esp
  * @return None
  */
void inthandler27(int *esp)
{
	io_out8(PIC0_OCW2, 0x67); 
	return;
}

/**
  * @brief  鼠标IRQ12中断,int 0x2c
  *
  * @param[in] esp
  * @return None
  */
void inthandler2c(int *esp)
{
    draw_rectangle(COL8_000000, 0, 0, 32*8, 16);
    print_string(0, 0, COL8_FFFFFF, "INT 2c(IRQ-12):PS/2 mouse");
    
    for(;;){
        io_hlt();
    }
}

