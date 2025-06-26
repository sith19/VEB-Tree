
#ifndef VEB_H
#define VEB_H
#include <stdbool.h>
struct VebTree;
struct VebTree * createVebTree(int u);
int minimum(struct VebTree *tree);
int maximum(struct VebTree *tree);
bool member(struct VebTree *tree, int x);
int successor(struct VebTree *tree, int x);
int predecessor(struct VebTree *tree, int x);
void insert(struct VebTree *tree, int x);
void delete(struct VebTree *tree, int x);
void print(struct VebTree *tree);
#endif //VEB_H
