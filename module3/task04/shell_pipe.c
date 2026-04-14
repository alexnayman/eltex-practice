#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_CMDS 16
#define MAX_ARGS 64
#define MAX_LINE 1024
#define MAX_CMD_LEN 256

typedef struct {
    char *args[MAX_ARGS];
    int argc;
    char *input_file;
    char *output_file;
} Command;

typedef struct {
    Command cmds[MAX_CMDS];
    int cmd_count;
} Pipeline;

static char *trim(char *str) {
    while (*str == ' ' || *str == '\t') str++;
    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r'))
        *end-- = '\0';
    return str;
}

static void parse_command(const char *cmd_str, Command *cmd) {
    cmd->argc = 0;
    cmd->input_file = NULL;
    cmd->output_file = NULL;

    char buf[MAX_CMD_LEN];
    strncpy(buf, cmd_str, MAX_CMD_LEN - 1);
    buf[MAX_CMD_LEN - 1] = '\0';

    char *p = buf;
    char *token;
    while ((token = strtok(p, " \t")) != NULL) {
        p = NULL;
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " \t");
            if (token) cmd->input_file = strdup(token);
            continue;
        }
        if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " \t");
            if (token) cmd->output_file = strdup(token);
            continue;
        }
        if (cmd->argc < MAX_ARGS - 1) {
            cmd->args[cmd->argc++] = strdup(token);
        }
    }
    cmd->args[cmd->argc] = NULL;
}

static int parse_pipeline(const char *line, Pipeline *pl) {
    pl->cmd_count = 0;

    char buf[MAX_LINE];
    strncpy(buf, line, MAX_LINE - 1);
    buf[MAX_LINE - 1] = '\0';

    char *p = buf;
    char *segment;
    while ((segment = strtok(p, "|")) != NULL) {
        p = NULL;
        if (pl->cmd_count < MAX_CMDS) {
            parse_command(trim(segment), &pl->cmds[pl->cmd_count]);
            pl->cmd_count++;
        }
    }
    return pl->cmd_count;
}

static void free_command(Command *cmd) {
    for (int i = 0; i < cmd->argc; i++) {
        free(cmd->args[i]);
    }
    if (cmd->input_file) free(cmd->input_file);
    if (cmd->output_file) free(cmd->output_file);
}

static int run_pipeline(Pipeline *pl) {
    if (pl->cmd_count == 0) return 0;

    if (pl->cmd_count == 1) {
        Command *cmd = &pl->cmds[0];
        pid_t pid = fork();
        if (pid == 0) {
            if (cmd->input_file) {
                int fd = open(cmd->input_file, O_RDONLY);
                if (fd < 0) { perror("open input"); exit(1); }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            if (cmd->output_file) {
                int fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) { perror("open output"); exit(1); }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            execvp(cmd->args[0], cmd->args);
            perror("execvp");
            exit(1);
        }
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }

    int fds[pl->cmd_count - 1][2];
    pid_t pids[pl->cmd_count];

    for (int i = 0; i < pl->cmd_count - 1; i++) {
        if (pipe(fds[i]) < 0) { perror("pipe"); return -1; }
    }

    for (int i = 0; i < pl->cmd_count; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            Command *cmd = &pl->cmds[i];

            if (i > 0) {
                dup2(fds[i - 1][0], STDIN_FILENO);
            } else if (cmd->input_file) {
                int fd = open(cmd->input_file, O_RDONLY);
                if (fd < 0) { perror("open input"); exit(1); }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            if (i < pl->cmd_count - 1) {
                dup2(fds[i][1], STDOUT_FILENO);
            } else if (cmd->output_file) {
                int fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) { perror("open output"); exit(1); }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            for (int j = 0; j < pl->cmd_count - 1; j++) {
                close(fds[j][0]);
                close(fds[j][1]);
            }

            execvp(cmd->args[0], cmd->args);
            perror("execvp");
            exit(1);
        }
    }

    for (int i = 0; i < pl->cmd_count - 1; i++) {
        close(fds[i][0]);
        close(fds[i][1]);
    }

    int status;
    for (int i = 0; i < pl->cmd_count; i++) {
        waitpid(pids[i], &status, 0);
    }
    return 0;
}

int main(void) {
    char line[MAX_LINE];

    printf("Shell with pipes (POSIX mode). Type 'exit' to quit.\n\n");

    while (1) {
        printf("pipe-shell> ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin))
            break;

        char *trimmed = trim(line);
        if (strlen(trimmed) == 0)
            continue;

        if (strcmp(trimmed, "exit") == 0 || strcmp(trimmed, "quit") == 0)
            break;

        Pipeline pl;
        int n = parse_pipeline(trimmed, &pl);
        if (n == 0)
            continue;

        run_pipeline(&pl);

        for (int i = 0; i < pl.cmd_count; i++) {
            free_command(&pl.cmds[i]);
        }
    }
    return 0;
}

