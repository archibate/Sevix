#ifndef	_STRING_H_INCLUDED
#define	_STRING_H_INCLUDED

extern void *memcpy(void *dst, const void *src, size_t size);
extern void *memset(void *buf, int val, size_t size);
extern char *strcpy(char *dst, const char *src);
extern size_t __pure strlen(const char *str);
extern int __pure strcmp(const char *src, const char *dst);
extern char *strncpy(char *dst, const char *src, size_t max);
extern size_t __pure strnlen(const char *str, size_t max);
extern int __pure strncmp(const char *src, const char *dst, size_t max);

#endif

