/*------------|--------------------|----------------------------|------------------|
 |   1.a+-b   |       2.a*b        |       3.a/b (b != 0)       | 4.a^b (b == num) |
 |------------|--------------------|----------------------------|------------------|
 |    (+-)'   |        (*)'        |            (/)'            |        (^)'      |
 |    /  \    |       /   \        |           /   \            |       /  \       |
 |  [a]  [b]  |     [a]   [b]      |         [a]   [b]          |     [a]  [b]     |
 |------------|--------------------|----------------------------|------------------|
 |    [+-]    |        [+]         |          [+]               |         [*]      |
 |    /  \    |       /   \        |        /    \              |        /  \      |
 | (a)'  (b)' |    [*]     [*]     |     [*]      [*]           |      [*]  (a)'   |
 |            |   /  \    /   \    |    /  \      /  \          |     /  \         |
 |            | (a)' [b] [a]  (b)' | (a)' [^]   [a]   [*]       |   [b]  [^]       |
 |            |                    |     /  \         / \       |        / \       |
 |            |                    |   [b] [-]      [*] (b)'    |      [a] [-]     |
 |            |                    |       / \     /   \        |          / \     |
 |            |                    |     [0] [1] [-]   [^]      |        [b] [1]   |
 |            |                    |            /  \   /  \     |                  |
 |            |                    |          [0] [1] [b] [-]   |                  |
 |            |                    |                      / \   |                  |
 |            |                    |                    [-] [1] |                  |
 |            |                    |                   /  \     |                  |
 |            |                    |                 [0]  [1]   |                  |
 |------------|--------------------|----------------------------|-----------------*/

#include "linus_unknow_linux.h"
#include <stdbool.h>

Tree* copyTree(const Tree* node); // 由 node 往下的树复制一份并返回根节点指针，副本树仅仅复制"op""num""var""left""right"，其他的都初始化为FALSE_CH
void initNode(Tree* node); // clear all of the node
void destroyTree(Tree* root);
Tree* addNode(void);
Tree* subNode(void);
Tree* timesNode(void);
Tree* powNode(void);
Tree* one(void);
Tree* sub01(void);
bool isNumNode(Tree* root);

/* differentiate module */
void diffAddSub(Tree* root) { // + or -
    root->isDiff = FALSE_CH; // 1 floor

    root->left->isDiff = TRUE_CH; // 2 floor
    root->right->isDiff = TRUE_CH;
}
void diffTimes(Tree* root) { // *
    root->isDiff = FALSE_CH; // 1 floor
    Tree *leftCopy = copyTree(root), *rightCopy = copyTree(root);
    root->op = '+';

    root->left = leftCopy; // 2 3 floor
    root->right = rightCopy;

    leftCopy->left->isDiff = TRUE_CH; // a' 3 floor
    rightCopy->right->isDiff = TRUE_CH; // b'
}
void diffDiv(Tree* root) { // /
    Tree *a = root->left, *b = root->right;
    Tree *aCpy = copyTree(a), *bCpy1 = copyTree(b), *bCpy2 = copyTree(b);

    root->isDiff = FALSE_CH; // 1
    root->op = '+';

    root->left = timesNode(); // 2
    root->right = timesNode();

    root->left->left = a; // 3
    a->isDiff = TRUE_CH;
    root->left->right = powNode();
    root->right->left = aCpy;
    root->right->right = timesNode();

    root->left->right->left = b; // 4
    root->left->right->right = sub01();
    root->right->right->left = timesNode();
    root->right->right->right = bCpy1;
    bCpy1->isDiff = TRUE_CH;

    root->right->right->left->left = sub01(); // 5
    root->right->right->left->right = powNode();

    root->right->right->left->right->left = bCpy2; // 6
    root->right->right->left->right->right = subNode();

    root->right->right->left->right->right->left = sub01(); // 7
    root->right->right->left->right->right->right = one();
}
void diffPow(Tree* root) { // ^
    Tree *a = root->left, *b = root->right;
    Tree* aCpy = copyTree(a);
    Tree* bCpy = copyTree(b);

    root->isDiff = FALSE_CH; // 1 floor
    root->op = '*';

    root->left = timesNode(); // 2 floor
    root->right = aCpy;
    aCpy->isDiff = TRUE_CH;

    root->left->left = b; // 3 floor
    root->left->right = powNode();

    root->left->right->left = a; // 4 floor
    root->left->right->right = subNode();

    root->left->right->right->left = bCpy; // 5 floor
    root->left->right->right->right = one();
}

void diff(Tree* root) {
    if (root == NULL)
        return;

    if (root->isDiff && root->isOp) {
        /* 纯数树直接为 0 */
        if (isNumNode(root)) {
            root->isOp = FALSE_CH;
            root->op = FALSE_CH;
            root->isNum = TRUE_CH;
            root->num = 0;
            destroyTree(root->left);
            destroyTree(root->right);
            root->left = NULL;
            root->right = NULL;
        }

        /* normal */
        if (root->op == '+' || root->op == '-')
            diffAddSub(root);
        else if (root->op == '*')
            diffTimes(root);
        else if (root->op == '/')
            diffDiv(root);
        else if (root->op == '^')
            diffPow(root);

        root->isDiff = FALSE_CH; //
    }
    else if (root->isDiff && root->isNum) {
        root->num = 0;
        root->isDiff = FALSE_CH; //
    }
    else if (root->isDiff && root->isVar) {
        root->isVar = FALSE_CH;
        root->var = FALSE_CH;
        root->isNum = TRUE_CH;
        root->num = 1;
        root->isDiff = FALSE_CH; //
    }

    diff(root->left);
    diff(root->right);
}

void substitutionX(Tree* root, double x) {
    if (root == NULL)
        return;
    substitutionX(root->left, x);
    substitutionX(root->right, x);
    if (root->isVar) {
        root->num = x;
        root->isNum = TRUE_CH;
        root->isVar = FALSE_CH;
        root->var = FALSE_CH;
    }
}