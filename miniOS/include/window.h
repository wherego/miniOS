#ifndef __WINDOW_H
#define __WINDOW_H

#include "stdint.h"
#include "sys_io.h"

/* �궨��------------------------------------------------*/
#define VRAM_ADDR		((char *)0xa0000)
#define BOOT_INFO_ADDR	((char *)0x0ff0)				/* ������Ϣ����������ʾ����Ϣ		*/
#define SCRNX			(*((short *)0x0ff4))
#define SCRNY			(*((short *)0x0ff6))

#define COL8_000000		0								/*  0: ��			*/
#define COL8_FF0000		1								/*  1:����		 	*/
#define COL8_00FF00		2								/*  2:����		 	*/
#define COL8_FFFF00		3								/*  3:����		 	*/
#define COL8_0000FF		4								/*  4:����		 	*/
#define COL8_FF00FF		5								/*  5:����		 	*/
#define COL8_00FFFF		6								/*  6:ǳ����			*/
#define COL8_FFFFFF		7								/*  7:��				*/
#define COL8_C6C6C6		8								/*  8:����		 	*/
#define COL8_840000		9								/*  9:����			*/
#define COL8_008400		10								/* 10:����		 	*/
#define COL8_848400		11								/* 11:����		 	*/
#define COL8_000084		12								/* 12:����		 	*/
#define COL8_840084		13								/* 13:����		 	*/
#define COL8_008484		14								/* 14:Ǯ����	 		*/
#define COL8_848484		15								/* 15:����		 	*/




/**
  * ����������Ϣ�Ľṹ������
  */
typedef struct{
	char cyls,leds,vmode,reserve;
	short scrnx,scrny;
	char *vram;
}boot_info_t;


/* ��������-----------------------------------------------*/
void palette_init(void);
void set_palette(int begin, int end, uint8_t *rgb);
void draw_rectangle(uint8_t color, int begin_x, int begin_y, int length_x, int length_y);
void desktop_init(void);

//����ַ����ַ���
void print_char(int x, int y, uint8_t color, int8_t chr);
void print_string(int x, int y, uint8_t color, int8_t *str);

//���
void mouse_cursor_init(char *mouse_buffer, char background_color);


#endif // !__WINDOW_H

