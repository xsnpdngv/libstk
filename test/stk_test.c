/**
 * @file     stk_test.c
 * @brief    expanding stack unit tests utilizing the cmocka framework
 * @author   Tamas Dezso
 * @date     January 3, 2017
 * @version  1.0
 */


#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmocka.h>

#include "stk.h"


/* ----- macros ------------------------------------------------------------ */


/** exact meaning of many when it comes to mass testing */
#define MANY 100000


/* ----- functions --------------------------------------------------------- */


/** tests typed pushes and pops */
static void test_pushPop()
{
    stk_t *s = stkNew();
    void *pop;

    assert_non_null(s);
    assert_true(stkIsEmpty(s));

    stkPushInt(s, 1);
    stkPushStr(s, "2");
    stkPushDbl(s, 3.0);
    stkPushInt(s, 3); stkValInt(s)++; // 4
    stkPushChr(s, '5');
    stkPushPtr(s, s);

    assert_false(stkIsEmpty(s));
    assert_true(stkIsPtr(s));
    assert_ptr_equal(stkValPtr(s), s);
    assert_false(stkIsStr(s));

    stkPop(s);
    assert_true(stkIsChr(s));
    assert_int_equal(stkValChr(s), '5');
    assert_false(stkIsStr(s));
    assert_false(stkIsInt(s));

    stkPop(s);
    assert_true(stkIsInt(s));
    assert_int_equal(stkValInt(s), 4);
    assert_false(stkIsDbl(s));
    assert_false(stkIsChr(s));

    stkPop(s);
    assert_true(stkIsDbl(s));
    assert_int_equal((int)stkValDbl(s), 3);
    assert_false(stkIsInt(s));

    stkPop(s);
    assert_true(stkIsStr(s));
    assert_string_equal(stkValStr(s), "2");
    assert_int_equal(*stkValStr(s), '2');
    assert_false(stkIsChr(s));

    stkPop(s);
    assert_true(stkIsInt(s));
    assert_int_equal(stkValInt(s), 1);
    assert_false(stkIsChr(s));

    pop = stkPop(s);
    assert_null(pop);
    assert_true(stkIsEmpty(s));
    assert_false(stkIsInt(s));
    assert_false(stkIsDbl(s));
    assert_false(stkIsChr(s));
    assert_false(stkIsStr(s));
    assert_false(stkIsPtr(s));

    stkDestroy(s);

} /* test_pushPop */


/** test dynamic allocations */
static void test_manyPushInts()
{
    stk_t *s = stkNew();
    char str[32];
    int i;

    for(i = 0; i < MANY; i++)
        stkPushInt(s, i);
    assert_true(stkIsInt(s));
    assert_false(stkIsEmpty(s));

    for(; i > 0; i--) {
        assert_int_equal(stkValInt(s), i-1);
        snprintf(str, sizeof(str), "%d", i-1);
        assert_string_equal(stkValToStr(s), str);
        stkPop(s);
    }
    assert_true(stkIsEmpty(s));

    stkDestroy(s);

} /* test_manyPushInts */


/** tests dynamic allocations with string entries */
static void test_manyPushStrs()
{
    stk_t *s = stkNew();
    char str[32];
    int i;

    for(i = 0; i < MANY; i++) {
        snprintf(str, sizeof(str), "%d", i);
        stkPushStr(s, str);
    }
    assert_true(stkIsStr(s));
    assert_false(stkIsEmpty(s));

    for(; i > 0; i--) {
        snprintf(str, sizeof(str), "%d", i-1);
        assert_string_equal(str, stkValStr(s));
        stkPop(s);
    }
    assert_true(stkIsEmpty(s));

    stkDestroy(s);

} /* test_manyPushStrs() */


/** tests clear after several pushes */
static void test_clear()
{
    stk_t *s = stkNew();
    char str[32];
    int i;

    for(i = 0; i < MANY; i++) {
        snprintf(str, sizeof(str), "%d", i);
        stkPushStr(s, str);
    }
    stkClear(s);
    stkDestroy(s);

} /* test_clear() */


/** tests destroy after several dynamic allocations with string entries */
static void test_destroy()
{
    stk_t *s = stkNew();
    char str[32];
    int i;

    for(i = 0; i < MANY; i++) {
        snprintf(str, sizeof(str), "%d", i);
        stkPushStr(s, str);
    }
    stkDestroy(s);

} /* test_destroy() */


int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_pushPop),      /* new, pushXxx, pop, isXxx, val, destroy */
        cmocka_unit_test(test_manyPushInts), /* new, pushInt, pop, valInt, isEmpty, destroy */
        cmocka_unit_test(test_manyPushStrs), /* new, pushStr, pop, valStr, isEmpty, destroy */
        cmocka_unit_test(test_clear),        /* new, pushStr, clear, destroy */
        cmocka_unit_test(test_destroy),      /* new, pushStr, destroy */
        //cmocka_unit_test_setup_teardown(test_Xxx, setup, teardown),
    };

    return cmocka_run_group_tests_name("Stack tests", tests, NULL, NULL);
}
