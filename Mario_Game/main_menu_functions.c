#include "mario_game.h"
#include <stdio.h>
#include <conio.h>

// 🦸 🦹 👮 🧙 🧝 🤴



void Show_History(int user_id, struct ACCOUNT_USER_INFO acc_user_info)
{
    struct ACCOUNT_FILE_HISTORY acc_his_info;
    struct ACCOUNT_GAME_HISTORY *games_history;
    int first_his = 0;

    acc_his_info = Get_Histroy_Info(user_id);
    games_history = Get_Game_History_List(user_id);

    struct WINDOW *map = Load_Map(LOAD_HISTORY_MAP);

    // define fore_color
    struct COLOR fore_color = {3, 4, 94};

    // reload map
    Reload(map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);

    // set colors
    Set_Back_Color(map->back_color.r, map->back_color.g, map->back_color.b);
    Set_Fore_Color(fore_color.r, fore_color.g, fore_color.b);

    Gotoxy(MAP_X_START + 5, MAP_Y_START + 2);

    int hour = (int)acc_his_info.Time_Play_S / 3600;
    int min = ((int)acc_his_info.Time_Play_S - hour * 3600) / 60;
    int sec = (int)acc_his_info.Time_Play_S - hour * 3600 - min * 60;

    // print time play
    printf(CLOCK_UN " Time Play %d : %d : %d    ", hour, min, (int)acc_his_info.Time_Play_S);

    // print game played
    printf(GAME_UN " Game Played : %d    ", acc_his_info.Games_Played);

    // print game wins
    printf(GAME_WIN_UN " Wins : %d    ", acc_his_info.Games_Win);

    // print best score
    printf(BEST_SCORE_UN " Best Score : %d", acc_his_info.Best_Score);

    while (TRUE) {
        for (int i = first_his; i < first_his + MAX_HIS_NUMBER && i < acc_his_info.Games_Played; i++) {
            Gotoxy(MAP_X_START + 5, MAP_Y_START + 4 + i - first_his);

            printf("%*c", MAP_MAX_X_SIZE * 2 - 10, ' ');

            Gotoxy(MAP_X_START + 5, MAP_Y_START + 4 + i - first_his);

            printf("%d. ", i + 1);

            printf(MAP_ICON_UN " Map:");

            switch (games_history[i].Map_ID) {
            case 0:
                printf("Map 1");
                break;
            case 1:
                printf("Map 2");
                break;
            default:
                break;
            }

            printf(" ");

            printf(GAME_WIN_UN " Game %s", (games_history[i].win_status == GAME_WIN) ? "WIN" : "LOSE");

            printf(" ");

            printf(CLOCK_UN " Time:%d", (int)games_history[i].Game_Time_S);

            printf(" ");

            printf(SCORE_UN " Score:%d", games_history[i].Game_Score);

            printf(" ");

            printf(COIN_UN " Coin:%d", games_history[i].Game_Coins);

            printf(" ");

            printf(REPEAT_UN " Repeat:%d", games_history[i].Repeat);

            printf(" ");

            printf(DEATH_UN " Death:%d", games_history[i].Death);
        }

        char cmd = getch();

        switch (cmd) {
        case ESC_KEY:
            cmd = getch();
            if (cmd == 72) {
                if (first_his > 0) {
                    first_his--;
                }
            }
            if (cmd == 80) {
                if (first_his + MAX_HIS_NUMBER < acc_his_info.Games_Played) {
                    first_his++;
                }
            }
            break;
        case 27:
            return;
            break;
        default:
            break;
        }
    }

    getchar();
}

