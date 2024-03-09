#include <stdio.h>
#include <stdlib.h>
#include "lib/code_status.h"
#include "lib/dequeue.h"
#include "lib/patient.h"

int
main() {
    Patient p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12;
    Dequeue* dequeue = NULL;
    void* data = malloc(sizeof(Patient));
    init_patient(&p1);
    init_patient(&p2);
    init_patient(&p3);
    init_patient(&p4);
    init_patient(&p5);
    init_patient(&p6);
    init_patient(&p7);
    init_patient(&p8);
    init_patient(&p9);
    init_patient(&p10);
    init_patient(&p11);
    init_patient(&p12);
    init_dequeue(&dequeue, 15);
    fill_patient(&p1, "a", 0, 0, 0);
    fill_patient(&p2, "b", 0, 0, 0);
    fill_patient(&p3, "c", 0, 0, 0);
    fill_patient(&p4, "d", 0, 0, 0);
    fill_patient(&p5, "e", 0, 0, 0);
    fill_patient(&p6, "f", 0, 0, 0);
    fill_patient(&p7, "g", 0, 0, 0);
    fill_patient(&p8, "h", 0, 0, 0);
    fill_patient(&p9, "j", 0, 0, 0);
    fill_patient(&p10, "k", 0, 0, 0);
    fill_patient(&p11, "l", 0, 0, 0);
    fill_patient(&p12, "m", 0, 0, 0);
    push_back(dequeue, &p1);
    print_dequeue(dequeue, print_patient);
    push_back(dequeue, &p2);
    print_dequeue(dequeue, print_patient);
    push_back(dequeue, &p3);
    print_dequeue(dequeue, print_patient);
    push_back(dequeue, &p4);
    print_dequeue(dequeue, print_patient);
    push_back(dequeue, &p5);
    print_dequeue(dequeue, print_patient);
    pop_back(dequeue, data);
    print_dequeue(dequeue, print_patient);
    pop_back(dequeue, data);
    print_dequeue(dequeue, print_patient);
    push_back(dequeue, &p6);
    print_dequeue(dequeue, print_patient);
    push_back(dequeue, &p7);
    print_dequeue(dequeue, print_patient);
    push_back(dequeue, &p8);
    print_dequeue(dequeue, print_patient);
    pop_back(dequeue, data);
    print_dequeue(dequeue, print_patient);
    push_back(dequeue, &p9);
    print_dequeue(dequeue, print_patient);
    push_back(dequeue, &p10);
    print_dequeue(dequeue, print_patient);
    push_back(dequeue, &p11);
    print_dequeue(dequeue, print_patient);
    push_back(dequeue, &p12);
    print_dequeue(dequeue, print_patient);
    pop_back(dequeue, data);
    print_dequeue(dequeue, print_patient);
    pop_back(dequeue, data);
    print_dequeue(dequeue, print_patient);
    pop_back(dequeue, data);
    print_dequeue(dequeue, print_patient);
    pop_back(dequeue, data);
    print_dequeue(dequeue, print_patient);
    pop_back(dequeue, data);
    print_dequeue(dequeue, print_patient);
    pop_back(dequeue, data);
    print_dequeue(dequeue, print_patient);
    pop_back(dequeue, data);
    print_dequeue(dequeue, print_patient);
    pop_back(dequeue, data);
    print_dequeue(dequeue, print_patient);
    pop_back(dequeue, data);
    print_dequeue(dequeue, print_patient);
    pop_back(dequeue, data);
    print_dequeue(dequeue, print_patient);
    pop_back(dequeue, data);
    print_dequeue(dequeue, print_patient);
    pop_back(dequeue, data);
    print_dequeue(dequeue, print_patient);
    dealloc_dequeue(dequeue);
    free(data);
    return 0;
}
