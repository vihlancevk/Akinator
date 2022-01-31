#ifndef AKINATOR_MODE_H_
#define AKINATOR_MODE_H_

#include "Tree.h"
#include "Stack.h"

#define DEBUG

TreeErrorCode GuessObject(Tree_t *tree);

stack_t* DefineObjectMode(const Tree_t *tree, const char *object, stack_t *stack);

void CompairObjectsMode(const Tree_t *tree, const char *object1, const char *object2);

void ShowTree(const Tree_t *tree);

#endif // AKINATOR_MODE_H_

