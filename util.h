#ifndef UTIL_H
#define UTIL_H

#include "stdio.h"
#include "stdlib.h"
#include "sys/stat.h"
#include "string.h"
#include "unistd.h"
#include "dirent.h"

#define SH_DIRNAME_BUFSIZE 100

int is_file(char* path);
int is_dir(char* path);
int is_quotation(char c);

int valid_mode(char *mode);

void print_tabs(int n_tabs);
void print_tree(char* path, int n_tabs);

char* concat(const char* str1, const char* str2);
char* pwd();

#endif // UTIL_H
