TOOLPATH 	= z_tools/
INCPATH  	= include/


MAKE 		= $(TOOLPATH)make.exe -r
NASK 		= $(TOOLPATH)nask.exe
CC1      	= $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK 	= $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  	= $(TOOLPATH)obj2bim.exe
MAKEFONT 	= $(TOOLPATH)makefont.exe
BIN2OBJ  	= $(TOOLPATH)bin2obj.exe
BIM2HRB  	= $(TOOLPATH)bim2hrb.exe
RULEFILE 	= $(INCPATH)miniOS.rul
EDIMG 		= $(TOOLPATH)edimg.exe
COPY		= copy
DEL			= del

#$(MAKE)和img之间注意要有空格
default:
	$(MAKE) img			 
	
#################################################################
#boot文件夹下的文件
boot/bootsector.bin: boot/bootsector.nas
	$(NASK) boot/bootsector.nas boot/bootsector.bin boot/bootsector.lst

boot/head.bin: boot/head.nas 
	$(NASK) boot/head.nas boot/head.bin boot/head.lst

#################################################################
#init/main.c
init/main.gas: init/main.c 
	$(CC1) -o init/main.gas init/main.c 

init/main.nas: init/main.gas
	$(GAS2NASK) init/main.gas init/main.nas

init/main.obj: init/main.nas 
	$(NASK) init/main.nas init/main.obj init/main.lst

#################################################################
#kernel
kernel/io.obj:
	$(MAKE) io.obj -C kernel
	
kernel/asm.obj:
	$(MAKE) asm.obj -C kernel
    
kernel/font.obj:
	$(MAKE) font.obj -C kernel
	
kernel/keyboard.obj:
	$(MAKE) keyboard.obj -C kernel
	
kernel/base.obj:
	$(MAKE)	base.obj -C kernel
	
kernel/dsctbl.obj:
	$(MAKE) dsctbl.obj -C kernel
	
kernel/int.obj:
	$(MAKE) int.obj -C kernel
	
#################################################################
#mm memory.c
mm/memory.gas: mm/memory.c
	$(CC1) -o mm/memory.gas mm/memory.c
	
mm/memory.nas: mm/memory.gas
	$(GAS2NASK) mm/memory.gas mm/memory.nas

mm/memory.obj: mm/memory.nas
	$(NASK) mm/memory.nas mm/memory.obj mm/memory.lst
	
mm/page.obj: mm/page.nas
	$(NASK) mm/page.nas mm/page.obj mm/page.lst
	
#################################################################
#window
GUI/window.obj:	
	$(MAKE) window.obj -C GUI

GUI/mouse.obj:	
	$(MAKE) mouse.obj -C GUI  

################################################################
init/main.bim: init/main.obj \
		kernel/io.obj kernel/asm.obj kernel/font.obj kernel/keyboard.obj \
		kernel/base.obj kernel/dsctbl.obj kernel/int.obj\
		mm/memory.obj mm/page.obj \
		GUI/window.obj GUI/mouse.obj     
	$(OBJ2BIM) @$(RULEFILE) out:init/main.bim stack:3136k map:init/main.map \
		init/main.obj \
		kernel/io.obj kernel/asm.obj kernel/font.obj kernel/keyboard.obj \
		kernel/base.obj kernel/dsctbl.obj kernel/int.obj \
		mm/memory.obj mm/page.obj \
		GUI/window.obj GUI/mouse.obj     
# 3MB+64KB=3136KB

init/main.hrb: init/main.bim
	$(BIM2HRB) init/main.bim init/main.hrb 0

miniOS.sys: boot/head.bin init/main.hrb
	copy /B boot\head.bin+init\main.hrb miniOS.sys
	
miniOS.img: boot/bootsector.bin miniOS.sys
	$(EDIMG) imgin:z_tools/fdimg0at.tek \
		wbinimg src:boot/bootsector.bin len:512 from:0 to:0\
		copy from:miniOS.sys to:@: \
		imgout:miniOS.img
		
		

#操作
img:
	$(MAKE) miniOS.img
	

run:
	$(MAKE) img
	$(COPY) miniOS.img z_tools\qemu\fdimage0.bin
	$(MAKE) -C z_tools/qemu

	
clear:
	$(DEL) miniOS.sys
	$(DEL) miniOS.img
	$(DEL) z_tools\qemu\fdimage0.bin
	$(DEL) boot\*.lst
	$(DEL) boot\*.bin
	$(DEL) mm\*.obj
	$(DEL) mm\*.lst
	$(DEL) mm\memory.gas
	$(DEL) mm\memory.nas
	$(DEL) mm\page.gas
	$(DEL) init\main.bim
	$(DEL) init\main.gas
	$(DEL) init\main.lst
	$(DEL) init\main.map
	$(DEL) init\main.nas
	$(DEL) init\main.obj
	$(DEL) init\main.hrb
	$(MAKE) clear -C kernel
	$(MAKE) clear -C GUI