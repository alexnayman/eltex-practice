#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

typedef struct Node {
    int data;
    int priority;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} PriorityQueue;

void init_queue(PriorityQueue* q);
void push(PriorityQueue* q, int val, int priority);
int pop_first(PriorityQueue* q, int* val);
int pop_at_priority(PriorityQueue* q, int priority, int* val);
int pop_min_priority(PriorityQueue* q, int min_p, int* val);

#endif
