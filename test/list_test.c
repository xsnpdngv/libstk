/**
 * @file     list_test.c
 * @brief    singly linked list unit tests utilizing the cmocka framework
 * @author   Tamas Dezso
 * @date     January 3, 2017
 * @version  1.0
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cmocka.h>

#include "list.h"


/* ----- macros ------------------------------------------------------------ */


#define NUMCMP(X, Y) (((X) > (Y)) - ((Y) > (X)))
#define EL_N(Arr)    sizeof(Arr)/sizeof(Arr[0])


/* ----- types ------------------------------------------------------------- */


typedef struct entry_t entry_t;
struct entry_t
{
    int i;
    entry_t *LIST_LINK;
    entry_t *LIST_LINK_(other);
};


/* ----- globals ----------------------------------------------------------- */


entry_t _entryArr[11];


/* ----- test functions ---------------------------------------------------- */


/** tests additions to list */
static void test_add()
{
    entry_t *head = NULL;
    entry_t *headOther = NULL;
    entry_t *pos;
    int i, j;

    /* add entries to list */
    memset(_entryArr, 0, sizeof(_entryArr));
    for(i = 0; i < EL_N(_entryArr); i++) {
        _entryArr[i].i = i;
        listAdd(&_entryArr[i], head);
        if(i % 2 == 0)
            listAdd(&_entryArr[j=i], headOther, other);
    }

    /* check if entries added correctly */
    listForEach(pos, head) {
        i--;
        assert_int_equal(i, pos->i);
    }

    /* reverse list */
    listReverse(head);

    /* check if reversed correctly */
    i = 0;
    listForEach(pos, head) {
        assert_int_equal(i, pos->i);
        i++;
    }

    /* check if entries added correctly (other list) */
    listForEach(pos, headOther, other) {
        assert_int_equal(j, pos->i);
        j -= 2;
    }
}


/** tests additions to list tail */
static void test_addTail()
{
    entry_t *head = NULL;
    entry_t *headOther = NULL;
    entry_t *pos;
    int i;

    /* add entries to list */
    memset(_entryArr, 0, sizeof(_entryArr));
    for(i = 0; i < EL_N(_entryArr) - 1; i++) {
        _entryArr[i].i = i;
        listAddTail(&_entryArr[i], head);
        if(i % 2 == 0)
            listAddTail(&_entryArr[i], headOther, other);
    }

    /* add entry if unique */
    _entryArr[i].i = 0;
    assert_false(listAddTailQ(&_entryArr[i], head,, NUMCMP, ->i));
    _entryArr[i].i = i/2;
    assert_false(listAddTailQ(&_entryArr[i], head,, NUMCMP, ->i));
    _entryArr[i].i = i;
    assert_true(listAddTailQ(&_entryArr[i], head,, NUMCMP, ->i));

    /* check if entries added correctly */
    i = 0;
    listForEach(pos, head) {
        assert_int_equal(i, pos->i);
        i++;
    }
    assert_int_equal(i, EL_N(_entryArr));

    /* check if entries added correctly (other list) */
    i = 0;
    listForEach(pos, headOther, other) {
        assert_int_equal(i, pos->i);
        i += 2;
    }
}


#define MODCMP(x, y) ((x) % (y) == 0)
#define ARG2(x, y)   y
#define MOD          15
/** tests different types of findings and deletions */
static void test_del()
{
    entry_t *head = NULL;
    entry_t *headOther = NULL;
    entry_t *pos;
    int i;

    /* add entries to list */
    memset(_entryArr, 0, sizeof(_entryArr));
    for(i = 0; i < EL_N(_entryArr); i++) {
        _entryArr[i].i = i;
        listAdd(&_entryArr[i], head);
        listAdd(&_entryArr[i], headOther, other);
    }

    /* find first matching entry */
    assert_non_null(pos = listFind(head,, NUMCMP, ->i, 3));
    assert_int_equal(pos->i, 3);

    /* try to find non-existing entry */
    assert_null(listFind(head,, NUMCMP, ->i, EL_N(_entryArr) + 1));

    /* delete entries */
    listDelMatch(head,, MODCMP, ->i, MOD);

    /* check remaining entries */
    for(i--; !MODCMP(i, MOD); i--)
        ;
    listForEach(pos, head) {
        assert_int_equal(pos->i, i);
        i -= MOD;
    }

    /* delete each entry */
    assert_non_null(head);
    listDelMatch(head,, ARG2, ->i, 0);
    assert_null(head);

    /* check listForEach for empty list */
    listForEach(pos, head)
        assert_int_equal(1, 0);

    /* delete entry at head for empty list */
    listDel(headOther, other);

    /* delete each entry the normal way (the other list) */
    while(listDel(headOther, other))
        ;
    assert_null(headOther);

    /* check listForEach for empty list (the other list) */
    listForEach(pos, headOther, other)
        assert_int_equal(1, 0);
}


