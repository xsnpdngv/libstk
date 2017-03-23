/**
 * @file     stk.h
 * @brief    expanding stack (last in - first out list) implementation
 * @author   Tamas Dezso <dezso.t.tamas@gmail.com>
 * @date     November 13, 2016
 * @version  1.0
 *
 * Expanding stack implementation for different types of values to accept
 * utilizing the singly linked list implementation (list.h).
 * Capacity of stack extends automatically on need, in relatively
 * small blocks, but never shrinks. Stack always holds a copy of
 * pushed variables, even for strings.
 *
 *        stk_t *
 *        |
 *        v
 *        +--------------+
 *        | value | type |
 *        +--------------+
 *        | value | type |
 *        +--------------+
 *        | ...          |
 *        +--------------+
 *        | value | type |
 *        +--------------+
 *
 * Usage example:
 *
 *        stk_t *s = stkNew(128);
 *        stkPushInt(s, 10);
 *        if(!stkIsEmpty(s))
 *            printf("stack top: %s\n", stkValStr(s));
 *        stkDestroy(s);
 */


#ifndef __STK_H
#define __STK_H


#include <stdlib.h>

#include "list.h"


/* ----- macros ------------------------------------------------------------ */


/* if not GNU C, elide __attribute__ */
#ifndef __GNUC__
#  define __attribute__(x) /* nothing */
#endif

/** pushes variable into stack by type */
#define stkPush(s, type, var)   __stkPush(s, type, (stkVar_t)(var))
#define stkPushInt(s, Int) stkPush(s, 'i', (int)Int)    /**< pushes integer into stack */
#define stkPushDbl(s, Dbl) stkPush(s, 'd', (double)Dbl) /**< pushes double into stack */
#define stkPushChr(s, Chr) stkPush(s, 'c', (char)Chr)   /**< pushes character into stack */
#define stkPushStr(s, Str) stkPush(s, 's', (char *)Str) /**< pushes string into stack */
#define stkPushPtr(s, Ptr) stkPush(s, 'p', (void *)Ptr) /**< pushes pointer into stack */

/** tests whether stack is empty */
#define stkIsEmpty(s)  ((s)->top == NULL)

/** gets top element's type */
#define stkType(s)  (stkIsEmpty(s) ? '\0' : (s)->top->type)
#define stkIsInt(s) (stkType(s) == 'i') /**< checks if an int is at stack's top */
#define stkIsDbl(s) (stkType(s) == 'd') /**< checks if a double is at stack's top */
#define stkIsChr(s) (stkType(s) == 'c') /**< checks if a character is at stack's top */
#define stkIsStr(s) (stkType(s) == 's') /**< checks if a string is at stack's top */
#define stkIsPtr(s) (stkType(s) == 'p') /**< checks if a pointer is at stack's top */

/** gets top element, use only of !stkIsEmpty */
#define stkVal(s)    ((s)->top->var)
#define stkValInt(s) (stkVal(s).i) /**< gets top value as int, use only if stkIsInt */
#define stkValDbl(s) (stkVal(s).d) /**< gets top value as double, use only if stkIsDbl */
#define stkValChr(s) (stkVal(s).c) /**< gets top value as character, use only if stkIsChr */
#define stkValStr(s) (stkVal(s).s) /**< gets top value as string, use only if stkIsStr */
#define stkValPtr(s) (stkVal(s).p) /**< gets top value as pointer, use only if stkIsPtr || stkIsStr */
/* NOTE: stkToStr() is also available (defined as function) */


/* ----- types ------------------------------------------------------------- */


typedef union
{
    int    i;                   /**< integer */
    double d;                   /**< double */
    char   c;                   /**< character */
    char  *s;                   /**< string */
    void  *p;                   /**< pointer */

} stkVar_t; /**< stack variable */


typedef struct stkEl_t
{
    stkVar_t var;               /**< variable, must be the first member */
    char type;                  /**< type of variable */
    struct stkEl_t *LIST_LINK;  /**< link to next element on list */

} stkEl_t; /**< stack variable wrapper element */


typedef struct stkBlk_t
{
    struct stkBlk_t *LIST_LINK; /**< link to next allocated block on list */

    /* NOTE: actually the utilisable space that is allocated as block
             comes after this struct */

} stkBlk_t; /**< allocated block of stack variable wrappers */


typedef struct
{
    stkEl_t *top;               /**< stack top (linked list of used elements) */

    /* members for administrative use only */

    size_t blkSz;               /**< stack block size - number of variable
                                     wrapper elements allocated together */
    stkEl_t *freeEls;           /**< linked list of free elements */
    stkEl_t *blkEl;             /**< next usable element in most recent block */
    stkBlk_t *blks;             /**< linked list of allocated element blocks */

} stk_t; /**< stack */


/* ----- function signatures ----------------------------------------------- */


/**
 * creates and initializes a new stack
 *
 * @param  blkSz  block size - number of variable wrapper elements to
 *                be allocated together on creation and expansion
 *
 * @return  the return value of inside called malloc(): new stack pointer
 *          on success; NULL otherwise
 */
stk_t *stkNew(size_t blkSz) __attribute__((malloc, warn_unused_result));


/**
 * pushes a variable into stack
 *
 * @param  s     stack, previously created with `stkNew()`
 * @param  type  type of variable to push
 *               ('i'nteger|'d'ouble|'c'haracter|'s'tring|'p'ointer)
 * @param  var   union of compatible variables to push
 * @return       address of the pushed variable's wrapper element on success;
 *               NULL otherwise (fails only if wrong type is given, or neither
 *               free wrapper element is available nor allocating new block is
 *               successful)
 * @warning      for string variables allocates storage and copies a duplicate
 *               into it; allocated space is going to be freed on pop, clear
 *               and destroy
 * @note         intended to be used through `stkPushXxx()` macros
 */
stkEl_t *__stkPush(stk_t *s, char type, stkVar_t var) __attribute__((nonnull(1)));


/**
 * removes the top element from stack and frees possibly allocated
 * resources belonging to it
 *
 * @return  address of new top element after pop; NULL if empty
 */
stkEl_t *stkPop(stk_t *s) __attribute__((nonnull(1)));


/**
 * clears stack by popping each element out from the stack
 */
void stkClear(stk_t *s) __attribute__((nonnull(1)));


/**
 * destroys stack by freeing all allocated resources regarding
 */
void stkDestroy(stk_t *s) __attribute__((nonnull(1)));


/**
 * converts top element in stack to string
 *
 * @return  in case top element is a string itself, returns it;
 *          otherwise returns the buffer to which the element is printed
 */
char *stkValToStr(stk_t *s) __attribute__((nonnull(1)));


#endif /* __STK_H */
