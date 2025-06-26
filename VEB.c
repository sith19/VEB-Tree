#include <stdio.h>
#include <tgmath.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
int upperSqrt(int u);
int lowerSqrt(int u);
double un = 100;
struct VebTree
{
    int u;
    int min;
    int max;
    struct VebTree * summery;
    struct VebTree ** cluster;
};

struct VebTree * createVebTree(int u)
{
    struct VebTree * node = malloc(sizeof(struct VebTree));
    node->u = u;
    node -> min = -1;
    node -> max = -1;
    if (u <= 2)
    {
        node -> summery = NULL;
        node -> cluster = NULL;
    }else
    {
        node -> summery = createVebTree(upperSqrt(u));
        node -> cluster = malloc(sizeof(struct VebTree*) * upperSqrt(u));
        for(int i = 0; i < upperSqrt(u); i++)
        {
            node -> cluster[i] = createVebTree(lowerSqrt(u));
        }

    }
    return node;
}

int upperSqrt(int u) {
    int k = (int)ceil(log2(u) / 2);
    return 1 << k;  // 2^k
}

int lowerSqrt(int u) {
    int k = (int)floor(log2(u) / 2);
    return 1 << k;  // 2^k
}

int high(struct VebTree *node, int x) {
    int divisor = lowerSqrt(node->u);
    return floor(x / divisor);
}

int low(struct VebTree *node, int x) {
    int mod = lowerSqrt(node->u);
    return x % mod;
}
int VEBindex(struct VebTree *node, int x, int y)
{
    return x * lowerSqrt(node->u) + y;
}
int minimum(struct VebTree *tree)
{
    return tree->min;
}
int maximum(struct VebTree *tree)
{
    return tree->max;
}


bool member(struct VebTree *tree, int x)
{
    if (x == tree->min || x == tree->max)
    {
        return true;
    }else if (tree->u == 2)
    {
        return false;
    }else
    {
        return member(tree -> cluster[high(tree,x)],low(tree,x));
    }
}
int successor(struct VebTree *tree, int x)
{
    if (tree -> u == 2)
    {
        if (x == 0 && tree-> max == 1)
        {
            return 1;
        }else
        {
            return -1;
        }
    }else if (tree -> min != -1 &&  x < tree->min)
    {
       return tree->min;
    }else
    {
        int maxlow = maximum(tree->cluster[high(tree,x)]);
        if (maxlow != -1 && low(tree,x) < maxlow)
        {
            int offset = successor(tree -> cluster[high(tree,x)],low(tree,x));
            return VEBindex(tree,high(tree,x),offset);
        }else
        {
            int succclust = successor(tree -> summery,x);
            if (succclust == -1)
            {
                return -1;
            }else
            {
                int offset = minimum(tree->cluster[succclust]);
                return VEBindex(tree,succclust,offset);
            }
        }
    }

}
int predecessor(struct VebTree *tree, int x)
{
    if (tree -> u == 2)
    {
        if (x == 1 && tree-> min == 0)
        {
            return 0;
        }else
        {
            return -1;
        }
    }else if (tree -> max != -1 && x > tree->max)
    {
        return tree->max;
    }else
    {
        int minlow = minimum(tree->cluster[high(tree,x)]);
        if (minlow != -1 && low(tree,x) > minlow)
        {
            int offset = predecessor(tree -> cluster[high(tree,x)],low(tree,x));
            return VEBindex(tree,high(tree,x),offset);

        }else
        {
            int predClust = predecessor(tree->summery,high(tree,x));
            if (predClust == -1)
            {
                if (tree-> min != -1 && x > tree->min)
                {
                    return tree->min;
                }else
                {
                    return -1;
                }
            }else
            {
                int offset = minimum(tree->cluster[predClust]);
                return VEBindex(tree,predClust,offset);
            }
        }
    }
    }
void emptyInsert(struct VebTree * tree, int x)
{
    tree -> max = x;
    tree -> min = x;
}
void insert(struct VebTree *tree, int x)
{
    if (tree -> min == -1)
    {
        emptyInsert(tree,x);
    }else if (x < tree -> min)
    {
        int temp = x;
        x = tree -> min;
        tree->min = temp;
        if (tree -> u > 2)
        {
            if (minimum(tree -> cluster[high(tree,x)] ) == -1)
            {
                insert(tree -> summery,high(tree,x));
                emptyInsert(tree -> cluster[high(tree,x)],low(tree,x));
            }else
            {
                insert(tree -> cluster[high(tree,x)],low(tree,x));
            }
            if (x > tree -> max)
            {
                tree -> max = x;
            }
        }
    }
}
void delete(struct VebTree *tree, int x){
    if (tree ->min == tree->max)
    {
        tree->min = -1;
        tree->max = -1;
    }else if (tree -> u == 2)
    {
        if (x == 0)
        {
            tree->min = 1;
        }else
        {
            tree->min = 0;
            tree->max = tree -> min;
        }
    }else if (x == tree -> min)
    {
        int firstcluster = minimum(tree -> summery);
        x = VEBindex(tree,firstcluster,minimum(tree->cluster[firstcluster]));
        tree->min = x;
        delete(tree->cluster[high(tree,x)],low(tree,x));
        if (minimum(tree -> cluster[high(tree,x)]) == -1)
        {
            delete(tree->summery,high(tree,x));
            if (x == tree -> max)
            {
                int summerymax = maximum(tree->summery);
                if (summerymax == -1)
                {
                    tree->max = tree -> min;
                }
                else
                {
                    tree->max = VEBindex(tree,summerymax,maximum(tree->cluster[summerymax]));
                }
            }
        }
    }else if (x == tree -> max)
    {
        tree->max = VEBindex(tree,high(tree,x),maximum(tree->cluster[high(tree,x)]));
    }
}


void print(struct VebTree *tree){
    if (tree == NULL || tree->min == -1) return;

    // Print minimum
    printf("%d ", tree->min);

    // If tree->min != tree->max, we need to print more
    if (tree->min != tree->max && tree->cluster != NULL)
    {
        int upper = upperSqrt(tree->u);
        int lower =  lowerSqrt(tree->u);

        // Traverse each non-empty cluster
        for (int i = 0; i < upper; i++)
        {
            if (tree->cluster[i] != NULL && tree->cluster[i]->min != -1)
            {
                // Recursively print the values in the cluster
                print(tree->cluster[i]);
            }
        }
    }

    // Print max if it's different from min (and not yet printed)
    if (tree->max != tree->min)
    {
        printf("%d ", tree->max);
    }
}


