
#include <stdio.h>
#include <stdlib.h>

#include "VEB.h"
int main()
{
    struct VebTree*tree = createVebTree(256);
    for (int i = 0; i < 100; i++)
    {
        int val = rand() % (256 + 1 - 0) + 0;
        if (!member(tree,val))
        {
            insert(tree, val);
        }

    }
    printf("%d",minimum(tree));

}