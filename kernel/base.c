/**
  *************************************************************************
  *
  *							base.c
  *						2016-02-16	by zlq
  *
  *------------------------------------------------------------------------
  * @berif 定义了一些基础的数据类型的处理函数
  *************************************************************************
  */
#include <miniOS/base.h>
#include "stdio.h"

void circular_queue_init(Circular_Queue *queue, int size, uint8_t *buf)
{
    queue->size = size;
    queue->free_size = size;
    queue->pop_index = 0;
    queue->push_index = 0;
    queue->flags = 0;
    
    queue->buf = buf;
    
    return;
}


uint8_t circular_queue_pop(Circular_Queue *queue, uint8_t *data)
{
    if(queue->free_size == queue->size){
        return -1;
    }
    
    *data = queue->buf[queue->pop_index++];
    if(queue->pop_index == queue->size){
        queue->pop_index = 0;
    }
    
    queue->free_size++;
    return 0;
}


uint8_t circular_queue_push(Circular_Queue *queue, uint8_t data)
{
       if(queue->free_size == 0){
        queue->flags |= OVERFLOW;
        return -1;
    }
    
    queue->buf[queue->push_index++] = data;
    if(queue->push_index == queue->size){
        queue->push_index = 0;
    }
    
    queue->free_size--;
    
    return 0; 
}

uint8_t is_empty(Circular_Queue *queue)
{
    if(queue == NULL){
        return -1;
    }
    
    if(queue->free_size == queue->size){
        return 0;
    }else{
        return 1;
    }
}


uint8_t is_full(Circular_Queue *queue)
{
    if(queue == NULL){
        return -1;
    }
    
    if(queue->free_size == 0){
        return 0;
    }else{
        return 1;
    }
}

