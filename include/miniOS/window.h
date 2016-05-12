#ifndef __WINDOW_H
#define __WINDOW_H

#include "stdint.h"
#include <miniOS/memory.h>


/* 宏定义------------------------------------------------*/
#define VRAM_ADDR		((char *)0xa0000)
#define BOOT_INFO_ADDR	((char *)0x0ff0)				/* 启动信息，保存有显示的信息		*/
#define SCRNX			(*((short *)0x0ff4))
#define SCRNY			(*((short *)0x0ff6))

#define COL8_000000		0								/*  0: 黑			*/
#define COL8_FF0000		1								/*  1:亮红		 	*/
#define COL8_00FF00		2								/*  2:亮绿		 	*/
#define COL8_FFFF00		3								/*  3:亮黄		 	*/
#define COL8_0000FF		4								/*  4:亮蓝		 	*/
#define COL8_FF00FF		5								/*  5:亮紫		 	*/
#define COL8_00FFFF		6								/*  6:浅亮蓝			*/
#define COL8_FFFFFF		7								/*  7:白				*/
#define COL8_C6C6C6		8								/*  8:亮灰		 	*/
#define COL8_840000		9								/*  9:暗红			*/
#define COL8_008400		10								/* 10:暗绿		 	*/
#define COL8_848400		11								/* 11:暗黄		 	*/
#define COL8_000084		12								/* 12:暗青		 	*/
#define COL8_840084		13								/* 13:暗紫		 	*/
#define COL8_008484		14								/* 14:钱暗蓝	 		*/
#define COL8_848484		15								/* 15:暗灰		 	*/


#define MAX_LAYERS      256

/**
  * 保存启动信息的结构体类型
  */
typedef struct{
	char cyls,leds,vmode,reserve;
	short scrnx,scrny;
	char *vram;
}boot_info_t;

/**
  * 描述鼠标的结构体
  */
typedef struct{
    uint8_t data[3],phase;
    int x,y,btn;
}Mouse_dsc;

typedef struct{
    unsigned char *buf;
    int begin_x, begin_y,  length_x, length_y, inv_col, height, flags;
}Layers;

typedef struct{
    int bottom;
    Layers *layerAddr[MAX_LAYERS];
    Layers layerInfo[MAX_LAYERS];
}Layers_tbl;


/* 函数声明-----------------------------------------------*/
void palette_init(void);
void set_palette(int begin, int end, uint8_t *rgb);
void draw_rectangle(unsigned char *vram, uint8_t color, int begin_x, int begin_y, int length_x, int length_y);
void draw_block(int begin_x, int begin_y, int length_x, int length_y, char *buf);
void desktop_init(unsigned char *vram);

//输出字符和字符串
void print_char(int x, int y, uint8_t color, int8_t chr);
void print_string(int x, int y, uint8_t color, int8_t *str);

////////////////////////////////////////////////////////
//鼠标
void mouse_enable(Mouse_dsc *mouse);
void mouse_cursor_init(char *mouse_buffer, char background_color);
uint8_t mouse_decode(Mouse_dsc *mouse, uint8_t data);

/////////////////////////////////////////////////////////
//图层表
Layers_tbl *layers_tbl_init(Memory_structure *mem);
Layers *layer_alloc(Layers_tbl *layTbl);
void setLayer(Layers *layer, unsigned char *buf, int length_x, int length_y, int inv_col);
void setLayerHeight(Layers_tbl *layTbl, Layers *layer, int height);
void layer_refresh(Layers_tbl *layTbl);
void layer_shift(Layers_tbl *layTbl, Layers *layer, unsigned int begin_x, unsigned int begin_y);
void layer_free(Layers_tbl *layTbl, Layers *layer);

#endif // !__WINDOW_H

