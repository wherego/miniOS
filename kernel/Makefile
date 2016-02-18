#
# Makefile for kernel
#
#
TOOLPATH 	= ../z_tools/
INCPATH  	= ../include/


MAKE 		= $(TOOLPATH)make.exe -r
NASK 		= $(TOOLPATH)nask.exe
CC1      	= $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK 	= $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  	= $(TOOLPATH)obj2bim.exe
MAKEFONT 	= $(TOOLPATH)makefont.exe
BIN2OBJ  	= $(TOOLPATH)bin2obj.exe
BIM2HRB  	= $(TOOLPATH)bim2hrb.exe
RULEFILE 	= $(TOOLPATH)miniOS/miniOS.rul
EDIMG 		= $(TOOLPATH)edimg.exe
COPY		= copy
DEL			= del

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#一般规则
#keyboard.c keyboard.h
#base.c base.h
#dsctbl.c dsctbl.h
#int.c int.h
#
#io.nas	
#kernel.nas	
%.gas: %.c
	$(CC1) -o $*.gas $*.c
	
%.nas: %.gas
	$(GAS2NASK) $*.gas $*.nas	

%.obj: %.nas
	$(NASK) $*.nas $*.obj $*.lst	

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#普通规则
#font.txt
font.bin: font.txt
	$(MAKEFONT) font.txt font.bin
	
font.obj: font.bin
	$(BIN2OBJ) font.bin font.obj _font


	
clear:
	-$(DEL) *.bin
	-$(DEL) *.lst
	-$(DEL) *.obj
	-$(DEL) *.gas
	-$(DEL) dsctbl.nas
	-$(DEL) int.nas
	-$(DEL) base.nas
	-$(DEL) keyboard.nas
    