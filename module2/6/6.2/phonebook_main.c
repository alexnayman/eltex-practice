#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "phonebook_funcs.h"

int main() {
    Node *head = NULL;
    int c;

    while (1) {
        printf("\n1.Список 2.Добавить 3.Поиск 4.Удалить 5.Ред 0.Выход\n> ");
        if (scanf("%d", &c) != 1) { clean(); continue; }
        clean();

        if (c == 0) break;
        if (c == 1) show_all(head);
        if (c == 2) head = add_ordered(head);
        if (c == 3) { 
            Node *res = find_node(head); 
            if (res) show_one(&res->data, 0); 
        }
        if (c == 4) {
            Node *res = find_node(head);
            if (res) head = del_node(head, res);
        }
	if (c == 5) {
            Node *res = find_node(head);
            if (res) {
                printf("\n--- РЕДАКТИРОВАНИЕ ---\n");
                printf("1: ФИО\n2: Работа\n3: Должность\n4: Телефон\n5: Email\n6: Соцсети\n");
                printf("Введите номера полей (например, '14' для ФИО и Тел): ");

                char s[10];
                scanf("%9s", s);
                clean();

                for(int i = 0; s[i]; i++) {
                    int field_num = s[i] - '0';
                    if (field_num >= 1 && field_num <= 6) {
                        edit_node(res, 1, field_num);
                    } else {
                        printf("! Номер %d игнорирован (неверное поле)\n", field_num);
                    }
                }
            }
        }
    }
    free_list(head);
    return 0;
}
