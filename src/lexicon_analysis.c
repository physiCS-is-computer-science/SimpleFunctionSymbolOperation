#include "temp.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

TreeNode* linkNode(TreeNode* leftLeaf, TreeNode* rightLeaf);
TreeNode* aNode(int type);
void destroyTree(TreeNode* root);

int identifyToken(const char* exp, Token* token, const char* comm) {
    char copyExp[COMMAND_SIZE];

    /* simple check expression string */
    strcpy(copyExp, exp);
    ptrdiff_t length = strchr(copyExp, '\0') - copyExp; // comput length of exp
    if (length < 3) {
        wrongPrint(comm, strchr(comm, '(') + 1, "-=-= Too short =-=-");
        return 0;
    }

    /* real check */
    int i;
    char *is_operator, *endptr;
    char* currentCh = copyExp;
    for (i = 0; *currentCh != '\0'; currentCh++) {
        is_operator = strchr(TOKEN_SYMBOL, *currentCh); // +-*/^()

        if ((currentCh == copyExp) && (is_operator != NULL)) { // if first element isn't positive
            if (*is_operator == '-') {
                currentCh++; // next symbol of '-'
                if (*currentCh == 'x') {
                    token[i].isvariable = 't';
                    strcpy(token[i].variable, "-x");
                    i++;
                }
                else if (isdigit(*currentCh)) { // is num
                    token[i].isnum = 't';
                    token[i].num = -(int)strtol(currentCh, &endptr, 10);
                    currentCh = endptr;
                    currentCh--;
                    i++;
                }
                else {
                    wrongPrint(comm, strchr(comm, '(') + 1, "-=-= Only [x] or [number] are allowed after the first [-] =-=-");
                    return 0;
                }
                continue; // jump to next loop
            }
        }
        if (!is_operator) { // is not operator
            if (*currentCh == 'x') { // is x
                token[i].isvariable = 't';
                strcpy(token[i].variable, "x");
                i++;
            }
            else if (isdigit(*currentCh)) { // is num
                token[i].isnum = 't';
                token[i].num = (int)strtol(currentCh, &endptr, 10);
                currentCh = endptr;
                currentCh--;
                i++;
            }
            else if (*currentCh == ' ') {
                continue;
            }
            else {
                wrongPrint(comm, strchr(comm, '(') + 1, "-=-= bufferClear() find some mistakes, Enter to clear =-=-");
                return 0;
            }
        }
        else { // is operator
            token[i].isoperator = 't';
            token[i].operator = *is_operator;
            i++;
        }
    }

    /* conbine first '-' and next term, number of token must be odd */
    if (i % 2 == 0) {
        wrongPrint(comm, strchr(comm, '(') + 1, "-=-= Number of character error =-=-");
        return 0;
    }

    if (token[i - 1].isoperator == 't' && token[i - 1].operator != ')') {
        wrongPrint(comm, strchr(comm, '(') + 1, "-=-= Final symbol must be [number] or not-[operator] =-=-");
        return 0;
    }

    return i;
}

void initializeToken(Token* token) {
    for (int i = 0; i < COMMAND_SIZE; i++) {
        token[i].isnum = 'f';
        token[i].isoperator = 'f';
        token[i].isvariable = 'f';
    }
}

int level(char operator) {
    switch (operator) {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    }
}

int needPop(const char* stack, int j, char currOperator) { // j--回到栈顶，currOperator一定是 +-*/^ 之一
    int i;
    if (currOperator == '^')
        return 0;
    for (i = 1, j--; j >= 0; i++, j--) {
        if (level(stack[j]) < level(currOperator) || stack[j] == '(')
            break;
    }

    return i - 1;
}

