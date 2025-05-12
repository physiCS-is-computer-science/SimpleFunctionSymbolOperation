/***
 * Please read the file called README.txt
 * 0Start time: 250501
 * Finish time: 25
 * Expectation finish time: 250701
 ***/

#include "temp_somethings.h"
#include <stdio.h>
#include <stdlib.h>

int format_math_argument(const char command[], int mode); // check the argument of command, return 1/0
int format_input_command(char command[]); // if the string command of inputing is format, return 1 ~ 5
void __process_quit_getchar(void);
void __f5_stop_getchar(void);
void __printer1(int a);
void main_menu(void);

int main(void) {
    int command_type, isformat;
    char input_command[COMMAND_SIZE];

    while (1) {
        main_menu();
        // __process_quit_getchar();

        /* 字符串与词法分析 */
        while (1) {
            /* function format_input_command() ensure the string of input_command[] must be like : correctCommand(,,)
             * 然而只保证括号闭合时至少 1~2 个逗号，大于2个逗号时，如果括号闭合，检测不出问题 */
            command_type = format_input_command(input_command);
            if (command_type == 6) { // enter to quit
                __f5_stop_getchar(); // sb vscode terminal
                exit(0);
            }
            else if (command_type == 0) {
                continue;
            }
            else if (command_type >= 1 && command_type <= 5) { // identify the command mode
                /* arguments analysis moduel，对命令的参数进一步分析，补全 format_input_command() 函数的问题
                 * 此部分需要与 lexical_analysis.c 模块结合，识别到参数与数学含义相对应为止，绝非简单检查逗号数量或者参数是否为空 */
                isformat = format_math_argument(input_command, command_type);
                if (isformat)
                    break;
                else
                    continue;
            }
        }
        __printer1(command_type);

        buffer_clear("-=-= testest =-=-"); // test

        // /* COMPUTE MODULE */
        // switch (command_type) {
        // case 1:
        //     struct diff(input_command);
        //     break;
        // case 2:
        //     struct diff_num(input_command);
        //     break;
        // case 3:
        //     struct inte(input_command);
        //     break;
        // case 4:
        //     struct inte_num(input_command);
        //     break;
        // case 5:
        //     strcut compute(input_command);
        //     break;
        // }
    }
}