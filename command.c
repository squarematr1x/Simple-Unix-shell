#include "command.h"

char *builtin_str[] = {
    "cd",
    "ls",
    "mkdir",
    "touch",
    "rm",
    "chmod",
    "help",
    "echo",
    "clear",
    "tree",
    "pwd",
    "exit"
};

int sh_n_builtins() 
{
    return sizeof(builtin_str) / sizeof(char *);
}

int sh_cd(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "sh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("sh");
        }
    }
    return 1;
}

int sh_ls(char **args)
{
    struct dirent **names;
    int n;

    n = scandir(".", &names, NULL, alphasort);

    if (args[1] != NULL) {
        fprintf(stderr, "sh: invalid argument for \"ls\"\n");
    } else {
        if (n < 0) {
            perror("sh");
        } else {
            while (n--) {
                if (names[n]->d_name[0] != '.') {
                    printf("%s    ", names[n]->d_name);
                }
                free(names[n]);
            }
            printf("\n");
            free(names);
        }
    }

    return 1;
}

int sh_mkdir(char **args)
{
    struct stat sb;

    if (args[1] == NULL) {
        fprintf(stderr, "sh: expected argument to \"mkdir\"\n");
    } else {
        int i = 1;
        while (args[i]) {
            if (!stat(args[i], &sb) && S_ISDIR(sb.st_mode)) {
                fprintf(stderr, "sh: cannot create dir, %s already exists\n", args[i]);  
            } else {
                mkdir(args[i], 0700);
            }
            i++;
        }
    }

    return 1;
}

int sh_touch(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "sh: expected argument(s) to \"touch\"\n");
    } else {
        int i = 1;
        while (args[i]) {
            creat(args[i], 0666);
            i++;
        }
    }

    return 1;
}

int is_rf(char* arg)
{
    if (!strcmp(arg, "-r") || !strcmp(arg, "-rf")) {
        return 1;
    }

    return 0;
}

int recursive_option(char **args)
{
    int i = 0;
    int recursive = 0;

    while (args[i]) {
        if (is_rf(args[i])) {
            recursive = 1;
            break;
        }
        i++;
    }

    return recursive;
}

int rmrf_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int rv = remove(fpath);

    if (rv) {
        perror(fpath);
    }

    return rv;
}

int sh_rm(char **args)
{
    int recursive_rm = recursive_option(args);

    if (args[1] == NULL) {
        fprintf(stderr, "sh: expected argument(s) to \"rm\"\n");
    }

    int i = 1;
    while (args[i]) {
        if (!is_rf(args[i])) {
            if (is_dir(args[i]) && recursive_rm) {
                if (nftw(args[i], rmrf_cb, 64, FTW_DEPTH|FTW_PHYS)) {
                    fprintf(stderr, "sh: cannot remove file %s\n", args[i]);
                } 
            } else {
                if (remove(args[i]) != 0) {
                    fprintf(stderr, "sh: unable to delete file: %s\n", args[i]);
                }
            }
        }
        i++;
    }

    return 1;
}

int sh_chmod(char **args)
{
    if (args[1] == NULL || args[2] == NULL) {
        fprintf(stderr, "sh: missing operands for \"chmod\"\n");
    } else if (args[3] != NULL) {
        fprintf(stderr, "sh: too many operands for \"chmod\"\n");
    } else if (!valid_mode(args[1])) {
        fprintf(stderr, "sh: invalid mode (%s) for \"chmod\"\n", args[1]);
    } else {
        int mode = strtoul(args[1], 0, 8);
        if (chmod(args[2], mode) < 0) {
            fprintf(stderr, "sh: error in chmod(%s, %s)\n", args[1], args[2]);
        }
    }

    return 1;
}

int sh_echo(char **args) 
{
    int i = 1;
    while (args[i]) {
        printf("%s ", args[i]);
        i++;
    }
    printf("\n");

    return 1;
}

int sh_help(char **args)
{
    printf("\nWelcome to Baby Shell\n");
    printf("Type program name and arguments, and hit enter.\n");
    printf("Built in commands:\n");

    for (int i = 0; i < sh_n_builtins(); i++) {
        printf("    %s\n", builtin_str[i]);
    }    
    printf("\n");

    return 1;
}

int sh_tree(char **args)
{
    if (args[1] != NULL) {
        fprintf(stderr, "sh: invalid argument for \"tree\"\n");
    } else {
        int ignore_tabs[SH_MAX_TABS];
        char* path = pwd();

        printf(".\n");
        print_tree(path, 0, ignore_tabs, 0);
        printf("\n");

        free(path);
    }

    return 1;
}

int sh_pwd(char **args)
{
    if (args[1] != NULL) {
        fprintf(stderr, "sh: \"pwd\" has no arguments\n");
    } else {
        char* path = pwd();
        printf("current path: %s\n", path);
        free(path);
    }

    return 1;
}

int sh_clear(char **args)
{
    if (args[1] != NULL) {
        fprintf(stderr, "sh: \"clear\" has no arguments\n");
    } else {
        system("clear");
    }

    return 1;
}

int sh_exit(char **args) 
{
    return 0;
}
