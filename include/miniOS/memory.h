#include "stdint.h"


int load_cr0(void);
void store_cr0(int cr0);

uint32_t memtest(uint32_t start, uint32_t end);
uint32_t memtest_sub(uint32_t start, uint32_t end);

