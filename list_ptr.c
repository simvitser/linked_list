#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "list_ptr.h"
#include "log.h"
#include "common.h"

int GLOBAL_FILE_LOG_PNG_NUM = 0;

static bool listVerifier(list_t *list);

static void listDump(list_t *list);

static node_t *getTail(list_t *list);
static node_t *getHead(list_t *list);

static void makeGraph(list_t *list, bool isBig);

void listCtor(list_t *list) {
    list->fict.next = &list->fict;
    list->fict.prev = &list->fict;
}

node_t *listPush(list_t *list, listElement_t el) {
    assert(listVerifier(list));

    log("try to push " LIST_ELEMENT_FORMATER, el);

    node_t *tail = getTail(list);
    list->size++;
    
    node_t *new_node = (node_t*)calloc(1, sizeof(node_t));
    if (new_node == NULL) {
        logError("can't calloc");
        return NULL;
    }

    new_node->prev = tail;
    new_node->next = &list->fict;
    new_node->data = el;

    list->fict.prev = new_node;
    tail->next = new_node;
    
    logOK("pushed");

    return new_node;
}

node_t *listInsert(list_t *list, size_t index, listElement_t el) {
    assert(listVerifier(list));
    
    log("try to insert " LIST_ELEMENT_FORMATER " to index %zu", el, index);

    if (list->size < index) {
        logError("index too big");
        return NULL;
    }

    node_t *next = listGetNodeByIndex(list, index);
    node_t *prev = (node_t*)next->prev;

    node_t *new_node = (node_t*)calloc(1, sizeof(node_t));
    if (new_node == NULL) {
        logError("can't calloc");
        return NULL;
    }

    new_node->data = el;
    new_node->next = next;
    new_node->prev = prev;

    next->prev = new_node;
    prev->next = new_node;

    list->size++; 

    return new_node;
}

node_t *listGet(list_t *list, size_t index, listElement_t *el) {
    assert(listVerifier(list));
    assert(el);

    if (index >= list->size) {
        logError("get from index > size");
        return NULL;
    }

    node_t *node = listGetNodeByIndex(list, index);

    *el = node->data;

    return node;
}


int listPop(list_t *list, size_t index, listElement_t *el) {
    assert(listVerifier(list));

    log("try to pop index %zu", index);

    if (index >= list->size) {
        logError("pop from index > size");
        return -1;
    }

    node_t *node = listGetNodeByIndex(list, index);

    if (el) *el = node->data;

    node_t *prev = (node_t*)node->prev;
    node_t *next = (node_t*)node->next;

    next->prev = prev;
    prev->next = next;

    free(node);

    list->size--; 

    logOK("popped");

    return 0;
}


size_t listGetSize(list_t *list) {
    assert(list);

    return list->size;
}

static bool listVerifier(list_t *list) {
    listDump(list); 
    return true;
}

void listPrint(list_t *list) {
    assert(listVerifier(list));

    printf("list: {");
    node_t *node = getHead(list);
    for (size_t i = 1; i < list->size; i++) {
        printf(LIST_ELEMENT_FORMATER ", ", node->data);
        node = (node_t*)node->next;
    }
    if (list->size) printf(LIST_ELEMENT_FORMATER, node->data);
    printf("}\n");
}

static void listDump(list_t *list) {
    assert(list);
    
    log("--DUMP-LIST--");

    // log("mem {");
    // for (size_t i = 0; i < list->capacity; i++) {
    //     log("    [%2zu] - data: " LIST_ELEMENT_FORMATER " prev: %2zd, next: %2zu;", i, list->data[i].data, list->data[i].prev, list->data[i].next);
    // }
    // log("}");
    //
    // log("size: %zu, capacity: %zu", list->size, list->capacity);
    // log("free: %zu, head: %zu, tail: %zu", list->free, getHead(list), getTail(list));
    //
    // log("list: {");
    // size_t index  = getHead(list);
    // size_t index2 = getTail(list);
    // for (size_t i = 0; i < list->size; i++) {
    //     if (index < list->capacity && index2 < list->capacity) {
    //         log("    [%2zu] - " LIST_ELEMENT_FORMATER "    " LIST_ELEMENT_FORMATER, i, list->data[index].data, list->data[index2].data);
    //         index  = list->data[index].next;
    //         index2 = list->data[index2].prev;
    //     }
    // }
    // log("}");
    //
    // makeGraph(list, false);
    //
    // char filename[20] = {};
    // sprintf(filename, "gr%d.png", GLOBAL_FILE_LOG_PNG_NUM);
    //
    // char command[60] = {};
    // sprintf(command, "dot graph.txt -Tpng -o %s", filename);
    // system(command);
    // log("<img height=100 src=\"%s\">", filename);
    //
    // GLOBAL_FILE_LOG_PNG_NUM++;
    //
    //
    // makeGraph(list, true);
    //
    // memset(filename, '\0', sizeof(filename));
    // sprintf(filename, "gr%d.png", GLOBAL_FILE_LOG_PNG_NUM);
    //
    // memset(command, '\0', sizeof(command));
    // sprintf(command, "dot graph.txt -Tpng -o %s", filename);
    // system(command);
    // log("<img height=100 src=\"%s\">", filename);
    //
    // GLOBAL_FILE_LOG_PNG_NUM++;

    log("--END-DUMP--");
}

