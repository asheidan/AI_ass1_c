#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifdef __DEBUG__
#include <stdio.h>

#define dprintf(stream,...)	fprintf(stream,__VA_ARGS__)
#define ifdebug(...)	__VA_ARGS__
#else
#define dprintf(...)
#define ifdebug(...)
#endif


#endif
