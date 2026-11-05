#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "log.h"
#include "common.h"

#define BASE_SIZE 200
#define POIZON (size_t)(-1)

#define RESIZE_IF_NEED(list) \
if (list->size >= list->capacity - 1) { \
    ErrorStatusList err = listResize(list, list->capacity * 2 + 1); \
    if (err != LIST_OK) { \
        return err; \
    } \
}

int GLOBAL_FILE_LOG_PNG_NUM = 0;

static bool listVerifier(list_t *list);

static ErrorStatusList listResize(list_t *list, size_t size);

static size_t getRealIndex(list_t *list, size_t index);

static void listDump(list_t *list);

static size_t getHead(list_t *list);
static size_t getTail(list_t *list);

static void makeGraph(list_t *list, bool isBig);


ErrorStatusList listCtor(list_t *list) {
    assert(list);

    log("start listCtor");

    list->data = (node_t*)calloc(BASE_SIZE + 1, sizeof(node_t));
    if (list->data == NULL) {
        logError("error calloc");
        return LIST_MEMORY_ERROR;
    }
     
    list->free = 1;
    list->size = 0;
    list->capacity = BASE_SIZE;

    for (size_t i = 1; i <= BASE_SIZE; i++) {
        list->data[i].prev = POIZON;
        list->data[i].next = i + 1;
    }

    logOK("sucessfull init list");
    return LIST_OK;
}

ErrorStatusList listPush(list_t *list, listElement_t el) {
    assert(listVerifier(list));

    log("try to push " LIST_ELEMENT_FORMATER, el);

    RESIZE_IF_NEED(list);

    // if (list->size == 0) {
    //     list->data[list->free].data = el;
    //     list->data[list->free].next = 0;
    //     list->data[list->free].prev = 0;
    //
    //     list->data[0].next = 1;
    //     list->data[0].prev = 1;
    //
    //     list->free = new_;
    //     list->size++;
    //
    //     return LIST_OK;
    // }

    list->data[getTail(list)].next = list->free; // теперь хвост там где пусто (пока что пусто)

    list->data[list->free].data = el; // пушим

    size_t new_free = list->data[list->free].next; // новый фри тут

    list->data[list->free].next = 0;  // следующий за хвостом - нулевой
    list->data[list->free].prev = getTail(list); // подвязываем к новому прошлый хвост
    
    list->data[0].prev = list->free; // обновляем хвост
    
    list->free = new_free;

    list->size++;

    logOK("pushed");

    return LIST_OK;
}

ErrorStatusList listInsert(list_t *list, size_t index, listElement_t el) {
    assert(listVerifier(list));
    
    log("try to insert " LIST_ELEMENT_FORMATER " to index %zu", el, index);

    if (list->size < index) {
        return LIST_INSERT_ERROR;
    }

    if (list->size == list->capacity) {
        ErrorStatusList err = listResize(list, list->capacity * 2);
        if (err != LIST_OK) {
            return err;
        }
    }

    size_t real_index = getRealIndex(list, index);

    list->data[list->free].data = el;

    size_t new_free = list->data[list->free].next;

    list->data[list->free].next = real_index;
    list->data[list->free].prev = list->data[real_index].prev;

    list->data[list->data[real_index].prev].next = list->free;
    list->data[real_index].prev = list->free;

    list->free = new_free;

    list->size++; 

    return LIST_OK;
}

ErrorStatusList listGet(list_t *list, size_t index, listElement_t *el) {
    assert(listVerifier(list));
    assert(el);

    if (index >= list->size) {
        logError("pop from idex > size");
        return LIST_POP_ERROR;
    }

    size_t real_index = getRealIndex(list, index);

    *el = list->data[real_index].data;

    return LIST_OK;
}


