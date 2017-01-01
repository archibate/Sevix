#ifndef	_STRING_H_INCLUDED
#define	_STRING_H_INCLUDED

void *memcpy(void *dst, const void *src, size_t size);
void *memset(void *buf, int val, size_t size);
char *strcpy(char *dst, const char *src);
size_t __pure strlen(const char *str);
int __pure strcmp(const char *src, const char *dst);
char *strncpy(char *dst, const char *src, size_t max);
size_t __pure strnlen(const char *str, size_t max);
int __pure strncmp(const char *src, const char *dst, size_t max);

#endif

