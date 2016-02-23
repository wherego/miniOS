#include <asm/io.h>
#include <miniOS/base.h>
#include <miniOS/window.h>
#include <miniOS/dsctbl.h>
#include <miniOS/int.h>
#include <miniOS/keyboard.h>
#include "stdio.h"
#include <miniOS/memory.h>


Circular_Queue key_queue;
Circular_Queue mouse_queue;
Mouse_dsc mouse_dsc;

void HariMain(void)
{
	char mouse_cursor_buf[16][16];
	boot_info_t *bootInfo = NULL;
	int mouse_x,mouse_y;
	int i;
    unsigned int memtotal;
    Memory_structure *memory = (Memory_structure *)0x3c0000;
    char s[40],key_int_buf[32],mouse_int_buf[128];
    
    
	////////////////////////////////////////////////////////////
	//
	bootInfo = (boot_info_t *)BOOT_INFO_ADDR;					/* 获取启动信息						*/	

	////////////////////////////////////////////////////////////
	//
	gdtidt_init();
	pic_init();
    io_sti();                                                   /* 打开在IDT/PIC初始化时关闭的中断 */
    
    io_out8(PIC0_IMR, 0xf9);                                    /* PIC1とキーボードを許可(11111001) */
	io_out8(PIC1_IMR, 0xef);                                    /* マウスを許可(11101111) */
    
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
    draw_block(mouse_x, mouse_y, 16, 16, (char *)mouse_cursor_buf);
    sprintf(s,"(%4d,%4d)", mouse_x,mouse_y);
    print_string(0, 0, COL8_FFFFFF, s);
    mouse_enable(&mouse_dsc);
    
    memtotal = memtest(0x00400000,0xbfffffff);
    memory_init(memory);
    mem_free(memory,0x00001000,0x0009e000);
    mem_free(memory,0x0040000,memtotal-0x0040000);
    sprintf(s,"memory: %dMB  free: %dKB", memtotal/(1024*1024),memory_total(memory)/1024);
    print_string(0, 32, COL8_FFFFFF, s);

    ////////////////////////////////////////////////////////////
	for (;;) {
        io_cli();
        if(is_empty(&key_queue) + is_empty(&mouse_queue) == 0){
            io_sti();
            HLT;
        }
        else{
            if(is_empty(&key_queue) == 1){
                circular_queue_pop(&key_queue,(char *)&i);
            
                io_sti();
            
                sprintf(s,"%02x", i);
                draw_rectangle(COL8_008484, 0, 16, 16, 16);
                print_string(0, 16, COL8_FFFFFF, s);
            }
            else if(is_empty(&mouse_queue) == 1){
                circular_queue_pop(&mouse_queue,(char *)&i);
            
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
                    draw_block(mouse_x, mouse_y, 16, 16, (char *)mouse_cursor_buf);
                }
               
            }
            
        }
		
	}
}

