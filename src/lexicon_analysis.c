/***
 * 转换为 token 数组时，convertToken()函数确保存储token的数组末尾存在'空token'
 *   该'空token'确保token数组有结束符(类似'\0')
 *
 * Rules for the expression and tokens:
 * 0.number of '(' and ')' % 2
 *   if != 0 -> wP
 * 1.number of '-' % 2
 *   if it is odd: -> '-'
 *   if it is even:-> '+'
 *   if "++++...": -> '+'
 * 2.
 *   2.1.unary '-':
 *     if exp[0] == '-': -> '~'
 *     if have "/^*(" in the left of '-': -> '~'
 *   2.2.unary '+':
 *     if exp[0] == '+': -> ' '
 *     if have "/^*(" in the left of '+': -> ' '
 * 3./ 和 * 左边一定为 num，右边一定为 num 或 '~'，否则 -> wP
 *   if /0: -> wP
 * 4.遍历一遍找非"+-/^*~() 1234567890"的字符
 */

#include "linus_unknow_linux.h"
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

void fillChToken(Token* aim, char ch);
void fillNumToken(Token* aim, double num);
Token* tokenChFind(Token* tokens, char aim);
char isEmptyToken(Token* tmp); // 判断单个 token 是否为空
char chPush(char stack[], char aim, int size);
char chPop(char stack[], int size);
char isChEmpty(char stack[]);
int opLevel(char op);
Token tokenOpPush(Token stack[], Token aim, int size); // 返回 被压入栈的 token 地址或 NULL
Token tokenOpPop(Token stack[], int size); // 失败返回空栈，成功弹出并返回栈顶
Token tokenOpTop(Token stack[], int size); // 栈顶
char isTokenOpStackEmpty(Token stack[]); // 判断 token 类型栈是否为空
Tree* treePush(Tree* stack[], Tree* aim, int size);
Tree* treePop(Tree* stack[], int size);
Tree* treeTop(Tree* stack[], int size);
char isTreeStackEmpty(Tree* stack[]);
Tree* aNode(Token token);
void destroyTree(Tree* root);
void freeStack(Tree* stack[]);
bool isNumNode(Tree* root);
void numSimp(Tree* root);

/* return NULL or pointer start */
char* isAllPlus(char* start, char* end) { // 判断是否全为 ++++++...
    if (start > end) // 确保 start 指针不大于 end 指针
        return NULL;

    if (start == end && *start == '+') // only one +
        return start;
    while (start != end) { // 此处 end 判断不到，因为到 end 就退出循环了
        if (*start != '+')
            return NULL;
        start++;
    }
    if (*end == '+') // judge the last one
        return start;
    else
        return NULL;
}

void opFill(char* start, char* end, char filler) { // 将这两指针之间填充为 ‘空格’ 和 ‘filler字符’，且 filler 在 end 处
    *end = filler;
    if (start == end) // 一个时
        return;
    while (start != end) {
        *start = ' ';
        start++;
    }
}

int minusNum(const char* start, const char* end) {
    if (start == end && *start == '-')
        return 1;

    int i = 0;
    while (start != end) {
        if (*start == '-')
            i++;
        start++;
    }
    if (*end == '-')
        return ++i;
    else
        return i;
}

char hasLeft(char exp[], char* op, char* aimStr) { // 判断 op 左边最后一个字符(空格跳过)是否是字符串 aimStr 之一
    char judge = FALSE_CH;
    char* currentCh = exp;

    for (; currentCh != op; currentCh++) {
        if (*currentCh == ' ')
            continue;
        if (strchr(aimStr, *currentCh) != NULL)
            judge = TRUE_CH;
        else
            judge = FALSE_CH;
    }

    return judge;
}

void chFind(char exp[], char* ops[], char op) {
    int i = 0;
    char* currentCh = exp;
    while (*currentCh != '\0') {
        if (*currentCh == op)
            ops[i++] = currentCh;
        currentCh++;
    }
}

