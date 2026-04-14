#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD 1024
#define MAX_ARGS 64
#define PATH_SEP ":"
#define MAX_PATH_LEN 4096

int find_executable(const char* name, char* out_path, size_t out_size) {
    snprintf(out_path, out_size, "./%s", name);
    if (access(out_path, X_OK) == 0) return 1;

    char* path_env = getenv("PATH");
    if (!path_env) return 0;

    char path_copy[MAX_PATH_LEN * 4];
    strncpy(path_copy, path_env, sizeof(path_copy) - 1);
    path_copy[sizeof(path_copy) - 1] = '\0';

    char* token = strtok(path_copy, PATH_SEP);
    while (token) {
        snprintf(out_path, out_size, "%s/%s", token, name);
        if (access(out_path, X_OK) == 0) return 1;
        token = strtok(NULL, PATH_SEP);
    }
    return 0;
}

void run_command(char** args, int nargs) {
    if (nargs == 0) return;

    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    if (strcmp(args[0], "cd") == 0) {
        if (nargs > 1) {
            chdir(args[1]);
        }
        return;
    }

    char exe_path[MAX_PATH_LEN];
    if (!find_executable(args[0], exe_path, sizeof(exe_path))) {
        fprintf(stderr, "Command not found: %s\n", args[0]);
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        args[nargs] = NULL;
        execvp(exe_path, args);
        perror("execvp");
        exit(127);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("fork");
    }
}

int main(void) {
    char line[MAX_CMD];
    char* args[MAX_ARGS];

    printf("=== Simple Shell ===\n");
    printf("Type 'exit' to quit.\n\n");

    while (1) {
        printf("mysh> ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin)) break;

        line[strcspn(line, "\r\n")] = '\0';

        if (line[0] == '\0') continue;

        int nargs = 0;
        char* token = strtok(line, " \t");
        while (token && nargs < MAX_ARGS - 1) {
            args[nargs++] = token;
            token = strtok(NULL, " \t");
        }
        args[nargs] = NULL;

        run_command(args, nargs);
    }

    return 0;
}
