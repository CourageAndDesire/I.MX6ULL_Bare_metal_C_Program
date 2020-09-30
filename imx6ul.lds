SECTIONS{
	. = 0x87800000;			/*定义链接的起始地址*/
	.text :
	{
		obj/start.o				/*启动文件放在最前面*/			
		*(.text)
	}
	.data ALIGN(4) : {*(.data)}
	.rodata ALIGN(4) : {*(.rodata)}
	__bss_start = .;
	.bss ALIGN(4) : {*(.bss) *(COMMON)}			/* 将所有输入文件的所有通用符号放入输出.bss section内*/
	__bss_end = .;
}
