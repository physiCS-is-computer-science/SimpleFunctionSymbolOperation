/*-----------|------------|---------------|-------------|---------|
 |  1.a+-b   | 2.f*c c*f  |     3.x^c     |     4.x     |   5.c   |
 |-----------|------------|---------------|-------------|---------|
 |   (+-)s   |     (*)s   |      (^)s     |     (x)s    |   (c)s  |
 |   /  \    |    /  \    |     /  \      |             |         |
 | [f]  [g]  |  [f]  [c]  |   [x]  [c]    |             |         |
 |-----------|------------|---------------|-------------|---------|
 |   [+-]    |     [*]    |       [/]     |       [/]   |   [*]   |
 |   /  \    |    /  \    |      /  \     |      /  \   |   / \   |
 | (f)s (g)s | (f)s  [c]  |    [^]  [c+1] |    [^]  [2] | [c] [x] |
 |           |            |   /  \        |   /  \      |         |
 |           |            | [x] [c+1]     | [x]  [2]    |         |
 |-----------|------------|---------------|-------------|--------*/
/***
 * just processing: inte(ax^n)
 */

#include "linus_unknow_linux.h"
#include <stdbool.h>

bool isNumNode(Tree* root);
Tree* copyTree(const Tree* node);
Tree* varNode(void);
Tree* one(void);
Tree* powNode(void);

/* find some situation:
 * 1.f(x)*f(x)
 * 2./ 运算 (c/c 已化简为 c)
 * 3.非 x^c 的 ^ 运算(c^c 已化简为 c) */
char inteTreeCorrect(Tree* root) {
    if (root == NULL)
        return TRUE_CH;
    if (inteTreeCorrect(root->left) == FALSE_CH)
        return FALSE_CH;
    if (inteTreeCorrect(root->right) == FALSE_CH)
        return FALSE_CH;

    if (root->isOp && root->op == '*') { // 1
        if (!isNumNode(root->left) && !isNumNode(root->right)) {
            printf("\n<< inteTreeCorrect(): The function is too complex!\n");
            return FALSE_CH;
        }
    }
    if (root->isOp && root->op == '/') { // 2
        printf("\n<< inteTreeCorrect(): The function is too complex!\n");
        return FALSE_CH;
    }
    if (root->isOp && root->op == '^') { // 3
        if (!(root->left->isVar && isNumNode(root->right))) {
            printf("\n<< inteTreeCorrect(): The function is too complex!\n");
            return FALSE_CH;
        }
    }

    return TRUE_CH;
}

void inte(Tree* root) {
    if (root == NULL)
        return;

    if (root->isInte && root->isNum) { // 5.num
        root->isInte = FALSE_CH;

        Tree* temp = one(); // 值为1的数字节点
        temp->num = root->num;

        root->left = temp;
        root->right = varNode(); // x

        root->isNum = FALSE_CH;
        root->num = 0;
        root->isOp = TRUE_CH;
        root->op = '*';
    }
    else if (root->isInte && root->isVar) { // 4.x
        root->isInte = FALSE_CH;

        /* x^2 */
        Tree* power = powNode();
        power->left = varNode();
        power->right = one();
        power->right->num = 2;

        root->left = power;
        root->right = one();
        root->right->num = 2;

        root->isVar = FALSE_CH;
        root->var = FALSE_CH;
        root->isOp = TRUE_CH;
        root->op = '/';
    }
    else if (root->isInte && root->isOp && (root->op == '+' || root->op == '-')) { // 1.+-
        root->isInte = FALSE_CH;
        root->left->isInte = TRUE_CH;
        root->right->isInte = TRUE_CH;

        inte(root->left);
        inte(root->right);
    }
    else if (root->isInte && root->isOp && root->op == '*') { // 2.just c*f(x) or f(x)*c
        root->isInte = FALSE_CH;

        if (!isNumNode(root->left) && isNumNode(root->right)) {
            root->left->isInte = TRUE_CH;
            inte(root->left);
        }
        if (isNumNode(root->left) && !isNumNode(root->right)) {
            root->right->isInte = TRUE_CH;
            inte(root->right);
        }
    }
    else if (root->isInte && root->isOp && root->op == '^') { // 3.just x^c
        root->isInte = FALSE_CH;

        if (root->left->isVar && root->right->isNum) {
            Tree* cpyPower = copyTree(root);
            cpyPower->right->num += 1;

            root->left = cpyPower;
            root->right = one();
            root->right->num = cpyPower->right->num;

            root->op = '/';
        }
    }
}