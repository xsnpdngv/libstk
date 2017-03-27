/**
 * @file     list.h
 * @brief    lightweight singly linked list implementation
 * @author   Tamas Dezso <dezso.t.tamas@gmail.com>
 * @date     November 2, 2016
 * @version  1.0
 *
 *        head/pos      pos                     pos
 *        |             |                       |
 *        v             V                       V
 *        +------+  .-> +------+  .--[ ... ]--> +------+
 *        | fld  |  |   | fld  |  |             | fld  |
 *        | fld  |  |   | fld  |  |             | fld  |
 *        | ...  |  |   | ...  |  |             | ...  |
 *        | LINK |--'   | LINK |--'             | LINK |--> NULL
 *        +------+      +------+                +------+
 *
 * @param  head   a pointer to the first entry on the list; must be set to
 *                NULL before the first operation on the list
 * @param  entry  element of the list having custom structure which obviously
 *                includes link member(s) along with the payload fields
 * @param  LINK   pointer member parameter of the entry struct to implement
 *                linking; in general it is named automatically, so no need
 *                to care about, in case of entry structs that can be linked
 *                to more than one lists it is needed to differentiate links
 *                for each
 * @param  fld    payload member fields of the entry struct
 * @param  pos    a pointer to any entry on the list
 */


#ifndef __LIST_H
#define __LIST_H


/* ----- macros ------------------------------------------------------------ */


/** name of link member parameter within the entry struct */
#define LIST_LINK        LIST_LINK_()


/**
 * if entries are needed to be linked to more lists at the same time, each
 * list needs a distinct linking member within the entry struct; for declaring
 * such links, the macro below (ending with underscore (_)) can be used with
 * a differentiator as argument - which has to be given for the list
 * handling macros as well
 */
#define LIST_LINK_(...)  __next_##__VA_ARGS__


/**
 * gets next list entry after the one at current position
 *
 * @param  pos   pointer to the list entry (struct *) next of which is got
 * @param  ...   unique link differentiator (optional)
 */
#define listNext(pos, ...) \
        ((pos)->LIST_LINK_(__VA_ARGS__))


/**
 * steps list cursor to next entry
 *
 * @param  pos   list cursor variable to step (struct *)
 * @param  ...   unique link differentiator (optional)
 */
#define listStep(pos, ...) \
        (pos = listNext(pos, __VA_ARGS__))


/**
 * tests whether list entry at cursor is last
 *
 * @param  pos   cursor of list entry to test
 * @param  ...   unique link differentiator (optional)
 */
#define listIsLast(pos, ...) \
        ((pos)->LIST_LINK_(__VA_ARGS__) == NULL)


/**
 * iterates over a list
 *
 * @param  pos   loop cursor variable (struct *)
 * @param  head  list head, pointer to first entry (struct *)
 * @param  ...   unique link differentiator (optional)
 */
#define listForEach(pos, head, ...) \
        for(pos = head; pos != NULL; listStep(pos, __VA_ARGS__))


/**
 * iterates over the addresses of links in a list
 *
 * @param  posP  loop cursor variable (struct **), on each iteration being
 *               set to the address of the preceding entry's link member, or
 *               to the head in case of the very first entry
 * @param  headP pointer to the list head variable (struct **)
 * @param  ...   unique link differentiator (optional)
 */
#define listForEachLink(posP, headP, ...) \
        for(posP = headP; *posP; posP = &(*posP)->LIST_LINK_(__VA_ARGS__))


/**
 * iterates over a list, being safe against entry removal
 *
 * @param  pos   loop cursor variable (struct *)
 * @param  tmp   temporary storage variable, same type as loop cursor
 * @param  head  the head variable (struct *) to iterate over the list
 *               pointed by; may be modified on entry deletion
 * @param  ...   unique link differentiator (optional)
 */
#define listForEachSafe(pos, tmp, head, ...)                                  \
        for((void)((pos = head) && (tmp = (head)->LIST_LINK_(__VA_ARGS__)));  \
            pos != NULL;                                                      \
            (void)((pos = tmp) && (tmp = pos->LIST_LINK_(__VA_ARGS__))))


/**
 * inserts new entry to list head
 *
 * @param  new   pointer to the new entry that is to be added (struct *)
 * @param  head  the head variable (struct *) to add entry to list pointed by;
 *               since entry becomes the first one, head is modified
 * @param  ...   unique link differentiator (optional)
 * @return       head
 */
#define listAdd(new, head, ...)                                               \
({                                                                            \
        typeof(new) __new = (new);                                            \
                                                                              \
        __new->LIST_LINK_(__VA_ARGS__) = head;                                \
        head = __new;                                                         \
})


/**
 * inserts new entry to list tail
 *
 * @param  new   pointer to the new entry that is to be added (struct *)
 * @param  head  the head variable (struct *) to add entry to list pointed by;
 *               may be modified
 * @param  ...   unique link differentiator (optional)
 * @return       new
 */
#define listAddTail(new, head, ...)                                           \
({                                                                            \
        typeof(&(head)) __i;                                                  \
        typeof(new) __new = (new);                                            \
                                                                              \
        __new->LIST_LINK_(__VA_ARGS__) = NULL;                                \
        listForEachLink(__i, &(head), __VA_ARGS__)                            \
            ;                                                                 \
        *__i = __new;                                                         \
})


