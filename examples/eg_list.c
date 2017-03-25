#include <stdio.h>
#include <string.h>
#include <list.h>

typedef struct entry_t entry_t;
struct entry_t
{
    int i;
    entry_t *LIST_LINK;
};

entry_t _entryArr[10];

int main(int argc, char **argv)
{
    entry_t *head = NULL;
    entry_t *pos;
    int i;

    /* add entries to list */
    memset(_entryArr, 0, sizeof(_entryArr));
    for(i = 0; i < sizeof(_entryArr)/sizeof(_entryArr[0]); i++)
    {
        _entryArr[i].i = i;
        listAdd(&_entryArr[i], head);
    }

    /* reverse list */
    listReverse(head);

    /* print entries */
    listForEach(pos, head) {
        printf("%d\n", pos->i);
    }

    return 0;
}
