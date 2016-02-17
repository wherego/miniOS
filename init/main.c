#include <asm/io.h>
#include <miniOS/base.h>
#include <miniOS/window.h>
#include <miniOS/dsctbl.h>
#include <miniOS/int.h>
#include <miniOS/keyboard.h>
#include "stdio.h"


Circular_Queue key_queue;
Circular_Queue mouse_queue;
Mouse_dsc mouse_dsc;

void HariMain(void)
{
	char mouse_cursor_buf[16][16];
	boot_info_t *bootInfo = NULL;
	int mouse_x,mouse_y;
	int i;
    char s[40],key_int_buf[32],mouse_int_buf[128];
    uint8_t mouse_data_buf[3],mouse_pyase;
    
    
	////////////////////////////////////////////////////////////
	//
	bootInfo = (boot_info_t *)BOOT_INFO_ADDR;					/* 获取启动信息						*/	

	////////////////////////////////////////////////////////////
	//
	gdtidt_init();
	pic_init();
    io_sti(); /* IDT/PICの初期化が終わったのでCPUの割り込み禁止を解除 */
    
    io_out8(PIC0_IMR, 0xf9); /* PIC1とキーボードを許可(11111001) */
	io_out8(PIC1_IMR, 0xef); /* マウスを許可(11101111) */
    
    circular_queue_init(&key_queue, 32, key_int_buf);
    circular_queue_init(&mouse_queue, 128, mouse_int_buf);
    
    keyboard_init();
	////////////////////////////////////////////////////////////
	//
	palette_init();												/* 初始化调色板						*/
	desktop_init();												/* 桌面初始化							*/
	
	////////////////////////////////////////////////////////////
	//鼠标显示
	mouse_x = (bootInfo->scrnx - 16)/2;
	mouse_y = (bootInfo->scrny - 16)/2;
	mouse_cursor_init((char *)mouse_cursor_buf,COL8_008484);
    draw_block(mouse_x, mouse_y, 16, 16, mouse_cursor_buf);
    sprintf(s,"(%4d,%4d)", mouse_x,mouse_y);
    print_string(0, 0, COL8_FFFFFF, s);
    
    
    /////////////////////////////////////////////////////////////////////////////
    //其他代码在上面的初始化下面写
    mouse_enable(&mouse_dsc);

    ////////////////////////////////////////////////////////////
	for (;;) {
        io_cli();
        if(is_empty(&key_queue) + is_empty(&mouse_queue) == 0){
            io_sti();
            HLT;
        }
        else{
            if(is_empty(&key_queue) == 1){
                circular_queue_pop(&key_queue,&i);
            
                io_sti();
            
                sprintf(s,"%02x", i);
                draw_rectangle(COL8_008484, 0, 16, 16, 16);
                print_string(0, 16, COL8_FFFFFF, s);
            }
            else if(is_empty(&mouse_queue) == 1){
                circular_queue_pop(&mouse_queue,&i);
            
                io_sti();
                
                if(mouse_decode(&mouse_dsc,i) == 1){
                    
                    sprintf(s,"[lcr %4d %4d]", mouse_dsc.x, mouse_dsc.y);
                    if((mouse_dsc.btn & 0x01) != 0){
                        s[1] = 'L'; 
                    }
                    if((mouse_dsc.btn & 0x02) != 0){
                        s[3] = 'R';
                    }
                    if((mouse_dsc.btn & 0x04) != 0){
                        s[2] = 'C';
                    }
                    draw_rectangle(COL8_008484, 32, 16, 16*15, 16);
                    print_string(32, 16, COL8_FFFFFF, s);
                    
                    //鼠标移动
                    draw_rectangle(COL8_008484,mouse_x,mouse_y,16,16);
                    mouse_x += mouse_dsc.x;
                    mouse_y += mouse_dsc.y;
                    
                    if(mouse_x < 0) mouse_x = 0;
                    if(mouse_y < 0) mouse_y = 0;
                    if(mouse_x > bootInfo->scrnx - 16) mouse_x = bootInfo->scrnx - 16;
                    if(mouse_y > bootInfo->scrny - 16) mouse_y = bootInfo->scrny - 16;
                    
                    sprintf(s,"(%4d,%4d)", mouse_x,mouse_y);
                    draw_rectangle(COL8_008484, 0, 0, 16*10, 16);
                    print_string(0, 0, COL8_FFFFFF, s);
                    draw_block(mouse_x, mouse_y, 16, 16, mouse_cursor_buf);
                }
               
            }
            
        }
		
	}
}

