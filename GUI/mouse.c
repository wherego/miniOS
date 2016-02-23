/**
  *************************************************************************
  *
  *							mouse.c
  *						2016-02-11	by zlq
  *
  *------------------------------------------------------------------------
  * @brief 本文件包含了关于鼠标的相关函数
  *
  *************************************************************************
  */

#include <miniOS/window.h>
#include <miniOS/keyboard.h>
#include "stdint.h"

#define MOUSE_ENABLE        0xf4

/**
  * @brief  使能鼠标，向键盘控制器发送指令0xd4下一个数据就会发给
  *         鼠标。
  * @attention
  *         鼠标激活后，会发送一个数据，0xfa
  *
  * @param[out] mouse，描述鼠标的结构体数据 
  * @return None
  */
void mouse_enable(Mouse_dsc *mouse)
{
    wait_KBC_ready();
    io_out8(0x64,0xd4);
    wait_KBC_ready();
    io_out8(0x60,MOUSE_ENABLE);
    mouse->phase = 0;
    return;
}


/**
  * @brief  解析鼠标数据
  * @attention
  *         鼠标激活后，会发送一个数据，0xfa
  *
  * @param[out] mouse，描述鼠标的结构体数据
  * @param[in]  data，鼠标的中断数据
  * 
  * @return 函数执行状态
  */
uint8_t mouse_decode(Mouse_dsc *mouse, uint8_t data)
{
    if(mouse->phase == 0){
        if(data == 0xfa){
            mouse->phase = 1;
        }
        return 0;
    }
    else if(mouse->phase == 1){
        //如果第一字节正确
        if((data & 0xc8) == 0x08){
            mouse->data[0] = data;
            mouse->phase = 2;
        }
        return 0;
    }
    else if(mouse->phase == 2){
        mouse->data[1] = data;
        mouse->phase = 3;
        return 0;
    }
    else if(mouse->phase == 3){
        mouse->data[2] = data;
        mouse->phase = 1;
        
        //鼠标数据解析
        mouse->btn = mouse->data[0] & 0x07;
        mouse->x = mouse->data[1];
        mouse->y = mouse->data[2];
        
        if((mouse->data[0] & 0x10) != 0){
            mouse->x |= 0xffffff00;
        }
        if((mouse->data[0] & 0x20) != 0){
            mouse->y |= 0xffffff00;
        }
        mouse->y = -mouse->y;//鼠标的y方向和画面方向相反
        
        return 1;
    }
    return -1;
}

/**
  * @brief  鼠标初始化
  *
  * @param[out] mouse_buffer，保存鼠标的数据
  * @param[in] background_color，背景颜色
  * @return None
  */
void mouse_cursor_init(char *mouse_buffer, char background_color)
{
	static char cursor[16][16] = {
		"*...............",
		"**..............",
		"*O*.............",
		"*OO*............",
		"*OOO*...........",
		"*OOOO*..........",
		"*OOOOO*.........",
		"*OOOOOO*........",
		"*OOOOOOO*.......",
		"*OOOOOOOO*......",
		"*OOOOOOOOO*.....",
		"*OOO*O*.*OO*....",
		"*OO*.*O*..**....",
		"*O*..*O*........",
		"**....*O*.......",
		"*.....***......."
	};
	int x, y;

	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			if (cursor[y][x] == '*') {
			     mouse_buffer[y * 16 + x] = COL8_000000;
			}
			if (cursor[y][x] == 'O') {
			     mouse_buffer[y * 16 + x] = COL8_FFFFFF;
			}
			if (cursor[y][x] == '.') {
			     mouse_buffer[y * 16 + x] = background_color;
			}
		}
    }
	return;
}
