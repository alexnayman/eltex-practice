#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "phonebook.h"

int load_contacts(Contact* contacts) {
    int fd = open(DATA_FILE, O_RDONLY);
    if (fd < 0) {
        printf("Data file not found (will create new).\n");
        return 0;
    }

    int count = 0;
    while (count < MAX_CONTACTS) {
        ssize_t bytes = read(fd, &contacts[count], sizeof(Contact));
        if (bytes == 0) break;
        if (bytes < (ssize_t)sizeof(Contact)) {
            fprintf(stderr, "Read error on record #%d\n", count);
            break;
        }
        count++;
    }
    close(fd);
    return count;
}

int save_contacts(Contact* contacts, int count) {
    int fd = open(DATA_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error opening file for write");
        return -1;
    }

    for (int i = 0; i < count; i++) {
        ssize_t bytes = write(fd, &contacts[i], sizeof(Contact));
        if (bytes < (ssize_t)sizeof(Contact)) {
            perror("Write error");
            close(fd);
            return -1;
        }
    }
    close(fd);
    return 0;
}
