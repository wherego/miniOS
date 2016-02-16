#include <asm/io.h>
#include <miniOS/window.h>
#include <miniOS/dsctbl.h>
#include <miniOS/int.h>
#include "stdio.h"

Key_buf key_buf;

void HariMain(void)
{
	char str_buffer[12];
	char mouse_cursor_buf[16][16];
	boot_info_t *bootInfo = NULL;
	int mouse_x,mouse_y;
	int i;
    char s[4];
	////////////////////////////////////////////////////////////
	//
	bootInfo = (boot_info_t *)BOOT_INFO_ADDR;					/* 获取启动信息						*/	

	////////////////////////////////////////////////////////////
	//
	gdtidt_init();
	pic_init();
    io_sti(); /* IDT/PICの初期化が終わったのでCPUの割り込み禁止を解除 */


	////////////////////////////////////////////////////////////
	//
	palette_init();												/* 初始化调色板						*/
	desktop_init();												/* 桌面初始化							*/
	
	////////////////////////////////////////////////////////////
	//鼠标显示
	mouse_x = (bootInfo->scrnx - 16)/2;
	mouse_y = (bootInfo->scrny - 16)/2;
	mouse_cursor_init((char *)mouse_cursor_buf,COL8_008484);


	////////////////////////////////////////////////////////////
	//
	io_out8(PIC0_IMR, 0xf9); /* PIC1とキーボードを許可(11111001) */
	io_out8(PIC1_IMR, 0xef); /* マウスを許可(11101111) */
    
    print_string(0, 0, COL8_FFFFFF, "Test the keybord interrupt:");

	for (;;) {
        io_cli();
        if(key_buf.counter == 0){
            io_sti();
            HLT;
        }
        else{
            i = key_buf.data[key_buf.pop_index++];
            key_buf.counter--;
            
            if(key_buf.pop_index == 32){
                key_buf.pop_index = 0;
            }
            
            io_sti();
            
            sprintf(s,"%02x", i);
            draw_rectangle(COL8_008484, 0, 16, 16, 16);
            print_string(0, 16, COL8_FFFFFF, s);
            
        }
		
	}
}

