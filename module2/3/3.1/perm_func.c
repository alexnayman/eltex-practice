#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "perm_func.h"

static const char* to_bin(mode_t m) {
    static char b[10];
    m &= 0777;
    for (int i = 8; i >= 0; i--) {
        b[8 - i] = ((m >> i) & 1) ? '1' : '0';
    }
    b[9] = '\0';
    return b;
}

void p_bin(mode_t m) {
    printf("%s", to_bin(m));
}

void p_sym(mode_t m) {
    char s[10] = "---------";
    if (m & S_IRUSR) s[0] = 'r';
    if (m & S_IWUSR) s[1] = 'w';
    if (m & S_IXUSR) s[2] = 'x';
    if (m & S_IRGRP) s[3] = 'r';
    if (m & S_IWGRP) s[4] = 'w';
    if (m & S_IXGRP) s[5] = 'x';
    if (m & S_IROTH) s[6] = 'r';
    if (m & S_IWOTH) s[7] = 'w';
    if (m & S_IXOTH) s[8] = 'x';
    printf("%s", s);
}

void show_perm(mode_t m) {
    printf("  Oct: %03o\n  Sym: ", m & 0777);
    p_sym(m);
    printf("\n  Bin: ");
    p_bin(m);
    printf("\n\n");
}

mode_t get_sym(const char* s) {
    mode_t m = 0;
    if (strlen(s) < 9) return 0;
    if (s[0] == 'r') m |= S_IRUSR;
    if (s[1] == 'w') m |= S_IWUSR;
    if (s[2] == 'x') m |= S_IXUSR;
    if (s[3] == 'r') m |= S_IRGRP;
    if (s[4] == 'w') m |= S_IWGRP;
    if (s[5] == 'x') m |= S_IXGRP;
    if (s[6] == 'r') m |= S_IROTH;
    if (s[7] == 'w') m |= S_IWOTH;
    if (s[8] == 'x') m |= S_IXOTH;
    return m;
}

mode_t get_oct(const char* s) {
    return (mode_t)strtol(s, NULL, 8) & 0777;
}

mode_t mod_perm(mode_t c, const char* s) {
    mode_t t_m = 0, a_m = 0;
    char op = 0, t_buf[10] = {0}, a_buf[10] = {0};
    int ti = 0, ai = 0, i = 0;
    
    printf("\n--- %s ---\n", s);

    while (s[i] == 'u' || s[i] == 'g' || s[i] == 'o' || s[i] == 'a') {
        t_buf[ti] = s[i];
        mode_t tm = 0;
        if (s[i] == 'u') tm = 0700;
        if (s[i] == 'g') tm = 0070;
        if (s[i] == 'o') tm = 0007;
        if (s[i] == 'a') tm = 0777;
        
        printf("маска %c = %03o (bin: %s)\n", s[i], tm, to_bin(tm));
        t_m |= tm;
        ti++; i++;
    }

    if (ti == 0) {
        t_buf[0] = 'a';
        t_m = 0777;
        printf("маска a = 0777 (bin: %s)\n", to_bin(t_m));
    } else if (ti > 1) {
        printf("маска %s = %03o (bin: %s)\n", t_buf, t_m, to_bin(t_m));
    }
    printf("\n");

    if (s[i] == '+' || s[i] == '-' || s[i] == '=') op = s[i++];
    else return c;

    while (s[i]) {
        a_buf[ai] = s[i];
        mode_t am = 0;
        if (s[i] == 'r') am = 0444;
        if (s[i] == 'w') am = 0222;
        if (s[i] == 'x') am = 0111;
        
        printf("маска %c = %03o (bin: %s)\n", s[i], am, to_bin(am));
        a_m |= am;
        ai++; i++;
    }

    if (ai > 1) printf("маска %s = %03o (bin: %s)\n", a_buf, a_m, to_bin(a_m));
    printf("\n");

    mode_t e_a = t_m & a_m;
    printf("маска %s%c%s = %03o (bin: %s)\n", t_buf, op, a_buf, e_a, to_bin(e_a));

    mode_t n = c;
    if (op == '+') {
        printf("итог: origin ИЛИ %s%c%s\n", t_buf, op, a_buf);
        n |= e_a;
    } else if (op == '-') {
        printf("итог: origin И (~%s%c%s)\n", t_buf, op, a_buf);
        n &= ~e_a;
    } else if (op == '=') {
        printf("итог: (origin И ~%s) ИЛИ %s%c%s\n", t_buf, t_buf, op, a_buf);
        n &= ~t_m;
        n |= e_a;
    }
    printf("---\n");

    return n & 0777;
}
