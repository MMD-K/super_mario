//////////////////////////// In The Name of Allah ///////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// CREATED BY MMD ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


#include "mario_game.h"
#include <conio.h>
#include <pthread.h>

void gotoxy(int x, int y);
void update(int *arr);

int main()
{


    Start_Game();

    while(Main_Game() != EXIT);

    End_Game();

    // while (1)
    //     getchar();

    return 0;
}
