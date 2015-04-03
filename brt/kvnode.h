/*
	File:	kvnode.h
	Author: Robin
	Time: 2015/03/13 15:51
*/
#ifndef __KV_NODE_H__
#define __KV_NODE_H__

#include <stdarg.h>

typedef enum {
	TKVNULL = 0,
	TKVINT,
	TKVDOUBLE,
	TKVSTRING,
	TKVOBJECT,
	
	TKVEND
}type_kv;

struct _kv;
typedef void (*print_fn)(struct _kv *kv);
typedef void (*free_fn)(struct _kv *kv);
typedef int (*cmp_fn)(struct _kv *key1, struct _kv *key2);
typedef int (*copy_fn)(struct _kv *k1, struct _kv *k2);

typedef struct _kvops_t
{
	print_fn  print;
	free_fn  free;
	cmp_fn  cmp;
	copy_fn copy;
}kvops_t;

typedef struct _kv
{
	type_kv t;
	union data{
		int dint;
		double ddouble;
		char *dstring;
		void *dobj;
	}o;
	kvops_t ops;
}kv_t;

#define EQUALTYPE(kv1, kv2)   (((kv1)->t) ==  ((kv2)->t))
#define _OPS(kv)  ((kv)->ops)	

/*key-value node*/
#define _TN(arg) TKVNULL, arg
#define _TI(arg) TKVINT, arg
#define _TD(arg) TKVDOUBLE, arg
#define _TS(arg) TKVSTRING, arg
#define _TO(arg) TKVOBJECT, &(arg), sizeof(arg)

/*key-value details*/
#define _DN(kv)  (((kv)-> o).dstring)
#define _DI(kv)  (((kv)-> o).dint)
#define _DD(kv)  (((kv)-> o).ddouble)
#define _DS(kv)  (((kv)-> o).dstring)
#define _DO(kv)  ((kvobj_t *)(((kv)-> o).dobj))

#define NEWKVBYARG(ap) ((kv_t *)newkvnode(((type_kv)va_arg((ap), type_kv)), ((va_list *)(&(ap)))))

kv_t *newkvnode(type_kv t, va_list *arg);
int freekvnode(kv_t *node);
kv_t * copykvnode(kv_t *node);

#endif
