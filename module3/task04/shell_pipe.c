#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_CMD 1024
#define MAX_ARGS 64

void execute_command(char** args, int input_fd, int output_fd) {
    pid_t pid = fork();
    if (pid == 0) {
        if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        if (output_fd != STDOUT_FILENO) {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
}

void parse_and_run(char* cmd_line) {
    char* commands[MAX_ARGS];
    int num_commands = 0;

    char* token = strtok(cmd_line, "|");
    while (token && num_commands < MAX_ARGS) {
        commands[num_commands++] = token;
        token = strtok(NULL, "|");
    }

    int pipe_fds[2];
    int input_fd = STDIN_FILENO;

    for (int i = 0; i < num_commands; i++) {
        char* args[MAX_ARGS];
        int nargs = 0;
        char* infile = NULL;
        char* outfile = NULL;

        char* arg = strtok(commands[i], " \t");
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
            input_fd = open(infile, O_RDONLY);
            if (input_fd < 0) {
                perror("open input");
                return;
            }
        }

        int current_output = STDOUT_FILENO;
        if (outfile) {
            current_output = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        } else if (i < num_commands - 1) {
            pipe(pipe_fds);
            current_output = pipe_fds[1];
        }

        execute_command(args, input_fd, current_output);

        if (input_fd != STDIN_FILENO) close(input_fd);
        if (current_output != STDOUT_FILENO) close(current_output);

        input_fd = pipe_fds[0];
    }

    while (wait(NULL) > 0);
}

int main(void) {
    char line[MAX_CMD];
    printf("=== Pipeline Shell ===\n");

    while (1) {
        printf("mysh> ");
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) break;
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '\0') continue;

        parse_and_run(line);
    }
    return 0;
}
