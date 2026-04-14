#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: sum_args <num1> <num2> ...\n");
        return 0;
    }
    double sum = 0;
    int count = 0;
    for (int i = 1; i < argc; i++) {
        char* end;
        double val = strtod(argv[i], &end);
        if (*end == '\0') {
            sum += val;
            count++;
        } else {
            fprintf(stderr, "Skipping non-numeric: %s\n", argv[i]);
        }
    }
    printf("Sum of %d numbers: %.2f\n", count, sum);
    return 0;
}
