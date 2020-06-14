#ifndef SNAKE_H
#define SNAKE_H

#include <stdlib.h>
#include <stdbool.h>
#include <termios.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#define FRAME_SIZE 50

typedef struct snake_node_st
{
    /* data */
    struct snake_node_st *last;
    struct snake_node_st *next;
    unsigned row;
    unsigned column;
}SNAKE_NODE;

typedef struct snake_st
{
    SNAKE_NODE *head;
    SNAKE_NODE *tail;
    unsigned length;    
}SNAKE;


int _kbhit();
void PrintGraph(char in[][FRAME_SIZE]);
void FrameInit(char p[][FRAME_SIZE], int size, char x);
void getch_init();
void SnakeInit(SNAKE *LocSnake);
unsigned SnakeMove(char Dir, char in[][FRAME_SIZE], SNAKE *Snake, unsigned Food[]);
void SnakeBodySet(SNAKE *Snake, char in[][FRAME_SIZE]);
void FoodReset(unsigned LocFood[2], char in[][FRAME_SIZE]);
extern char bar[FRAME_SIZE][FRAME_SIZE];
void ForbidReverse(char *Dir, char DirOld);
unsigned CmdProcess(char *ch, char *Dir, char DirOld, unsigned *speed);
void FreeAll(SNAKE *Snake);

#endif