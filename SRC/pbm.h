#ifndef PBM_H
#define PBM_H

#define _CRT_SECURE_NO_WARNINGS

#define PBM_MAGIC "P4"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"

int FileBuffer(char *file, unsigned char **buffer);

char *SkipSpaces(char *p);

char *SkipBlanks(char *p);

char *SkipLine(char *p);

char *ReadUntil(char *p, char symbol);

char *ReadUntilBlank(char *p);

int IsNumber(char *p);

void PbmLoad(char *file, unsigned char **buffer, int *w, int *h);

#endif