#ifndef AKINATOR_MODE_H_
#define AKINATOR_MODE_H_

#include "Tree.h"
#include "Stack.h"

#define DEBUG

TreeErrorCode QuessingMode(Tree_t *tree);

stack_t* ObjectDefinitionMode(const Tree_t *tree, const char *object, stack_t *stack);

#endif // AKINATOR_MODE_H_
