#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>

int is_number(const char* str) {
    if (!str || !*str) return 0;
    const char* p = str;
    if (*p == '-' || *p == '+') p++;
    if (!*p) return 0;
    int has_dot = 0;
    int has_digit = 0;
    while (*p) {
        if (*p == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        } else if (isdigit((unsigned char)*p)) {
            has_digit = 1;
        } else {
            return 0;
        }
        p++;
    }
    return has_digit;
}

void process_arg(const char* label, const char* arg) {
    if (is_number(arg)) {
        double val = atof(arg);
        if (strchr(arg, '.') == NULL) {
            long long ival = atoll(arg);
            printf("  %s: [Number] %s -> %lld * 2 = %lld\n", label, arg, ival, ival * 2);
        } else {
            printf("  %s: [Number] %s -> %.6f * 2 = %.6f\n", label, arg, val, val * 2.0);
        }
    } else {
        printf("  %s: [String] %s\n", label, arg);
    }
}

int main(int argc, char* argv[]) {
    if (argc > 2 && strcmp(argv[1], "--child") == 0) {
        printf("=== Child process (PID=%ld) ===\n", (long)getpid());
        for (int i = 2; i < argc; i++) {
            process_arg("Child", argv[i]);
        }
        return 0;
    }

    if (argc < 2) {
        printf("Usage: %s <arg1> [arg2] ...\n", argv[0]);
        return 1;
    }

    int num_args = argc - 1;
    int mid = (num_args + 1) / 2;

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        printf("=== Child process (PID=%ld) ===\n", (long)getpid());
        for (int i = mid; i < num_args; i++) {
            process_arg("Child", argv[i + 1]);
        }
        exit(0);
    } else {
        printf("=== Parent process (PID=%ld) ===\n", (long)getpid());
        for (int i = 0; i < mid; i++) {
            process_arg("Parent", argv[i + 1]);
        }
        wait(NULL);
    }

    printf("=== Done (PID=%ld) ===\n", (long)getpid());
    return 0;
}
