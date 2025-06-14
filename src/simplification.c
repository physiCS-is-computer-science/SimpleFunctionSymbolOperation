#include "temp.h"

void simp(Tree* root) {
    if (root == NULL)
        return;

    simp(root->left);
    simp(root->right);

    if (root->isOp && root->left->isNum && root->right->isNum && root->op != '/') { // 除了'/'之外的纯数节点
        int lNum = root->left->num, rNum = root->right->num;

        if (root->op == '+')
            root->num = lNum + rNum;
        else if (root->op == '-')
            root->num = lNum - rNum;
        else if (root->op == '*')
            root->num = lNum * rNum;
        root->isOp = FALSE_CH;
        root->op = FALSE_CH;
        root->isNum = TRUE_CH;

        printf("[%d]: %p will be simplified\n", lNum, root->left);
        printf("[%d]: %p will be simplified\n", rNum, root->right);
        free(root->left);
        free(root->right);
        root->left = NULL;
        root->right = NULL;
    }
    if (root->isOp && root->left->isNum && root->right->isNum && root->op == '/') { // 仅仅为了'/'节点
        int lNum = root->left->num, rNum = root->right->num;
        if (lNum % rNum == 0) {
            root->num = lNum / rNum;
            root->isOp = FALSE_CH;
            root->op = FALSE_CH;
            root->isNum = TRUE_CH;

            printf("[%d]: %p will be simplified\n", lNum, root->left);
            printf("[%d]: %p will be simplified\n", rNum, root->right);
            free(root->left);
            free(root->right);
            root->left = NULL;
            root->right = NULL;
        }
    }
}