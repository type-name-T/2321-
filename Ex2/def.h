#ifndef DEF_H
#define DEF_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#define DIRECT_CONCAT(a,b) a##b
#define CONCAT(a,b) DIRECT_CONCAT(a,b)
#define overload(func) CONCAT(func##_,T)
#endif
