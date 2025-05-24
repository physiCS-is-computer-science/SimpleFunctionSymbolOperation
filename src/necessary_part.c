/*
 * this part include some important tools
 * function _xxx() is temprory test function that will be deleted
 */

#include "temp.h"
#include <stdio.h>
#include <stdlib.h>

void __processQuitGetchar(void) {
    printf("-=-= Enter to end the process =-=-");
    getchar();
    exit(0);
}

void __f5StopGetchar(void) {
    printf("\n-=-=-= Process has ended, press key Enter to close it =-=-=-");
    getchar();
}

void __printer1(int a) {
    switch (a) {
    case 1:
        printf(" 1.%s(function, order)\n", DIFF_STR);
        break;
    case 2:
        printf(" 2.%s(function, order, num)\n", DIFF_STR);
        break;
    case 3:
        printf(" 3.%s(funcion, order)\n", INTE_STR);
        break;
    case 4:
        printf(" 4.%s(function, order, [left, right])\n", INTE_STR);
        break;
    case 5:
        printf(" 5.%s(function1, function2, operator)\n", COMP_STR);
        break;
    }
}

void bufferClear(char* screenPrint) {
    if (screenPrint != NULL) {
        printf("%s", screenPrint);
    }
    while (getchar() != '\n')
        continue;
}

void destroyTree(TreeNode* root) {
    if (root == NULL)
        return;
    destroyTree(root->left);
    destroyTree(root->right);

    if (root->is_operand == 't')
        printf("freedom '%d'\n", root->operand);
    if (root->is_operator == 't')
        printf("freedom '%c'\n", root->operator);
    if (root->is_x == 't')
        printf("freedom '%s'\n", root->x);

    free(root);
}

TreeNode* linkNode(TreeNode* leftLeaf, TreeNode* rightLeaf) {
    TreeNode* current = (TreeNode*)malloc(sizeof(TreeNode));
    current->left = leftLeaf;
    current->right = rightLeaf;
    return current;
}

/* 创建左右树叶指向NULL的节点 */
TreeNode* aNode(int type) {
    TreeNode* node;
    node = (TreeNode*)malloc(sizeof(TreeNode));
    if (type == 1) { // num
        node->is_operand = 't';
        node->is_x = 'f';
        node->is_operator = 'f';
    }
    else if (type == 2) { // x
        node->is_operand = 'f';
        node->is_x = 't';
        node->is_operator = 'f';
    }
    else if (type == 3) { // +-*/^
        node->is_operand = 'f';
        node->is_x = 'f';
        node->is_operator = 't';
    }
    else {
        free(node);
        bufferClear("-=-= aNode(): No expected parameters was =-=-");
        return NULL;
    }
    node->left = NULL;
    node->right = NULL;

    return node;
}
