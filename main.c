#include <stdio.h>

#include "list_ptr.h"
#include "log.h"

int main() {
    // logInit("log.htm");
    log("<pre>");
    log("<h1>log of link list</h1>");

    list_t list = {};
    listCtor(&list);
    listPrint(&list);

    const int gg = 100000000;

    for (int i = 0; i < gg; i++) {
        listPush(&list, i);
    }

    while (listGetSize(&list) > 0) listRemove(&list, 0);

    // for (int i = 0; i < gg; i++) {
    //     listRemove(&list, (i + 100) % gg);
    // }



    // listPush(&list, 10);
    // listPush(&list, 20);
    // listPush(&list, 30);
    // listPush(&list, 40);
    // listPush(&list, 50);
    // listPrint(&list);
    //
    // listInsert(&list, 0, 6767);
    // listInsert(&list, 2, 77);
    // listInsert(&list, 4, 67);
    // listPrint(&list);
    //
    // listRemove(&list, 1);
    // listPrint(&list);
    //
    // listPush(&list, 87);
    // listPrint(&list);
    //
    // listRemove(&list, 0);
    // listPrint(&list);
    // listRemove(&list, 3);
    // listPrint(&list);


    // list.data[5].next = -5;
    // listPrint(&list);
    //
    // list.data[5].next--;
    // list.data[5].prev++;
    // listPrint(&list);

    
    // ErrorStatusList err = listRemove(&list, 2);
    // printf("err rem: %d\n", err);
    //
    // err = listInsert(&list, 7, 6767);
    // printf("err ins: %d\n", err);
    
    // listPush(&list, 10);
    // listPush(&list, 20);
    // listPrint(&list);
    // listRemove(&list, 0);
    // listRemove(&list, 0);
    //
    // listPrint(&list);
    //
    // listPush(&list, 30);
    // listPush(&list, 40);
    // listPush(&list, 50);
    // listPrint(&list);
    // listRemove(&list, 0);
    // listRemove(&list, 0);
    //
    // listPrint(&list);


    listDtor(&list);
    logDestroy();
    return 0;
}
