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


#define _bnd(X, bnd)    (((sizeof (X)) + (bnd)) & (~(bnd)))								//��sizeof(X)��sizeof(int)����

#define va_end(ap) ( ap = (va_list)0 ) 


/*����������������飬
�����û�������������Բ�����ַ����ǿ������ת�����õ��û�����Ҫ��ֵ
�ڼ������������ʵ�ʴ�С����ָ������������Ľ�β��Ҳ������һ���������׵�ַ���Ա��������*/
/*x86ƽ̨����Ϊap=(char*)0;ʹap���� ָ���ջ,���Ǹ�NULLһ��.��Щֱ�Ӷ���Ϊ((void*)0),��������������Ϊva_end��������,
����gcc��linux��x86ƽ̨�������������. ��������Ҫע��һ������:���ڲ����ĵ�ַ����va_start��,���Բ�����������Ϊ�Ĵ�����������Ϊ��������������. */
#define va_arg(ap, T)   (*(T *)(((ap) += (_bnd (T, _AUPBND))) - (_bnd (T,_ADNBND))))
#define va_end(ap)      (void) 0


/*va_start�Ķ���Ϊ &A+_INTSIZEOF(A) ,����&A�����һ���̶���������ʼ��ַ���ټ�����ʵ��ռ�ô�С�󣬾͵õ��˵�һ���ɱ��������ʼ�ڴ��ַ��
������������va_start(ap, v)�Ժ�,apָ���һ���ɱ�����ڵ��ڴ��ַ*/
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
