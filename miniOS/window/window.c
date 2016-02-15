/**
  *************************************************************************
  *
  *							window.c
  *						2016-02-03	by zlq
  *
  *------------------------------------------------------------------------
  * @brief ���ļ������˹���GUI����غ���
  *
  *************************************************************************
  */
#include <miniOS/window.h>
#include <asm/sys_io.h>


extern char font[4096];											/* ���壬����					*/

/**
  * @brief  ��ʼ����ɫ��
  *
  * @param  None
  * @return None
  */
void palette_init(void)
{
	static uint8_t table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,						/*  0: ��		 */
		0xff, 0x00, 0x00,						/*  1:����		 */
		0x00, 0xff, 0x00,						/*  2:����		 */
		0xff, 0xff, 0x00,						/*  3:����		 */
		0x00, 0x00, 0xff,						/*  4:����		 */
		0xff, 0x00, 0xff,						/*  5:����		 */
		0x00, 0xff, 0xff,						/*  6:ǳ����		 */
		0xff, 0xff, 0xff,						/*  7:��			 */
		0xc6, 0xc6, 0xc6,						/*  8:����		 */
		0x84, 0x00, 0x00,						/*  9:����		 */
		0x00, 0x84, 0x00,						/* 10:����		 */
		0x84, 0x84, 0x00,						/* 11:����		 */
		0x00, 0x00, 0x84,						/* 12:����		 */
		0x84, 0x00, 0x84,						/* 13:����		 */
		0x00, 0x84, 0x84,						/* 14:Ǯ����		 */
		0x84, 0x84, 0x84						/* 15:����		 */
	};
	set_palette(0, 15, table_rgb);
	return;
}


/**
  * @brief  ���õ�ɫ��
  *
  * @param[in] begin
  * @param[in] end
  * @param[in] rgb��д���ɫ�������
  * @return None
  */
void set_palette(int begin, int end, uint8_t *rgb)
{
	int i, eflags;
	eflags = io_load_eflags();					/* ��¼�ж���ɱ�־��ֵ			 */
	io_cli(); 									/* ��ֹ�ж�						 */

	//���õ�ɫ�壬���岽��
	io_out8(0x03c8, begin);
	for (i = begin; i <= end; i++) {
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}

	io_store_eflags(eflags);					/* �ָ��ж���ɱ�־��ֵ			 */
	return;
}


/**
  * @brief  ���ƾ���
  *
  * @param[in] color�����ε���ɫ
  * @param[in] begin_x����ʼ��x����
  * @param[in] begin_y����ʼ��y����
  * @param[in] length_x��x��ĳ���
  * @param[in] length_y��y��ĳ���
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
/**
  * @brief  �����ʼ��
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
  * @brief  ��ӡ�ַ�
  *
  * @param[in] (x,y)��ʼ����
  * @param[in] color���ַ���ɫ
  * @param[in] chr���ַ�����
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
  * @brief  ����ַ���
  *
  * @param[in] (x,y)���ַ�����ʾ����
  * @param[in] color���ַ�����ɫ
  * @param[in] str��ָ���ַ�����ָ��
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