/**
 * adds new unique entry to list tail if a matching one is not yet present
 *
 * @param  new    pointer to the new entry that is to be added (struct *)
 * @param  head   the head variable (struct *) to add entry to list pointed by;
 *                may be modified
 * @param  ldif   unique link differentiator (optional)
 * @param  cmpfn  compare function to determine wheter new entry is unique
 * @param  fld    name of the member fields to be compared, together with
 *                the member access operator (->), or empty to compare the
 *                whole structs
 * @param  args   optional additional arguments to pass over to the compare
 *                function (e.g., size or such)
 * @return        true if operation was successful; false if matching entry is
 *                already in the list, so no addition was done
 */
#define listAddTailQ(new, head, ldif, cmpfn, fld, args...)                    \
({                                                                            \
        typeof(&(head)) __i;                                                  \
        typeof(new) __new = new;                                              \
                                                                              \
        __new->LIST_LINK_(ldif) = NULL;                                       \
        listForEachLink(__i, &(head), ldif)                                   \
            if(cmpfn((*__i) fld, __new fld, ##args) == 0)                     \
                break;                                                        \
        *__i == NULL && (*__i = __new);                                       \
})


/**
 * removes entry at head (if list is not empty)
 *
 * @param  head  the head variable (struct *) to remove entry from list pointed
 *               by; since the first entry is removed, head is modified
 * @param  ...   unique link differentiator (optional)
 * @return       head
 * @warning      removed element's link keeps it original value
 */
#define listDel(head, ...) \
        (head = head ? (head)->LIST_LINK_(__VA_ARGS__) : head)


/**
 * moves entry from one head to another (if source list is not empty)
 *
 * @param  headT  the target head variable (struct *) to add entry to
 *                list pointed by; to be modified
 * @param  headS  the source head variable (struct *) to remove entry from
 *                list pointed by; to be modified
 * @param  ...    unique link differentiator for both lists (optional)
 * @return        headS
 */
#define listMove(headT, headS, ...)                                           \
({                                                                            \
        if(headS) {                                                           \
            typeof((headS)->LIST_LINK_(__VA_ARGS__)) __tmp =                  \
                headS->LIST_LINK_(__VA_ARGS__);                               \
            (headS)->LIST_LINK_(__VA_ARGS__) = headT;                         \
            headT = headS;                                                    \
            headS = __tmp;                                                    \
        }                                                                     \
        headS;                                                                \
})


/**
 * reverses a list at head
 *
 * @param  head  the head variable (struct *) to reverse entries on list
 *               pointed by
 * @param  ...   unique link differentiator for both lists (optional)
 * @return       head
 */
#define listReverse(head, ...)                                                \
({                                                                            \
        typeof(head) tmpHead = NULL;                                          \
                                                                              \
        while(listMove(tmpHead, head, __VA_ARGS__))                           \
            ;                                                                 \
        head = tmpHead;                                                       \
})


/**
 * finds first matching list entry
 *
 * @param  head   list head, pointer to first element (struct *)
 * @param  ldif   unique link differentiator (optional)
 * @param  cmpfn  compare function
 * @param  fld    member field to be compared, together with the member
 *                access operator (->), or empty to compare the whole struct
 * @param  args   arguments to pass over to the compare function containing
 *                the other entity to compare with at the first place and
 *                other optional parameters of comparision afterwards
 * @return        pointer of first true entry, if any; NULL otherwise
 */
#define listFind(head, ldif, cmpfn, fld, args...)                             \
({                                                                            \
        typeof(head) __i;                                                     \
                                                                              \
        listForEach(__i, head, ldif)                                          \
            if(cmpfn(__i fld, args) == 0)                                     \
                break;                                                        \
        __i;                                                                  \
})


/**
 * finds first matching list entry, then moves it to the list head
 *
 * @param  head   the head variable (struct *) to add entry to list pointed by;
 *                may be modified
 * @param  ldif   unique link differentiator (optional)
 * @param  cmpfn  compare function to determine wheter new entry is unique
 * @param  fld    member field to be compared, together with the member
 *                access operator (->), or empty to compare the whole struct
 * @param  args   arguments to pass over to the compare function containing
 *                the other entity to compare with at the first place and
 *                other optional parameters of comparision afterwards
 * @return        true if operation is successful; false if matching entry is
 *                not found
 */
#define listFindCache(head, ldif, cmpfn, fld, args...)                        \
({                                                                            \
        typeof(&(head)) __i;                                                  \
        typeof(*__i) __entry = NULL;                                          \
                                                                              \
        listForEachLink(__i, &(head), ldif)                                   \
            if(cmpfn((*__i) fld, args) == 0) {                                \
                __entry = *__i;                                               \
                listDel(*__i, ldif);                                          \
                listAdd(__entry, head, ldif);                                 \
                break;                                                        \
            }                                                                 \
        __entry;                                                              \
})


/**
 * deletes each matching list entry
 *
 * @param  head   the head variable (struct *) to delete element(s) from list
 *                pointed by; may be modified
 * @param  ldif   unique link differentiator (optional)
 * @param  cmpfn  compare function
 * @param  fld    member field to be compared, together with the member
 *                access operator (->), or empty to compare the whole struct
 * @param  args   arguments to pass over to the compare function containing
 *                the other entity to compare with at the first place and
 *                other optional parameters of comparision afterwards
 * @return        void
 */
#define listDelMatch(head, ldif, cmpfn, fld, args...)                         \
({                                                                            \
        typeof(&(head)) __i = &(head);                                        \
                                                                              \
        while(*__i)                                                           \
            if(cmpfn((*__i) fld, args) == 0)                                  \
                listDel(*__i, ldif);                                          \
            else                                                              \
                __i = &(*__i)->LIST_LINK_(ldif);                              \
})


#endif /* __LIST_H */
