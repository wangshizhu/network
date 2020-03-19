#ifndef STRING_UTIL_H_
#define STRING_UTIL_H_

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#pragma warning(disable:4996)
#define kbe_isnan _isnan
#define kbe_isinf(x) (!_finite(x) && !_isnan(x))
#define kbe_snprintf _snprintf
#define kbe_vsnprintf _vsnprintf
#define kbe_vsnwprintf _vsnwprintf
#define kbe_snwprintf _snwprintf
#define kbe_stricmp _stricmp
#define kbe_strnicmp _strnicmp
#define kbe_fileno _fileno
#define kbe_va_copy( dst, src) dst = src

#define strtoq   _strtoi64
#define strtouq  _strtoui64
#define strtoll  _strtoi64
#define strtoull _strtoui64
#define atoll    _atoi64

#else

#define kbe_isnan isnan
#define kbe_isinf isinf
#define kbe_snprintf snprintf
#define kbe_vsnprintf vsnprintf
#define kbe_vsnwprintf vsnwprintf
#define kbe_snwprintf swprintf
#define kbe_stricmp strcasecmp
#define kbe_strnicmp strncasecmp
#define kbe_fileno fileno
#define kbe_va_copy va_copy

#endif

inline wchar_t* char2wchar(const char* cs, size_t* outlen)
{
	int len = (int)((strlen(cs) + 1) * sizeof(wchar_t));
	wchar_t* ccattr = (wchar_t *)malloc(len);
	memset(ccattr, 0, len);

	size_t slen = mbstowcs(ccattr, cs, len);

	if (outlen)
	{
		if ((size_t)-1 != slen)
			*outlen = slen;
		else
			*outlen = 0;
	}

	return ccattr;
};


#endif // !STRING_UTIL_H_
