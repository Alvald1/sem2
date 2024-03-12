#include "lib/code_status.h"
#include "lib/dequeue.h"
#include "lib/task.h"

int
main() {
    Dequeue* dequeue;
    init_dequeue(&dequeue, 15);
    task(dequeue);
    dealloc_dequeue(dequeue);
    return 0;
}
