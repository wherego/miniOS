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
void draw_rectangle(unsigned char *vram, uint8_t color, int begin_x, int begin_y, int length_x, int length_y)
{
	int x, y;

	for (y = begin_y; y < begin_y + length_y; y++) {
		for (x = begin_x;x < begin_x + length_x;x++) {
			vram[y*SCRNX + x] = color;
		}
	}
	return;
}


/**
  * @brief  根据指定的数据，绘制图形
  *
  * @param[in] begin_x，开始的x坐标
  * @param[in] begin_y，开始的y坐标
  * @param[in] length_x，x轴的长度
  * @param[in] length_y，y轴的长度
  * @param[in] buf，指定数据的内存空间
  *
  * @return None
  */
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
void desktop_init(unsigned char *vram)
{
	draw_rectangle(vram, COL8_008484, 0, 0, SCRNX, SCRNY - 25);
	draw_rectangle(vram, COL8_C6C6C6, 0, SCRNY - 25, SCRNX, 25);
	draw_rectangle(vram, COL8_848484, 0, SCRNY - 25, 50, 25);

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


/**
  * @brief  图层表初始化
  *
  * @param[in] mem,内存
  * @return 图层表首地址
  */
Layers_tbl *layers_tbl_init(Memory_structure *mem)
{
    Layers_tbl *layTbl;
    int i;
    
    layTbl = (Layers_tbl *)mem_alloc_4k(mem,sizeof(Layers_tbl));
    if(layTbl == 0){
        return 0;
    }
    
    layTbl->bottom = -1;//没有涂层
    
    for(i = 0; i < MAX_LAYERS; i++){
        layTbl->layerInfo[i].flags = 0;         /* 标记为未使用               */
    }
    return layTbl;
}



/**
  * @brief  从图层表中申请图层
  *
  * @param[in] layTbl，图层表首地址
  * @return 图层地址;0 申请失败
  */
Layers *layer_alloc(Layers_tbl *layTbl)
{
    Layers *layer;
    int i;
    for(i = 0; i < MAX_LAYERS; i++){
        if(layTbl->layerInfo[i].flags == 0){
            layer = &layTbl->layerInfo[i];
            layer->flags = 1;
            layer->height = -1;                     /* 隐藏                   */
            return layer;
        }
    }
    return 0;
}


/**
  * @brief  设置图层参数
  *
  * @param[in] layTbl，图层表首地址
  * @return None
  */
void setLayer(Layers *layer, unsigned char *buf, int length_x, int length_y, int inv_col)
{
    layer->buf = buf;
    layer->length_x = length_x;
    layer->length_y = length_y;
    layer->inv_col = inv_col;
    return;
}


/**
  * @brief  设置图层高度
  *
  * @param[in] layTbl，图层表首地址
  * @param[in] layer，需要设置的图层
  * @param[in] height，图层高度
  * @return None
  */
void setLayerHeight(Layers_tbl *layTbl, Layers *layer, int height)
{
    int i,old_hg;
    old_hg = layer->height;
    
    /* 检查设置高度值               */
    if(height > layTbl->bottom + 1){
        height = layTbl->bottom+1;
    }
    if(height < -1){
        height = -1;
    }
    layer->height = height;
    
    
    //sort
    if(height < old_hg){     //图层上移
        if(height >= 0){
            //将其之前的下移一层
            for(i = old_hg; i > height; i--){               
                layTbl->layerAddr[i] = layTbl->layerAddr[i - 1];
                layTbl->layerAddr[i]->height = i;
            }
            layTbl->layerAddr[height] = layer;
        }
        else {
            if(layTbl->bottom > old_hg){
                //将其之后的上移一层
                for(i = old_hg; i < layTbl->bottom; i++){
                    layTbl->layerAddr[i] = layTbl->layerAddr[i + 1];
                    layTbl->layerAddr[i]->height = i;
                }
            }
            layTbl->bottom--;
        }
        layer_refresh(layTbl);
    }
    else if(old_hg > height){ //图层下移
        if(old_hg > 0){
            for(i = old_hg; i >= height; i++){
                layTbl->layerAddr[i] = layTbl->layerAddr[i + 1];
                layTbl->layerAddr[i]->height = i;
            }
            layTbl->layerAddr[height] = layer;
        }
        else{
            for(i = layTbl->bottom; i >= height; i--){
                layTbl->layerAddr[i + 1] = layTbl->layerAddr[i];
                layTbl->layerAddr[i + 1]->height = i + 1;
            }
            layTbl->layerAddr[height] = layer;
            layTbl->bottom++;
        }
        layer_refresh(layTbl);
    }
    return;
}


void layer_refresh(Layers_tbl *layTbl)
{
    int hg, i,j,x,y;
    unsigned char *buf,col;
    Layers *layer;
    for(hg =0; hg <= layTbl->bottom; hg++){
        layer = layTbl->layerAddr[hg];
        buf = layer->buf;
        
        for(j = 0; j < layer->length_x;j++){
            y = layer->begin_y + j;
            for(i = 0; i < layer->length_x; i++){
                x = layer->begin_x + i;
                col = buf[j*layer->length_x + i];
                if(col != layer->inv_col){
                    VRAM_ADDR[y*SCRNX + x] = col;
                }
            }
        }
    }
    return;
}

void layer_shift(Layers_tbl *layTbl, Layers *layer, unsigned int begin_x, unsigned int begin_y)
{
    layer->begin_x = begin_x;
    layer->begin_y = begin_y;
    if(layer >= 0){
        layer_refresh(layTbl);
    }
    return;
}

void layer_free(Layers_tbl *layTbl, Layers *layer)
{
    if(layer->height >= 0){
        setLayerHeight(layTbl,layer,-1);
    }
    layer->flags = 0;
    return;
}