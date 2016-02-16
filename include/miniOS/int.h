#ifndef __INT_H
#define __INT_H


#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

void pic_init(void);
void inthandler21(int *esp);
void inthandler2c(int *esp);

//kernel.nas
void asm_inthandler21(void);
void asm_inthandler2c(void);

#endif //!__INT_H