ErrorStatusList listPop(list_t *list, size_t index, listElement_t *el) {
    assert(listVerifier(list));

    log("try to pop index %zu", index);

    if (index >= list->size) {
        logError("pop from idex > size");
        return LIST_POP_ERROR;
    }

    size_t real_index = getRealIndex(list, index);
    log("index: %zu, real_index: %zu", index, real_index);

    if (el) *el = list->data[real_index].data;
 
    list->data[list->data[real_index].prev].next = list->data[real_index].next;
    list->data[list->data[real_index].next].prev = list->data[real_index].prev;

    list->data[real_index].data = 0;
    list->data[real_index].next = list->free;
    list->data[real_index].prev = POIZON;
    list->free = real_index;

    list->size--; 

    logOK("popped");

    return LIST_OK;
}


size_t listGetSize(list_t *list) {
    assert(list);

    return list->size;
}

static bool listVerifier(list_t *list) {
    // listDump(list);

    // for (size_t i = 0; i < list->capacity; i++) {
    //     if (list->data[i].next > list->capacity) return false;
    //     if (list->data[i].prev > list->capacity && list->data[i].prev != POIZON) return false;
    // }

    if (list->size > list->capacity) return false;

    return true;
}

static ErrorStatusList listResize(list_t *list, size_t size) {
    assert(list);
    assert(size >= BASE_SIZE);

    log("try to resize list from sz: %zu, to sz: %zu", list->capacity, size);

    node_t *ptr = (node_t*)realloc(list->data, sizeof(node_t) * size);
    if (ptr == NULL) {
        logError("can't realloc\n");
        return LIST_RESIZE_ERROR;
    }

    list->data = ptr;
    
    for (size_t i = list->capacity; i < size; i++) {
        list->data[i].prev = POIZON;
        list->data[i].next = i + 1;
    }

    list->capacity = size;

    logOK("resized");
    return LIST_OK;
}

void listPrint(list_t *list) {
    assert(listVerifier(list));

    printf("list: {");
    size_t index = getHead(list);
    for (size_t i = 1; i < list->size; i++) {
        printf(LIST_ELEMENT_FORMATER ", ", list->data[index].data);
        index = list->data[index].next;
    }
    if (list->size) printf(LIST_ELEMENT_FORMATER, list->data[index].data);
    printf("}\n");
}

static void listDump(list_t *list) {
    assert(list);
    
    log("--DUMP-LIST--");

    log("mem {");
    for (size_t i = 0; i < list->capacity; i++) {
        log("    [%2zu] - data: " LIST_ELEMENT_FORMATER " prev: %2zd, next: %2zu;", i, list->data[i].data, list->data[i].prev, list->data[i].next);
    }
    log("}");

    log("size: %zu, capacity: %zu", list->size, list->capacity);
    log("free: %zu, head: %zu, tail: %zu", list->free, getHead(list), getTail(list));

    log("list: {");
    size_t index  = getHead(list);
    size_t index2 = getTail(list);
    for (size_t i = 0; i < list->size; i++) {
        if (index < list->capacity && index2 < list->capacity) {
            log("    [%2zu] - " LIST_ELEMENT_FORMATER "    " LIST_ELEMENT_FORMATER, i, list->data[index].data, list->data[index2].data);
            index  = list->data[index].next;
            index2 = list->data[index2].prev;
        }
    }
    log("}");
   
    makeGraph(list, false);

    char filename[20] = {};
    sprintf(filename, "gr%d.png", GLOBAL_FILE_LOG_PNG_NUM);

    char command[60] = {};
    sprintf(command, "dot graph.txt -Tpng -o %s", filename);
    system(command);
    log("<img height=100 src=\"%s\">", filename);

    GLOBAL_FILE_LOG_PNG_NUM++;


    makeGraph(list, true);

    memset(filename, '\0', sizeof(filename));
    sprintf(filename, "gr%d.png", GLOBAL_FILE_LOG_PNG_NUM);

    memset(command, '\0', sizeof(command));
    sprintf(command, "dot graph.txt -Tpng -o %s", filename);
    system(command);
    log("<img height=100 src=\"%s\">", filename);

    GLOBAL_FILE_LOG_PNG_NUM++;

    log("--END-DUMP--");
}

void listDtor(list_t *list) {
    assert(list);

    free(list->data);
}

