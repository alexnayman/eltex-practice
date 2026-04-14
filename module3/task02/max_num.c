#include <stdio.h>
#include <stdlib.h>
#include <float.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: max_num <num1> <num2> ...\n");
        return 0;
    }
    double max_val = -DBL_MAX;
    int found = 0;
    for (int i = 1; i < argc; i++) {
        char* end;
        double val = strtod(argv[i], &end);
        if (*end == '\0') {
            if (!found || val > max_val) {
                max_val = val;
            }
            found = 1;
        }
    }
    if (found) {
        printf("Maximum: %.2f\n", max_val);
    } else {
        printf("No numbers found\n");
    }
    return 0;
}
