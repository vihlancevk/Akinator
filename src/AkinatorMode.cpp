#include "AkinatorMode.h"

#define DEBUG

const int ANSWER_MAX_SIZE = 100;
const int STACK_SIZE = 10;
const int OUTPUT_MESSAGE_MAX_SIZE = 100;
const int INPUT_MESSAGE_MAX_SIZE = 100;

char inputMessage[INPUT_MESSAGE_MAX_SIZE] = {};
char outputMessage[OUTPUT_MESSAGE_MAX_SIZE] = {};

static void OutputMessage(const char *message)
{
    assert(message != nullptr);

    printf("%s\n", message);
    sprintf(outputMessage, "espeak-ng -g 10 -s 150 -v ru \"%s\"", message);
    system(outputMessage);
    memset(outputMessage, 0, sizeof(char) * OUTPUT_MESSAGE_MAX_SIZE);
    memset(inputMessage , 0, sizeof(char) * INPUT_MESSAGE_MAX_SIZE);
}

static void ReadAnswer(char *answer)
{
    assert(answer != nullptr);

    memset(answer, 0, sizeof(char) * ANSWER_MAX_SIZE);
    fgets(answer, ANSWER_MAX_SIZE, stdin);
    answer[strcspn(answer, "\n")] = '\0';
    printf("\n");
}

TreeErrorCode GuessObject(Tree_t *tree)
{
    assert(tree != nullptr);

    OutputMessage("Akinator.");
    OutputMessage("Запущен режим угадывания объекта по признакам.");
    printf("\n");

    TreeErrorCode treeError = TREE_NO_ERROR;
    Node_t *node = tree->root;
    char answer[ANSWER_MAX_SIZE] = {};

    while (node->left != nullptr && node->right != nullptr)
    {
        sprintf(inputMessage, "%s", node->elem);
        OutputMessage(inputMessage);
        ReadAnswer(answer);
        node = ((strcmp(answer, "нет") == 0) ? node = node->left : node = node->right);
    }

    sprintf(inputMessage, "Это %s?", node->elem);
    OutputMessage(inputMessage);

    ReadAnswer(answer);

    if (strcmp(answer, "нет") == 0)
    {
        OutputMessage("Кто это?");

        ReadAnswer(answer);

        TreeInsert(tree, node, node->elem, LEFT_CHILD, &treeError);
        TreeInsert(tree, node, answer, RIGHT_CHILD, &treeError);

        sprintf(inputMessage, "Чем %s отличается от %s?", answer, node->elem);
        OutputMessage(inputMessage);

        ReadAnswer(answer);

        strcpy(node->elem, answer);

        OutputMessage("Сохранить базу данных?");

        ReadAnswer(answer);
        if (strcmp(answer, "нет") == 0)
        {
            OutputMessage("База данных не сохранена!");
        }
        else
        {
            treeError = TreeSaveInFile(tree);
            OutputMessage("База данных сохранена!");
        }
    }
    else
    {
        OutputMessage("Система угадала правильный ответ!");
    }

    return treeError;
}

static stack_t* NodeObjectDefinition(const Node_t *node, const char *object, stack_t *stack)
{
    assert(node != nullptr);
    assert(stack != nullptr);
    assert(object != nullptr);

    char* ptrStr[1] = {};

    if (node->left != nullptr)
    {
        if (node->left != nullptr)
        {
            StackPush(stack, (stackData_t)"не");
            StackPush(stack, node->elem);
            stack = NodeObjectDefinition(node->left, object, stack);
        }

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

        if (node->right != nullptr)
        {
            StackPush(stack, node->elem);
            stack = NodeObjectDefinition(node->right, object, stack);
        }

        StackPop(stack, ptrStr);
        if (strcmp(*ptrStr, object) == 0)
        {
            StackPush(stack, (stackData_t)object);
            return stack;
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
            OutputMessage("Такого объекта не существует в базе.");
            return;
        }
    }

    sprintf(inputMessage, "%s : ", object);
    OutputMessage(inputMessage);
    size_t i = stack->size;
    for (; i > 1; i--)
    {
        StackPop(stack, ptrStr);
        if (strcmp(*ptrStr, "не") == 0)
        {
            sprintf(inputMessage, "%s ", *ptrStr);
            OutputMessage(inputMessage);
            StackPop(stack, ptrStr);
            i--;
        }
        if (i == 2)
        {
            sprintf(inputMessage, "%s.", *ptrStr);
            OutputMessage(inputMessage);
        }
        else
        {
            sprintf(inputMessage, "%s, ", *ptrStr);
            OutputMessage(inputMessage);
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

stack_t* DefineObjectMode(const Tree_t *tree, const char *object, stack_t *stack)
{
    assert(tree != nullptr);
    assert(object != nullptr);
    assert(stack != nullptr);

    stack = ObjectDefinition(tree, object, stack);

    ObjectDefinitionPrint(object, stack);

    return stack;
}

void CompairObjectsMode(const Tree_t *tree, const char *object1, const char *object2)
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

    sprintf(inputMessage, "%s и %s", object1, object2);
    OutputMessage(inputMessage);

    OutputMessage("схожи тем, что оба:");

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
            sprintf(inputMessage, "%s ", *ptrStr1);
            OutputMessage(inputMessage);
        }
        else
        {
            sprintf(inputMessage, "%s ", *ptrStr1);
            OutputMessage(inputMessage);
        }
    }

    OutputMessage("а отличаются тем, что");
    ObjectDefinitionPrint(object1, &stack1);

    OutputMessage("A");
    ObjectDefinitionPrint(object2, &stack2);

    StackDtor(&stack1);
    StackDtor(&stack2);
}

void ShowTree(const Tree_t *tree)
{
    assert(tree != nullptr);

    system("mimeopen -d /home/kostya/Akinator/graphviz.png\n");
}
