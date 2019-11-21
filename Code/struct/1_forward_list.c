//
// Created by Ruize Tang on 2019/11/15.
//

#include <stdio.h>
#include <stdlib.h>

struct forward_list {
    int data;
    struct forward_list *next;
} _head, *head = &_head, *tail = &_head;

typedef struct forward_list forward_list;

int main() {
    for (int i = 0; i < 3; ++i) {
        tail->next = (forward_list *)malloc(sizeof(forward_list));
        tail = tail->next;
        tail->data = i;
        tail->next = NULL;
    }
    for (forward_list *p = head->next, *q = p; p != NULL; q = p) {
        printf("%d\n", p->data);
        p = p->next;
        free(q);
        head->next = p;
        if (tail == q)
            tail = head;
    }
    return 0;
}
