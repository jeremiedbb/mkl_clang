#ifndef cmodule_h__
#define cmodule_h__
#define _GNU_SOURCE
#include <link.h>

void prange_blas(int);
int callback(struct dl_phdr_info*, size_t, void*);

#endif