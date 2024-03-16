#ifndef EVENTMACRO_HPP
#define EVENTMACRO_HPP

#ifndef _EVENT_EXPORT
#ifdef _WIN32
#define _EVENT_EXPORT __declspec(dllexport)
#else 
#define _EVENT_EXPORT 
#endif
#endif

#endif