int infixToPostfix(const Token* token, int tokenNum, Token* postfix, const char* comm) {
    int i, j, k, needNum;
    int door = 1;
    char operatorStack[COMMAND_SIZE] = {'\0'};

    if (token[0].isoperator == 't' && token[0].operator == '(') // the situation of the first is '('
        door = 2;

    for (i = 0, j = 0, k = 0; i < tokenNum; i++) {
        switch (door) {
        case 1: {
            if (token[i].isnum == 't' || token[i].isvariable == 't') {
                postfix[k] = token[i]; // strcture can do this
                k++;
                door = 2;
            }
            else {
                bufferClear("-=-= There is a [character] that should be [number] =-=-\n");
                return 0;
            }
            break;
        }
        case 2: { // 栈序号为栈顶序号下一个
            if (token[i].isoperator == 't' && token[i].operator == '(') { // 直接入栈
                operatorStack[j] = token[i].operator;
                j++;
                door = 1;
            }
            else if (token[i].isoperator == 't' && token[i].operator == ')') { // 弹出()之间所有操作符
                for (j--; operatorStack[j] != '('; k++, j--) { // 左j--回到栈顶，循环结束是j为'('，自身即为栈顶的next
                    postfix[k].isoperator = 't';
                    postfix[k].operator = operatorStack[j];
                }
                door = 2;
            }
            else if (token[i].isoperator == 't' && strchr(OPERATOR, token[i].operator)) {
                needNum = needPop(operatorStack, j, token[i].operator); // 返回需要弹出栈次数

                if (needNum != 0) { // 弹出优先级不高于它的操作符，然后它入栈
                    j--; // 回栈顶
                    for (int count = 1; count <= needNum; count++) {
                        postfix[k].isoperator = 't';
                        postfix[k].operator = operatorStack[j];
                        k++;
                        j--;
                    }
                    j++; // 回栈顶的next
                }
                operatorStack[j] = token[i].operator; // 入栈
                j++;
                door = 1;
            }
            else {
                bufferClear("\n-=-= There is/are (a) [character] that should be [opetator] =-=-");
                return 0;
            }

            if (token[i + 1].isoperator == 't' && token[i + 1].operator == '(')
                door = 2;

            break;
        }
        }
    }

    j--; // 回栈顶
    for (; j >= 0; j--, k++) { // 清空栈
        postfix[k].isoperator = 't';
        postfix[k].operator = operatorStack[j];
    }

    return k;
}

TreeNode* convertTree(const Token* postfix, int num, const char* comm) {
    int topUp = 0;
    TreeNode* stack[COMMAND_SIZE];
    TreeNode* current;

    for (int i = 0; i < num; i++, topUp++) { // 1-num 2-x 3-+-*/^ #### topUp++是为了回栈顶下一个
        if (postfix[i].isnum == 't') {
            current = aNode(1); // create a node
            current->operand = postfix[i].num; // initialized it
            stack[topUp] = current;
        }
        else if (postfix[i].isvariable == 't') {
            current = aNode(2);
            strcpy(current->x, postfix[i].variable); // initialized it, but it is a string("-x\0" or "x\0")
            stack[topUp] = current;
        }
        else if (postfix[i].isoperator == 't') {
            /* wrong situation */
            if (topUp < 2) {
                if (topUp == 0) // stack is empty
                    bufferClear("-=-= The expression have some problems(stack is empty) =-=-");
                else if (topUp == 1) {
                    destroyTree(stack[topUp - 1]);
                    bufferClear("-=-= The expression have some problems(only one operand left in the stack) =-=-");
                }
                return NULL;
            }

            current = linkNode(stack[topUp - 2], stack[topUp - 1]);
            current->is_operator = 't'; // initialize
            current->operator = postfix[i].operator;

            topUp = topUp - 2;
            stack[topUp] = current;
        }
    }

    /* 不知什么情况 */
    if (topUp != 1) {
        for (int i = 0; i < num; i++)
            destroyTree(stack[i]);
        bufferClear("-=-= The expresion heve some situations =-=-");
    }

    return stack[topUp - 1];
}
