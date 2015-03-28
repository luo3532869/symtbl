/*
	File:	compobject.h
	Author: Robin
	Time: 2015/03/13 17:11
*/
#ifndef __COMPOBJECT_H__
#define __COMPOBJECT_H__

typedef void (*obj_print_fn)(void *obj);
typedef void (*obj_free_fn)(void *obj);
typedef int (*obj_cmp_fn)(void *obj1, void *obj2);

typedef struct _kvobj_t
{
	obj_print_fn  print;
	obj_free_fn  free;
	obj_cmp_fn  cmp;
}kvobj_t;


#endif
