
#patsubst :ģʽ�ַ����滻���� $(patsubst <pattern>,<replacement>,<text>)   ���أ��������ر��滻������ַ���
#foreach : ��Ϊ���������������ѭ���õ�  $(foreach <var>,<list>,<text>)  
			#�����������˼�ǣ��Ѳ���<list>�еĵ�����һȡ���ŵ�����<var>��ָ���ı����У�
			#Ȼ����ִ��<text>�������ı��ʽ��ÿһ��<text>�᷵��һ���ַ�����ѭ�������У�<text>
			#�������ص�ÿ���ַ������Կո�ָ����������ѭ������ʱ��<text>�����ص�ÿ���ַ���
			#����ɵ������ַ������Կո�ָ��������� foreach �����ķ���ֵ��
#ͨ�����%��ֻ�����ڹ����У�ֻ���ڹ��������Ż�չ��������ڱ�������ͺ���ʹ��ʱ��ͨ��������Զ�չ�������ʱ���Ҫ�õ����� wildcard��ʹ�÷������£�
#$(wildcard PATTERN��)

#���磺
#$(wildcard *.c)
#����Ĵ�����������ȡ��ǰĿ¼�����е�.c �ļ������ơ�%����

#notdir :ȡ�ļ����� $(notdir <names...>) �������ļ�������<names>�ķ�Ŀ¼����

#-w����˼�ǹرձ���ʱ�ľ��棬Ҳ���Ǳ������ʾ�κ�warning����Ϊ��ʱ�ڱ���֮�����������ʾһЩ��������ת��֮��ľ��棬��Щ����������ƽʱ���Ժ��Եġ�
#-Wallѡ����˼�Ǳ������ʾ���о��档
#-Wѡ������-Wall������ʾ���棬����ֻ��ʾ��������Ϊ����ִ���ľ���

#-nostdlib���ã�������ϵͳ��׼�����ļ��ͱ�׼���ļ���ֻ��ָ�����ļ����ݸ������������ѡ����ڱ����ںˡ�bootloader�ȳ������ǲ���Ҫ�����ļ�����׼���ļ���
#C���Գ���ִ�еĵ�һ��ָ�������main����������һ��C����Ŀ�ִ���ļ�ʱ������ͨ���������ǵĴ����ϼ��ϼ�������Ϊ�����ļ��Ĵ�crt1.o,crti.o,crtend.o,crtn.o�ȣ�
#�����Ǳ�׼���ļ�����Щ��������C����Ķ�ջ�ȣ�Ȼ�����main���������������ڲ���ϵͳ����������޷�ִ�У����������Լ�дһ����
#���ԣ������Լ�д��*.S����ļ�����һ�������ļ��������úö�ջ�����main��������ˣ����ǲ���Ҫϵͳ�Դ��������ļ���

#	$@:�����е�Ŀ�꼯�ϣ���ģʽ�����У�����ж��Ŀ��Ļ�����$@����ʾƥ��ģʽ�ж����Ŀ�꼯�ϡ�
#	$<:����Ŀ���еĵ�һ��Ŀ�����֡��������Ŀ������ģʽ����"%"������ģ���ô"$<"���Ƿ���ģʽ��һϵ�е��ļ�����ע�⣬����һ��һ��ȡ�����ġ�
#	$^ :���������ļ��ļ��ϣ�ʹ�ÿո�ֿ�������������ļ����ж���ظ����ļ�����$^����ȥ���ظ��������ļ���ֵ����һ�ݡ�

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

VPATH				:= $(SRCDIRS)			/*VPATH ��ָ������Ŀ¼�ģ�����ָ��������Ŀ¼���Ǳ��� SRCDIRS �������Ŀ¼�������������ʱ�������.S ��.c �ļ��ͻ��� SRCDIRS ��ָ����Ŀ¼�в��ҡ�*/


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

						