/* return FALSE_CH or TRUE_CH. The function just check the string of exp out simplely */
char expCorrect(char exp[]) {
    /* 0.bracket check */
    char bracketStack[COMMAND_SIZE] = {'\0'};
    char* bracketPtr = exp;
    for (int i = 0; *bracketPtr != '\0'; i++, bracketPtr++) {
        if (*bracketPtr == '(') {
            if (chPush(bracketStack, '(', sizeof(bracketStack)) == FALSE_CH) {
                wrongPrint(exp, bracketPtr, "\n<< expCorrect(): Error");
                return FALSE_CH;
            }
        }
        else if (*bracketPtr == ')') {
            if (chPop(bracketStack, sizeof(bracketStack)) == FALSE_CH) {
                wrongPrint(exp, bracketPtr, "\n<< expCorrect(): Error");
                return FALSE_CH;
            }
        }
    }
    if (!isChEmpty(bracketStack)) {
        wrongPrint(exp, exp, "\n<< expCorrect(): The bracket stack should be empty");
        return FALSE_CH;
    }

    /* 1.-+ check */
    char door = '1';
    char *opPtr = exp, *isThat = NULL, *opStart = NULL, *opEnd = NULL;

    for (; *opPtr != '\0'; opPtr++) {
        if (*opPtr == ' ')
            continue;

        isThat = strchr("-+", *opPtr); // 遇到 +- 时关上门开下门
        if (door == '1') {
            if (isThat) {
                opStart = opPtr;
                door = '2';
            }
        }
        else if (door == '2') {
            if (!isThat) {
                opEnd = opPtr;
                opEnd--;
                door = '3';
            }
        }

        if (door == '3') {
            if (isAllPlus(opStart, opEnd))
                opFill(opStart, opEnd, '+');
            else if (minusNum(opStart, opEnd) % 2 == 0)
                opFill(opStart, opEnd, '+');
            else if (minusNum(opStart, opEnd) % 2 == 1)
                opFill(opStart, opEnd, '-');
            door = '1';
        }
    }
    if (isThat) { // the situation such as "x----"
        wrongPrint(exp, opStart, "\n<< expCorrect(): Illegal end");
        return FALSE_CH;
    }

    /* 2.unary -+ check */
    char *unaryMinus[COMMAND_SIZE] = {NULL}, *unaryPlus[COMMAND_SIZE] = {NULL};
    if (exp[0] == '-')
        exp[0] = '~';
    if (exp[0] == '+')
        exp[0] = ' ';
    chFind(exp, unaryMinus, '-'); // 找到所有的第三参数符号，存储其指针(位于 exp 的指针)到 unaryMinus 数组
    for (int i = 0; unaryMinus[i] != NULL; i++) {
        if (hasLeft(exp, unaryMinus[i], "/*^("))
            *unaryMinus[i] = '~';
    }
    chFind(exp, unaryPlus, '+');
    for (int i = 0; unaryPlus[i] != NULL; i++) {
        if (hasLeft(exp, unaryPlus[i], "/*^("))
            *unaryPlus[i] = ' ';
    }

    /* 3.'/' and '*' check
     * 仅仅检查这两符号左侧最后一个字符是否为 "1234567890x)" 之一(空格不算) */
    char *allDivision[COMMAND_SIZE] = {NULL}, *allTimes[COMMAND_SIZE] = {NULL};
    chFind(exp, allDivision, '/');
    chFind(exp, allTimes, '*');
    for (int i = 0; allDivision[i] != NULL; i++) {
        if (!hasLeft(exp, allDivision[i], "1234567890x)")) {
            wrongPrint(exp, allDivision[i], "\n<< expCorrect(): Divisiion error");
            return FALSE_CH;
        }
    }
    for (int i = 0; allTimes[i] != NULL; i++) {
        if (!hasLeft(exp, allTimes[i], "1234567890x)")) {
            wrongPrint(exp, allTimes[i], "\n<< expCorrect(): Times error");
            return FALSE_CH;
        }
    }

    /* 4.查非法字符(非"1234567890 x()+-/^*~."), 空格为合法字符 */
    for (char* currPtr = exp; *currPtr != '\0'; currPtr++) {
        if (strchr("1234567890 x()+-/^*~.", *currPtr) == NULL) {
            wrongPrint(exp, currPtr, "\n<< expCorrect(): Illegal characters");
            return FALSE_CH;
        }
    }

    /* check the final character */
    char* currPtr = exp;
    for (; *currPtr != '\0'; currPtr++)
        continue;
    currPtr--; // the previous of '\0'
    if (strchr("+-*/^(.", *currPtr) != NULL) {
        wrongPrint(exp, currPtr, "\n<< expCorrect(): Should be not an operator");
        return FALSE_CH;
    }

    return TRUE_CH; // 全部过时
}

char convertToken(char exp[], Token tokens[]) { // the size of expTokens and expression is COMMAND_SIZE
    char *numStart = NULL, *numEnd = NULL;
    int numLength;
    double num;

    for (int tokenI = 0, expI = 0; tokenI < COMMAND_SIZE - 3; expI++) { // 确保 token 末尾有空 token 作为结束
        if (exp[expI] == FALSE_CH) // '\0'
            return TRUE_CH;
        if (exp[expI] == ' ')
            continue;

        if (strchr("+-*/^~()x", exp[expI]) != NULL) {
            fillChToken(&tokens[tokenI], exp[expI]);
            tokenI++;
        }
        else if (isdigit(exp[expI]) || exp[expI] == '.') {
            numStart = &exp[expI];
            num = (double)strtod(numStart, &numEnd); // numEnd 是最后一个数字字符的下一个字符

            if (*numEnd == '.') {
                wrongPrint(exp, numEnd, "\n<< convertToken(): Point error");
                return FALSE_CH;
            }

            numLength = (int)(numEnd - numStart);
            expI = expI + numLength - 1; // 字符串指针偏移量，循环结束后 expI 会自增因此此处减1

            fillNumToken(&tokens[tokenI], num);
            tokenI++;
        }
    }

    return FALSE_CH; // 只要出了循环就算超出
}

