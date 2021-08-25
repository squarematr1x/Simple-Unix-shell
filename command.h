#ifndef COMMAND_H
#define COMMAND_H

#define _XOPEN_SOURCE 700

#include "ftw.h"
#include "fcntl.h"

#include "util.h"

extern char *builtin_str[];

int sh_n_builtins();

int sh_cd(char **args);
int sh_ls(char **args);
int sh_mkdir(char **args);
int sh_touch(char **args);
int sh_rm(char **args);
int sh_chmod(char **args);
int sh_help(char **args);
int sh_echo(char **args);
int sh_clear(char **args);
int sh_tree(char **args);
int sh_pwd(char **args);
int sh_exit(char **args);

#endif // COMMAND_H
