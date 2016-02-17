/**
  *************************************************************************
  *
  *							window.c
  *						2016-02-03	by zlq
  *
  *------------------------------------------------------------------------
  * @brief 本文件包含了关于GUI的相关函数
  *
  *************************************************************************
  */
#include <miniOS/window.h>
#include <asm/io.h>


extern char font[4096];											/* 字体，汉字					*/

/**
  * @brief  初始化调色板
  *
  * @param  None
  * @return None
  */
void palette_init(void)
{
	static uint8_t table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,						/*  0: 黑		 */
		0xff, 0x00, 0x00,						/*  1:亮红		 */
		0x00, 0xff, 0x00,						/*  2:亮绿		 */
		0xff, 0xff, 0x00,						/*  3:亮黄		 */
		0x00, 0x00, 0xff,						/*  4:亮蓝		 */
		0xff, 0x00, 0xff,						/*  5:亮紫		 */
		0x00, 0xff, 0xff,						/*  6:浅亮蓝		 */
		0xff, 0xff, 0xff,						/*  7:白			 */
		0xc6, 0xc6, 0xc6,						/*  8:亮灰		 */
		0x84, 0x00, 0x00,						/*  9:暗红		 */
		0x00, 0x84, 0x00,						/* 10:暗绿		 */
		0x84, 0x84, 0x00,						/* 11:暗黄		 */
		0x00, 0x00, 0x84,						/* 12:暗青		 */
		0x84, 0x00, 0x84,						/* 13:暗紫		 */
		0x00, 0x84, 0x84,						/* 14:钱暗蓝		 */
		0x84, 0x84, 0x84						/* 15:暗灰		 */
	};
	set_palette(0, 15, table_rgb);
	return;
}


/**
  * @brief  设置调色板
  *
  * @param[in] begin
  * @param[in] end
  * @param[in] rgb，写入调色板的数据
  * @return None
  */
void set_palette(int begin, int end, uint8_t *rgb)
{
	int i, eflags;
	eflags = io_load_eflags();					/* 记录中断许可标志的值			 */
	io_cli(); 									/* 禁止中断						 */

	//设置调色板，具体步骤
	io_out8(0x03c8, begin);
	for (i = begin; i <= end; i++) {
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}

	io_store_eflags(eflags);					/* 恢复中断许可标志的值			 */
	return;
}


/**
  * @brief  绘制矩形
  *
  * @param[in] color，矩形的颜色
  * @param[in] begin_x，开始的x坐标
  * @param[in] begin_y，开始的y坐标
  * @param[in] length_x，x轴的长度
  * @param[in] length_y，y轴的长度
  *
  * @return None
  */
void draw_rectangle(uint8_t color, int begin_x, int begin_y, int length_x, int length_y)
{
	int x, y;

	for (y = begin_y;y < begin_y + length_y;y++) {
		for (x = begin_x;x < begin_x + length_x;x++) {
			VRAM_ADDR[y*SCRNX + x] = color;
		}
	}
	return;
}

void draw_block(int begin_x, int begin_y, int length_x, int length_y, char *buf)
{
    int x, y;

	for (y = 0; y < length_y; y++) {
		for (x = 0; x < length_x; x++) {
			VRAM_ADDR[(y + begin_y)*SCRNX + (x + begin_x)] = buf[y*16 + x];
		}
	}
	return;
}
/**
  * @brief  桌面初始化
  *
  * @param  None
  * @return None
  */
void desktop_init(void)
{
	draw_rectangle(COL8_008484, 0, 0, SCRNX, SCRNY - 25);
	draw_rectangle(COL8_C6C6C6, 0, SCRNY - 25, SCRNX, 25);
	draw_rectangle(COL8_848484, 0, SCRNY - 25, 50, 25);

	return;
}



/**
  * @brief  打印字符
  *
  * @param[in] (x,y)开始坐标
  * @param[in] color，字符颜色
  * @param[in] chr，字符编码
  * @return None
  */
void print_char(int x, int y, uint8_t color, int8_t chr)
{
	int i;
	int8_t *p, d;
	
	for (i = 0;i < 16;i++) {
		p = VRAM_ADDR + (y + i)*SCRNX + x;
		d = (chr * 16 + font)[i];

		if ((d & 0x80) != 0) { p[0] = color; }
		if ((d & 0x40) != 0) { p[1] = color; }
		if ((d & 0x20) != 0) { p[2] = color; }
		if ((d & 0x10) != 0) { p[3] = color; }
		if ((d & 0x08) != 0) { p[4] = color; }
		if ((d & 0x04) != 0) { p[5] = color; }
		if ((d & 0x02) != 0) { p[6] = color; }
		if ((d & 0x01) != 0) { p[7] = color; }
	}
	return;
}

/**
  * @brief  输出字符串
  *
  * @param[in] (x,y)，字符串显示坐标
  * @param[in] color，字符串颜色
  * @param[in] str，指向字符串的指针
  * @return None
  */
void print_string(int x, int y, uint8_t color, int8_t *str)
{
	for (; *str != 0x00; str++) {
		print_char(x, y, color, *str);
		x += 8;
	}
	return;
}



