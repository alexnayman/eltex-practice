#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: concat <str1> <str2> ...\n");
        return 0;
    }
    printf("Concatenated: ");
    for (int i = 1; i < argc; i++) {
        printf("%s", argv[i]);
    }
    printf("\n");
    return 0;
}
