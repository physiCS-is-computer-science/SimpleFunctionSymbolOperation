#include "temp_somethings.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

TreeNode* link_node(TreeNode* left_leaf, TreeNode* right_leaf);
TreeNode* a_node(int type);
void destroy_tree(TreeNode* root);

int identify_token(const char* exp, Token* token, const char* comm) {
    char copy_exp[COMMAND_SIZE];

    /* simple check expression string */
    strcpy(copy_exp, exp);
    ptrdiff_t length = strchr(copy_exp, '\0') - copy_exp; // comput length of exp
    if (length < 3) {
        wrong_print(comm, strchr(comm, '(') + 1, "-=-= Too short =-=-");
        return 0;
    }

    /* real check */
    int i;
    char *is_operator, *endptr;
    char* current_ch = copy_exp;
    for (i = 0; *current_ch != '\0'; current_ch++) {
        is_operator = strchr(TOKEN_SYMBOL, *current_ch); // +-*/^()

        if ((current_ch == copy_exp) && (is_operator != NULL)) { // if first element isn't positive
            if (*is_operator == '-') {
                current_ch++; // next symbol of '-'
                if (*current_ch == 'x') {
                    token[i].isvariable = 't';
                    strcpy(token[i].variable, "-x");
                    i++;
                }
                else if (isdigit(*current_ch)) { // is num
                    token[i].isnum = 't';
                    token[i].num = -(int)strtol(current_ch, &endptr, 10);
                    current_ch = endptr;
                    current_ch--;
                    i++;
                }
                else {
                    wrong_print(comm, strchr(comm, '(') + 1, "-=-= Only [x] or [number] are allowed after the first [-] =-=-");
                    return 0;
                }
                continue; // jump to next loop
            }
        }
        if (!is_operator) { // is not operator
            if (*current_ch == 'x') { // is x
                token[i].isvariable = 't';
                strcpy(token[i].variable, "x");
                i++;
            }
            else if (isdigit(*current_ch)) { // is num
                token[i].isnum = 't';
                token[i].num = (int)strtol(current_ch, &endptr, 10);
                current_ch = endptr;
                current_ch--;
                i++;
            }
            else if (*current_ch == ' ') {
                continue;
            }
            else {
                wrong_print(comm, strchr(comm, '(') + 1, "-=-= buffer_clear() find some mistakes, Enter to clear =-=-");
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
        wrong_print(comm, strchr(comm, '(') + 1, "-=-= Number of character error =-=-");
        return 0;
    }

    if (token[i - 1].isoperator == 't' && token[i - 1].operator != ')') {
        wrong_print(comm, strchr(comm, '(') + 1, "-=-= Final symbol must be [number] or not-[operator] =-=-");
        return 0;
    }

    return i;
}

void initialize_token(Token* token) {
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

int need_pop(const char* stack, int j, char curr_operator) { // j--回到栈顶，curr_operator一定是 +-*/^ 之一
    int i;
    if (curr_operator == '^')
        return 0;
    for (i = 1, j--; j >= 0; i++, j--) {
        if (level(stack[j]) < level(curr_operator) || stack[j] == '(')
            break;
    }

    return i - 1;
}

int infix_to_postfix(const Token* token, int token_num, Token* postfix, const char* comm) {
    int i, j, k, need_num;
    int door = 1;
    char operator_stack[COMMAND_SIZE] = {'\0'};

    if (token[0].isoperator == 't' && token[0].operator == '(') // the situation of the first is '('
        door = 2;

    for (i = 0, j = 0, k = 0; i < token_num; i++) {
        switch (door) {
        case 1: {
            if (token[i].isnum == 't' || token[i].isvariable == 't') {
                postfix[k] = token[i]; // strcture can do this
                k++;
                door = 2;
            }
            else {
                buffer_clear("-=-= There is a [character] that should be [number] =-=-\n");
                return 0;
            }
            break;
        }
        case 2: { // 栈序号为栈顶序号下一个
            if (token[i].isoperator == 't' && token[i].operator == '(') { // 直接入栈
                operator_stack[j] = token[i].operator;
                j++;
                door = 1;
            }
            else if (token[i].isoperator == 't' && token[i].operator == ')') { // 弹出()之间所有操作符
                for (j--; operator_stack[j] != '('; k++, j--) { // 左j--回到栈顶，循环结束是j为'('，自身即为栈顶的next
                    postfix[k].isoperator = 't';
                    postfix[k].operator = operator_stack[j];
                }
                door = 2;
            }
            else if (token[i].isoperator == 't' && strchr(OPERATOR, token[i].operator)) {
                need_num = need_pop(operator_stack, j, token[i].operator); // 返回需要弹出栈次数

                if (need_num != 0) { // 弹出优先级不高于它的操作符，然后它入栈
                    j--; // 回栈顶
                    for (int count = 1; count <= need_num; count++) {
                        postfix[k].isoperator = 't';
                        postfix[k].operator = operator_stack[j];
                        k++;
                        j--;
                    }
                    j++; // 回栈顶的next
                }
                operator_stack[j] = token[i].operator; // 入栈
                j++;
                door = 1;
            }
            else {
                buffer_clear("\n-=-= There is/are (a) [character] that should be [opetator] =-=-");
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
        postfix[k].operator = operator_stack[j];
    }

    return k;
}

TreeNode* convert_tree(const Token* postfix, int num, const char* comm) {
    int top_up = 0;
    TreeNode* stack[COMMAND_SIZE];
    TreeNode* current;

    for (int i = 0; i < num; i++, top_up++) { // 1-num 2-x 3-+-*/^ #### top_up++是为了回栈顶下一个
        if (postfix[i].isnum == 't') {
            current = a_node(1); // create a node
            current->operand = postfix[i].num; // initialized it
            stack[top_up] = current;
        }
        else if (postfix[i].isvariable == 't') {
            current = a_node(2);
            strcpy(current->x, postfix[i].variable); // initialized it, but it is a string("-x\0" or "x\0")
            stack[top_up] = current;
        }
        else if (postfix[i].isoperator == 't') {
            /* wrong situation */
            if (top_up < 2) {
                if (top_up == 0) // stack is empty
                    buffer_clear("-=-= The expression have some problems(stack is empty) =-=-");
                else if (top_up == 1) {
                    destroy_tree(stack[top_up - 1]);
                    buffer_clear("-=-= The expression have some problems(only one operand left in the stack) =-=-");
                }
                return NULL;
            }

            current = link_node(stack[top_up - 2], stack[top_up - 1]);
            current->is_operator = 't'; // initialize
            current->operator = postfix[i].operator;

            top_up = top_up - 2;
            stack[top_up] = current;
        }
    }

    /* 不知什么情况 */
    if (top_up != 1) {
        for (int i = 0; i < num; i++)
            destroy_tree(stack[i]);
        buffer_clear("-=-= The expresion heve some situations =-=-");
    }

    return stack[top_up - 1];
}
