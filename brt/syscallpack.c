/*
	File:	syscallpack.c
	Author: Robin
	Time: 2015/03/12 10:17
*/
#include <stdlib.h>
#include <string.h>
#include "rodebug.h"

#ifdef MEMDGB
#define SCPPRINT(fmt, arg...)		printf(fmt"\n", ##arg)
#else
#define SCPPRINT(fmt, arg...)
#endif

/*for memory count and debug*/
static int nmemalloc = 0;

void *Malloc(size_t size, 			const char *file, int line)
{
	void *ptr;
	SCPPRINT("malloc: %s_%d", file, line);
	nmemalloc++;
	ptr = malloc(size);
	CONDERR(NULL == ptr, exit(-1), "fatal: malloc failed.");
	
	return ptr;
}

void *Calloc(size_t nmemb, size_t size, 			const char *file, int line)
{
	void *ptr;
	SCPPRINT("calloc: %s_%d", file, line);
	
	ptr = calloc(nmemb, size);
	nmemalloc++;
	CONDERR(NULL == ptr, exit(-1), "fatal: calloc failed.");
	
	return ptr;
}

void *Realloc(void *ptr, size_t size, 			const char *file, int line)
{
	void *nptr;
	SCPPRINT("realloc: %s_%d", file, line);
	
	nptr = realloc(ptr, size);
	nmemalloc++;
	CONDERR(NULL == nptr, exit(-1), "fatal: realloc failed.");
	
	return nptr;
}

void Free(void *ptr, 			const char *file, int line)
{
	SCPPRINT("free: %s_%d", file, line);
	ASSERT(NULL != ptr);
	free(ptr);
	nmemalloc--;
}

char *Strdup(const char *s, 		const char *file, int line)
{
	SCPPRINT("strdup: %s_%d", file, line);
	nmemalloc++;
	return strdup(s);
}

void printnmalloc()
{
	SCPPRINT("memory alloc times:%d\n", nmemalloc);
}

void *Memcpy(void *dest, const void *src, size_t n)
{
	ASSERT(NULL != dest && NULL != src);	

	return memcpy(dest, src, n);
}

