#ifndef DEF_H
#define DEF_H
#include<stddef.h>
#include<stdbool.h>
#include<stdlib.h>
typedef unsigned char byte;
#define DIRECT_CONCAT(a,b) a##b
#define CONCAT(a,b) DIRECT_CONCAT(a,b)
#define _template(class) CONCAT(class##_,T)
#define make_sufix(class,func) CONCAT(class,_##func)
#define def_memfn(class,func,...) make_sufix(class,func)(class* _this __VA_OPT__(,)__VA_ARGS__)
#define memfn(class,func) make_sufix(class,func)
#define def_construct(class,...) void make_sufix(class,construct)(class* _this __VA_OPT__(,)__VA_ARGS__)
#define construct(class,name,...)\
    class name;\
    make_sufix(class,construct)(&name __VA_OPT__(,)__VA_ARGS__)
#define def_destruct(class) void make_sufix(class,destruct)(class* _this)
#define destruct(class,_this) make_sufix(class,destruct)(_this)
#define def_copy(class) class make_sufix(class,copy)(const class* other)
#define copy(class) make_sufix(class,copy)(const class* other)
#define _new(reciever,class,...)\
    reciever=(class*)malloc(sizeof(class));\
    make_sufix(class,construct)(reciever __VA_OPT__(,)__VA_ARGS__)
#define _delete(class,ptr)\
    make_sufix(class,destruct)(ptr);\
    free(ptr)
#define _static(class,func) make_sufix(class,func)
typedef bool (*Comp)(const T*,const T*);
#define pair(T1,T2) pair_##T1##_##T2
#define def_pair(T1,T2)\
typedef struct pair(T1,T2){\
    T1 first;\
    T2 second;\
}pair(T1,T2)
#endif