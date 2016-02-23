#include <asm/io.h>
#include <miniOS/memory.h>


#define EFLAGS_AC_BIT           0x00040000
#define CR0_CACHE_DISABLE       0x60000000


uint32_t memtest(uint32_t start, uint32_t end)
{
    char flg486 = 0;
    uint32_t eflg,cr0,i;
    
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
