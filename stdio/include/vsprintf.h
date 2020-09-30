/*
 * Use local definitions of C library macros and functions
 * NOTE: The function implementations may not be as efficient
 * as an inline or assembly code implementation provided by a
 * native C library.
 */

#include "types.h"

#ifndef va_arg

#ifndef _VALIST
#define _VALIST
typedef char *va_list;
#endif /* _VALIST */

/*
 * Storage alignment properties
 */
#define	 NATIVE_INT		 int
#define  _AUPBND         (sizeof (NATIVE_INT) - 1)
#define  _ADNBND         (sizeof (NATIVE_INT) - 1)

/*
 * Variable argument list macro definitions
 */


#define _bnd(X, bnd)    (((sizeof (X)) + (bnd)) & (~(bnd)))								//将sizeof(X)按sizeof(int)对齐

#define va_end(ap) ( ap = (va_list)0 ) 


/*这个宏做了两个事情，
①用用户输入的类型名对参数地址进行强制类型转换，得到用户所需要的值
②计算出本参数的实际大小，将指针调到本参数的结尾，也就是下一个参数的首地址，以便后续处理。*/
/*x86平台定义为ap=(char*)0;使ap不再 指向堆栈,而是跟NULL一样.有些直接定义为((void*)0),这样编译器不会为va_end产生代码,
例如gcc在linux的x86平台就是这样定义的. 在这里大家要注意一个问题:由于参数的地址用于va_start宏,所以参数不能声明为寄存器变量或作为函数或数组类型. */
#define va_arg(ap, T)   (*(T *)(((ap) += (_bnd (T, _AUPBND))) - (_bnd (T,_ADNBND))))
#define va_end(ap)      (void) 0


/*va_start的定义为 &A+_INTSIZEOF(A) ,这里&A是最后一个固定参数的起始地址，再加上其实际占用大小后，就得到了第一个可变参数的起始内存地址。
所以我们运行va_start(ap, v)以后,ap指向第一个可变参数在的内存地址*/
#define va_start(ap, A) (void) ((ap) = (((char *) &(A)) + (_bnd (A,_AUPBND))))

#endif /* va_arg */

unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base);
long simple_strtol(const char *cp,char **endp,unsigned int base);
unsigned long long simple_strtoull(const char *cp,char **endp,unsigned int base);
long long simple_strtoll(const char *cp,char **endp,unsigned int base);
//static int  skip_atoi(const char **s);
//char * number(char * buf, char * end, long long num, int base, int size, int precision, int type);
int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);
int snprintf(char * buf, size_t size, const char *fmt, ...);
int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char * buf, const char *fmt, ...);
int vsscanf(const char * buf, const char * fmt, va_list args);
int sscanf(const char * buf, const char * fmt, ...);
