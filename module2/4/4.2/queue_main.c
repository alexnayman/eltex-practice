#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "priority_queue.h"

void print_queue(PriorityQueue* q) {
    printf("  [Queue State]: ");
    if (!q->head) {
        printf("Empty\n");
        return;
    }
    Node* curr = q->head;
    while (curr) {
        printf("(P:%d, V:%d) -> ", curr->priority, curr->data);
        curr = curr->next;
    }
    printf("NULL\n");
}

int main() {
    PriorityQueue q;
    init_queue(&q);
    srand(time(NULL));

    printf("=== 1. GENERATING MESSAGES ===\n");
    for (int i = 0; i < 6; i++) {
        int val = rand() % 100;
        int p = rand() % 256;
        printf("Action: Push Value %d with Priority %d\n", val, p);
        push(&q, val, p);
        print_queue(&q);
    }

    int val;
    printf("\n=== 2. POP FIRST (HIGHEST PRIORITY) ===\n");
    if (pop_first(&q, &val)) {
        printf("Result: Extracted highest priority element with value %d\n", val);
    }
    print_queue(&q);

    printf("\n=== 3. POP BY EXACT PRIORITY ===\n");
    int targets[] = {100, 150, 200}; // Проверим несколько
    for(int i = 0; i < 3; i++) {
        printf("Action: Searching for exact priority %d...\n", targets[i]);
        if (pop_at_priority(&q, targets[i], &val)) {
            printf("Result: SUCCESS! Found and extracted value %d\n", val);
        } else {
            printf("Result: NOTICE: No message with priority %d exists.\n", targets[i]);
        }
        print_queue(&q);
    }

    printf("\n=== 4. POP WITH PRIORITY >= THRESHOLD ===\n");
    int threshold = 128;
    printf("Action: Searching for any priority >= %d...\n", threshold);
    if (pop_min_priority(&q, threshold, &val)) {
        printf("Result: SUCCESS! Extracted value %d\n", val);
    } else {
        printf("Result: NOTICE: Nothing found with priority >= %d\n", threshold);
    }
    print_queue(&q);

    printf("\n=== 5. CLEARING REMAINING QUEUE ===\n");
    while (pop_first(&q, &val)) {
        printf("Action: Popping... Value: %d\n", val);
    }
    print_queue(&q);

    return 0;
}