/** tests moving from one list to other */
static void test_move()
{
    entry_t *head = NULL;
    entry_t *headOther = NULL;
    entry_t *pos;
    int i;

    assert_null(head);
    assert_null(headOther);
    assert_null(listMove(head, headOther));
    assert_null(listMove(headOther, head));

    /* add entries to list */
    memset(_entryArr, 0, sizeof(_entryArr));
    for(i = 0; i < EL_N(_entryArr); i++) {
        _entryArr[i].i = i;
        listAdd(&_entryArr[i], head);
    }
    assert_non_null(head);
    assert_null(headOther);

    /* move entries from one list to other and check if it was OK */
    while(listMove(headOther, head))
        ;
    assert_null(head);
    assert_non_null(headOther);
    i = 0;
    listForEach(pos, headOther) {
        assert_int_equal(i, pos->i);
        i++;
    }

    /* move entries back and check if it was OK */
    while(listMove(head, headOther))
        ;
    assert_null(headOther);
    listForEach(pos, head) {
        i--;
        assert_int_equal(i, pos->i);
    }

    /* remove each entry from list */
    while(listDel(head))
        ;
    assert_null(head);
}

/** tests moving from one list to other using special link */
static void test_move2()
{
    entry_t *head = NULL;
    entry_t *headOther = NULL;
    entry_t *pos;
    int i;

    assert_null(head);
    assert_null(headOther);
    assert_null(listMove(head, headOther, other));
    assert_null(listMove(headOther, head, other));

    /* add entries to list */
    memset(_entryArr, 0, sizeof(_entryArr));
    for(i = 0; i < EL_N(_entryArr); i++) {
        _entryArr[i].i = i;
        listAdd(&_entryArr[i], head, other);
    }
    assert_non_null(head);
    assert_null(headOther);

    /* move entries from one list to other and check if it was OK */
    while(listMove(headOther, head, other))
        ;
    assert_null(head);
    assert_non_null(headOther);
    i = 0;
    listForEach(pos, headOther, other) {
        assert_int_equal(i, pos->i);
        i++;
    }

    /* move entries back and check if it was OK */
    while(listMove(head, headOther, other))
        ;
    listForEach(pos, head, other) {
        i--;
        assert_int_equal(i, pos->i);
    }

    /* remove each entry from list */
    while(listDel(head))
        ;
    assert_null(head);
}

/** tests caching on list */
static void test_findCache()
{
    entry_t *head = NULL;
    int i;

    /* for empty list */
    assert_null(listFindCache(head, other, NUMCMP, ->i, 3));

    /* add entries to list */
    memset(_entryArr, 0, sizeof(_entryArr));
    for(i = 0; i < EL_N(_entryArr); i++) {
        _entryArr[i].i = i;
        listAdd(&_entryArr[i], head, other);
    }
    assert_non_null(head);

    /* check if we have enough entries */
    assert_true(EL_N(_entryArr) > 9);

    /* for middle entry */
    assert_ptr_equal(head, &_entryArr[i-1]);
    assert_ptr_equal(listFindCache(head, other, NUMCMP, ->i, 6), &_entryArr[6]);
    assert_ptr_equal(head, &_entryArr[6]);
    /*
    entry_t *pos;
    listForEach(pos, head)
        printf("%d -> ", pos->i);
    printf("\n");
    */

    /* for first entry */
    assert_ptr_equal(listFindCache(head, other, NUMCMP, ->i, 6), &_entryArr[6]);
    assert_ptr_equal(head, &_entryArr[6]);

    /* for last entry */
    assert_ptr_equal(listFindCache(head, other, NUMCMP, ->i, 0), &_entryArr[0]);
    assert_ptr_equal(head, &_entryArr[0]);

    /* for non-existing entry */
    assert_ptr_equal(listFindCache(head, other, NUMCMP, ->i, 16), NULL);
    assert_ptr_equal(head, &_entryArr[0]);

    /* for middle entry */
    assert_ptr_equal(listFindCache(head, other, NUMCMP, ->i, 9), &_entryArr[9]);
    assert_ptr_equal(head, &_entryArr[9]);
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_add),      /* add, reverse, forEach */
        cmocka_unit_test(test_addTail),  /* addTail, addTailQ, forEach */
        cmocka_unit_test(test_del),      /* add, del, delMatch, forEach, find */
        cmocka_unit_test(test_move),     /* add, move, forEach */
        cmocka_unit_test(test_move2),    /* add, move, forEach */
        cmocka_unit_test(test_findCache),/* add, findCache, forEach */
        //cmocka_unit_test_setup_teardown(test_Xxx, setup, teardown),
    };

    return cmocka_run_group_tests_name("List tests", tests, NULL, NULL);
}
