#include "util.h"

int is_file(char* path)
{
    struct stat path_status;
    stat(path, &path_status);

    return S_ISREG(path_status.st_mode);
}

int is_dir(char* path)
{
    struct stat path_status;
    int root = 0;

    if (stat(path, &path_status) != 0) {
        return 0;
    } else {
        int len = strlen(path);
        for (int i = 0; i < len; i++) {
            if (path[i] == '.') {
                root = 1;
                break;
            }
        }
    }
    
    if (root) {
        return 0;
    }

    return S_ISDIR(path_status.st_mode);
}

int is_quotation(char c)
{
    return (c == '\"');
}

int valid_mode(char *mode)
{
    int i = 0;
    int valid = 1;
    int n_nonzero_digits = 0;

    while (mode[i]) {
        if (mode[i] < '0' || mode[i] > '7') {
            valid = 0;
            break;
        } else if (mode[i] != '0') {
            n_nonzero_digits++;
        }
        i++;
    }

    if (n_nonzero_digits > 3) {
        valid = 0;
    }

    return valid;
}

void print_tabs(int n_tabs)
{
    for (int i = 0; i < n_tabs; i++) {
        printf("│");
        printf("    ");
    }
}

void print_tree(char* path, int n_tabs)
{
    struct dirent **names;
    int n;
    n = scandir(path, &names, NULL, alphasort);

    if (n < 0) {
        perror("sh");
    } else {
        for (int i = 0; i < n; i++) {
            print_tabs(n_tabs);
            
            if (i < n - 1) {
                printf("├── %s\n", names[i]->d_name);
            } else {
                printf("└── %s\n", names[i]->d_name);
            }

            char* temp_dir = concat(path, "/");
            char* sub_dir  = concat(temp_dir, names[i]->d_name);
            free(temp_dir);

            if (is_dir(sub_dir)) {
                print_tree(sub_dir, n_tabs + 1);
            }

            free(sub_dir);
            free(names[i]);
        }
        free(names);
    }
}

char* concat(const char* str1, const char* str2)
{
    char* result = malloc(strlen(str1) + strlen(str2) + 1);

    if (!result) {
        fprintf(stderr, "sh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    strcpy(result, str1);
    strcat(result, str2);

    return result;
}

char* pwd()
{
    char* current_dir = malloc(SH_DIRNAME_BUFSIZE);
    current_dir = getcwd(current_dir, SH_DIRNAME_BUFSIZE);

    return current_dir;
}
