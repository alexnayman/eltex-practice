#include <stdlib.h>
#include "priority_queue.h"

void init_queue(PriorityQueue* q) {
    q->head = NULL;
}

void push(PriorityQueue* q, int val, int priority) {
    Node* new_node = malloc(sizeof(Node));
    new_node->data = val;
    new_node->priority = priority;

    if (!q->head || priority > q->head->priority) {
        new_node->next = q->head;
        q->head = new_node;
    } else {
        Node* curr = q->head;
        while (curr->next && curr->next->priority >= priority) {
            curr = curr->next;
        }
        new_node->next = curr->next;
        curr->next = new_node;
    }
}

int pop_first(PriorityQueue* q, int* val) {
    if (!q->head) return 0;
    Node* temp = q->head;
    *val = temp->data;
    q->head = q->head->next;
    free(temp);
    return 1;
}

int pop_at_priority(PriorityQueue* q, int priority, int* val) {
    if (!q->head) return 0;

    if (q->head->priority == priority) {
        return pop_first(q, val);
    }

    Node* curr = q->head;
    while (curr->next && curr->next->priority != priority) {
        curr = curr->next;
    }

    if (curr->next) {
        Node* temp = curr->next;
        *val = temp->data;
        curr->next = temp->next;
        free(temp);
        return 1;
    }
    return 0;
}

int pop_min_priority(PriorityQueue* q, int min_p, int* val) {
    if (!q->head || q->head->priority < min_p) return 0;
    return pop_first(q, val);
}
