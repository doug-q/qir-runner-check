// build with: gcc teleport.c -Ltarget/debug/ -lqir_backend -lm -o teleport
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

void __quantum__rt__initialize(char*);
void* __quantum__rt__qubit_allocate();
void __quantum__rt__qubit_release(void*);
void __quantum__qis__h__body(void*);
void __quantum__qis__z__body(void*);
void __quantum__qis__x__body(void*);
void* __quantum__qis__m__body(void*);
void __quantum__qis__cx__body(void*, void*);
bool __quantum__rt__result_equal(void*, void*);
void* __quantum__rt__result_get_one();
void* __quantum__rt__result_get_zero();
void __quantum__qis__reset__body(void*);

void __quantum__rt__bool_record_output(bool, char*);

bool measure(void* qb) {
    void* r_0 = __quantum__rt__result_get_zero();
    void* r_1 = __quantum__rt__result_get_one();
    void* r = __quantum__qis__m__body(qb);
    if(r == r_0) { return false; }
    if(r == r_1) { return true; }

    fprintf(stderr, "Error: measure: result is neither 0 nor 1\n");
    abort();
}

void teleport(void* src, void* dest) {
    void* ancilla = __quantum__rt__qubit_allocate();
    __quantum__qis__reset__body(ancilla);
    __quantum__qis__h__body(ancilla);
    __quantum__qis__cx__body(ancilla, dest);
    __quantum__qis__cx__body(src, ancilla);
    __quantum__qis__h__body(src);
    if (measure(src)) {
        __quantum__qis__z__body(dest);
    }
    if (measure(ancilla)) {
        __quantum__qis__x__body(dest);
    }
}

int main() {
    __quantum__rt__initialize("dougrulz");

    void* qb0 = __quantum__rt__qubit_allocate();
    void* qb1 = __quantum__rt__qubit_allocate();

    // Initialise the source state
    __quantum__qis__reset__body(qb0);
    __quantum__qis__x__body(qb0);

    teleport(qb0,qb1);
    __quantum__rt__bool_record_output(measure(qb1), NULL);


    __quantum__rt__qubit_release(qb1);
    __quantum__rt__qubit_release(qb0);
}
