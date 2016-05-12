/**
  *************************************************************************
  *
  *							memory.c
  *						2016-02-17	by zlq
  *
  *------------------------------------------------------------------------
  * @brief 本文件包含了关于内存管理的函数
  * 
  * @attention
  *     
  *
  *************************************************************************
  */
#include <asm/io.h>
#include <miniOS/memory.h>


#define EFLAGS_AC_BIT           0x00040000
#define CR0_CACHE_DISABLE       0x60000000
#define MEM_FREES               4096

/**
  * @brief  内存测试函数，并获取内存大小
  *
  * @param[in] start,开始地址
  * @param[in] end,结束地址
  * @return 内存容量
  */
unsigned int memtest(unsigned int start, unsigned int end)
{
    char flg486 = 0;
    unsigned int eflg,cr0,i;
    
    //确认是386 or 486以上的
    eflg = io_load_eflags();
    eflg |= EFLAGS_AC_BIT;
    io_store_eflags(eflg);
    eflg = io_load_eflags();
    if((eflg & EFLAGS_AC_BIT) != 0){
        flg486 = 1;
    }
    eflg &= ~EFLAGS_AC_BIT;
    io_store_eflags(eflg);
    
    if(flg486 != 0){
        cr0 = load_cr0();
        cr0 |= CR0_CACHE_DISABLE;
        store_cr0(cr0);
    }
    
    i = memtest_sub(start,end);
    
    if(flg486 != 0){
        cr0 = load_cr0();
        cr0 &= ~CR0_CACHE_DISABLE;
        store_cr0(cr0);
    }
    
    return i;
}

/**
  * @brief  内存初始化
  *
  * @param[in] start,开始地址
  * @param[in] end,结束地址
  * @return 内存容量
  */
void memory_init(Memory_structure *mem)
{
    mem->frees = 0;
    mem->maxfrees = 0;
    mem->lostsize = 0;
    mem->losts = 0;
    return;
}


unsigned int memory_total(Memory_structure *mem)
{
    unsigned int i,total = 0;
    for(i = 0; i < mem->frees; i++){
        total += mem->free[i].size;
    }
    return total;
}

unsigned int mem_alloc(Memory_structure *mem,unsigned int size)
{
    unsigned int i,addr;
    for(i=0; i < mem->frees; i++){
        if(mem->free[i].size > size){
            addr = mem->free[i].addr;
            mem->free[i].addr+=size;
            mem->free[i].size -= size;
            if(mem->free[i].size == 0){
                mem->frees--;
                for(; i<mem->frees; i++){
                    mem->free[i] = mem->free[i +1];
                }
            }
            return addr;
        }       
    }
    return 0;
}


int mem_free(Memory_structure *mem, unsigned int addr, unsigned int size)
{
    int i,j;
    
    //寻找存放地址
    for(i = 0; i< mem->frees;i++){
        if(mem->free[i].addr > addr){
            break;
        }
    }
    
    /* free[i - 1].addr < addr < free[i].addr*/
    if(i > 0){
        //前面有可用的内存
        if(mem->free[i-1].addr + mem->free[i-1].size == addr){
            //可与和前面的内存合在一起
            mem->free[i-1].size += size;
            if(i < mem->frees){
                //后面有可用的内存
                if(addr + size == mem->free[i].addr){
                    //也可以和后面的内存结合在一起
                    mem->free[i - 1].size += mem->free[i].size;
                    mem->frees--;
                    for(;i<mem->frees;i++){
                        mem->free[i] = mem->free[i+1];
                    }
                }
            }
            return 0;
        }
    }
    if(i< mem->frees){
        if(addr + size == mem->free[i].addr){
            mem->free[i].addr = addr;
            mem->free[i].size = size;
            return 0;
        }
    }
    if(mem->frees < MEM_FREES){
        for(j = mem->frees; j > i; j--){
            mem->free[j] = mem->free[j-1];
        }
        mem->frees++;
        if(mem->maxfrees>mem->frees){
            mem->maxfrees = mem->frees;
        }
        mem->free[i].addr = addr;
        mem->free[i].size = size;
        return 0;
    }
    //不能向后移动
    mem->losts++;
    mem->lostsize += size;
    return -1;
}


unsigned int mem_alloc_4k(Memory_structure *mem, unsigned int size)
{
    unsigned int a;
    size = (size + 0xff) & 0xffffff00;
    a = mem_alloc(mem,size);
    return a;
}


int mem_free_4k(Memory_structure *mem, unsigned int addr, unsigned int size)
{
   int i;
   size = (size + 0xff) & 0xffffff00;
   i = mem_free(mem,addr,size);
   return i;
}


