#include <stdio.h>
#include <stk.h>

int main(int argc, char **argv)
{
    stk_t *s = stkNew(128);
    int i;

    for(i = 0; i < 10; i++)
        stkPushInt(s, i);

    while(!stkIsEmpty(s))
    {
        printf("%s\n", stkValToStr(s));
        stkPop(s);
    }

    stkDestroy(s);
    return 0;
}
