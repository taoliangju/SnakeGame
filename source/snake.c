#include "snake.h"

char bar[FRAME_SIZE][FRAME_SIZE];
unsigned Seconds = 0;
char Dir = 'w';

void FrameInit(char p[][FRAME_SIZE], int size, char x)
{
    for (int i = 0; i < size; i++)
    {
        p[0][i] = x;
        p[size - 1][i] = x;
        if (i < size - 1 && i > 0)
        {
            p[i][0] = x;
            p[i][size - 1] = x;
        }
    }
}
unsigned Move(SNAKE *Snake, char in[][FRAME_SIZE], unsigned row, unsigned column)
{
    unsigned gameover = 0;
    if (in[row][column] == '*')
    {
        gameover = 1;
    }
    else
    {
        in[Snake->tail->row][Snake->tail->column] = 0;
        in[row][column] = 0;
        Snake->tail->row = row;
        Snake->tail->column = column;
        Snake->tail->next = Snake->head;
        Snake->head->last = Snake->tail;
        Snake->tail = Snake->tail->last;
        Snake->head = Snake->head->last;
        Snake->tail->next = NULL;
        Snake->head->last = NULL;
    }
    return gameover;
}

void PrintGraph(char in[][FRAME_SIZE])
{
    char *p;
    for (int i = 0; i < FRAME_SIZE; i++) //row
    {
        p = in[i];
        for (int j = 0; j < FRAME_SIZE; j++) //column
        {
            if (p[j] == 0)
            {
                printf(" ·");
            }
            else
            {
                printf("%2c", p[j]);
            }
        }
        printf("\r\n");
    }
}

void SnakeInit(SNAKE *LocSnake)
{
    SNAKE_NODE *head = (SNAKE_NODE *)malloc(sizeof(SNAKE_NODE));
    head->row = 24;
    head->column = 25;
    head->next = NULL;
    head->last = NULL;
    LocSnake->head = head;
    LocSnake->tail = head;
    LocSnake->length = 1;

    return;
}

unsigned SnakeMove(char Dir, char in[][FRAME_SIZE], SNAKE *Snake, unsigned Food[])
{
    unsigned x, y, GameOver;
    x = Snake->head->row;
    y = Snake->head->column;
    GameOver = 0;
    switch (Dir)
    {
    case 'w':
        x -= 1;
        break;
    case 's':
        x += 1;
        break;
    case 'a':
        y -= 1;
        break;
    case 'd':
        y += 1;
        break;
    default:
        break;
    }
    if (x == Food[0] && y == Food[1])
    {
        SNAKE_NODE *temp = (SNAKE_NODE *)malloc(sizeof(SNAKE_NODE));
        temp->row = x;
        temp->column = y;
        temp->next = Snake->head;
        Snake->head->last = temp;
        Snake->head = temp;
        Snake->length += 1;
        in[Snake->head->row][Snake->head->column] = '*';
        FoodReset(Food, in);
        in[Food[0]][Food[1]] = '$';
    }
    else
    {
        GameOver = Move(Snake, in, x, y);
    }
    if (Snake->length == 1)
    {
        GameOver = 0;
    }
    return GameOver;
}

void SnakeBodySet(SNAKE *Snake, char in[][FRAME_SIZE])
{
    SNAKE_NODE *tmp = Snake->head;
    FrameInit(in, FRAME_SIZE, '*');
    while (tmp != NULL)
    {
        in[tmp->row][tmp->column] = '*';
        tmp = tmp->next;
    }
}

void FoodReset(unsigned LocFood[2], char in[][FRAME_SIZE])
{
    do
    {
        unsigned timea = (unsigned)time(NULL);
        srand(timea);
        LocFood[0] = (unsigned)rand();
        LocFood[0] = LocFood[0] % 48 + 1;

        unsigned timeb = (unsigned)time(NULL) * 2;
        srand(timeb);
        LocFood[1] = (unsigned)rand();
        LocFood[1] = LocFood[0] % 48 + 1;
    } while (in[LocFood[0]][LocFood[1]] != 0);

    return;
}
void ForbidReverse(char *Dir, char DirOld)
{
    switch (*Dir)
    {
    case 'w':
        if (DirOld == 's')
        {
            *Dir = DirOld;
        }
        break;
    case 's':
        if (DirOld == 'w')
        {
            *Dir = DirOld;
        }
        break;
    case 'a':
        if (DirOld == 'd')
        {
            *Dir = DirOld;
        }
        break;
    case 'd':
        if (DirOld == 'a')
        {
            *Dir = DirOld;
        }
        break;

    default:
        break;
    }
}
unsigned CmdProcess(char *ch, char *Dir, char DirOld, unsigned *speed)
{
    unsigned IfBreak = 0;
    if (*ch == 'q')
    {
        IfBreak = 1;
    }
    else if(*ch == ' ')
    {
        IfBreak = 2;
    }
    else if (*ch == 'i')
    {
        *speed *= 2;
        *ch = DirOld;
    }
    else if (*ch == 'o')
    {
        *speed /= 2;
        *ch = DirOld;
    }
    else if (*ch == 'w' || *ch == 's' || *ch == 'a' || *ch == 'd')
    {
        *Dir = *ch;
    }

    if (*speed < 1)
    {
        *speed = 1;
    }
    return IfBreak;
}
int _kbhit()
{
    static const int STDIN = 0;
    static bool initialized = false;
    if (!initialized)
    {
        // Use termios to turn off line buffering
        struct termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }
    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

void FreeAll(SNAKE *Snake)
{
    SNAKE_NODE *CurrentNode = Snake->head;
    while(CurrentNode != NULL)
    {
        SNAKE_NODE *tmp = CurrentNode->next;
        CurrentNode->last = NULL;
        CurrentNode->next = NULL;
        free(CurrentNode);
        CurrentNode = tmp;
    }
    Snake->head = NULL;
    Snake->tail = NULL;
    Snake->length = 0;
}

void getch_init()
{
    initscr(); //初始化curses模式，配合getch使用,否则getch会一直返回值
    // raw();
    noecho(); //输入不显示在屏幕上
    // fcntl(0, F_SETFL, O_NONBLOCK); //将文件内函数设置为非阻塞模式
    // fcntl(0, F_SETFL, 0); //将文件内函数设置为非阻塞模式
    //fcntl()可配合getch使用，若使用kbhit()，则可以不用fcntl
}