#
# Makefile for window
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

OBJS = window.obj mouse.obj

default:
	$(MAKE) window.obj	
	$(MAKE) mouse.obj	
	
	
	
################################################################
#window.c
window.gas: window.c 
	$(CC1) -o window.gas window.c

window.nas: window.gas
	$(GAS2NASK) window.gas window.nas
	
window.obj: window.nas
	$(NASK) window.nas window.obj window.lst
	
################################################################
#mouse.c
mouse.gas: mouse.c
	$(CC1) -o mouse.gas mouse.c
	
mouse.nas: mouse.gas
	$(GAS2NASK) mouse.gas mouse.nas
	
mouse.obj: mouse.nas
	$(NASK) mouse.nas mouse.obj mouse.lst
	
clear:
	$(DEL) *.lst
	$(DEL) *.obj
	$(DEL) *.gas
	$(DEL) *.nas