void Creator()
{
    struct WINDOW *map = Load_Map(LOAD_LOADING_MAP);

    // define fore_color
    struct COLOR fore_color = {3, 4, 94};

    // reload map
    Reload(map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);

    Set_Back_Color(map->back_color.r, map->back_color.g, map->back_color.b);
    Set_Fore_Color(fore_color.r, fore_color.g, fore_color.b);

    Gotoxy(DISPLAY_X_SIZE / 2 - strlen("SUPER MARIO 2025") / 2, MAP_Y_START + 7);
    printf("SUPER MARIO 2025");
    Gotoxy(DISPLAY_X_SIZE / 2 - strlen("Created By MMD") / 2, MAP_Y_START + 10);
    printf("Created By MMD");

    while (TRUE) {
        if (getch() == 27) {
            return;
        }
    }

    Reset_Color();
}

void Setting()
{
    struct WINDOW *map = Load_Map(LOAD_LOADING_MAP);

    // define fore_color
    struct COLOR fore_color = {3, 4, 94};

    // reload map
    Reload(map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);

    Set_Back_Color(map->back_color.r, map->back_color.g, map->back_color.b);
    Set_Fore_Color(fore_color.r, fore_color.g, fore_color.b);

    Gotoxy(DISPLAY_X_SIZE / 2 - strlen("You REALLY want a setting menu?") / 2, MAP_Y_START + 9);
    printf("You REALLY want a setting menu?");


    char *menu_names[2] = {"     Yse     ", "     NO      "};
    struct MENU menu;
    menu.number = 2;
    menu.active = 0;
    menu.name = menu_names;

    // menu function
    int cmd = Menu(&menu, DISPLAY_X_SIZE / 2, MAP_Y_START + 10, map->back_color, fore_color, 0);

    switch (cmd) {
    case 0:
        break;
    case 1:
        return;
        break;
    default:
        return;
        break;
    }

    // reload map
    Reload(map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);

    Set_Back_Color(map->back_color.r, map->back_color.g, map->back_color.b);
    Set_Fore_Color(fore_color.r, fore_color.g, fore_color.b);

    Gotoxy(DISPLAY_X_SIZE / 2 - strlen("Seriously") / 2, MAP_Y_START + 9);
    printf("Seriously");
    Gotoxy(DISPLAY_X_SIZE / 2 - strlen("You ACTUALLY want a setting menu?") / 2, MAP_Y_START + 10);
    printf("You ACTUALLY want a setting menu?");

    // menu function
    cmd = Menu(&menu, DISPLAY_X_SIZE / 2, MAP_Y_START + 11, map->back_color, fore_color, 0);

    switch (cmd) {
        break;
    case 0:
        break;
    case 1:
        return;
    default:
        return;
        break;
    }

    // reload map
    Reload(map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);

    Set_Back_Color(map->back_color.r, map->back_color.g, map->back_color.b);
    Set_Fore_Color(fore_color.r, fore_color.g, fore_color.b);

    Gotoxy(DISPLAY_X_SIZE / 2 - strlen("Do you want a better graphic?") / 2, MAP_Y_START + 9);
    printf("Do you want a better graphic?");
    Gotoxy(DISPLAY_X_SIZE / 2 - strlen("Or do you want a more FPS?") / 2, MAP_Y_START + 10);
    printf("Or do you want a more FPS?");

    char *menu_names2[2] = {" better graphic ", "    more FPS    "};

    menu.name = menu_names2;
    // menu function
    cmd = Menu(&menu, DISPLAY_X_SIZE / 2, MAP_Y_START + 11, map->back_color, fore_color, 0);

    switch (cmd) {
    case 0:
        break;
    case 1:
        break;
    default:
        return;
        break;
    }

    // reload map
    Reload(map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);

    Set_Back_Color(map->back_color.r, map->back_color.g, map->back_color.b);
    Set_Fore_Color(fore_color.r, fore_color.g, fore_color.b);

    Gotoxy(DISPLAY_X_SIZE / 2 - strlen("Not possible") / 2, MAP_Y_START + 10);
    printf("Not possible");

    while (TRUE) {
        if (getch() == 27) {
            return;
        }
    }

    Reset_Color();
}











