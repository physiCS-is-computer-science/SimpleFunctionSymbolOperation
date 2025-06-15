#include "linus_unknow_linux.h"
#include <math.h>

void simp(Tree* root) {
    if (root == NULL)
        return;

    simp(root->left);
    simp(root->right);

    if (root->isOp && root->left->isNum && root->right->isNum) { // 除了'/'之外的纯数节点
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
}