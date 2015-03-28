/*
	File:	syscallpack.h
	Author: Robin
	Time: 2015/03/12 10:31
*/
#ifndef __SYSCALLPACK_H__
#define __SYSCALLPACK_H__

void *Malloc(size_t size, const char *file, int line);
void *Calloc(size_t nmemb, size_t size, const char *file, int line);
void Free(void *ptr, const char *file, int line);
char *Strdup(const char *s, const char *file, int line);
void *Memcpy(void *dest, const void *src, size_t n);

#define MALLOC(size)  Malloc(size, __FUNCTION__, __LINE__)
#define CALLOC(nmemb, size)  Calloc(nmemb, size, __FUNCTION__, __LINE__)
#define FREE(ptr) Free(ptr, __FUNCTION__, __LINE__)
#define STRDUP(s) Strdup(s, __FUNCTION__, __LINE__)  


#endif
