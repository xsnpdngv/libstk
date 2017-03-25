/**
 * @file     stk.c
 * @brief    expanding stack (last in - first out list) implementation
 * @author   Tamas Dezso <dezso.t.tamas@gmail.com>
 * @date     November 13, 2016
 * @version  1.0
 */


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef UNIT_TESTING
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#endif


#include "stk.h"


/* ----- function definitions ---------------------------------------------- */


#ifdef UNIT_TESTING
#define strdup test_strdup

/** calls (mocked) malloc() explicitly for cmocka to be able to track
 *  memory allocations */
char *test_strdup(const char *s)
{
    char *s2;
    size_t size;

    if((s2 = malloc(size = (strlen(s)+1))))
        memcpy(s2, s, size);
    return s2;
}
#endif


stk_t *
stkNew(size_t blkSz)
{
    stk_t *s;

    if((s = malloc(sizeof(*s))))
    {
        memset(s, 0, sizeof(*s));
        s->blkSz = blkSz;
    }
    return s;
} /* stkNew */


stkEl_t *
_stkPush(stk_t *s, char type, stkVar_t var)
{
    struct stkEl_t *el;

    /* get variable wrapper element */
    if(s->freeEls)
    {
        /* from linked list of free ones */
        el = s->freeEls;
    }
    else if(s->blks && s->blkEl < (struct stkEl_t *)(s->blks+1) + s->blkSz)
    {
        /* from already allocated block */
        el = s->blkEl;
    }
    else
    {
        /* allocate new block */
        if((el = malloc(sizeof(struct stkBlk_t) +
                        sizeof(struct stkEl_t) * s->blkSz)) == NULL)
            return NULL;
        listAdd((struct stkBlk_t *)el, s->blks);
        s->blkEl = (struct stkEl_t *)((struct stkBlk_t *)el + 1);

        /* from just allocated block */
        el = s->blkEl;
    }

    /* initialize wrapper element */
    switch(el->type = type)
    {
        case 'i': el->var.i = var.i; break;
        case 'd': el->var.d = var.d; break;
        case 'c': el->var.c = var.c; break;
        case 'p': el->var.p = var.p; break;
        case 's': if((el->var.s = strdup(var.s)) == NULL)
                      return NULL;
                  break;
        default: return NULL;
    }

    /* push element into stack */
    if(el == s->freeEls)
        listMove(s->top, s->freeEls);
    else /* el == s->blkEl */
        listAdd(s->blkEl++, s->top);

    return el;
} /* _stkPush */


stkEl_t *
stkPop(stk_t *s)
{
    if(s->top)
    {
        if(s->top->type == 's')
            free(s->top->var.s);
        listMove(s->freeEls, s->top);
    }
    return s->top;
} /* stkPop */


void
stkClear(stk_t *s)
{
    while(stkPop(s))
        ;
} /* stkClear */


void
stkDestroy(stk_t *s)
{
    struct stkBlk_t *blk, *tmpBlk;

    stkClear(s);
    listForEachSafe(blk, tmpBlk, s->blks)
        free(blk);
    free(s);
    return;
} /* stkDestroy */


char *
stkValToStr(stk_t *s)
{
    static char str[32];

    str[0] = '\0';
    if(s->top)
    {
        switch(s->top->type)
        {
            case 's': return s->top->var.s;
            case 'i': snprintf(str, sizeof(str), "%d", s->top->var.i); break;
            case 'd': snprintf(str, sizeof(str), "%f", s->top->var.d); break;
            case 'c': snprintf(str, sizeof(str), "%c", s->top->var.c); break;
            case 'p': snprintf(str, sizeof(str), "%p", s->top->var.p); break;
        }
    }
    return str;
} /* stkValToStr */
