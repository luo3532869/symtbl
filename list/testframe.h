/*
	File:	testframe.c
	Author: Robin
	Time: 2015/03/25 10:26
*/

#ifndef __TEST_FRAME_H__
#define __TEST_FRAME_H__

void checkvalue(void *symtbl, type_kv tkv, ...);
void checklength(void *symtbl, int nelems);
void checkdel(void *symtbl, type_kv t, ...);

#endif
