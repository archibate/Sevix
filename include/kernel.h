#ifndef	_KERNEL_H_INCLUDED
#define	_KERNEL_H_INCLUDED

#ifndef	_KERNEL	/* FOR VIM SYNCHECK */
#ifndef	_BITS
#define	_BITS	32
#endif
#ifndef	_SOURCE_EXTNAME_C
#define	_SOURCE_EXTNAME_C
#endif
#endif

#undef	__CLANG__
#undef	__ASSEMBLER__
#if	defined _SOURCE_EXTNAME_C
#define	__LANGN__	1
#define	__CLANG__	1
#elif	defined _SOURCE_EXTNAME_S
#define	__LANGN__	3
#define	__ASSEMBLER__	1
#elif	defined _SOURCE_EXTNAME_CXX
#define	__LANGN__	2
#define	__CPLUSPLUS__	1
#define	__CLANG__	2
#elif	defined _SOURCE_EXTNAME_LD
#elif	defined _SOURCE_EXTNAME_UNKNOWN
#define	__LANGN__	0
#else
#error	"configuational macro _SOURCE_EXTNAME_* is invalid or not given"
#endif	/* _SOURCE_EXTNAME_* */

#define	EXPAND(expr)	expr
#define	CSYMSTOSTR(s)	#s

#define	TRUE	1
#define	FALSE	0

#ifdef	__CLANG__
#define	__CHASIT(x)	x
#else	/* __CLANG__ */
#define	__CHASIT(x)
#endif	/* __CLANG__ */
#define	__CTYPE_CVNT(type)	__CHASIT((type))
#define	__R_T_IMM(imm)	(__CTYPE_CVNT(r_t) (imm))

#ifdef	__CLANG__

#ifndef	NULL
#ifndef	__cplusplus
#define	NULL	((void *) 0)
#else
#define	NULL	(0)	/* C++ does not need to cast zero constant to pointer */
#endif	/* __cplusplus */
#endif	/* NULL */
#ifndef	EOF
#define	EOF	(-1)
#endif	/* EOF */

#ifndef	_CKEYWORDS_EXTENSION_DEFINED
#define	_CKEYWORDS_EXTENSION_DEFINED
#define	DO	do
#define	LOOP	while (TRUE)
#define	DONE	while (FALSE)
#define	EXITDO	break
#define	OR	or
#define	AND	and
#define	XOR	xor
#define	NOT	not
#endif	/* _CKEYWORDS_EXTENSION_DEFINED */

#ifndef	_USEFUL_MACROS_DEFINED
#define	_USEFUL_MACROS_DEFINED
#ifndef	_PEDANTIC
#define	MINIMUM(a, b) ({ \
		typeof(a) __a = (a); \
		typeof(b) __b = (b); \
		(void) (__a == __b); \
		__a < __b ? __a : __b; \
	})
#define	MAXIMUM(a, b) ({ \
		typeof(a) __a = (a); \
		typeof(b) __b = (b); \
		(void) (__a == __b); \
		__a > __b ? __a : __b; \
	})
#else	/* _PEDANTIC */
#define	MINIMUM(a, b) \
	((a) < (b) ? (a) : (b))
#define	MAXIMUM(a, b) \
	((a) > (b) ? (a) : (b))
#endif	/* _PEDANTIC */
#define	SIZEOF(x) \
	(sizeof(x))
#define	TYPEOF(x) \
	typeof(x)
#define	OFFSET_OF(type, member) \
	((size_t) ((type *) 0)->member)
#define	COUNTOF(a) \
	(sizeof((a)) / sizeof((a)[0]))
#endif	/* _USEFUL_MACROS_DEFINED */

#define	__attribute(x)	__attribute__((x))
#define	__volatile	__attribute(volatile)
#define	__inline	__attribute(inline)
#define	__const		__attribute(const)
#define	__pure		__attribute(pure)
#define	__static	__attribute(static)
#define	__noreturn	__attribute(noreturn)
#define	__noinline	__attribute(noinline)
#define	__packed	__attribute(packed)
#define	__aligned(nr)	__attribute(aligned(nr))
#define	__section(sect)	__attribute(section(sect))
#define	__asm		__attribute(asm)
#if	__GNUC__ == 2 && __GNUC_MINOR__ < 96	/* GCC version lesser than 2.86: NO built-in */
#warning "GCC version lesser than 2.86: NO built-in supported"
#define	__builtin_expect(x, expected)	(x)
#define	__builtin_return_address(ndx)	(((void (**)(void)) asm("fp"))[1])
#define	__builtin_va_list		char *
#define	__builtin_va_start(va, arg)	((void) ((va) = &(arg)))
#define	__builtin_va_arg(va, type)	(* (type *) ((va) += sizeof(type)))
#define	__builtin_va_end(va)		((void) ((va) = NULL))
#endif	/* __GNUC__ == 2 && __GNUC_MINOR__ < 96 */

#ifndef	VA_LIST_DEFINED
#define	VA_LIST_DEFINED
#define	va_list		__builtin_va_list
#define	va_start	__builtin_va_start
#define	va_arg		__builtin_va_arg
#define	va_end		__builtin_va_end
#endif	/* VA_LIST_DEFINED */
#define	likely(x)	(__builtin_expect(!!(x), 1))	/* !!(x) converts 'int' to 'bool': 0 or 1 */
#define	unlikely(x)	(__builtin_expect(!!(x), 0))

#ifdef	_DEBUG
extern void __noreturn __debug_assertion_panic(const char *expr,
		const char *file, unsigned int line,
		const char *info);
#define	__assert(x, info) ({ \
		if (unlikely(!(x))) \
			__debug_assertion_panic(#x, \
				__FILE__, __LINE__, \
				info); \
	})
#else
#define	__assert(x, info) 	/* Nothing */
#endif	/* _DEBUG */
#define	assert(x)		__assert(x, "Giving up")
#define	assert_info(x, info)	__assert(x, info)

#define	__u8	unsigned char
#define	__u16	unsigned short
#define	__u32	unsigned long
#define	__u64	unsigned long long
#if	_BITS == 32
#define	__r_t	__u32
#elif	_BITS == 64
#define	__r_t	__u64
#else
#error	"macro constant _BITS invalid"
#endif	/* _BITS */

typedef __u8 u8;
typedef __u16 u16;
typedef __u32 u32;
typedef __u64 u64;
typedef unsigned char byte_t;
typedef unsigned short half_t;
typedef unsigned long word_t;
typedef unsigned long long gaint_t;
typedef __r_t size_t;
typedef size_t r_t;
typedef r_t addr_t;
typedef int bool_t;
typedef float math_float_t;

#endif	/* __CLANG__ */

#endif

