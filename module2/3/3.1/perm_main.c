#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>
#include "perm_func.h"

int main() {
    mode_t cur = 0;
    int cmd;
    char buf[256];

    while (1) {
        printf("1.Manual 2.File 3.Mod 4.Exit: ");
        if (scanf("%d", &cmd) != 1) break;
        
        if (cmd == 1) {
            scanf("%255s", buf);
            cur = isdigit(buf[0]) ? get_oct(buf) : get_sym(buf);
            show_perm(cur);
        } else if (cmd == 2) {
            scanf("%255s", buf);
            struct stat st;
            if (stat(buf, &st) == 0) {
                cur = st.st_mode & 0777;
                show_perm(cur);
            }
        } else if (cmd == 3) {
            scanf("%255s", buf);
            cur = mod_perm(cur, buf);
            show_perm(cur);
        } else {
            break;
        }
    }
    return 0;
}
