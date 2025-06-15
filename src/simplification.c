#include "linus_unknow_linux.h"
#include <math.h>

void destroyTree(Tree* root);

void simp(Tree* root) {
    if (root == NULL)
        return;

    simp(root->left);
    simp(root->right);

    if (root->isOp && root->left->isNum && root->right->isNum) { // num +-*/^ num
        double lNum = root->left->num, rNum = root->right->num;

        if (root->op == '+')
            root->num = lNum + rNum;
        else if (root->op == '-')
            root->num = lNum - rNum;
        else if (root->op == '*')
            root->num = lNum * rNum;
        else if (root->op == '/')
            root->num = lNum / rNum;
        else if (root->op == '^')
            root->num = pow(lNum, rNum);

        root->isOp = FALSE_CH;
        root->op = FALSE_CH;
        root->isNum = TRUE_CH;

        printf("[%.2lf]: %p will be simplified\n", lNum, root->left);
        printf("[%.2lf]: %p will be simplified\n", rNum, root->right);
        free(root->left);
        free(root->right);
        root->left = NULL;
        root->right = NULL;
    }

    if (root->op == '*') { // such as: 0 * 0
        if ((root->left->isNum && root->left->num == 0) || (root->right->isNum && root->right->num == 0)) {
            root->isOp = FALSE_CH;
            root->op = FALSE_CH;
            root->isNum = TRUE_CH;
            root->num = 0;

            destroyTree(root->left);
            destroyTree(root->right);
        }
    }

    if (root->op == '/' && root->left->isNum && root->left->num == 0) { // 0 / something
        root->isOp = FALSE_CH;
        root->op = FALSE_CH;
        root->isNum = TRUE_CH;
        root->num = 0;

        destroyTree(root->left);
        destroyTree(root->right);
    }
}