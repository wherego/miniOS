/**
  *************************************************************************
  *
  *							keyboard.c
  *						2016-02-17	by zlq
  *
  *------------------------------------------------------------------------
  * @brief 本文件包含了关于键盘的相关函数
  * 
  * @attention
  *     1、处理中断过程中注意要清理中断标志；
  *     2、字符显示需要占用比较多的时间，所以可以制作一个缓冲区，存放键盘数据，
  *        然后让操作系统主动检查这个缓冲区，加快中断处理的速度。
  *
  *************************************************************************
  */
#include <miniOS/keyboard.h>
#include <asm/io.h>

/**
  * @brief  等待键盘控制器响应
  *
  * @param None
  * @return None
  */
void wait_KBC_ready(void)
{
    for(;(io_in8(0x64) & 0x02) != 0;){
       break;
    }
    return;
}


/**
  * @brief  键盘初始化
  *
  * @param None
  * @return None
  */
void keyboard_init(void)
{
    wait_KBC_ready();
    io_out8(0x64,0x60);
    wait_KBC_ready();
    io_out8(0x60,0x47);
    return;
}

