#ifndef _LIST_PTR_H
#define _LIST_PTR_H

#include <stdlib.h>

typedef int listElement_t;
#define LIST_ELEMENT_FORMATER "%-3d"

typedef struct {
    listElement_t data;
    void *prev;
    void *next;
} node_t;

typedef struct {
    node_t fict;

    size_t size;
} list_t;

node_t *listPush(list_t *list, listElement_t el);

void listCtor(list_t *list);

node_t *listInsert(list_t *list, size_t index, listElement_t el);

int listPop(list_t *list, size_t index, listElement_t *el);
#define listRemove(list, index) listPop(list, index, NULL);

node_t* listGet(list_t *list, size_t index, listElement_t *el);

void listPrint(list_t *list);

node_t *listGetNodeByIndex(list_t *list, size_t index);

size_t listGetSize(list_t *list);

void listDtor(list_t *list);

#endif
