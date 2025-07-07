#ifndef CONSOLE_OUTPUT_H
#define CONSOLE_OUTPUT_H

#endif // CONSOLE_OUTPUT_H

#include <windows.h>

//////////////////////////////// Define ////////////////////////////////////////

#define Set_Back_Color(r, g, b) printf("\x1b[48;2;%d;%d;%dm", r, g, b)
#define Set_Fore_Color(r, g, b) printf("\x1b[38;2;%d;%d;%dm", r, g, b)
#define Reset_Color() printf("\x1b[0m")
#define Gotoxy(x, y) printf("\x1b[%d;%dH", y, x)

#define BACKGROUND_COLOR 74

#define DISPLAY_X_SIZE 120
#define DISPLAY_Y_SIZE 30

typedef struct DISPLAY_CHAR {
    char ch;
    unsigned char color;
    unsigned char attr;
} DIS_CHAR;

//////////////////////////////// Structures ////////////////////////////////////

struct PIXEL {
    char Type;
    char ID;
    char is_changed;
};

struct COLOR {
    int r;
    int g;
    int b;
};

struct WINDOW {
    struct PIXEL **screen;
    COORD scrol;
    COORD start;
    COORD size;
    struct COLOR back_color;
};

//////////////////////////////// Functions ////////////////////////////////////

struct WINDOW *Create_Window(COORD start, COORD size);
void Delete_Window(struct WINDOW *win);
//void Gotoxy(int x, int y);
void Reload(const struct WINDOW *win, int xmax, int ymax);
void Print_Ch(int x, int y, char ch);
void Print_Un(int x, int y, char ch);
void Clear();
void Set_Attr(unsigned char color, unsigned char attr);

void Game_Update();
void Geme_Reload();










