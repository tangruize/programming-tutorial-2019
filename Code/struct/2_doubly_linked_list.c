//
// Created by Ruize Tang on 2019/11/15.
//

#include <stdio.h>
#include <stdlib.h>

struct list {
    int data;
    struct list *previous;
    struct list *next;
} _head, *head = &_head, *tail = &_head;

typedef struct list list;

void insert_node(list *p, int data) {
    list *node = (list *)malloc(sizeof(list));
    node->data = data;
    node->previous = p;
    node->next = p->next;
    if (p->next)
        p->next->previous = node;
    else
        tail = node;
    p->next = node;
}

void delete_node(list *p) {
    p->previous->next = p->next;
    if (p->next)
        p->next->previous = p->previous;
    else
        tail = p->previous;
    free(p);
}

int main() {
    for (int i = 0; i < 3; ++i)
        insert_node(tail, i);
    for (list *p = head->next; p != NULL; p = p->next)
        printf("%d\n", p->data);
    delete_node(head->next->next);
    delete_node(tail);
    delete_node(head->next);
    return 0;
}
