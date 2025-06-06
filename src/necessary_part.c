/*
 * this part include some important tools
 * function __xxx() is temprory test function that will be deleted
 */

#include "temp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void __processQuitGetchar(void) { // 临时中断程序函数
    printf("-=-= Enter to end the process(from function __processQuitGetchar()) =-=-");
    getchar();
    exit(0);
}

void __printer1(int a) {
    putchar('\n');
    if (a == 1)
        printf("1.%s(function, order)\n", DIFF_STR);
    if (a == 2)
        printf("2.%s(function, order, num)\n", DIFF_STR);
    if (a == 3)
        printf("3.%s(funcion, order)\n", INTE_STR);
    if (a == 4)
        printf("4.%s(function, order, [left, right])\n", INTE_STR);
    if (a == 5)
        printf("5.%s(function1, function2, operator)\n", COMP_STR);
}

void bufferClear(char* screenPrint) {
    if (screenPrint != NULL) {
        printf("%s", screenPrint);
    }
    while (getchar() != '\n')
        continue;
}

void destroyTree(Tree* root) {
    if (root == NULL)
        return;
    destroyTree(root->left);
    destroyTree(root->right);

    /* -=-=-=-=-= StartTest =-=-=-=-=- */
    if (root->isNum)
        printf("freedom '%d'\n", root->num);
    if (root->isOp == 't')
        printf("freedom '%c'\n", root->op);
    if (root->isVar == 't')
        printf("freedom '%s'\n", root->var);
    /* -=-=-=-=-= 00EndTest =-=-=-=-=- */

    free(root);
}

Tree* linkNode(Tree* leftLeaf, Tree* rightLeaf) {
    Tree* current = (Tree*)malloc(sizeof(Tree));
    current->left = leftLeaf;
    current->right = rightLeaf;
    return current;
}

/* 创建左右树叶儿子都指向NULL的节点 */
Tree* aNode(int type) {
}

/* 四种基本栈操作(char类型栈) */
char chPush(char stack[], char aim, int size) { // 成功则返回压入字符，失败返回 FALSE_CH
    int i = 0;
    while (stack[i] != FALSE_CH) {
        i++;
        if (i >= size - 1)
            return FALSE_CH;
    }
    stack[i] = aim;
    return stack[i];
}
char chPop(char stack[], int size) { // 成功则返回弹出の栈顶字符，失败返回 FALSE_CH，栈满时同样弹出栈顶
    if (stack[0] == FALSE_CH) // 空栈
        return FALSE_CH;

    char top;
    if (stack[size - 1] != FALSE_CH) { // 满栈
        top = stack[size - 1];
        stack[size - 1] = FALSE_CH;
        return top;
    }

    int i = -1;
    while (stack[++i] != FALSE_CH) // 普通情况
        continue;
    top = stack[--i];
    stack[i] = FALSE_CH;
    return top;
}
char chTop(char stack[], int size) { // 返回栈顶或 FALSE_CH
    if (stack[size - 1] != FALSE_CH) // 满栈
        return stack[size - 1];

    if (stack[0] == FALSE_CH) // 空栈
        return FALSE_CH;

    int i = -1;
    while (stack[++i] != FALSE_CH)
        continue;
    return stack[--i];
}
char isChEmpty(char stack[]) { // 空栈返回 TRUE_CHAR，非空栈返回 FALSE_CH
    if (stack[0] == FALSE_CH)
        return TRUE_CH;
    else
        return FALSE_CH;
}

/* 四种基本栈操作(Token*类型栈) */
