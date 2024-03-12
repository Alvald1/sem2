#include "sort.h"

void
swap(Patient** patient_1, Patient** patient_2) {
    Patient* temp;
    temp = *patient_1;
    *patient_1 = *patient_2;
    *patient_2 = temp;
}

void
gnome_sort(Patients* patients) {
    size_t index = 0;
    size_t len = patients->len;
    Patient** arr = patients->arr;
    while (index < len) {
        if (index == 0) {
            index++;
        }
        if ((*(arr + index))->ta >= (*(arr + index - 1))->ta) {
            index++;
        } else {
            swap((arr + index), (arr + index - 1));
            index--;
        }
    }
}