char tokenCorrect(Token tokens[]) { // max size is COMMAND_SIZE，专门检查 ^ 之后是不是 ~
    Token* current = tokens;
    while (1) {
        current = tokenChFind(current, '^');
        if (current != NULL) {
            current++;
            if (current->isOp && current->op == '~') {
                wrongPrintT(tokens, current, "\n<< tokenCorrect(): '-' is'n allowed directly after the '^, the correct format is: ^(-)");
                return FALSE_CH;
            }
        }
        else
            return TRUE_CH;
    }
}

char tokenToPostfix(Token tokens[], Token postfix[]) { // max: COMMAND_SIZE
    Token opStack[COMMAND_SIZE] = {FALSE_CH};

    for (int i = 0, j = 0; i < COMMAND_SIZE; i++) {
        if (tokens[i].isNum || tokens[i].isVar) { // 数字/x直接入
            postfix[j++] = tokens[i];
            continue;
        }

        /* op */
        Token tmpToken;
        if (isTokenOpStackEmpty(opStack) && !isEmptyToken(&tokens[i])) { // 空栈并且非token的结尾时直接入栈
            tokenOpPush(opStack, tokens[i], COMMAND_SIZE);
            continue;
        }

        if (tokens[i].op == '(') { // 左括号直接入栈
            tmpToken = tokenOpPush(opStack, tokens[i], COMMAND_SIZE);
            if (isEmptyToken(&tmpToken)) {
                wrongPrintT(tokens, &tokens[i], "\n<< tokenToPostfix(): Full stack");
                return FALSE_CH;
            }
            continue;
        }

        if (tokens[i].op == ')') { // 不断弹出直到遇到左括号
            while (1) { // 出出出出出出出出出出出栈
                tmpToken = tokenOpPop(opStack, COMMAND_SIZE);
                if (tmpToken.op == '(' || isEmptyToken(&tmpToken))
                    break;
                postfix[j++] = tmpToken; // 写入后缀表达式
            }
            continue;
        }

        Token stackTop = tokenOpTop(opStack, COMMAND_SIZE); // normal operator
        if (opLevel(stackTop.op) < opLevel(tokens[i].op) && opLevel(tokens[i].op) != -1) { // 1
            tokenOpPush(opStack, tokens[i], COMMAND_SIZE);
            continue;
        }
        if (opLevel(stackTop.op) == opLevel(tokens[i].op) && opLevel(tokens[i].op) != -1) { // 2
            if (stackTop.op == '^' || stackTop.op == '~') {
                tokenOpPush(opStack, tokens[i], COMMAND_SIZE);
                continue;
            }

            while (1) { // 此时为非右结合却相等的运算符，满足压栈条件或者遇到'('停止
                postfix[j++] = tokenOpPop(opStack, COMMAND_SIZE);
                stackTop = tokenOpTop(opStack, COMMAND_SIZE); // 弹出操作后的栈顶
                if (isEmptyToken(&stackTop) || stackTop.op == '(' || opLevel(stackTop.op) < opLevel(tokens[i].op))
                    break;
            }
            tmpToken = tokenOpPush(opStack, tokens[i], COMMAND_SIZE);
            if (isEmptyToken(&tmpToken)) {
                wrongPrintT(tokens, &tokens[i], "\n<< tokenToPostfix(): Full stack");
                return FALSE_CH;
            }
            continue;
        }
        if (opLevel(stackTop.op) > opLevel(tokens[i].op) && opLevel(tokens[i].op) != -1) { // 3
            while (1) { // 满足压栈条件或者遇到'('停止，同上
                postfix[j++] = tokenOpPop(opStack, COMMAND_SIZE);
                stackTop = tokenOpTop(opStack, COMMAND_SIZE); // 此栈顶为上一行执行了弹出操作之后的栈顶
                if (isEmptyToken(&stackTop) || stackTop.op == '(' || opLevel(stackTop.op) < opLevel(tokens[i].op))
                    break;
            }
            tmpToken = tokenOpPush(opStack, tokens[i], COMMAND_SIZE);
            if (isEmptyToken(&tmpToken)) {
                wrongPrintT(tokens, &tokens[i], "\n<< tokenToPostfix(): Full stack");
                return FALSE_CH;
            }
            continue;
        }

        if (isEmptyToken(&tokens[i])) { // 'token字符串' 末尾时
            while (1) { // 出出出出出出出出出出出栈！
                tmpToken = tokenOpPop(opStack, COMMAND_SIZE); // pop
                if (tmpToken.op == '(' || tmpToken.op == ')') // 表达式读取结束时，栈中仍然存在'('或')'，一定为错误
                    return FALSE_CH;
                if (isEmptyToken(&tmpToken)) // 此时不保证完全正确
                    return TRUE_CH;
                postfix[j++] = tmpToken;
            }
        }
    }

    wrongPrintT(tokens, tokens, "\n<< tokenToPostfix(): Expression is too long");
    return FALSE_CH;
}

