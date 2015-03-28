/*
	File:	rodebug.h
	Author: Robin
	Time: 2015/03/12 10:16
*/
#ifndef __RO_DEBUG_H__
#define __RO_DEBUG_H__

#include <stdio.h>
#include <assert.h>
#define ROPRINT(fmt, arg...)		printf(fmt"\n", ##arg)
#define ROHERE(fmt, arg...)  printf("%s_%d "fmt"\n", __FUNCTION__, __LINE__, ##arg)

#define CONDERR(cond, oper, prompt)  		\
	do {												\
		if (cond)									\
		{											\
			ROHERE("%s", prompt);						\
			oper;								\
		}											\
	}while(0)				

#define ASSERT 		assert

#endif	
