#include <asm/sys_io.h>
#include <miniOS/window.h>
#include <miniOS/dsctbl.h>
#include <miniOS/int.h>
#include "stdio.h"


void HariMain(void)
{
	char str_buffer[12];
	char mouse_cursor_buf[16][16];
	boot_info_t *bootInfo = NULL;
	int mouse_x,mouse_y;
	
	////////////////////////////////////////////////////////////
	//
	bootInfo = (boot_info_t *)BOOT_INFO_ADDR;					/* 获取启动信息						*/			
	
	////////////////////////////////////////////////////////////
	//
	palette_init();												/* 初始化调色板						*/
	desktop_init();												/* 桌面初始化						*/
	
	////////////////////////////////////////////////////////////
	//鼠标显示
	mouse_x = (bootInfo->scrnx - 16)/2;
	mouse_y = (bootInfo->scrny - 16)/2;
	mouse_cursor_init((char *)mouse_cursor_buf,COL8_008484);

	gdtidt_init();
	pic_init();
	////////////////////////////////////////////////////////////
	//
	sprintf(str_buffer, "screen = %d", bootInfo->scrnx);
	print_string(10, 10, COL8_000000, str_buffer);

	for (;;) {
		HLT;
	}
}

