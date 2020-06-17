#ifndef PLATFORM_H_
#define PLATFORM_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <cstring> 
#include <map>
#include "string_util.h"
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#pragma warning(disable:4996)
#ifndef WIN32_LEAN_AND_MEAN 
#include <winsock2.h>		// 必须在windows.h之前包含， 否则网络模块编译会出错
#include <mswsock.h> 
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h> 
#include <unordered_map>
#include <functional>
#include <memory>
#include <Ws2tcpip.h>
#define _SCL_SECURE_NO_WARNINGS
#else
// linux include
#include <poll.h>
#include <errno.h>
#include <float.h>
#include <pthread.h>	
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <iconv.h>
#include <langinfo.h>   /* CODESET */
#include <stdint.h>
#include <signal.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <tr1/unordered_map>
#include <tr1/functional>
#include <tr1/memory>
#include <linux/types.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/resource.h> 
#include <linux/errqueue.h>
#endif



namespace General
{
/** 定义引擎字节序 */
#define GENERAL_LITTLE_ENDIAN							0
#define GENERAL_BIG_ENDIAN								1
#if !defined(GENERAL_ENDIAN)
#  if defined (USE_BIG_ENDIAN)
#    define GENERAL_ENDIAN GENERAL_BIG_ENDIAN
#  else 
#    define GENERAL_ENDIAN GENERAL_LITTLE_ENDIAN
#  endif 
#endif

// current platform and compiler
#define PLATFORM_WIN32			0
#define PLATFORM_UNIX			1
#define UNIX_FLAVOUR_LINUX		1

#define    LISTENQ        1024

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#define GENERAL_PLATFORM PLATFORM_WIN32
#define GENERAL_SNPRINTF _snprintf
#else
#define GENERAL_PLATFORM PLATFORM_UNIX
#define GENERAL_SNPRINTF snprintf
#endif
}

#define COMPILER_MICROSOFT 0
#define COMPILER_GNU	   1

#ifdef _MSC_VER
#  define GENERAL_COMPILER COMPILER_MICROSOFT
#elif defined( __GNUC__ )
#  define GENERAL_COMPILER COMPILER_GNU
#else
#  pragma error "FATAL ERROR: Unknown compiler."
#endif

#ifndef X64
#if defined( _WIN64 ) || defined( __x86_64__ ) || defined( __amd64 ) || defined( __LP64__ )
#define X64
#endif
#endif

#ifdef X64
#define GENERAL_ARCH "X64"
#else
#define GENERAL_ARCH "X86"
#endif

#ifndef TCHAR
#ifdef _UNICODE
typedef wchar_t												TCHAR;
#else
typedef char												TCHAR;
#endif
#endif

typedef unsigned char											uchar;
typedef unsigned short											ushort;
typedef unsigned int											uint;
typedef unsigned long											ulong;

#if GENERAL_COMPILER != COMPILER_GNU
typedef signed __int64											int64;
typedef signed __int32											int32;
typedef signed __int16											int16;
typedef signed __int8											int8;
typedef unsigned __int64										uint64;
typedef unsigned __int32										uint32;
typedef unsigned __int16										uint16;
typedef unsigned __int8											uint8;
typedef INT_PTR													intptr;
typedef UINT_PTR        										uintptr;
#define PRI64													"lld"
#define PRIu64													"llu"
#define PRIx64													"llx"
#define PRIX64													"llX"
#define PRIzu													"lu"
#define PRIzd													"ld"
#define PRTime													PRI64
#else
typedef int64_t													int64;
typedef int32_t													int32;
typedef int16_t													int16;
typedef int8_t													int8;
typedef uint64_t												uint64;
typedef uint32_t												uint32;
typedef uint16_t												uint16;
typedef uint8_t													uint8;
typedef uint16_t												WORD;
typedef uint32_t												DWORD;
#endif

#define PRAppID													PRIu64
#define PRDBID													PRIu64

#if GENERAL_PLATFORM == PLATFORM_WIN32
#define IFNAMSIZ											16
typedef UINT_PTR											GENERALSOCKET;
#ifndef socklen_t
typedef	int													socklen_t;
#endif
typedef unsigned short										u_int16_t;
typedef unsigned long										u_int32_t;

#ifndef IFF_UP
enum
{
	IFF_UP = 0x1,
	IFF_BROADCAST = 0x2,
	IFF_DEBUG = 0x4,
	IFF_LOOPBACK = 0x8,
	IFF_POINTOPOINT = 0x10,
	IFF_NOTRAILERS = 0x20,
	IFF_RUNNING = 0x40,
	IFF_NOARP = 0x80,
	IFF_PROMISC = 0x100,
	IFF_MULTICAST = 0x1000
};
#endif
#else
typedef int													GENERALSOCKET;
#endif

