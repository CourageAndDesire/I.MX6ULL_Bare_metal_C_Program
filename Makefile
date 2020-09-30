
#patsubst :模式字符串替换函数 $(patsubst <pattern>,<replacement>,<text>)   返回：函数返回被替换过后的字符串
#foreach : 因为这个函数是用来做循环用的  $(foreach <var>,<list>,<text>)  
			#这个函数的意思是，把参数<list>中的单词逐一取出放到参数<var>所指定的变量中，
			#然后再执行<text>所包含的表达式。每一次<text>会返回一个字符串，循环过程中，<text>
			#的所返回的每个字符串会以空格分隔，最后当整个循环结束时，<text>所返回的每个字符串
			#所组成的整个字符串（以空格分隔）将会是 foreach 函数的返回值。
#通配符“%”只能用在规则中，只有在规则中它才会展开，如果在变量定义和函数使用时，通配符不会自动展开，这个时候就要用到函数 wildcard，使用方法如下：
#$(wildcard PATTERN…)

#比如：
#$(wildcard *.c)
#上面的代码是用来获取当前目录下所有的.c 文件，类似“%”。

#notdir :取文件函数 $(notdir <names...>) ，返回文件名序列<names>的非目录部分

#-w的意思是关闭编译时的警告，也就是编译后不显示任何warning，因为有时在编译之后编译器会显示一些例如数据转换之类的警告，这些警告是我们平时可以忽略的。
#-Wall选项意思是编译后显示所有警告。
#-W选项类似-Wall，会显示警告，但是只显示编译器认为会出现错误的警告

#-nostdlib作用：不连接系统标准启动文件和标准库文件，只把指定的文件传递给连接器。这个选项常用于编译内核、bootloader等程序，它们不需要启动文件、标准库文件。
#C语言程序执行的第一条指令。并不是main函数。生成一个C程序的可执行文件时编译器通常会在我们的代码上加上几个被称为启动文件的代crt1.o,crti.o,crtend.o,crtn.o等，
#他们是标准库文件。这些代码设置C程序的堆栈等，然后调用main函数。他们依赖于操作系统，在裸板上无法执行，所以我们自己写一个。
#所以，我们自己写的*.S汇编文件就是一个启动文件，它设置好堆栈后调用main函数。因此，我们不需要系统自带的启动文件。

#	$@:规则中的目标集合，在模式规则中，如果有多个目标的话，“$@”表示匹配模式中定义的目标集合。
#	$<:依赖目标中的第一个目标名字。如果依赖目标是以模式（即"%"）定义的，那么"$<"将是符合模式的一系列的文件集。注意，其是一个一个取出来的。
#	$^ :所有依赖文件的集合，使用空格分开，如果在依赖文件中有多个重复的文件，“$^”会去除重复的依赖文件，值保留一份。

CROSS_COMPILE		?=	arm-linux-gnueabihf-
TARGET				?=	LED

CC					:=	$(CROSS_COMPILE)gcc
LD					:=	$(CROSS_COMPILE)ld
OBJCOPY				:=	$(CROSS_COMPILE)objcopy
OBJDUMP				:=	$(CROSS_COMPILE)objdump

SRCDIRS				:=	src    		\
						src/led    	\
						src/clock		\
						stdio/lib		\

INCDIRS				:=	inc		\
						stdio/include\

INCLUDE				:=	$(patsubst %, -I %, $(INCDIRS))


 #@echo $(wildcard  $(dir)/*.c)

SFILES				:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.s))

CFILES				:=	$(foreach dir, $(SRCDIRS),	$(wildcard $(dir)/*.c))


SFILENDIR			:=	$(notdir $(SFILES))
CFILENDIR			:=	$(notdir $(CFILES))

SOBJS				:=	$(patsubst %, obj/%, $(SFILENDIR:.s=.o))
COBJS				:=	$(patsubst %, obj/%, $(CFILENDIR:.c=.o))
OBJS				:= $(SOBJS)	$(COBJS)

VPATH				:= $(SRCDIRS)			/*VPATH 是指定搜索目录的，这里指定的搜素目录就是变量 SRCDIRS 所保存的目录，这样当编译的时候所需的.S 和.c 文件就会在 SRCDIRS 中指定的目录中查找。*/


.PHONY: clean

	

$(TARGET).bin:$(OBJS)
	$(LD) 		-Timx6ul.lds	-o	$(TARGET).elf	 $^
	$(OBJCOPY)	-O 	binary	-S 	$(TARGET).elf   $@
	$(OBJDUMP)	-D -m arm  $(TARGET).elf >	$(TARGET).dis

$(SOBJS):obj/%.o:%.s
	$(CC) -Wall -nostdlib -c -O2 $(INCLUDE) 	-o $@	$<

$(COBJS):obj/%.o:%.c
	$(CC) -Wall -nostdlib -c  -O2  $(INCLUDE) -o   $@  $<
	

clean:
	rm -rf	$(TARGET).elf	$(TARGET).dis  $(TARGET).bin $(COBJS) $(SOBJS)

						
