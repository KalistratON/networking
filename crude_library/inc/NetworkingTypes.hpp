#ifndef NETWORKINGTYPES_HPP
#define NETWORKINGTYPES_HPP

#ifdef _WIN32
#include <WinSock2.h>
#endif

namespace networking {

#ifndef _NETWORKING_EXPORT
#ifdef _WIN32
#define _NETWORKING_EXPORT __declspec(dllexport)
#else 
#define _NETWORKING_EXPORT 
#endif
#endif

#ifdef __linux__
using SocketType = int;
#elif _WIN32
using SocketType = SOCKET;
#endif
}

#endif