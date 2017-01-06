#ifndef	_VSPRINTF_H_INCLUDED
#define	_VSPRINTF_H_INCLUDED

extern int vsprintf_s(char *buf, size_t size, const char *fmt, va_list va);
extern int vsprintf(char *buf, const char *fmt, va_list va);
extern int sprintf_s(char *buf, size_t size, const char *fmt, ...);
extern int sprintf(char *buf, const char *fmt, ...);

#endif