Tree* postfixToTree(Token postfix[]) {
    if (postfix[0].isOp) {
        wrongPrintT(postfix, &postfix[0], "\n<< postfixToTree(): The first character should not be an operator");
        return NULL;
    }

    Tree *current = NULL, *tmp = NULL;
    Tree* nodeStack[COMMAND_SIZE] = {NULL}; // 存储申请过内存的节点的指针
    Token zero = {.isNum = TRUE_CH, .num = 0}; // 供 ~ 转化
    for (int i = 0; !isEmptyToken(&postfix[i]); i++) {
        current = aNode(postfix[i]); // aNode()接受一个 Token 并申请内存转换为 Tree，随后返回该内存指针

        if (current->isNum || current->isVar) { // 数字 或 x 直接入栈
            if (treePush(nodeStack, current, COMMAND_SIZE) == NULL) {
                wrongPrintT(postfix, &postfix[i], "\n<< postfixToTree(): Stack full");
                destroyTree(current);
                freeStack(nodeStack);
                return NULL;
            }
            continue;
        }

        if (current->isOp && current->op == '~') { // when unary minus
            tmp = treePop(nodeStack, COMMAND_SIZE);
            if (tmp == NULL) {
                wrongPrintT(postfix, &postfix[i], "\n<< postfixToTree(): Stack empty");
                destroyTree(current);
                freeStack(nodeStack);
                return NULL;
            }

            current->op = '-';
            current->left = aNode(zero);
            current->right = tmp;

            tmp = treePush(nodeStack, current, COMMAND_SIZE); //
            if (tmp == NULL) {
                wrongPrintT(postfix, &postfix[i], "\n<< postfixToTree(): Stack full");
                destroyTree(current);
                freeStack(nodeStack);
                return NULL;
            }
            continue;
        }

        if (current->isOp && current->op != '~') { // 非一元运算符 '-' 时
            tmp = treePop(nodeStack, COMMAND_SIZE);
            if (tmp == NULL) {
                wrongPrintT(postfix, &postfix[i], "\n<< postfixToTree(): Stack empty");
                destroyTree(current);
                freeStack(nodeStack);
                return NULL;
            }
            current->right = tmp;

            tmp = treePop(nodeStack, COMMAND_SIZE);
            if (tmp == NULL) {
                wrongPrintT(postfix, &postfix[i], "\n<< postfixToTree(): Stack empty");
                destroyTree(current);
                freeStack(nodeStack);
                return NULL;
            }
            current->left = tmp;

            tmp = treePush(nodeStack, current, COMMAND_SIZE);
            if (tmp == NULL) {
                wrongPrintT(postfix, &postfix[i], "\n<< postfixToTree(): Stack full");
                destroyTree(current);
                freeStack(nodeStack);
                return NULL;
            }
            continue;
        }
    }

    if (nodeStack[1] != NULL) {
        wrongPrintT(postfix, postfix, "\n<< postfixToTree(): The nodeStack has more than one element");
        freeStack(nodeStack);
        return NULL;
    }
    if (nodeStack[0] == NULL) {
        wrongPrintT(postfix, postfix, "\n<< postfixToTRee(): The nodeStack[0] is not exit");
        freeStack(nodeStack);
        return NULL;
    }

    return nodeStack[0];
}

char treeCorrect(Tree* root) {
    if (root == NULL)
        return TRUE_CH;
    if (treeCorrect(root->left) == FALSE_CH)
        return FALSE_CH;
    if (treeCorrect(root->right) == FALSE_CH)
        return FALSE_CH;

    if (root->op == '/' && isNumNode(root->right)) {
        numSimp(root->right);
        if (root->right->num == 0) {
            printf("\n<< treeCorrect(): Cannot 0 after '/'\n");
            return FALSE_CH;
        }
    }
    if (root->op == '^' && !isNumNode(root->right)) {
        printf("\n<< treeCorrect(): The function is too complex\n");
        return FALSE_CH;
    }

    return TRUE_CH;
}