#ifndef __MEMORY_H
#define __MEMORY_H


typedef struct{
    int addr,size;
}mem_block;

typedef struct{
    int frees,maxfrees,lostsize,losts;
    mem_block free[2000];
}Memory_structure;

int load_cr0(void);
void store_cr0(int cr0);

unsigned int memtest(unsigned int start, unsigned int end);
unsigned int memtest_sub(unsigned int start, unsigned int end);

void memory_init(Memory_structure *mem);
unsigned int memory_total(Memory_structure *mem);
unsigned int mem_alloc(Memory_structure *mem,unsigned int size);
int mem_free(Memory_structure *mem, unsigned int addr, unsigned int size);
unsigned int mem_alloc_4k(Memory_structure *mem, unsigned int size);
int mem_free_4k(Memory_structure *mem, unsigned int addr, unsigned int size);


#endif //!__MEMORY_H
