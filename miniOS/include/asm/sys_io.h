/**
  *****************************************************************
  *
  *						sys_io.h
  *							2016-02-02	by zlq
  *
  *****************************************************************
  */
#ifndef __SYS_IO_H
#define __SYS_IO_H

/* 宏定义----------------------------------------------------------*/
#define HLT io_hlt()			


/* 函数声明-------------------------------------------------------*/
void io_hlt(void);
void io_cli(void);
void io_sti(void);

int io_in8(int port);
int io_in16(int port);
int io_in32(int port);

void io_out8(int port, int data);
void io_out16(int port, int data);
void io_out32(int port, int data);

int io_load_eflags(void);
void io_store_eflags(int eflags);

#endif // !__SYS_IO_H

