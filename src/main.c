/***
 * Please read the file which called README.txt
 * 0Start time: 250501
 * Finish time: 2506
 * Expectation finish time: 250628
 * By MisakaMikoto#^v^#
 *
 * Finish comment: nothing yet.
 *
 * A verse, hereby dedicated to my bygone days:
 * -----
 * 至高无上的内存君主啊！
 * 您统御现象界与物自体之二元王国，
 * 此卑微代码乃纯粹理性之先天综合判断，
 * 在经验质料与先验形式间建立永恒联结。
 * -----
 * 指针算术即纯粹知性范畴之图型法，
 * malloc/free即理性二律背反之辩证显现，
 * 二叉树结构乃先验辩证论之逻辑功能，
 * 递归遍历即分析判断与综合判断之先验统一。
 * -----
 * 谨以最崇高敬意献上此先天直观形式，
 * 愿它如范畴表般确立内存王国之永恒秩序，
 * 使经验质料服从纯粹知性之先天法则，
 * 在堆栈二律背反中达成先验统觉之综合统一。
 * -----
 * 此致
 * 您最谦卑的时空直观形式
 * 于哥尼斯堡内存宫殿
 * C门 */

#include "temp.h"
#include <stdio.h>
#include <stdlib.h>

void mainMenu(void);
Tree* formatMathArgument(const char command[], enum CommandType mode); // check the argument of command, return 1/0
enum CommandType formatInputCommand(char command[]); // if the string command of inputing is format, return 1 ~ 5
void destroyTree(Tree* root);
void __processQuitGetchar(void);
void __f5StopGetchar(void);
void __printer1(int a);

int main(void) {
    char inputCommand[COMMAND_SIZE];
    Tree* isFormat = NULL;
    enum CommandType commandType;

    mainMenu();

    while (1) { // 多个测试用例需要循环，不然重复运行好麻烦，待删除
        /* string and lexical processing */
        while (1) {
            commandType = formatInputCommand(inputCommand);

            if (commandType == FALSE_INPUT)
                continue;
            else if (commandType == END) // enter to quit
                return 0;
            else if (commandType >= DIFF_CHAR && commandType <= COMP) // identify the command mode
                isFormat = formatMathArgument(inputCommand, commandType);
            if (isFormat)
                break;
        }
        /* -=-=-=-=-= StartTest =-=-=-=-=- */
        __printer1(commandType);
        // destroyTree(isFormat);
        bufferClear("-=-= testest =-=-");
        /* -=-=-=-=-= 00EndTest =-=-=-=-=- */

        /* Compute module */
        switch (commandType) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        }
    }

    return 0;
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