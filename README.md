# miniOS
> windows、linux、unix、macOS，移动端ios、Android，以及嵌入式实时操作系统这些年来都逐渐变得成熟功能强大，但是作为一个计算机专业的学生，自己动手写一个简单的操作系统还是比较有必要的。<br>
> miniOS是一个完全从零开始开发的操作系统，开发过程中将参考Linux、Windows、uC/OS等操作系统。miniOS使用汇编和C开发，为了提高可移植性尽量使用C语言进行开发。开发该操作系统的主要有以下几个目的：<br>
* 学习并熟练使用汇编和C语言;<br>
* 学习计算机组成原理以及使用部分数据结构;<br>
* 深入理解计算机操作系统以及相关知识。<br>
 

#特性
* GUI
* 多任务
* ...

#目录结构
    miniOS
    |-- boot
    |   |-- bootsector.nas
    |   `-- head.nas
    |-- fs
    |-- include
    |   |-- asm
    |   |   `-- io.h
    |   |-- miniOS
    |   |   |-- base.h
    |   |   |-- dsctbl.h
    |   |   |-- int.h
    |   |   |-- keyboard.h
    |   |   `-- window.h
    |   |-- sys
    |   |-- miniOS.rul
    |   |-- stdio.h
    |   |-- stdint.h
    |   `-- stdarg.h
    |-- init
    |   `-- main.c
    |-- kernel
    |   |-- io.nas
    |   |-- asm.nas
    |   |-- font.txt
    |   |-- keyboard.c
    |   |-- base.c
    |   |-- int.c
    |   |-- dsctbl.c
    |   `-- Makefile
    |-- lib
    |   |-- golibc.lib
    |   `-- harilibc.lib
    |-- GUI
    |   |-- mouse.c
    |   |-- window.c
    |   `-- Makefile
    |-- z_tools
    |-- Makefile
    |-- GPL
    `-- LGPL

#下载和使用
目前miniOS的的开发平台是windows平台，没有使用集成开发环境，大部分开发工具来源于《30自制操作系统》，均改自一些开源软件，可以根据需求和开源协议使用，另外自己只需要提供一个代码编辑器即可，（如果没有合适的，推荐notepad++、visual studio code）。<br>
下载后，不需要安装，找到根目录下的`make.bat`文件，双击运行，在cmd下控制运行，常用的指令为：<br>

* `'make'`，编译操作系统源代码并生成镜像文件`miniOS.img`，镜像文件放在`miniOS`文件夹的根目录下，
可以使用`USBWriter.exe`软件(在`/z_tools/`目录下)将镜像写入U盘，并在实体机上运行。<br>
* `'make run'`，编译生成镜像文件，并且在'qemu.exe'虚拟机上运行，该软件在`/z_tools/qemu/`目录下。
* `'make clear'`,清理编译连接过程中的中间文件，最终生成的镜像文件也会删除。
 
![miniOS_GUI](https://github.com/ffiirree/miniOS/blob/master/miniOS_GUI.png) 

#更新日志
* 2016-02-11 GDT/IDT 初始化完成
* 2016-02-15 PIC初始化
* 2016-02-17 键盘和鼠标中断消息处理