/*---------------------------------------------------------------------------------
定会多种平台上的多线程相关
---------------------------------------------------------------------------------*/
#if GENERAL_PLATFORM == PLATFORM_WIN32
#define THREAD_ID											HANDLE
#define THREAD_SINGNAL										HANDLE
#define THREAD_SINGNAL_INIT(x)								x = CreateEvent(NULL, TRUE, FALSE, NULL)
#define THREAD_SINGNAL_DELETE(x)							CloseHandle(x)
#define THREAD_SINGNAL_SET(x)								SetEvent(x)
#define THREAD_MUTEX										CRITICAL_SECTION
#define THREAD_MUTEX_INIT(x)								InitializeCriticalSection(&x)
#define THREAD_MUTEX_DELETE(x)								DeleteCriticalSection(&x)
#define THREAD_MUTEX_LOCK(x)								EnterCriticalSection(&x)
#define THREAD_MUTEX_UNLOCK(x)								LeaveCriticalSection(&x)	
#else
#define THREAD_ID											pthread_t
#define THREAD_SINGNAL										pthread_cond_t
#define THREAD_SINGNAL_INIT(x)								pthread_cond_init(&x, NULL)
#define THREAD_SINGNAL_DELETE(x)							pthread_cond_destroy(&x)
#define THREAD_SINGNAL_SET(x)								pthread_cond_signal(&x);
#define THREAD_MUTEX										pthread_mutex_t
#define THREAD_MUTEX_INIT(x)								pthread_mutex_init (&x, NULL)
#define THREAD_MUTEX_DELETE(x)								pthread_mutex_destroy(&x)
#define THREAD_MUTEX_LOCK(x)								pthread_mutex_lock(&x)
#define THREAD_MUTEX_UNLOCK(x)								pthread_mutex_unlock(&x)		
#endif

#if GENERAL_PLATFORM == PLATFORM_UNIX
#define CLOSE_SOCKET(s) ::close(s);
#else
#define CLOSE_SOCKET(s) ::closesocket(s);
#endif

#define ARRAYSZ(v)					(sizeof(v) / sizeof(v[0]))

#if defined(_WIN32)

#undef min
#define min min
#undef max
#define max max

template <class T>
inline const T & min(const T & a, const T & b)
{
	return b < a ? b : a;
}

template <class T>
inline const T & max(const T & a, const T & b)
{
	return a < b ? b : a;
}

#define GENERAL_MIN min
#define GENERAL_MAX max
#define NOMINMAX

#else

#define GENERAL_MIN std::min
#define GENERAL_MAX std::max

#endif


// ip字符串的最大长度
#define MAX_IP_LEN 256


/** 获取进程ID */
inline int32 getProcessPID()
{
#if GENERAL_PLATFORM != PLATFORM_WIN32
	return getpid();
#else
	return (int32)GetCurrentProcessId();
#endif
}

inline int CatchLastError()
{
#if GENERAL_PLATFORM != PLATFORM_WIN32
	return errno;
#else
	return WSAGetLastError();
#endif
}

inline u_int32_t IPToN(int family, const char* ip, void* out)
{
#if GENERAL_PLATFORM == PLATFORM_WIN32
	return ::InetPton(family, char2wchar(ip,nullptr), out);
#else
	return inet_pton(family, ip, out);
#endif
}


/** sleep 跨平台 */
#if GENERAL_PLATFORM == PLATFORM_WIN32
inline void GeneralSleep(uint32 ms)
{
	::Sleep(ms);
}
#else
inline void GeneralSleep(uint32 ms)
{
	struct timeval tval;
	tval.tv_sec = ms / 1000;
	tval.tv_usec = (ms * 1000) % 1000000;
	select(0, NULL, NULL, NULL, &tval);
}
#endif

/** 判断平台是否为小端字节序 */
inline bool isPlatformLittleEndian()
{
	int n = 1;
	return *((char*)&n) ? true : false;
}

/** 设置环境变量 */
#if GENERAL_PLATFORM == PLATFORM_WIN32
inline void setenv(const std::string& name, const std::string& value, int overwrite)
{
	_putenv_s(name.c_str(), value.c_str());
}
#else
// Linux下面直接使用setenv
#endif

#endif // !PLATFORM_H_
