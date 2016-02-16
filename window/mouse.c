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
				VRAM_ADDR[((SCRNY - 16) / 2 + y) * SCRNX + ((SCRNX - 16) / 2 + x)] = mouse_buffer[y * 16 + x] = COL8_000000;
			}
			if (cursor[y][x] == 'O') {
				VRAM_ADDR[((SCRNY - 16) / 2 + y) * SCRNX + ((SCRNX - 16) / 2 + x)] = mouse_buffer[y * 16 + x] = COL8_FFFFFF;
			}
			if (cursor[y][x] == '.') {
				VRAM_ADDR[((SCRNY - 16) / 2 + y) * SCRNX + ((SCRNX - 16) / 2 + x)] = mouse_buffer[y * 16 + x] = background_color;
			}
		}
	}

	return;
}
