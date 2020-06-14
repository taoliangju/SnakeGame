#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "snake.h"

#define CLEAR() system("printf \"\033c\"")

int main()
{
    static clock_t starttime;
    static unsigned cnt;
    char ch = 0;
    char Dir = 0;
    char DirOld = 0;
    SNAKE Snake = {NULL, NULL, 1};
    unsigned Food[2];
    unsigned CmdCode = 0;
    FrameInit(bar, FRAME_SIZE, '*');
    getch_init();
    SnakeInit(&Snake);
    FoodReset(Food, bar);
    bar[Food[0]][Food[1]] = '$';
    unsigned Speed = 1;
    while (1)
    {
        cnt = clock() - starttime;
        if (cnt >= CLOCKS_PER_SEC / Speed)
        {
            CLEAR();
            starttime = clock();
            if (_kbhit()) //使用kbhit之后需要使用getch将缓冲区的字符删除，否贼会一直返回 1
            {
                ch = getch();
            }
            CmdCode = CmdProcess(&ch, &Dir, DirOld, &Speed);
            if (1 == CmdCode)
            {
                break;
            }
            else if (2 == CmdCode)
            {
                //now suspend
            }
            else
            {

                ForbidReverse(&Dir, DirOld);
                if (SnakeMove(Dir, bar, &Snake, Food))
                {
                    break;
                }
                else
                {
                    SnakeBodySet(&Snake, bar);
                }
            }
            printf("Speed x %d\r\n", Speed);
            printf("Your score is %d\r\n", Snake.length);
            PrintGraph(bar);
            printf("Keyboard tips:\r\nW : Up | S : Down | A : Left | D : Right\r\nI : Speed up | O : Speed down\r\n");
            printf("Space : Suspend | Anykey : Restart\r\nQ : Quit\r\n");
            refresh(); //配合initscr使用，否则printf则不输出到屏幕
            DirOld = Dir;
        }
    }
    endwin(); //还原curses模式之前的状态，与initscr配合使用
    printf("Game over!\r\n");
    printf("Your score is %d\r\n", Snake.length);
    FreeAll(&Snake);
    return 0;
}
