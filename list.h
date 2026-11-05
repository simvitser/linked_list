#ifndef _LIST_H
#define _LIST_H

#include <stdlib.h>

typedef int listElement_t;
#define LIST_ELEMENT_FORMATER "%-3d"

typedef struct {
    listElement_t data;
    size_t prev;
    size_t next;
} node_t;

typedef struct {
    node_t *data;
    size_t free;

    size_t size;
    size_t capacity;
} list_t;

typedef enum {
    LIST_OK,
    LIST_MEMORY_ERROR,
    LIST_RESIZE_ERROR,
    LIST_POP_ERROR,
    LIST_INSERT_ERROR
} ErrorStatusList;

ErrorStatusList listCtor(list_t *list);

ErrorStatusList listPush(list_t *list, listElement_t el);

ErrorStatusList listInsert(list_t *list, size_t index, listElement_t el);

ErrorStatusList listPop(list_t *list, size_t index, listElement_t *el);
#define listRemove(list, index) listPop(list, index, NULL);

ErrorStatusList listGet(list_t *list, size_t index, listElement_t *el);

void listPrint(list_t *list);

size_t listGetSize(list_t *list);

void listDtor(list_t *list);

#endif
