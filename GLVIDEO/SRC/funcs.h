#ifndef FUNCS_H
#define FUNCS_H

//types
#define BYTE_SIZE 8
typedef unsigned char byte;

void EnableDebug();

void DisableDebug();

void ExitError(char *str, ...);

void *Calloc(size_t count, size_t size);

void Free(void* p);

#endif
