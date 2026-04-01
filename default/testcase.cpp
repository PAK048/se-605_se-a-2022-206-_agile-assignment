#include "catch.hpp"
#include "dynarray.h"

// ==========================================
//   PUSH TESTS
// ==========================================
void test_push_single() {
    DynArray da;
    initArray(da, 2);
    push(da, 10);
    REQUIRE(da.count == 1);
    REQUIRE(da.data[0] == 10);
    freeArray(da);
}

void test_push_fill_capacity() {
    DynArray da;
    initArray(da, 2);
    push(da, 1);
    push(da, 2);
    REQUIRE(da.count == 2);
    REQUIRE(da.capacity == 2);
    REQUIRE(da.data[0] == 1);
    REQUIRE(da.data[1] == 2);
    freeArray(da);
}

void test_push_triggers_grow() {
    DynArray da;
    initArray(da, 2);
    push(da, 1);
    push(da, 2);
    push(da, 3);
    REQUIRE(da.count == 3);
    REQUIRE(da.capacity == 4);
    REQUIRE(da.data[0] == 1);
    REQUIRE(da.data[1] == 2);
    REQUIRE(da.data[2] == 3);
    freeArray(da);
}

void test_push_multiple_grows() {
    DynArray da;
    initArray(da, 1);
    for (int i = 0; i < 10; i++) {
        push(da, i * 10);
    }
    REQUIRE(da.count == 10);
    REQUIRE(da.capacity >= 10);
    for (int i = 0; i < 10; i++) {
        REQUIRE(da.data[i] == i * 10);
    }
    freeArray(da);
}

// ==========================================
//   POP TESTS
// ==========================================
void test_pop_single() {
    DynArray da;
    initArray(da, 2);
    push(da, 42);
    int val = pop(da);
    REQUIRE(val == 42);
    REQUIRE(da.count == 0);
    freeArray(da);
}

void test_pop_empty() {
    DynArray da;
    initArray(da, 2);
    int val = pop(da);
    REQUIRE(val == -1);
    REQUIRE(da.count == 0);
    freeArray(da);
}

void test_pop_order() {
    DynArray da;
    initArray(da, 4);
    push(da, 10);
    push(da, 20);
    push(da, 30);
    REQUIRE(pop(da) == 30);
    REQUIRE(pop(da) == 20);
    REQUIRE(pop(da) == 10);
    REQUIRE(da.count == 0);
    freeArray(da);
}

// ==========================================
//   AUTO GROW TESTS
// ==========================================
void test_grow_doubles_capacity() {
    DynArray da;
    initArray(da, 4);
    da.count = 4;
    for (int i = 0; i < 4; i++) da.data[i] = i + 1;
    autoGrow(da);
    REQUIRE(da.capacity == 8);
    REQUIRE(da.count == 4);
    for (int i = 0; i < 4; i++) {
        REQUIRE(da.data[i] == i + 1);
    }
    freeArray(da);
}

void test_grow_preserves_data() {
    DynArray da;
    initArray(da, 2);
    push(da, 100);
    push(da, 200);
    push(da, 300);
    REQUIRE(da.data[0] == 100);
    REQUIRE(da.data[1] == 200);
    REQUIRE(da.data[2] == 300);
    freeArray(da);
}

// ==========================================
//   PUSH + POP COMBINED
// ==========================================
void test_push_pop_interleaved() {
    DynArray da;
    initArray(da, 2);
    push(da, 5);
    push(da, 10);
    REQUIRE(pop(da) == 10);
    push(da, 15);
    push(da, 20);
    REQUIRE(da.count == 3);
    REQUIRE(da.data[0] == 5);
    REQUIRE(da.data[1] == 15);
    REQUIRE(da.data[2] == 20);
    freeArray(da);
}

// ==========================================
//   TEST REGISTRATION
// ==========================================
namespace {
    Catch::Registrar r1("Push: Single element", test_push_single);
    Catch::Registrar r2("Push: Fill to capacity", test_push_fill_capacity);
    Catch::Registrar r3("Push: Triggers auto grow", test_push_triggers_grow);
    Catch::Registrar r4("Push: Multiple grows from capacity 1", test_push_multiple_grows);
    Catch::Registrar r5("Pop: Single element", test_pop_single);
    Catch::Registrar r6("Pop: Empty array returns -1", test_pop_empty);
    Catch::Registrar r7("Pop: LIFO order", test_pop_order);
    Catch::Registrar r8("AutoGrow: Doubles capacity", test_grow_doubles_capacity);
    Catch::Registrar r9("AutoGrow: Preserves existing data", test_grow_preserves_data);
    Catch::Registrar r10("Combined: Push and pop interleaved", test_push_pop_interleaved);
}
