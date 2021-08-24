#include "sys/wait.h"
#include "sys/types.h"
#include "ctype.h"

#include "command.h"

#define SH_RL_BUFSIZE 1024
#define SH_TOK_BUFSIZE 64

#define SH_TOK_DELIM " \t\r\n\a"

void sh_loop();

char *sh_read_line();
char **sh_parse_line(char *line);

int sh_execute(char **args);
int sh_launch(char **args);

int (*builtin_func[]) (char **) = {
    &sh_cd,
    &sh_ls,
    &sh_mkdir,
    &sh_touch,
    &sh_rm,
    &sh_chmod,
    &sh_help,
    &sh_echo,
    &sh_clear,
    &sh_tree,
    &sh_pwd,
    &sh_exit
};

char *sh_read_line()
{
    int bufsize = SH_RL_BUFSIZE;
    int pos = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c; // EOF is int, not char

    if (!buffer) {
        fprintf(stderr, "sh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Read a char
        c = getchar();

        // If we hit EOF, replace it with null char and return
        if (c == EOF || c == '\n') {
            buffer[pos] = '\0';
            return buffer;
        } else {
            buffer[pos] = c;
        }
        pos++;

        if (pos >= bufsize) {
            bufsize += SH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "sh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **sh_parse_line(char* line)
{
    int bufsize = SH_TOK_BUFSIZE;
    int pos = 0;
    char **tokens = malloc(bufsize * sizeof(char*));

    if (!tokens) {
        fprintf(stderr, "sh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    int len = strlen(line);
    char token[SH_TOK_BUFSIZE];
    int j = 0;
    int n_quotations = 0;

    for (int i = 0; i < len; i++) {
        if (is_quotation(line[i])) {
            if (n_quotations == 0) {
                n_quotations++;
            } else {
                n_quotations = 0;
            }
        } else if (isspace(line[i])) {
            if (n_quotations != 0) { // Use while isspace?
                token[j] = line[i];
                j++;
            } else {
                token[j] = '\0';
                tokens[pos] = malloc(bufsize * sizeof(char*));
                strcpy(tokens[pos], token);
                pos++;
                j = 0; 
            }
        } else {
            token[j] = line[i];
            j++;
        }
    }

    if (j != 0) {
        token[j] = '\0';
        tokens[pos] = malloc(bufsize * sizeof(char*));
        strcpy(tokens[pos], token);
        pos++;
    }
    tokens[pos] = NULL;

    return tokens;
}

int sh_launch(char **args) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("sh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("sh");
    } else {
        // Parent process
        do {
            pid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int sh_execute(char **args)
{
    if (args[0] == NULL) {
        // Empty command was entered.
        return 1;
    }

    for (int i = 0; i < sh_n_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return sh_launch(args);
}

void sh_loop()
{
    char *current_dir;
    char *line;
    char **args;
    int status;

    do {
        current_dir = pwd();

        printf("%s:\\> ", current_dir);
        line = sh_read_line();
        args  = sh_parse_line(line);
        status = sh_execute(args);

        free(current_dir);
        free(line);
        free(args);
    } while (status);
}

int main (int argc, char **argv) 
{   
    sh_loop();

    return EXIT_SUCCESS;
}
