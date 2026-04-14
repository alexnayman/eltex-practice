#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#define MAX_NAME 64
#define MAX_PHONE 32
#define MAX_WORK 64
#define MAX_JOB 64
#define MAX_MAIL 64
#define MAX_SOC 64
#define MAX_CONTACTS 256
#define DATA_FILE "phonebook.txt"

typedef struct {
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    char work[MAX_WORK];
    char job[MAX_JOB];
    char mail[MAX_MAIL];
    char soc[MAX_SOC];
} Contact;

int load_contacts(Contact* contacts);

int save_contacts(Contact* contacts, int count);

#endif
