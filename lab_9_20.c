#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define the Stack structure
struct Node {
    int data;
    struct Node* next;
};

struct Stack {
    struct Node* head;
};

struct Queue {
    struct Stack stk;
};

int ops = 0; // Global variable to count operations

// Queue Functions
// n-1 + 8*(n-1) - 1 + 8*(n-2) - 1 .. + 7 = 4*n*(n+1) = 4*10*11 = 440

void push(struct Stack* s, int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->next = s->head;
    s->head = newNode;
    ops++;
}

int pop(struct Stack* s) {
    if (s->head == NULL) {
        printf("Stack is empty\n");
        return -1;
    }
    int value = s->head->data;
    struct Node* temp = s->head;
    s->head = s->head->next;
    free(temp);
    ops++;
    return value;
}

int top(struct Stack* s) {
    if (s->head == NULL) {
        printf("Stack is empty\n");
        return -1;
    }
    ops++;
    return s->head->data;
}

void flip(struct Stack* dest, struct Stack* src) {
    while (src->head != NULL) {
        push(dest, pop(src));
    }
    ops++; // Count flip as one operation
}

void enqueue(struct Queue* que, int value) {
    push(&(que->stk), value);
}

void dequeue(struct Queue* que) {
    struct Stack tmp;
    tmp.head = NULL;
    flip(&tmp, &(que->stk));
    pop(&tmp);
    flip(&(que->stk), &tmp);
}

int front(struct Queue* que) {
    struct Stack tmp;
    tmp.head = NULL;
    flip(&tmp, &(que->stk));
    int res = top(&tmp);
    flip(&(que->stk), &tmp);
    return res;
}

// The main function
int main() {
    struct Queue q;
    q.stk.head = NULL;

    for(int i = 0; i < 10; i++) {
        int before = ops;
        enqueue(&q, i);
        int after = ops;
        printf("Enqueued: %d, Operations: %d\n", i, after - before);
    }

    printf("Total operations for enqueue: %d\n", ops);

    for(int i = 0; i < 10; i++) {
        int before = ops;
        int value = front(&q);
        printf("Front: %d, Operations: %d\n", value, ops - before);
        
        before = ops;
        dequeue(&q);
        printf("Dequeued, Operations: %d\n", ops - before);
    }

    printf("Total operations: %d\n", ops);

    return 0;
}