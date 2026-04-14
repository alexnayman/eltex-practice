#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_CMD 1024
#define MAX_ARGS 64

void execute_proc(char** args, int in_fd, int out_fd) {
    pid_t pid = fork();
    if (pid == 0) {
        if (in_fd != STDIN_FILENO) {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        if (out_fd != STDOUT_FILENO) {
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
}

void process_line(char* line) {
    char* cmds[MAX_ARGS];
    int n_cmds = 0;
    char* token = strtok(line, "|");
    
    while (token && n_cmds < MAX_ARGS) {
        cmds[n_cmds++] = token;
        token = strtok(NULL, "|");
    }

    int in_fd = STDIN_FILENO;
    int pipe_fds[2];

    for (int i = 0; i < n_cmds; i++) {
        char* args[MAX_ARGS];
        int nargs = 0;
        char* infile = NULL;
        char* outfile = NULL;

        char* arg = strtok(cmds[i], " \t");
        while (arg) {
            if (strcmp(arg, "<") == 0) {
                infile = strtok(NULL, " \t");
            } else if (strcmp(arg, ">") == 0) {
                outfile = strtok(NULL, " \t");
            } else {
                args[nargs++] = arg;
            }
            arg = strtok(NULL, " \t");
        }
        args[nargs] = NULL;

        if (nargs == 0) continue;

        if (strcmp(args[0], "exit") == 0) exit(0);
        if (strcmp(args[0], "cd") == 0) {
            if (nargs > 1) chdir(args[1]);
            continue;
        }

        if (infile) {
            in_fd = open(infile, O_RDONLY);
            if (in_fd < 0) {
                perror("open input");
                return;
            }
        }

        int out_fd = STDOUT_FILENO;
        if (outfile) {
            out_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        } else if (i < n_cmds - 1) {
            if (pipe(pipe_fds) < 0) {
                perror("pipe");
                return;
            }
            out_fd = pipe_fds[1];
        }

        execute_proc(args, in_fd, out_fd);

        if (in_fd != STDIN_FILENO) close(in_fd);
        if (out_fd != STDOUT_FILENO) close(out_fd);

        in_fd = pipe_fds[0];
    }

    while (wait(NULL) > 0);
}

int main(void) {
    char line[MAX_CMD];

    while (1) {
        printf("mysh> ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin)) break;
        line[strcspn(line, "\n")] = '\0';
        if (line[0] == '\0') continue;

        process_line(line);
    }

    return 0;
}
