#ifndef ARDUINO
#include "queue.h"
#include "../testing.h"

TEST_FUNC_BEGIN {
    queue<int, 10> q(-1);
    for (int i = 0; i < 10; ++i) {
        if (!q.push(i)) {
            TEST_FAIL("Queue insertion");
        } else {
            TEST_PASS("Queue insertion");
        }
    }
    if (q.push(17)) {
        TEST_FAIL("Queue over-insertion");
    } else {
        TEST_PASS("Queue over-insertion");
    }
    for (int i = 0; i < 3; ++i) {
        if (q.pop() != i) {
            TEST_FAIL("Queue removal");
        } else {
            TEST_PASS("Queue removal");
        }
    }
    for (int i = 10; i < 13; ++i) {
        if (!q.push(i)) {
            TEST_FAIL("Queue re-insertion");
        } else {
            TEST_PASS("Queue re-insertion");
        }
    }
    if (q.push(20)) {
        TEST_FAIL("Queue over-re-insertion");
    } else {
        TEST_PASS("Queue over-re-insertion");
    }
    for (int i = 3; i < 13; ++i) {
        if (q.pop() != i) {
            TEST_FAIL("Queue re-removal");
        } else {
            TEST_PASS("Queue re-removal");
        }
    }
    if (q.pop() != -1) {
        TEST_FAIL("Queue over-removal");
    } else {
        TEST_PASS("Queue over-removal");
    }
} TEST_FUNC_END("queue_test")

#endif