void listDtor(list_t *list) {
    assert(list);

    node_t *node = getHead(list);
    while (node != &list->fict) {
        node = (node_t*)node->next;
        free(node->prev);
    }
    list->size = 0;
}

node_t *listGetNodeByIndex(list_t *list, size_t index) {
    assert(list);

    node_t *node = getHead(list);
    for (size_t i = 0; i < index; i++) {
        node = (node_t*)node->next;
    }
    return node;
}

static node_t *getTail(list_t *list) {
    return (node_t*)list->fict.prev;
}

static node_t *getHead(list_t *list) {
    return (node_t*)list->fict.next;
}

static void makeGraph(list_t *list, bool isBig) {
    return;
    // FILE *file = fopen("graph.txt", "w");
    // if (file == NULL) return;
    //
    // fprintf(file, "digraph {\n");
    // fprintf(file, "rankdir=LR;\n");
    // if (!isBig) fprintf(file, "splines=ortho;\n");
    // fprintf(file, "nodesep=0.5;\n");
    //
    // for (size_t i = 0; i < list->size - 1; i++) {
    //     if (isBig) {
    //         fprintf(file, "V%zu -> V%zu[color = \"blue\", style=\"bold\"];\n", i, i + 1);
    //     } else {
    //         fprintf(file, "V%zu -> V%zu[color = \"white\", style=\"bold\"];\n", i, i + 1);
    //     }
    // }
    //
    // node_t *node = list->fict.next;
    //
    // for (size_t i = 0; i < list->size; i++) { 
    //     fprintf(file, "V%zu[color=", i);
    //
    //     if (i == 0) {
    //         fprintf(file, "\"#800000\", fillcolor=\"#FFC0C0\", ");
    //     } else if (node == getHead(list)) {
    //         fprintf(file, "\"#808000\", fillcolor=\"#FFFFC0\", ");
    //     } else if (node == getTail(list)) {
    //         fprintf(file, "\"#000080\", fillcolor=\"#C0C0FF\", ");
    //     } else {
    //         fprintf(file, "\"#008000\", fillcolor=\"#C0FFC0\", ");
    //     }
    //
    //     fprintf(file, "shape=\"Mrecord\",style=\"filled\", label = \" index = %zu | {next = %zu | prev = %zd} | data =" LIST_ELEMENT_FORMATER "\"];\n", i, next, (ssize_t)prev, list->data[i].data);
    //
    //     // if (isBig) {
    //     //     if (next != list->capacity) fprintf(file, "V%zu -> V%zu[color=\"darkgreen\", label=\"%zu\", weight=10];\n", i, next, next);
    //     //     if (prev != -1)             fprintf(file, "V%zu -> V%zu[color=\"darkred\"  , label=\"%zu\", weight=10];\n", i, prev, prev);
    //     // } else {
    //     //     if (next != list->capacity) fprintf(file, "V%zu -> V%zu[color=\"darkgreen\", constraint=false];\n", i, next);
    //     //     if (prev != -1)             fprintf(file, "V%zu -> V%zu[color=\"darkred\"  , constraint=false];\n", i, prev);
    //     // }
    //
    //     if (isBig) {
    //         if (next < list->capacity) fprintf(file, "V%zu -> V%zu[color=\"darkgreen\", label=\"%zu\", weight=10];\n", i, next, next);
    //         if (prev != POIZON)        fprintf(file, "V%zu -> V%zu[color=\"darkred\"  , label=\"%zu\", weight=10];\n", i, prev, prev);
    //     } else {
    //         if (prev == POIZON) {
    //             if (next != list->capacity) fprintf(file, "V%zu -> V%zu[color=\"blue\"]\n", i, next);
    //         } else {
    //             if (next < list->capacity) {
    //                 if (list->data[next].prev == i) {
    //                     fprintf(file, "V%zu -> V%zu[color=\"darkgreen\", constraint=false, dir=\"both\"];\n", i, next);
    //                 } else {
    //                     fprintf(file, "V%zu -> V%zu[color=\"red\", style=\"bold\", constraint=false];\n", i, next);
    //                 }
    //             } else {
    //                 fprintf(file, "V%zu -> TOOBIG[color=\"red\", style=\"bold\", constraint=false]\n", i);
    //             }
    //         }
    //     }
    // }
    // fprintf(file, "FREE->V%zu\n", list->free);
    //
    // fprintf(file, "}\n");
    // fclose(file);
}

