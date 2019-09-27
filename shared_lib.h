#ifndef shared_lib_h__
#define shared_lib_h__
#define _GNU_SOURCE
#include <link.h>
 
int func();
int callback(struct dl_phdr_info*, size_t, void*);

#endif