#include "queue.h"

int main() {
    NODE* SD_Qptr = malloc(sizeof(NODE));
    SD_Qptr -> nextPtr = NULL;

    printq(&SD_Qptr);
    enq(&SD_Qptr, 4);
    enq(&SD_Qptr, 5);
    int num = deq(&SD_Qptr);
    printf("%d\n", num);
    printq(&SD_Qptr);
    return 0;

}