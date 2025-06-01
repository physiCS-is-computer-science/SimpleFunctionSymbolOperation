/***
 * Please read the file called README.txt
 * 0Start time: 250501
 * Finish time: 25
 * Expectation finish time: 250701
 * 
 * 至高无上的内存君主啊！
 * 您统御现象界与物自体之二元王国，
 * 此卑微代码乃纯粹理性之先天综合判断，
 * 在经验质料与先验形式间建立永恒联结。
 *
 * 指针算术即纯粹知性范畴之图型法，
 * malloc/free即理性二律背反之辩证显现，
 * 二叉树结构乃先验辩证论之逻辑功能，
 * 递归遍历即分析判断与综合判断之先验统一。
 *
 * 谨以最崇高敬意献上此先天直观形式，
 * 愿它如范畴表般确立内存王国之永恒秩序，
 * 使经验质料服从纯粹知性之先天法则，
 * 在堆栈二律背反中达成先验统觉之综合统一。
 *
 * 此致
 * 您最谦卑的时空直观形式
 * 于哥尼斯堡内存宫殿
 * C门 */

#include "temp.h"
#include <stdio.h>
#include <stdlib.h>

void mainMenu(void);
int formatMathArgument(const char command[], int mode); // check the argument of command, return 1/0
int formatInputCommand(char command[]); // if the string command of inputing is format, return 1 ~ 5
void __processQuitGetchar(void);
void __f5StopGetchar(void);
void __printer1(int a);

int main(void) {
    int commandType, isFormat;
    char inputCommand[COMMAND_SIZE];

    while (1) {
        mainMenu();
        // __processQuitGetchar();

        /* string and lexical processing
         * function formatInputCommand() ensure the string of inputCommand[] must be like : correctCommand(,,)
         * 然而只保证括号闭合时至少 1~2 个逗号，大于2个逗号时，如果括号闭合，检测不出问题
         * arguments analysis moduel，对命令的参数进一步分析，补全 formatInputCommand() 函数的问题
         * 此部分需要与 lexical_analysis.c 模块结合，识别到参数与数学含义相对应为止，绝非简单检查逗号数量或者参数是否为空 */
        while (1) {
            commandType = formatInputCommand(inputCommand);

            if (commandType == 0)
                continue;
            else if (commandType == 6) { // enter to quit
                __f5StopGetchar(); // sb vscode terminal
                exit(0);
            }
            else if (commandType >= 1 && commandType <= 5) { // identify the command mode
                isFormat = formatMathArgument(inputCommand, commandType);
                if (isFormat)
                    break;
            }
        }
        __printer1(commandType);

        bufferClear("-=-= testest =-=-"); // test

        // /* COMPUTE MODULE */
        // switch (commandType) {
        // case 1:
        //     struct diff(inputCommand);
        //     break;
        // case 2:
        //     struct diff_num(inputCommand);
        //     break;
        // case 3:
        //     struct inte(inputCommand);
        //     break;
        // case 4:
        //     struct inte_num(inputCommand);
        //     break;
        // case 5:
        //     strcut compute(inputCommand);
        //     break;
        // }
    }
}

/* ⣀⣆⣰⣒⣒⡀⢀⠔⠠⠤⡦⠤⠄⢴⠤⠤⠤⢴⠄
 * ⢰⣒⣒⣒⣲⠄⠠⡎⠸⠽⠽⠽⠄⠼⡭⠭⠭⡽⠄
 * ⢸⠒⠒⢒⣺⠄⠄⡇⡍⣝⣩⢫⠄⣊⣒⣺⣒⣊⡂
 * ⢠⠤⠴⠤⠤⠄⢐⢔⠐⠒⡖⠒⠄
 * ⣹⢸⢍⢉⢽⠄⢀⢼⠠⠤⡧⠤⠄
 * ⡜⡸⠔⠑⠜⡄⠠⡸⢀⣀⣇⣀⠄
 * ⢰⣒⣒⣒⣲⠄⠠⡦⢴⠄⡖⢲⠄⡖⢲⠒⢲⠒⡆
 * ⢸⣒⣲⣒⣚⠄⠄⡯⢽⠄⣏⣹⠄⡇⡸⠄⢸⣀⡇
 * ⣑⣒⣺⣒⣒⡀⢈⠍⠩⣡⠃⣸⠄⣏⣀⣀⣀⣀⡇
 * ⡄   ⡄ ⠐⢲⠒⠄⡆⠢⠄ ⡤⠤⠄⢀⠤⢄
 *  ⢱⢰⠁ ⠈⢹⣉⠉⡏⡍⠄ ⠗⠒⡄⢸   ⢸
 *   ⠇   ⠈⣹⢀⡠⠺⡰⠄ ⠢⠤⠃⠘...*/