#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "phonebook.h"

void clean_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void read_str(const char* prompt, char* buf, int size, int required) {
    printf("%s", prompt);
    if (fgets(buf, size, stdin)) {
        buf[strcspn(buf, "\r\n")] = '\0';
        if (required && buf[0] == '\0') {
            printf("! Required, repeat: ");
            fgets(buf, size, stdin);
            buf[strcspn(buf, "\r\n")] = '\0';
        }
    }
}

void show_contact(const Contact* c, int i) {
    printf("\n[%d] Name: %s\n    Phone: %s\n    Work: %s (%s)\n    Email: %s\n    Social: %s\n",
           i + 1, c->name, c->phone,
           c->work[0] ? c->work : "-",
           c->job[0] ? c->job : "-",
           c->mail[0] ? c->mail : "-",
           c->soc[0] ? c->soc : "-");
}

int add_contact(Contact* contacts, int count) {
    if (count >= MAX_CONTACTS) {
        printf("Maximum contacts reached!\n");
        return count;
    }
    Contact* c = &contacts[count];
    memset(c, 0, sizeof(Contact));

    read_str("Phone: ", c->phone, MAX_PHONE, 1);

    for (int i = 0; i < count; i++) {
        if (strcmp(contacts[i].phone, c->phone) == 0) {
            printf("Number already exists!\n");
            return count;
        }
    }

    read_str("Name: ", c->name, MAX_NAME, 1);
    read_str("Work: ", c->work, MAX_WORK, 0);
    read_str("Job: ", c->job, MAX_JOB, 0);
    read_str("Email: ", c->mail, MAX_MAIL, 0);
    read_str("Social: ", c->soc, MAX_SOC, 0);

    return count + 1;
}

int find_contact(Contact* contacts, int count) {
    if (count == 0) { printf("Empty\n"); return -1; }
    printf("1-Name, 2-Phone, 3-Number: ");
    int choice;
    scanf("%d", &choice);
    clean_input();

    if (choice == 3) {
        int num;
        printf("Number: ");
        scanf("%d", &num);
        clean_input();
        return (num > 0 && num <= count) ? num - 1 : -1;
    }

    char query[128];
    printf("Query: ");
    if (!fgets(query, sizeof(query), stdin)) return -1;
    query[strcspn(query, "\r\n")] = '\0';

    int found[64], nfound = 0;
    for (int i = 0; i < count; i++) {
        char* field = (choice == 1) ? contacts[i].name : contacts[i].phone;
        if (strstr(field, query)) {
            printf("[%d] %s\n", i + 1, contacts[i].name);
            if (nfound < 64) found[nfound++] = i;
        }
    }
    if (nfound == 0) return -1;
    if (nfound == 1) return found[0];

    printf("Select number: ");
    int sel;
    scanf("%d", &sel);
    clean_input();
    return (sel > 0 && sel <= nfound) ? found[sel - 1] : -1;
}

void edit_contact(Contact* contacts, int count) {
    int idx = find_contact(contacts, count);
    if (idx < 0) { printf("Not found\n"); return; }

    Contact* c = &contacts[idx];
    printf("Editing: %s\n", c->name);
    printf("Fields: 1-Name 2-Work 3-Job 4-Phone 5-Email 6-Social\nDigits (e.g. 124): ");
    char keys[10];
    scanf("%9s", keys);
    clean_input();

    for (int i = 0; keys[i]; i++) {
        switch (keys[i]) {
            case '1': read_str("Name: ", c->name, MAX_NAME, 1); break;
            case '2': read_str("Work: ", c->work, MAX_WORK, 0); break;
            case '3': read_str("Job: ", c->job, MAX_JOB, 0); break;
            case '4': read_str("Phone: ", c->phone, MAX_PHONE, 1); break;
            case '5': read_str("Email: ", c->mail, MAX_MAIL, 0); break;
            case '6': read_str("Social: ", c->soc, MAX_SOC, 0); break;
        }
    }
}

int delete_contact(Contact* contacts, int count) {
    int idx = find_contact(contacts, count);
    if (idx < 0) { printf("Not found\n"); return count; }
    printf("Delete? (1-Yes): ");
    int confirm;
    scanf("%d", &confirm);
    clean_input();
    if (confirm == 1) {
        for (int i = idx; i < count - 1; i++)
            contacts[i] = contacts[i + 1];
        return count - 1;
    }
    return count;
}

int main(void) {
    Contact contacts[MAX_CONTACTS];
    int count = load_contacts(contacts);
    printf("Loaded contacts: %d\n", count);

    int cmd;
    while (1) {
        printf("\n1-List 2-Add 3-Search 4-Delete 5-Edit 0-Exit\n> ");
        if (scanf("%d", &cmd) != 1) { clean_input(); continue; }
        clean_input();

        if (cmd == 0) break;
        switch (cmd) {
            case 1:
                if (count == 0) printf("Empty\n");
                else for (int i = 0; i < count; i++) show_contact(&contacts[i], i);
                break;
            case 2: count = add_contact(contacts, count); break;
            case 3: {
                int idx = find_contact(contacts, count);
                if (idx >= 0) show_contact(&contacts[idx], idx);
                break;
            }
            case 4: count = delete_contact(contacts, count); break;
            case 5: edit_contact(contacts, count); break;
        }
    }

    if (save_contacts(contacts, count) == 0)
        printf("Saved %d records.\n", count);

    return 0;
}