static size_t getRealIndex(list_t *list, size_t index) {
    assert(list);

    size_t real_index = getHead(list);
    for (size_t i = 0; i < index; i++) real_index = list->data[real_index].next;
    return real_index;
}

static size_t getHead(list_t *list) {
    return list->data[0].next;
}

static size_t getTail(list_t *list) {
    return list->data[0].prev;
}

static void makeGraph(list_t *list, bool isBig) {
    FILE *file = fopen("graph.txt", "w");
    if (file == NULL) return;

    fprintf(file, "digraph {\n");
    fprintf(file, "rankdir=LR;\n");
    if (!isBig) fprintf(file, "splines=ortho;\n");
    fprintf(file, "nodesep=0.5;\n");
    
    for (size_t i = 0; i < list->capacity - 1; i++) {
        if (isBig) {
            fprintf(file, "V%zu -> V%zu[color = \"blue\", style=\"bold\"];\n", i, i + 1);
        } else {
            fprintf(file, "V%zu -> V%zu[color = \"white\", style=\"bold\"];\n", i, i + 1);
        }
    }
    
    for (size_t i = 0; i < list->capacity; i++) {
        size_t next = list->data[i].next;
        size_t prev = list->data[i].prev;
        
        fprintf(file, "V%zu[color=", i);

        if (i == 0) {
            fprintf(file, "\"#800000\", fillcolor=\"#FFC0C0\", ");
        } else if (prev == (size_t)-1) {
            fprintf(file, "\"#000000\", fillcolor=\"#C0C0C0\", ");
        } else if (i == getHead(list)) {
            fprintf(file, "\"#808000\", fillcolor=\"#FFFFC0\", ");
        } else if (i == getTail(list)) {
            fprintf(file, "\"#000080\", fillcolor=\"#C0C0FF\", ");
        } else {
            fprintf(file, "\"#008000\", fillcolor=\"#C0FFC0\", ");
        }

        fprintf(file, "shape=\"Mrecord\",style=\"filled\", label = \" index = %zu | {next = %zu | prev = %zd} | data =" LIST_ELEMENT_FORMATER "\"];\n", i, next, (ssize_t)prev, list->data[i].data);
        
        // if (isBig) {
        //     if (next != list->capacity) fprintf(file, "V%zu -> V%zu[color=\"darkgreen\", label=\"%zu\", weight=10];\n", i, next, next);
        //     if (prev != -1)             fprintf(file, "V%zu -> V%zu[color=\"darkred\"  , label=\"%zu\", weight=10];\n", i, prev, prev);
        // } else {
        //     if (next != list->capacity) fprintf(file, "V%zu -> V%zu[color=\"darkgreen\", constraint=false];\n", i, next);
        //     if (prev != -1)             fprintf(file, "V%zu -> V%zu[color=\"darkred\"  , constraint=false];\n", i, prev);
        // }

        if (isBig) {
            if (next < list->capacity) fprintf(file, "V%zu -> V%zu[color=\"darkgreen\", label=\"%zu\", weight=10];\n", i, next, next);
            if (prev != POIZON)        fprintf(file, "V%zu -> V%zu[color=\"darkred\"  , label=\"%zu\", weight=10];\n", i, prev, prev);
        } else {
            if (prev == POIZON) {
                if (next != list->capacity) fprintf(file, "V%zu -> V%zu[color=\"blue\"]\n", i, next);
            } else {
                if (next < list->capacity) {
                    if (list->data[next].prev == i) {
                        fprintf(file, "V%zu -> V%zu[color=\"darkgreen\", constraint=false, dir=\"both\"];\n", i, next);
                    } else {
                        fprintf(file, "V%zu -> V%zu[color=\"red\", style=\"bold\", constraint=false];\n", i, next);
                    }
                } else {
                    fprintf(file, "V%zu -> TOOBIG[color=\"red\", style=\"bold\", constraint=false]\n", i);
                }
            }
        }
    }
    fprintf(file, "FREE->V%zu\n", list->free);

    fprintf(file, "}\n");
    fclose(file);
}

