/*
 * vsprintf.c
 *
 * - 内核的标准库之字符串格式化输出
 */

#include <kernel.h>
#include <vsprintf.h>
#include <string.h>

#define	USE_MY_DIGITS_ARRAY		TRUE
#define	USE_SAFETY_OVERFLOW_CHECKS	TRUE
#define	USE_SIZE_TO_CHECK_OVERFLOW	TRUE
#define	PRINTF_BUF_MAX			1024

#if 	USE_SAFETY_OVERFLOW_CHECKS
#define	BUFFER_OVERFLOW	(unlikely(size-- == 0) ? !!(size++) : 0)	/* let me explain it later */
#else
#define	BUFFER_OVERFLOW	(0)	/* never */
#endif /* USE_SAFETY_OVERFLOW_CHECKS */

#ifdef	USE_MY_DIGITS_ARRAY
static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
static const char upper_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
#endif

int vsprintf_s(char *buf, size_t size, const char *fmt, va_list va)
{
	char c;
	char *const buf_orig = buf;
#if	!USE_SIZE_TO_CHECK_OVERFLOW
	char *const buf_end = buf + size;
#endif
	if (BUFFER_OVERFLOW)	/* reserve one char space for '\0' */
		goto out;
	while (c = *fmt++) {
		if (likely(c != '%')) {
			if (BUFFER_OVERFLOW)
				goto overflow;
			*buf++ = c;
			continue;
		}
		switch (*fmt++) {
			const char *str;
			unsigned ui;
			r_t r;
			size_t cnt;
			char *p;
		case 'd':
		case 'u':
			ui = va_arg(va, unsigned);
			do {
#if	USE_MY_DIGITS_ARRAY
				*buf++ = digits[ui % 10];
#else
				*buf++ = (ui % 10) + '0';
#endif
			} while ((ui /= 10) && !BUFFER_OVERFLOW);
			break;
		case 's':
			str = va_arg(va, const char *);
			assert(str);
			/*strcpy(buf, a_str);*/
			while ((c = *str++) && !BUFFER_OVERFLOW)
				*buf++ = c;
			break;
#if	USE_MY_DIGITS_ARRAY
		case 'x':
			r = va_arg(va, r_t);
			do {
				*buf++ = digits[r & 0xf];
			} while ((r >>= 4) && !BUFFER_OVERFLOW);
			break;
		case 'X':
			r = va_arg(va, r_t);
			do {
				*buf++ = upper_digits[r & 0xf];
			} while ((r >>= 4) && !BUFFER_OVERFLOW);
			break;
		case 'p':
			r = va_arg(va, r_t);
			cnt = sizeof(r) * 2;	/* each bytes in 'r' will convert into
							   2 characters of 'buf' */
#if 	DO_SAFETY_WORKS
			size -= cnt;
			if (size <= 0)	/* TODO: bugmaybe, crack-me */
				break;
#endif
			buf += cnt;
			p = buf;
			do {
				*--p = upper_digits[r & 0xf];
				r >>= 4;
			} while (--cnt);
			break;
#else	/* USE_MY_DIGITS_ARRAY */
		case 'x':
			r = va_arg(va, r_t);
			do {
				byte_t hb = r & 0xf;
				*buf++ = likely(hb < 10) ? hb + '0' : hb - 10 + 'a';
			} while ((r >>= 4) && !BUFFER_OVERFLOW);
			break;
		case 'X':
			r = va_arg(va, r_t);
			do {
				byte_t hb = r & 0xf;
				*buf++ = likely(hb < 10) ? hb + '0' : hb - 10 + 'A';
			} while ((r >>= 4) && !BUFFER_OVERFLOW);
			break;
		case 'p':
			r = va_arg(va, r_t);
			c = sizeof(r_t) * 2;
			do {	/* TODO */
				byte_t hb = r & 0xf;
				*buf++ = likely(hb < 10) ? hb + '0' : hb - 10 + 'A';
				r >>= 4;
			} while (--c && !BUFFER_OVERFLOW);
			break;
#endif	/* USE_MY_DIGITS_ARRAY */
		case 'c':
			c = va_arg(va, int);	/*
						 * yeah, we use 'int' here instead of 'char'.
						 * we know that sizeof(char) is 1,
						 * while stack aligned in 4 (or 8)
						 */
			if (BUFFER_OVERFLOW)
				goto out;
			*buf++ = c;
			break;
		case '%':
			*buf++ = '%';
			break;
		default:
			assert(0);
			break;
		}
	}
overflow:
	return -1;
out:	*buf = 0;
	return buf - buf_orig;	/* returns string length in 'buf' */
}

int vsprintf(char *buf, const char *fmt, va_list va)	/* this function is NOT safe */
{
	return vsprintf_s(buf, ~0, fmt, va);
}

int sprintf_s(char *buf, size_t size, const char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	int res = vsprintf_s(buf, size, fmt, va);
	va_end(va);
	return res;
}

int sprintf(char *buf, const char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	int res = vsprintf(buf, fmt, va);
	va_end(va);
	return res;
}

