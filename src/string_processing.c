#include "temp_somethings.h"
#include <stdio.h>
#include <string.h>

void wrong_print(const char* wrong_str, const char* first_wrong_ch, char* print_str);
TreeNode* convert_tree(const Token* postfix, int num);
int infix_to_postfix(const Token* token, int token_num, Token* postfix);
void destroy_tree(TreeNode* root);
int identify_token(const char* exp, Token* token);
void initialize_token(Token* token);
void print_token(const Token* token, int size);
void buffer_clear(char* screen_print);
int format_argument_char(const char* command_input, const char* left_bracket);

/* 分析输入命令是哪个，返回 0 ~ 7 八种值。主函数将返回值赋给词法分析模块函数，该模块对应返回值分析 */
int format_input_command(char command[]) {
    char *end_ptr, *enter_ptr;
    ptrdiff_t defference;

    printf(">> ");
    fgets(command, COMMAND_SIZE, stdin);
    if (command[0] == '\n')
        return 6;

    enter_ptr = strchr(command, '\n'); // because fgets() read the character '\n'
    if (enter_ptr != NULL)
        *enter_ptr = '\0';

    end_ptr = strchr(command, '\0');
    defference = end_ptr - command;
    if (defference >= COMMAND_SIZE - 1) {
        buffer_clear(NULL); // 只有此时才需要 buffer_clear()，否则字符串没超过最大限定长度，缓冲区没东西，buffer_clear() 导致程序暂停
        printf("\nThe string length has reached the maxium allowable length(%d)", COMMAND_SIZE);
        wrong_print(command, end_ptr, NULL);
        return 0;
    }

    /* scan the string of command */
    char command_cpy[COMMAND_SIZE];
    char* temp_ptr;
    int type, arg_mode;

    strcpy(command_cpy, command); // 不改变原参数而需要复制，前面已经保证了此处字符串可以装的下
    temp_ptr = strchr(command_cpy, '('); // find command string
    if (temp_ptr == NULL) {
        wrong_print(command_cpy, command_cpy, "\nThis is the first error for this string: \n");
        return 0;
    }
    else
        *temp_ptr = '\0'; // 此时 command_cpy 字符串被截断只剩命令关键字

    if (strcmp(DIFF_STR, command_cpy) == 0)
        type = 1;
    else if (strcmp(INTE_STR, command_cpy) == 0)
        type = 2;
    else if (strcmp(COMP_STR, command_cpy) == 0)
        type = 3;
    else {
        wrong_print(command_cpy, command_cpy, "\nThis is the first error for this string: \n");
        return 0;
    }
    *temp_ptr = '('; // 恢复先

    switch (type) {
    case 1: // diff
        arg_mode = format_argument_char(command_cpy, temp_ptr);
        if (arg_mode == 0)
            return 0;
        else if (arg_mode == 1) // non-number mode
            return 1;
        else if (arg_mode == 2) // number mode
            return 2;
        break;
    case 2: // inte
        arg_mode = format_argument_char(command_cpy, temp_ptr);
        if (arg_mode == 0)
            return 0;
        else if (arg_mode == 1)
            return 3;
        else if (arg_mode == 2)
            return 4;
        break;
    case 3: // comp
        arg_mode = format_argument_char(command_cpy, temp_ptr);
        if (arg_mode == 0)
            return 0;
        else if (arg_mode == 1) {
            wrong_print(command, temp_ptr + 1, "\nThis is the first error for this string: \n");
            return 0;
        }
        else if (arg_mode == 2)
            return 5;

        break;
    }

    printf("There are some wrongs in this string\n");
    wrong_print(command, command, "\nThis is the first error for this string: \n");
    return 0;
}

/* return mode: 0-wrong 1-non-number-mode 2-number-mode
 * 仅仅检查：命令正确、拥有左括号 的字符串。检查其括号是否闭合，如果闭合，则保证括号内有 1~2 个及以上个数逗号
 * 可能要重写 */
int format_argument_char(const char* command_output, const char* left_bracket) {
    char *first_comma, *second_comma, *end_bracket;

    first_comma = strchr(left_bracket, ',');
    if (first_comma == NULL) {
        wrong_print(command_output, left_bracket + 1, "\nThis is the first error for this string: \n");
        return 0;
    }

    second_comma = strchr(first_comma + 1, ',');
    if (second_comma == NULL) { // 此时可能是两个参数(一个逗号)
        end_bracket = strchr(first_comma, ')');
        if (end_bracket == NULL) {
            wrong_print(command_output, first_comma + 1, "\nThis is the first error for this string: \n");
            return 0;
        }
        else {
            if (*(end_bracket + 1) == '\0')
                return 1;
            else {
                wrong_print(command_output, end_bracket + 1, "\nThis is the first error for this string: \n");
                return 0;
            }
        }
    }
    else { // 可能是三个参数(两个逗号)
        end_bracket = strchr(second_comma, ')');
        if (end_bracket == NULL) {
            wrong_print(command_output, second_comma + 1, "\nThis is the first error for this string: \n");
            return 0;
        }
        else {
            if (*(end_bracket + 1) == '\0')
                return 2;
            else {
                wrong_print(command_output, end_bracket + 1, "\nThis is the first error for this string: \n");
                return 0;
            }
        }
    }
}

/* 格式正确返回 1，错误返回 0 */
int format_math_argument(const char command[], int mode) {
    TreeNode* exp_root;
    char* exp_start;
    char expression[COMMAND_SIZE];

    exp_start = strchr(command, '(');
    exp_start++;
    if (*exp_start == ',') { // this situation is like: diff(,)
        wrong_print(command, exp_start, "-=-= The expression is not exist =-=-");
        return 0;
    }
    strcpy(expression, exp_start); // exp_start is  the next pointer of '('
    *strchr(expression, ',') = '\0';

    /* identify the/first(mode5 have 2 expresion) expression
     * input and convert infix expression to token,
     * and check the expression tree times */
    int exp_token_num, exp_postfix_num;
    Token exp_token[COMMAND_SIZE], exp_postfix[COMMAND_SIZE];
    while (1) {
        initialize_token(exp_token); // reset
        initialize_token(exp_postfix);

        exp_token_num = identify_token(expression, exp_token); // convert to token and check
        if (exp_token_num == 0)
            return 0;
        print_token(exp_token, exp_token_num);

        exp_postfix_num = infix_to_postfix(exp_token, exp_token_num, exp_postfix); // create postfix and check
        if (exp_postfix_num == 0)
            return 0;
        print_token(exp_postfix, exp_postfix_num);

        exp_root = convert_tree(exp_postfix, exp_postfix_num); // creat expression tree and check
        if (exp_root == NULL)
            return 0;

        buffer_clear("Enter to close the process"); // testest
        break;
    }
    destroy_tree(exp_root);
    buffer_clear("buffer_clear(): test"); // testest

    switch (mode) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    }
}