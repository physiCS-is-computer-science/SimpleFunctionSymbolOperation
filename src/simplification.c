/***
 * Do the best to simplify the tree!!!
 */
#include "linus_unknow_linux.h"
#include <math.h>
#include <stdbool.h>

void destroyTree(Tree* root);
bool isNumNode(Tree* root);

void numSimp(Tree* root) {
    if (root == NULL)
        return;

    numSimp(root->left);
    numSimp(root->right);

    /* a +-*^ a，除法总是出小数，不考虑 */
    if (root->isOp && root->left->isNum && root->right->isNum && root->op != '/') {
        double lNum = root->left->num, rNum = root->right->num;

        if (root->op == '+')
            root->num = lNum + rNum;
        else if (root->op == '-')
            root->num = lNum - rNum;
        else if (root->op == '*')
            root->num = lNum * rNum;
        else if (root->op == '^')
            root->num = pow(lNum, rNum);

        root->isOp = FALSE_CH;
        root->op = FALSE_CH;
        root->isNum = TRUE_CH;

        destroyTree(root->left);
        destroyTree(root->right);
        root->left = NULL;
        root->right = NULL;
    }

    if (root->isOp && root->op == '*') {
        /* 0*a == 0 or a*0 == 0 */
        if ((root->left->isNum && root->left->num == 0) || (root->right->isNum && root->right->num == 0)) {
            root->isOp = FALSE_CH;
            root->op = FALSE_CH;
            root->isNum = TRUE_CH;
            root->num = 0;

            destroyTree(root->left);
            destroyTree(root->right);
            root->left = NULL;
            root->right = NULL;
        }
    }

    /* 0/a == 0 */
    if (root->isOp && root->op == '/' && root->left->isNum && root->left->num == 0) {
        root->isOp = FALSE_CH;
        root->op = FALSE_CH;
        root->isNum = TRUE_CH;
        root->num = 0;

        destroyTree(root->left);
        destroyTree(root->right);
        root->left = NULL;
        root->right = NULL;
    }
}

/* 1*a == a or a*1 == a */
void timesOneSimp(Tree* root) {
    if (root == NULL)
        return;

    timesOneSimp(root->left);
    timesOneSimp(root->right);

    if (root->isOp && root->op == '*') {
        if (root->left->isNum && root->left->num == 1) {
            Tree* tmp = root->right;
            destroyTree(root->left);
            *root = *(root->right);

            printf("{%p}will be freedom\n", tmp); // test
            free(tmp); // 此时 tmp 和 root 都连着相同的两个儿子，只能释放 tmp 节点自身
        }
        else if (root->right->isNum && root->right->num == 1) {
            Tree* tmp = root->left;
            destroyTree(root->right);
            *root = *(root->left);

            printf("{%p}will be freedom\n", tmp); // test
            free(tmp); // 同上
        }
    }
}

/* a/1 == a */
void divOneSimp(Tree* root) {
    if (root == NULL)
        return;

    divOneSimp(root->left);
    divOneSimp(root->right);

    if (root->isOp && root->op == '/') {
        if (root->right->isNum && root->right->num == 1) {
            Tree* tmp = root->left;
            destroyTree(root->right);
            *root = *(root->left);

            printf("{%p}will be freedom\n", tmp); // test
            free(tmp);
        }
    }
}

/* a^1 == a */
void powOneSimp(Tree* root) {
    if (root == NULL)
        return;
    powOneSimp(root->left);
    powOneSimp(root->right);

    if (root->isOp && root->op == '^' && isNumNode(root->right)) {
        numSimp(root->right);
        if (root->right->isNum && root->right->num == 1) {
            Tree* tmp = root->left;
            destroyTree(root->right);
            *root = *(root->left);

            printf("{%p}will be freedom\n", tmp); // test
            free(tmp);
        }
    }
}

/* a^0 == 1 */
void powZeroSimp(Tree* root) {
    if (root == NULL)
        return;
    powZeroSimp(root->left);
    powZeroSimp(root->right);

    if (root->isOp && root->op == '^' && isNumNode(root->right)) {
        numSimp(root->right);
        if (root->right->isNum && root->right->num == 0) {
            destroyTree(root->left);
            destroyTree(root->right);
            root->left = NULL;
            root->right = NULL;

            root->isOp = FALSE_CH;
            root->op = FALSE_CH;
            root->isNum = TRUE_CH;
            root->num = 1;
        }
    }
}

/* 0+a == a or a+0 == a */
void addZeroSimp(Tree* root) {
    if (root == NULL)
        return;
    addZeroSimp(root->left);
    addZeroSimp(root->right);

    if (root->isOp && root->op == '+') {
        if (root->left->isNum && root->left->num == 0) {
            Tree* tmp = root->right;
            destroyTree(root->left);
            *root = *(root->right);

            printf("{%p}will be freedom\n", tmp); // test
            free(tmp);
        }
        else if (root->right->isNum && root->right->num == 0) {
            Tree* tmp = root->left;
            destroyTree(root->right);
            *root = *(root->left);

            printf("{%p}will be freedom\n", tmp); // test
            free(tmp);
        }
    }
}

/* a-0 == a */
void subZeroSimp(Tree* root) {
    if (root == NULL)
        return;
    subZeroSimp(root->left);
    subZeroSimp(root->right);

    if (root->isOp && root->op == '-' && isNumNode(root->right)) {
        numSimp(root->right);
        if (root->right->isNum && root->right->num == 0) {
            Tree* tmp = root->left;
            destroyTree(root->right);
            *root = *(root->left);

            printf("{%p}will be freedom\n", tmp); // test
            free(tmp);
        }
    }
}