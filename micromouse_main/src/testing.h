#ifndef TESTING_H
#define TESTING_H
#ifdef ARDUINO
#error Do not do tests on the Arduino!
#endif
#include <stdio.h>
#include <stdlib.h>

#define TEST_FUNC_BEGIN \
int main() { \
    int _pass = 0, _fail = 0;

#define TEST_PASS(name) \
    do { \
        ++_pass; \
        puts("[\033[32mPASS\033[0m] " name); \
    } while (0)

#define TEST_FAIL(name) \
    do { \
        ++_fail; \
        puts("[\033[31mFAIL\033[0m] " name); \
    } while (0)

#define TEST_FUNC_END(name) \
    if (_fail > 0) { \
        if (_pass > 0) { \
            printf("[\033[31mFAILURE\033[0m] " name " results: %d pass, %d fail\n", _pass, _fail); \
        } else { \
            printf("[\033[31mFAILURE\033[0m] " name " failed all %d test%s.\n", _fail, _fail == 1 ? "" : "s"); \
        } \
        return EXIT_FAILURE; \
    } else { \
        printf("[\033[32mSUCCESS\033[0m] " name " ran all %d test%s successfully.\n", _pass, _pass == 1 ? "" : "s"); \
        return EXIT_SUCCESS; \
    } \
}

#endif
