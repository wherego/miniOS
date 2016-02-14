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


LD = $(TOOLPATH)ld.exe

OBJS = sys_io.obj font.obj dsctbl.obj int.obj


default:
	$(MAKE) sys_io.obj		
	$(MAKE) font.obj
	$(MAKE) dsctbl.obj
	$(MAKE) int.obj

################################################################
#sys_io.nas	
sys_io.obj: sys_io.nas
	$(NASK) sys_io.nas sys_io.obj sys_io.lst
	
	
	
################################################################
#font.txt
font.bin: font.txt
	$(MAKEFONT) font.txt font.bin
	
font.obj: font.bin
	$(BIN2OBJ) font.bin font.obj _font
	
	
################################################################
#dsctbl.c dsctbl.h
dsctbl.gas: dsctbl.c
	$(CC1) -o dsctbl.gas dsctbl.c

dsctbl.nas: dsctbl.gas 
	$(GAS2NASK) dsctbl.gas dsctbl.nas

dsctbl.obj: dsctbl.nas
	$(NASK) dsctbl.nas dsctbl.obj dsctbl.lst

################################################################
#int.c int.h
int.gas: int.c 
	$(CC1) -o int.gas int.c

int.nas: int.gas
	$(GAS2NASK) int.gas int.nas

int.obj: int.nas
	$(NASK) int.nas int.obj int.lst
	
	
clear:
	-$(DEL) *.bin
	-$(DEL) *.lst
	-$(DEL) *.obj
	-$(DEL) *.gas
	-$(DEL) dsctbl.nas
	-$(DEL) int.nas