/**
  *************************************************************************
  *
  *							base.h
  *						2016-02-16	by zlq
  *
  *------------------------------------------------------------------------
  * @berif 定义了一些基础的数据类型
  *************************************************************************
  */
#ifndef __BASE_H
#define __BASE_H

#include "stdint.h"


#define OVERFLOW        0x00000001

/**
  * 制作FIFO键盘数据缓冲区
  * @{
  */
typedef struct{
    uint8_t *buf;
    int size;                   /* 缓冲区大小        */
    int free_size;              /* */
    int flags;
    int pop_index,push_index;
}Circular_Queue;
/**
  * @}
  */

void circular_queue_init(Circular_Queue *queue, int size, uint8_t *buf);
uint8_t circular_queue_pop(Circular_Queue *queue, uint8_t *data);
uint8_t circular_queue_push(Circular_Queue *queue, uint8_t data);
uint8_t is_full(Circular_Queue *queue);
uint8_t is_empty(Circular_Queue *queue);

#endif //!__BASE_H

