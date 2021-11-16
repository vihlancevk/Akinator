#include "AkinatorMode.h"

#define DEBUG

const int ANSWER_MAX_SIZE = 100;
const int STACK_SIZE = 10;

TreeErrorCode QuessingMode(Tree_t *tree)
{
    assert(tree != nullptr);

    printf("Akinator.\n\n");
    printf("Запущен режим угадывания объекта по признакам.\n\n");

    TreeErrorCode treeError = TREE_NO_ERROR;
    Node_t *node = tree->root;
    char *answer = (char*)calloc(ANSWER_MAX_SIZE, sizeof(char));

    #define READ_ANSWER_()                                     \
        do                                                     \
        {                                                      \
            memset(answer, 0, sizeof(char) * ANSWER_MAX_SIZE); \
            fgets(answer, ANSWER_MAX_SIZE, stdin);             \
            answer[strcspn(answer, "\n")] = '\0';              \
            printf("\n");                                      \
        } while(0)

    while (node->left != nullptr && node->right != nullptr)
    {
        printf("%s?\n", node->elem);
        READ_ANSWER_();
        node = ((strcmp(answer, "нет") == 0) ? node = node->left : node = node->right);
    }

    printf("Это %s?\n", node->elem);
    READ_ANSWER_();

    if (strcmp(answer, "нет") == 0)
    {
        printf("Кто это?\n");
        READ_ANSWER_();
        TreeInsert(tree, node, node->elem, LEFT_CHILD, &treeError);
        TreeInsert(tree, node, answer, RIGHT_CHILD, &treeError);
        printf("Чем %s отличается от %s?\n", answer, node->elem);
        READ_ANSWER_();
        strcpy(node->elem, answer);
        printf("Сохранить базу данных?\n");
        READ_ANSWER_();
        if (strcmp(answer, "нет") == 0)
        {
            printf("База данных не сохранена!\n");
        }
        else
        {
            treeError = TreeSaveInFile(tree);
            printf("База данных сохранена!\n");
        }
    }
    else
    {
        printf("Система угадала правильный ответ!\n");
    }

    free(answer);

    #ifdef DEBUG
        TreeDump(tree);
    #endif // DEBUG

    #undef READ_ANSWER_

    return treeError;
}

static stack_t* NodeObjectDefinition(const Node_t *node, const char *object, stack_t *stack)
{
    assert(node != nullptr);
    assert(stack != nullptr);
    assert(object != nullptr);

    char* ptrStr[1] = {};

    if (stack->size != 0)
    {
        StackPop(stack, ptrStr);
        if (strcmp(*ptrStr, object) == 0)
        {
            StackPush(stack, (stackData_t)object);
            return stack;
        }
        StackPush(stack, (stackData_t)*ptrStr);
    }

    if (node->left != nullptr)
    {
        if (node->left != nullptr)
        {
            StackPush(stack, (stackData_t)"не");
            StackPush(stack, node->elem);
            stack = NodeObjectDefinition(node->left, object, stack);
        }

        if (stack->size != 0)
        {
            StackPop(stack, ptrStr);
            if (strcmp(*ptrStr, object) == 0)
            {
                StackPush(stack, (stackData_t)object);
                return stack;
            }
            else
            {
                StackPop(stack, ptrStr);
            }
        }

        if (node->right != nullptr)
        {
            StackPush(stack, node->elem);
            stack = NodeObjectDefinition(node->right, object, stack);
        }

    }
    else
    {
        if (strcmp(node->elem, object) == 0) StackPush(stack, node->elem);
    }

    return stack;
}

static void ObjectDefinitionPrint(const char *object, stack_t *stack)
{
    assert(object != nullptr);
    assert(stack != nullptr);

    char* ptrStr[1] = {};

    if (stack->size == 4)
    {
        StackPop(stack, ptrStr);
        char* buf[1] = {};
        StackPop(stack, ptrStr);
        StackPop(stack, buf);
        if (strcmp(*ptrStr, *buf) == 0)
        {
            printf("Такого объекта не существует в базе.\n");
            return;
        }
    }

    printf("%s : ", object);
    size_t i = stack->size;
    for (; i > 1; i--)
    {
        StackPop(stack, ptrStr);
        if (strcmp(*ptrStr, "не") == 0)
        {
            printf("%s ", *ptrStr);
            StackPop(stack, ptrStr);
            i--;
        }
        if (i == 2)
        {
            printf("%s.", *ptrStr);
        }
        else
        {
            printf("%s, ", *ptrStr);
        }
    }
}

static stack_t* ObjectDefinition(const Tree_t *tree, const char *object, stack_t *stack)
{
    assert(tree != nullptr);
    assert(object != nullptr);
    assert(stack != nullptr);

    stack = NodeObjectDefinition(tree->root, object, stack);

    StackReverse(stack);

    return stack;
}

stack_t* ObjectDefinitionMode(const Tree_t *tree, const char *object, stack_t *stack)
{
    assert(tree != nullptr);
    assert(object != nullptr);
    assert(stack != nullptr);

    stack = ObjectDefinition(tree, object, stack);

    ObjectDefinitionPrint(object, stack);

    return stack;
}

void ObjectComparisonMode(const Tree_t *tree, const char *object1, const char *object2)
{
    assert(tree != nullptr);
    assert(object1 != nullptr);
    assert(object2 != nullptr);

    stack_t stack1 = {};
    stack_t stack2 = {};

    STACKCTOR_(&stack1, STACK_SIZE);
    STACKCTOR_(&stack2, STACK_SIZE);

    stack1 = *ObjectDefinition(tree, object1, &stack1);
    stack2 = *ObjectDefinition(tree, object2, &stack2);

    size_t sizeStack1 = stack1.size, sizeStack2 = stack2.size;
    char* ptrStr1[1] = {};
    char *ptrStr2[1] = {};

    printf("%s и %s схожи тем, что он и он : ", object1, object2);

    size_t i1 = 0, i2 = 0;
    for (; i1 < sizeStack1 && i2 < sizeStack2; i1++, i2++)
    {
        StackPop(&stack1, ptrStr1);
        StackPop(&stack2, ptrStr2);

        if (strcmp(*ptrStr1, *ptrStr2) != 0)
        {
            StackPush(&stack1, (stackData_t)*ptrStr1);
            StackPush(&stack2, (stackData_t)*ptrStr2);
            break;
        }

        if (strcmp(*ptrStr1, "не") == 0)
        {
            printf("%s ", *ptrStr1);
        }
        else
        {
            printf("%s, ", *ptrStr1);
        }
    }

    printf("а отличаются тем, что ");
    ObjectDefinitionPrint(object1, &stack1);

    printf(" A ");
    ObjectDefinitionPrint(object2, &stack2);

    StackDtor(&stack1);
    StackDtor(&stack2);
}

void TreeShow(const Tree_t *tree)
{
    assert(tree != nullptr);

    system("mimeopen -d /home/kostya/Akinator/graphviz.png\n");
    system("1");
}
