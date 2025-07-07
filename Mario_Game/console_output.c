#include "console_output.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

//////////////////////////////// Define ////////////////////////////////////////

#define DEBUG

//////////////////////////////// Structures ////////////////////////////////////



//////////////////////////////// Functions ////////////////////////////////////

struct WINDOW *Create_Window(COORD start, COORD size)
{
    // create screen
    //char *screen = (char *)calloc(size.Y * size.X, sizeof(char));
    //char *screen = (char *)calloc(size.Y * size.X, sizeof(struct PIXEL));
    // char **screen = (char **)calloc(size.Y, sizeof(char *));
    // for (int i = 0; i < size.Y; i++) {
    //     screen[i] = (char *)calloc(size.X, sizeof(char));
    // }

    struct PIXEL **screen = (struct PIXEL **)calloc(size.Y, sizeof(struct PIXEL *));

    for (int i = 0; i < size.Y; i++) {
        screen[i] = (struct PIXEL *)calloc(size.X, sizeof(struct PIXEL));
    }


    struct WINDOW *win = (struct WINDOW *)calloc(1, sizeof(struct WINDOW));

    // set size and start point
    win->screen = screen;
    win->size = size;
    win->start = start;
    win->scrol.X = 0;
    win->scrol.Y = 0;

    return win;
}

void Delete_Window(struct WINDOW *win)
{
    free(win->screen);
    free(win);
}

// void Gotoxy(int x, int y)
// {
//     printf("\x1b[%d;%dH", )
// }

void Update(const struct WINDOW *win);

void Print_Ch(int x, int y, char ch);

void Print_Un(int x, int y, char ch)
{
    // set cursor position
    Gotoxy(x, y);

    // print unicode
    switch (ch) {
    case '#':
        printf("🧱");
        break;
    case '.':
        printf("  ");
        break;
    default:
        break;
    }
}

void Clear()
{
    Set_Back_Color(0, 0, 0);
    for (int i = 0; i < DISPLAY_Y_SIZE; i++) {
        Gotoxy(0, i);
        printf("%*c", DISPLAY_X_SIZE,' ');
    }
}













