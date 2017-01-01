/*
 * string.c
 *
 * - 内核的标准库之字符串处理
 */

#include <kernel.h>
#include <string.h>

void *memcpy(void *dst, const void *src, size_t size)
{
	assert(dst);
	assert(src);
	byte_t *di = dst;
	const byte_t *si = src;
	while (--size)
		*di++ = *si++;
	return dst;
}

void *memset(void *buf, int val, size_t size)
{
	assert(buf);
	byte_t *di = buf;
	while (--size)
		*di++ = val;
	return buf;
}

char *strcpy(char *dst, const char *src)
{
	assert(dst);
	assert(src);
	char *dst_orig = dst;
	while (*dst++ = *src++);
	return dst_orig;
}

size_t strlen(const char *str)
{
	assert(str);
	const char *str_orig = str;
	while (*str++);
	return str - str_orig - 1;
}

int strcmp(const char *src, const char *dst)
{
	assert(dst);
	assert(src);
	int res = 0;
	const unsigned char *si = (typeof(si)) src;
	const unsigned char *di = (typeof(di)) dst;
	while (!(res = *si - *di++) && *si++);
	return res;
}

char *strncpy(char *dst, const char *src, size_t max)
{
	assert(dst);
	assert(src);
	char *dst_orig = dst;
	while (*dst++ = *src++ && --max);
	return dst_orig;
}

size_t strnlen(const char *str, size_t max)
{
	assert(str);
	const char *str_orig = str;
	while (*str++ && --max);
	return str - str_orig - 1;
}

int strncmp(const char *src, const char *dst, size_t max)
{
	assert(dst);
	assert(src);
	int res = 0;
	const unsigned char *si = (typeof(si)) src;
	const unsigned char *di = (typeof(di)) dst;
	while (!(res = *si - *di++) && *si++ && --max);
	return res;
}
