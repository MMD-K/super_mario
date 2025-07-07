#ifndef MARIO_GAME_H
#define MARIO_GAME_H

#endif // MARIO_GAME_H

#include "mario_game.h"
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//////////////////////////////// Define ////////////////////////////////////////


struct MARIO_SKIN G_Mario_Skin = {
    1001,
    "Mario",
    "🦸",
    1, 4, 0.25, 0
};

struct MARIO_SKIN G_Frunk_Skin = {
    1002,
    "Frunk",
    "👮",
    3, 4, 0.2, 10
};

struct MARIO_SKIN G_Astra_Skin = {
    1003,
    "Astra",
    "🧙",
    1, 5, 0.15, 500
};

struct MARIO_SKIN G_Omen_Skin = {
    1004,
    "Omen",
    "🦹",
    2, 4, 0.15, 750
};

struct MARIO_SKIN G_Spike_Skin = {
    1005,
    "Spike",
    "🧝",
    2, 5, 0.15, 1500
};

struct MARIO_SKIN G_King_MMD_Skin = {
    1006,
    "👑KING MMD",
    "🤴",
    3, 5, 0.1, 2000
};


char G_Item_Type[256 * sizeof(char)] = {};

char *G_UNICODE[ITEM_NUMBER] = {};

static char G_item_number[ITEM_NUMBER] = {};
static char G_last_id[ITEM_NUMBER] = {};
static char G_item_pass[ITEM_NUMBER] = {};
static struct MAP G_map_info;
static struct PIXEL **G_game_map;
static char G_map_area_number = 0;
static struct MAP_AREA *G_map_area;

enum GAME_STATUS G_game_status;

static char G_input_key = 0;
static struct MARIO G_mario;
static struct ACCOUNT_GAME_HISTORY G_game_history;

static struct SPACE G_space;
static struct BLOCK_INDIS G_block_indis;
static struct BLOCK_DIS G_block_dis;
static struct BLOCK_WHITE G_block_white;
static struct BLOCK_GREEN G_block_green;
static struct COIN G_coin;
static struct COIN_BAG G_coin_bag;
static struct SHIELD G_shield;
static struct FLAG G_flag;
static struct BLOCK_COIN *G_block_coin;
static struct BLOCK_MUSHROOM G_block_mushroom;
static struct MUSHROOM *G_mushroom;
static struct CACTUS *G_cactus;
static struct FLOWER *G_flower;
static struct SOUL *G_soul;
static struct TELEPORT *G_teleport;

//////////////////////////////// Structures ////////////////////////////////////



//////////////////////////////// Main Functions ////////////////////////////////

void Start_Game()
{
    // enable unicode
    SetConsoleOutputCP(CP_UTF8);
    // disable cursor
    printf("\x1b[?25l");

    //getcwd(G_location, ADDRESS_SIZE);

    {   // load g_item_type
        G_Item_Type[SPACE_CHAR] = SPACE_TYPE;
        G_Item_Type[MARIO_CHAR] = MARIO_TYPE;
        G_Item_Type[BLOCK_INDIS_CHAR] = BLOCK_INDIS_TYPE;
        G_Item_Type[BLOCK_DIS_CHAR] = BLOCK_DIS_TYPE;
        G_Item_Type[BLOCK_WHITE_CHAR] = BLOCK_WHITE_TYPE;
        G_Item_Type[BLOCK_GREEN_CHAR] = BLOCK_GREEN_TYPE;
        G_Item_Type[COIN_CHAR] = COIN_TYPE;
        G_Item_Type[COIN_BAG_CHAR] = COIN_BAG_TYPE;
        G_Item_Type[SHIELD_CHAR] = SHIELD_TYPE;
        G_Item_Type[FLAG_CHAR] = FLAG_TYPE;
        G_Item_Type[BLOCK_COIN_CHAR] = BLOCK_COIN_TYPE;
        G_Item_Type[BLOCK_MUSHROOM_CHAR] = BLOCK_MUSHROOM_TYPE;
        G_Item_Type[MUSHROOM_CHAR] = MUSHROOM_TYPE;
        G_Item_Type[CACTUS_CHAR] = CACTUS_TYPE;
        G_Item_Type[FLOWER_CHAR] = FLOWER_TYPE;
        G_Item_Type[SOUL_CHAR] = SOUL_TYPE;

        for (int i = '0'; i <= '9'; i++) {
            G_Item_Type[i] = TELEPORT_TYPE;
        }
    }

    {   // load g_item_pass
        G_item_pass[SPACE_TYPE] = 1;
        G_item_pass[MARIO_TYPE] = 1;
        G_item_pass[BLOCK_INDIS_TYPE] = 0;
        G_item_pass[BLOCK_DIS_TYPE] = 0;
        G_item_pass[BLOCK_WHITE_TYPE] = 0;
        G_item_pass[BLOCK_GREEN_TYPE] = 0;
        G_item_pass[COIN_TYPE] = 1;
        G_item_pass[COIN_BAG_TYPE] = 1;
        G_item_pass[SHIELD_TYPE] = 1;
        G_item_pass[FLAG_TYPE] = 1;
        G_item_pass[BLOCK_COIN_TYPE] = 0;
        G_item_pass[BLOCK_MUSHROOM_TYPE] = 0;
        G_item_pass[MUSHROOM_TYPE] = 1;
        G_item_pass[CACTUS_TYPE] = 1;
        G_item_pass[FLOWER_TYPE] = 1;
        G_item_pass[SOUL_TYPE] = 1;
        G_item_pass[TELEPORT_TYPE] = 1;
    }

    {
        G_UNICODE[SPACE_TYPE] = SPACE_UN;
        G_UNICODE[MARIO_TYPE] = MARIO_UN;
        G_UNICODE[BLOCK_INDIS_TYPE] = BLOCK_INDIS_UN;
        G_UNICODE[BLOCK_DIS_TYPE] = BLOCK_DIS_UN;
        G_UNICODE[BLOCK_WHITE_TYPE] = "\x1b[38;5;255m▉▉\x1b[39m";
        G_UNICODE[BLOCK_GREEN_TYPE] = "\x1b[38;5;46m▉▉\x1b[39m";
        G_UNICODE[COIN_TYPE] = COIN_UN;
        G_UNICODE[COIN_BAG_TYPE] = COIN_BAG_UN;
        G_UNICODE[SHIELD_TYPE] = SHIELD_UN;
        G_UNICODE[FLAG_TYPE] = FLAG_UN;
        G_UNICODE[BLOCK_COIN_TYPE] = BLOCK_COIN_UN;
        G_UNICODE[BLOCK_MUSHROOM_TYPE] = BLOCK_MUSHROOM_UN;
        G_UNICODE[MUSHROOM_TYPE] = MUSHROOM_UN;
        G_UNICODE[CACTUS_TYPE] = CACTUS_UN;
        G_UNICODE[FLOWER_TYPE] = FLOWER_UN;
        G_UNICODE[SOUL_TYPE] = SOUL_UN;
        G_UNICODE[TELEPORT_TYPE] = TELEPORT_UN;
    }

    Loader_Page();
}

int Main_Game()
{
    Clear();
    int cur_user_id = 0, start_time = 0;
    long long int cur_acc_indicator = 0;

    // login process
    if ((cur_user_id = LogIn(&cur_acc_indicator)) == EXIT) {    // exit game
        return EXIT;
    }

    // if login was succesful -> load account
    struct PERSONAL_USER_INFO cur_per_user_info;
    struct ACCOUNT_USER_INFO cur_acc_info;
    Load_Account(cur_acc_indicator, &cur_per_user_info, &cur_acc_info);

    // get start time
    start_time = (int)clock() / CLOCKS_PER_SEC;

    // create main menu
    struct WINDOW *mainmenu_map = Load_Map(LOAD_MAIN_MENU_MAP);

    // define fore_color
    struct COLOR fore_color = {3, 4, 94};

    // if you dont sign in you will continue while
    while (TRUE) {
        // create main menu list
        char *menu_names[8] = {"        Play        ", "        Shop        ", "      History       ", " Account Management "
                               , "      Setting        ", "      Creators      ","       Logout       ", "        Exit        "};
        struct MENU mainmenu;
        mainmenu.number = 8;
        mainmenu.active = 0;
        mainmenu.name = menu_names;

        // reload map
        Reload(mainmenu_map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);
        Print_Account_Info(&cur_acc_info, &cur_per_user_info, mainmenu_map->back_color, fore_color);

        // menu function
        int cmd = Menu(&mainmenu, DISPLAY_X_SIZE / 2, MAP_Y_START + 3, mainmenu_map->back_color, fore_color, 0);

        switch (cmd) {
        case 0:
            Game(cur_user_id, &cur_acc_info);
            break;
        case 1:
            Shop(&cur_acc_info, &cur_per_user_info);
            break;
        case 2:
            {   // update history
                struct ACCOUNT_FILE_HISTORY acc_his_file = Get_Histroy_Info(cur_user_id);

                acc_his_file.Time_Play_S += (int)clock() / CLOCKS_PER_SEC - start_time;

                start_time = (int)clock() / CLOCKS_PER_SEC;

                Do_Update_History_Info(&acc_his_file);
            }
            Show_History(cur_user_id, cur_acc_info);
            break;
        case 3:
            if (Account_Management(&cur_per_user_info, cur_acc_indicator, &cur_acc_info) == EXIT) {
                return SUCCESSFUL;
            }
            break;
        case 4:
            Setting();
            break;
        case 5:
            Creator();
            break;
        case 6:
            Logout(cur_user_id, &cur_per_user_info, start_time);
            return SUCCESFUL;
            break;
        case 7:
            Exit(cur_user_id, start_time);
            return EXIT;
            break;
        default:
            break;
        }

    }
}

int End_Game()
{

}



//////////////////////////////// Load Functions ////////////////////////////////

struct WINDOW *Load_Map(enum LOAD_MAP_STATUS load_map_status)
{
    COORD pos = {0, 0};
    FILE *map_file;
    struct WINDOW *map;
    COORD map_size = {0, 0}, start = {MAP_X_START, MAP_Y_START};
    char *temp;
    char (*Item_Loader_Func[ITEM_NUMBER])(COORD pos) = {Space_Loader, Mario_Loader, Block_Indis_Loader
                                                        , Block_dis_Loader, Block_White_Loader, Block_Green_Loader
                                                        , Coin_Loader, Coin_Bag_Loader, Shield_Loader
                                                        , Flag_Loader, Block_Coin_Loader, Block_Mushroom_Loader
                                                        , Mushroom_Loader, Cactus_Loader, Flower_Loader
                                                        , Soul_Loader, Teleport_Loader};


    // load map file
    map_file = Do_Load_Map(load_map_status);

    fseek(map_file, 0, SEEK_SET);

    // read map information
    fscanf(map_file, "#size %d %d\n", &map_size.X, &map_size.Y);
    //map_size.X++;

    temp = (char *)calloc(map_size.X + 1, sizeof(char));

    // create new map size
    map = Create_Window(start, map_size);

    // read map background color
    fscanf(map_file, "#color %d %d %d\n", &map->back_color.r ,&map->back_color.g, &map->back_color.b);

    for (pos.Y = 0; pos.Y < map->size.Y; pos.Y++) {
        fscanf(map_file, "%s", temp);

        for (pos.X = 0; pos.X < map->size.X; pos.X++) {
            map->screen[pos.Y][pos.X].Type = G_Item_Type[temp[pos.X]];
            map->screen[pos.Y][pos.X].ID = 0;
            map->screen[pos.Y][pos.X].is_changed = 0;
        }
    }

    free(temp);

    return map;
}

//////////////////////////////// Game Function ////////////////////////////////

void Game(int user_id, struct ACCOUNT_USER_INFO *acc_user_info)
{
    Clear();
    G_game_status = IN_GAME;
    pthread_t game_ptr, input_ptr;
    int selected_map = 0, selected_skin = 0;
    char cmd = 0;

    // select map
    if ((selected_map = Select_Map()) == EXIT) {
        return;
    }

    if ((selected_skin = Select_Player(acc_user_info)) == EXIT) {
        return;
    }

    // next game
    next_game:

    {   // default value og death and repeat
        G_game_history.Repeat = 0;
        G_game_history.Death = 0;
    }

    // play again
    play_again:

    // load game
    Load_Game(selected_map, selected_skin);

    Loader_Page();

    // pause start
    game_continue:

    // print game map
    Geme_Reload();

    // update game info
    Print_InGame_Health_Info();
    Print_InGame_Death_Info();
    Print_InGame_Coin_Info();
    Print_InGame_Score_Info();
    Print_InGame_Time_Info();

    // game handling
    pthread_create(&game_ptr, NULL, Game_Process, NULL);
    pthread_create(&input_ptr, NULL, Game_Input, NULL);

    pthread_join(game_ptr, NULL);
    pthread_join(input_ptr, NULL);

    if (G_game_status == GAME_PAUSE) {
        cmd = Pause_Game();
        switch (cmd) {
        case 0:
            G_game_status = IN_GAME;
            goto game_continue;
            break;
        case 1:
            G_game_status = IN_GAME;
            G_game_history.Repeat += 1;
            Make_It_Free();
            goto play_again;
            break;
        case 2:
            G_game_history.win_status = GAME_LOSE;
            break;
        default:
            G_game_status = IN_GAME;
            goto game_continue;
            break;
        }
    }
    else if (G_game_status == END_GAME) {
        G_game_history.Game_Score += G_mario.health * 1000;
        G_game_history.Game_Score += ((G_map_info.map_tima_s - (int)G_game_history.Game_Time_S) / 10) * 100;
        cmd = End_In_Game();

        if (G_game_history.win_status == GAME_WIN) {
            switch (cmd) {
            case 0:     // next map
                if (selected_map + 1 != MAP_NUMBER) {
                    G_game_status = IN_GAME;
                    selected_map += 1;
                    UnLoad_Game(user_id, acc_user_info);
                    goto next_game;
                }
                break;
            case 1:     // repeat
                G_game_status = IN_GAME;
                G_game_history.Repeat += 1;
                Make_It_Free();
                goto play_again;
                break;
            case 2:     // exit

                break;
            default:
                break;
            }
        }
        else if (G_game_history.win_status == GAME_LOSE) {
            switch (cmd) {
            case 0:     // repeat
                G_game_status = IN_GAME;
                G_game_history.Repeat += 1;
                Make_It_Free();
                goto play_again;
                break;
            case 1:     // exit

                break;
            default:
                break;
            }
        }
    }

    // end game
    UnLoad_Game(user_id, acc_user_info);
}

void *Game_Process(void *arg)
{
    // define hit functions
    void (*Item_Hit_Func[ITEM_NUMBER])(void) = {Space_Hit, Space_Hit, Block_Indis_Hit
                                                        , Block_dis_Hit, Block_White_Hit, Block_Green_Hit
                                                        , Coin_Hit, Coin_Bag_Hit, Shield_Hit
                                                        , Flag_Hit, Block_Coin_Hit, Block_Mushroom_Hit
                                                        , Mushroom_Hit, Cactus_Hit, Flower_Hit
                                                        , Soul_Hit, Teleport_Hit};



    while (G_game_status == IN_GAME)
    {
        float cur_time = (float)clock() / CLOCKS_PER_SEC;
#ifdef DEBUG
        clock_t t = clock();
#endif
        int xmove = 0, ymove = 0;

        // Updating Objecys
        Soul_Updater();
        Flower_Updater();
        Mushroom_Updater();


        // Updating mario Hit
        Item_Hit_Func[G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].Type]();


        // Updating mario Y
        if (Mario_Y_Updater() == 1) {
            // Updating mario Hit
            Item_Hit_Func[G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].Type]();
        }

        // Updating mario X
        if (Mario_X_Updater() == 1) {
            // Updating mario Hit
            Item_Hit_Func[G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].Type]();
        }

        // reset G_input_key
        G_input_key = 0;

        // set map area
        Set_Map_Area();

        // Update x start position of map (for scrolling)
        xmove = Set_X_Start_Position();
        G_map_info.start.X += xmove;
        if (xmove == 0) {

        }
        else if (xmove > -2 && xmove < 2) {
            //Scroll_Map_X(xmove);
            Scroll_Map_X(xmove);
        }
        else {
            Geme_Reload();
        }

        // Update y start position of map (for scrolling)
        ymove = Set_Y_Start_Position();
        G_map_info.start.Y += ymove;
        if (ymove == 0) {

        }
        else if (ymove > -2 && ymove < 2) {
            //Scroll_Map_Y(ymove);
            Scroll_Map_Y(ymove);
        }
        else {
            Geme_Reload();
        }


        // update screen
        Game_Update();


        // update game info
        Print_InGame_Time_Info();

        // check mario health
        if (G_mario.health < 1 || G_game_history.Game_Time_S == G_map_info.map_tima_s) {
            G_game_history.win_status = GAME_LOSE;
            G_game_history.Death += 1;
            G_game_status = END_GAME;
        }

        // add time
        G_game_history.Game_Time_S += (float)clock() / CLOCKS_PER_SEC - cur_time;

#ifdef DEBUG
        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC;
        if (time_taken && (((int)clock())/CLOCKS_PER_SEC) % 2 == 1)
            printf("\x1b[H\x1b[38;5;10m%.0lf", 1/time_taken);
#endif
    }
}

void *Game_Input(void *arg)
{
    while (G_game_status == IN_GAME) {
        G_input_key = getch();

        if (G_input_key == 27) {
            G_game_status = GAME_PAUSE;
        }
    }
}

int Select_Map()
{
    struct WINDOW *map = Load_Map(LOAD_LOGIN_MAP);

    // define fore_color
    struct COLOR fore_color = {3, 4, 94};


    // create menu list
    char *menu_names[2] = {"        Map 1        ", "        Map 2        "};
    struct MENU menu;
    menu.number = 2;
    menu.active = 0;
    menu.name = menu_names;

    // reload map
    Reload(map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);

    // menu function
    int cmd = Menu(&menu, DISPLAY_X_SIZE / 2, MAP_Y_START + 3, map->back_color, fore_color, 1);

    return cmd;

}

int Select_Player(struct ACCOUNT_USER_INFO *acc_user_info)
{
    struct COLOR fore_color = {3, 4, 94};

    struct WINDOW *map = Load_Map(LOAD_SHOP_MAP);

    // reload map
    Reload(map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);

    return Select_Mario_skin(DISPLAY_X_SIZE / 2 - 10, MAP_Y_START + 3, map->back_color, fore_color, acc_user_info);
}

void Load_Game(int selected_map, int selected_skin)
{
    FILE *map_file;
    struct MARIO_SKIN mario_skin[MARIO_SKIN_NUMBER] = {G_Mario_Skin, G_Frunk_Skin, G_Astra_Skin, G_Omen_Skin, G_Spike_Skin, G_King_MMD_Skin};

    map_file = Do_Load_Map(selected_map);

    fseek(map_file, 0, SEEK_SET);

    {   // load header info of map file
        G_map_info.active_area = 0;

        fscanf(map_file, "#size %d %d\n", &G_map_info.size.X, &G_map_info.size.Y);

        fscanf(map_file, "#area %d", &G_map_area_number);
        G_map_area = (struct MAP_AREA *)calloc(G_map_area_number, sizeof(struct MAP_AREA));

        for (int i = 0; i < G_map_area_number; i++) {
            fscanf(map_file, " | %d %d %d %d", &G_map_area[i].start.X, &G_map_area[i].start.Y
                                                , &G_map_area[i].end.X, &G_map_area[i].end.Y);
            G_map_area[i].size.X = G_map_area[i].end.X - G_map_area[i].start.X;
            G_map_area[i].size.Y = G_map_area[i].end.Y - G_map_area[i].start.Y;
        }
        fscanf(map_file, "\n");

        fscanf(map_file, "#color %d %d %d\n", &G_map_info.map_color.r, &G_map_info.map_color.g, &G_map_info.map_color.b);

        fscanf(map_file, "#time %d\n", &G_map_info.map_tima_s);

        fscanf(map_file, "#block_coin %d\n", &G_item_number[BLOCK_COIN_TYPE]);
        G_block_coin = (struct BLOCK_COIN *)calloc(G_item_number[BLOCK_COIN_TYPE], sizeof(struct BLOCK_COIN));

        fscanf(map_file, "#mushroom %d\n", &G_item_number[MUSHROOM_TYPE]);
        G_mushroom = (struct MUSHROOM *)calloc(G_item_number[MUSHROOM_TYPE], sizeof(struct MUSHROOM));

        fscanf(map_file, "#cactus %d\n", &G_item_number[CACTUS_TYPE]);
        G_cactus = (struct CACTUS *)calloc(G_item_number[CACTUS_TYPE], sizeof(struct CACTUS));

        fscanf(map_file, "#flower %d\n", &G_item_number[FLOWER_TYPE]);
        G_flower = (struct FLOWER *)calloc(G_item_number[FLOWER_TYPE], sizeof(struct FLOWER));

        fscanf(map_file, "#soul %d\n", &G_item_number[SOUL_TYPE]);
        G_soul = (struct SOUL *)calloc(G_item_number[SOUL_TYPE], sizeof(struct SOUL));

        fscanf(map_file, "#teleport %d", &G_item_number[TELEPORT_TYPE]);
        G_teleport = (struct TELEPORT *)calloc(G_item_number[TELEPORT_TYPE], sizeof(struct TELEPORT));

        for (int i = 0; i < G_item_number[TELEPORT_TYPE]; i++) {
            G_teleport[i].pass = 0;
            fscanf(map_file, " | %d %d", &G_teleport[i].pos_end.X, &G_teleport[i].pos_end.Y);
        }

        fscanf(map_file, "\n");
    }

    {
        G_map_info.start.X = 0;
        G_map_info.start.Y = 0;
    }


    {   // load mario
        int j = 0;
        for (j = 0; j < MARIO_SKIN_NUMBER; j++) {
            if (selected_skin == mario_skin[j].ID) {
                strcpy(G_mario.skin, mario_skin[j].skin);
                G_mario.health = mario_skin[j].health;
                G_mario.jump_height = mario_skin[j].jump_height;
                G_mario.speedX = mario_skin[j].speedX;
            }
        }

        G_mario.shield = 0;
        G_mario.jump_status = 0;
        G_mario.jump_step = 0;
        G_mario.speedY = 0.2;
        G_mario.TY = 0;
        G_mario.dir = RIGHT;
        G_mario.TX = 0;
        G_mario.speedk = 5;
        G_mario.TK = 0;
        G_mario.kill_bonus = 1;
    }


    {   // load items
        G_space.pass = 1;
        G_block_indis.pass = 0;
        G_block_dis.pass = 0;
        G_block_white.pass = 0;
        G_block_green.pass = 0;
        G_coin.pass = 0, G_coin.val = 1;
        G_coin_bag.pass = 0, G_coin_bag.val = 10;
        G_shield.pass = 0;
        G_flag.pass = 0;
        G_block_mushroom.pass = 0;
    }

    {   // reset last id
        for (int i = 0; i < ITEM_NUMBER; i++) {
            G_last_id[i] = 0;
        }
    }

    {   // reset game history
        time_t cur_time;
        G_game_history.Game_Coins = 0;
        G_game_history.Game_Score = 0;
        G_game_history.Game_Time_S = 0;
        //G_game_history.Time_Played = *localtime(&cur_time);
        G_game_history.Map_ID = selected_map;
    }

    // set map skin
    if (selected_map == 0) {
        G_UNICODE[SPACE_TYPE] = "  ";
        G_UNICODE[MARIO_TYPE] = G_mario.skin;
        G_UNICODE[BLOCK_INDIS_TYPE] = "🧱";
        G_UNICODE[BLOCK_DIS_TYPE] = "🟧";
        G_UNICODE[BLOCK_WHITE_TYPE] = "\x1b[38;5;255m▉▉\x1b[39m";
        G_UNICODE[BLOCK_GREEN_TYPE] = "\x1b[38;5;46m▉▉\x1b[39m";
        G_UNICODE[COIN_TYPE] = "🪙";
        G_UNICODE[COIN_BAG_TYPE] = "💰";
        G_UNICODE[SHIELD_TYPE] = "🛡️";
        G_UNICODE[FLAG_TYPE] = "🚩";
        G_UNICODE[BLOCK_COIN_TYPE] = "🟨";
        G_UNICODE[BLOCK_MUSHROOM_TYPE] = "🟨";
        G_UNICODE[MUSHROOM_TYPE] = "🍄";
        G_UNICODE[CACTUS_TYPE] = "🌵";
        G_UNICODE[FLOWER_TYPE] = "🌱";
        G_UNICODE[SOUL_TYPE] = "🐙";
        G_UNICODE[TELEPORT_TYPE] = "  ";
    }
    else if (selected_map == 1) {
        G_UNICODE[SPACE_TYPE] = SPACE_UN_2;
        G_UNICODE[MARIO_TYPE] = G_mario.skin;
        G_UNICODE[BLOCK_INDIS_TYPE] = BLOCK_INDIS_UN_2;
        G_UNICODE[BLOCK_DIS_TYPE] = BLOCK_DIS_UN_2;
        G_UNICODE[BLOCK_WHITE_TYPE] = "\x1b[38;5;255m▉▉\x1b[39m";
        G_UNICODE[BLOCK_GREEN_TYPE] = "\x1b[38;5;46m▉▉\x1b[39m";
        G_UNICODE[COIN_TYPE] = COIN_UN_2;
        G_UNICODE[COIN_BAG_TYPE] = COIN_BAG_UN_2;
        G_UNICODE[SHIELD_TYPE] = SHIELD_UN_2;
        G_UNICODE[FLAG_TYPE] = FLAG_UN_2;
        G_UNICODE[BLOCK_COIN_TYPE] = BLOCK_COIN_UN_2;
        G_UNICODE[BLOCK_MUSHROOM_TYPE] = BLOCK_MUSHROOM_UN_2;
        G_UNICODE[MUSHROOM_TYPE] = MUSHROOM_UN_2;
        G_UNICODE[CACTUS_TYPE] = CACTUS_UN_2;
        G_UNICODE[FLOWER_TYPE] = FLOWER_UN_2;
        G_UNICODE[SOUL_TYPE] = SOUL_UN_2;
        G_UNICODE[TELEPORT_TYPE] = TELEPORT_UN_2;
    }

    Reset_Color();

    Load_Map_Game(map_file);

    fclose(map_file);
}

void UnLoad_Game(int user_id, struct ACCOUNT_USER_INFO *acc_user_info)
{
    // save game history
    Do_Add_History(&G_game_history, user_id);

    // get account file history
    struct ACCOUNT_FILE_HISTORY acc_his_info = Get_Histroy_Info(user_id);
    acc_his_info.Games_Played += 1;
    if (G_game_history.win_status == GAME_WIN) {
        acc_his_info.Games_Win += 1;
        if (G_game_history.Game_Score > acc_his_info.Best_Score) {
            acc_his_info.Best_Score = G_game_history.Game_Score;
        }
    }

    // update account user info
    acc_user_info->Score += G_game_history.Game_Score;
    acc_user_info->Coins += G_game_history.Game_Coins;


    // update files
    Do_Update_user_id_Info(acc_user_info);
    Do_Update_History_Info(&acc_his_info);

    // get acc_info

    // make free allocated things
    Make_It_Free();

    strcpy(G_mario.skin, MARIO_UN);
    G_mario.health = G_Mario_Skin.health;
    G_mario.jump_height = G_Mario_Skin.jump_height;
    G_mario.speedX = G_Mario_Skin.speedX;

    {
        G_UNICODE[SPACE_TYPE] = "  ";
        G_UNICODE[MARIO_TYPE] = G_mario.skin;
        G_UNICODE[BLOCK_INDIS_TYPE] = "🧱";
        G_UNICODE[BLOCK_DIS_TYPE] = "🟧";
        G_UNICODE[BLOCK_WHITE_TYPE] = "\x1b[38;5;255m▉▉\x1b[39m";
        G_UNICODE[BLOCK_GREEN_TYPE] = "\x1b[38;5;46m▉▉\x1b[39m";
        G_UNICODE[COIN_TYPE] = "🪙";
        G_UNICODE[COIN_BAG_TYPE] = "💰";
        G_UNICODE[SHIELD_TYPE] = "🛡️";
        G_UNICODE[FLAG_TYPE] = "🚩";
        G_UNICODE[BLOCK_COIN_TYPE] = "🟨";
        G_UNICODE[BLOCK_MUSHROOM_TYPE] = "🟨";
        G_UNICODE[MUSHROOM_TYPE] = "🍄";
        G_UNICODE[CACTUS_TYPE] = "🌵";
        G_UNICODE[FLOWER_TYPE] = "🌱";
        G_UNICODE[SOUL_TYPE] = "🐙";
        G_UNICODE[TELEPORT_TYPE] = "  ";
    }

    Reset_Color();
    Clear();
}

void Load_Map_Game(FILE *map_file)
{
    COORD pos = {0, 0};
    char *temp;
    // define func loader list
    char (*Item_Loader_Func[ITEM_NUMBER])(COORD pos) = {Space_Loader, Mario_Loader, Block_Indis_Loader
                                                                        , Block_dis_Loader, Block_White_Loader, Block_Green_Loader
                                                                        , Coin_Loader, Coin_Bag_Loader, Shield_Loader
                                                                        , Flag_Loader, Block_Coin_Loader, Block_Mushroom_Loader
                                                                        , Mushroom_Loader, Cactus_Loader, Flower_Loader
                                                                        , Soul_Loader, Teleport_Loader};

    temp = (char *)calloc(G_map_info.size.X + 1, sizeof(char));

    G_game_map = (struct PIXEL **)calloc(G_map_info.size.Y, sizeof(struct PIXEL *));

    for (int i = 0; i < G_map_info.size.Y; i++) {
        G_game_map[i] = (struct PIXEL *)calloc(G_map_info.size.X, sizeof(struct PIXEL));
    }

    for (pos.Y = 0; pos.Y < G_map_info.size.Y; pos.Y++) {
        fscanf(map_file, "%s", temp);

        for (pos.X = 0; pos.X < G_map_info.size.X; pos.X++) {
            G_game_map[pos.Y][pos.X].Type = G_Item_Type[temp[pos.X]];
            G_last_id[TELEPORT_TYPE] = temp[pos.X] - '0';
            G_game_map[pos.Y][pos.X].ID = Item_Loader_Func[G_Item_Type[temp[pos.X]]](pos);
            G_game_map[pos.Y][pos.X].is_changed = 0;
        }
    }

    free(temp);
}

char Pause_Game()
{
    int win_size_x = 20, win_size_y = 11, cmd = 0;
    // define fore_color
    struct COLOR fore_color = {0, 119, 182};
    struct COLOR back_color = {3, 4, 94};

    Set_Back_Color(back_color.r, back_color.g, back_color.b);

    for (int i = 0; i < win_size_y; i++) {
        printf("\x1b[%d;%dH%*c", MAP_Y_START + MAP_MAX_Y_SIZE / 2 - win_size_y / 2 + i
               , DISPLAY_X_SIZE / 2 - win_size_x / 2, win_size_x, ' ');
    }

    // create menu list
    char *menu_names[3] = {"    Continue    ", "    Play Again    ","      Exit      "};
    struct MENU menu;
    menu.number = 3;
    menu.active = 0;
    menu.name = menu_names;

    // menu function
    cmd = Menu(&menu, DISPLAY_X_SIZE / 2, MAP_Y_START + MAP_MAX_Y_SIZE / 2 - 1, back_color, fore_color, 1);

    return cmd;
}

char End_In_Game()
{
    int win_size_x = 20, win_size_y = 11, cmd = 0;
    // define fore_color
    struct COLOR fore_color = {0, 119, 182};
    struct COLOR back_color = {3, 4, 94};

    Set_Back_Color(back_color.r, back_color.g, back_color.b);

    for (int i = 0; i < win_size_y; i++) {
        printf("\x1b[%d;%dH%*c", MAP_Y_START + MAP_MAX_Y_SIZE / 2 - win_size_y / 2 + i
               , DISPLAY_X_SIZE / 2 - win_size_x / 2, win_size_x, ' ');
    }

    // write game history
    printf("\x1b[%d;%dH%s %d", MAP_Y_START + 5, DISPLAY_X_SIZE / 2 - 7, CLOCK_UN, (int)G_game_history.Game_Time_S);
    printf("\x1b[%d;%dH%s %d", MAP_Y_START + 5, DISPLAY_X_SIZE / 2 + 2, REPEAT_UN, G_game_history.Repeat);
    printf("\x1b[%d;%dH%s %d", MAP_Y_START + 6, DISPLAY_X_SIZE / 2 - 7, SCORE_UN, G_game_history.Game_Score);
    printf("\x1b[%d;%dH%s %d", MAP_Y_START + 6, DISPLAY_X_SIZE / 2 + 2, COIN_UN, G_game_history.Game_Coins);
    printf("\x1b[%d;%dH%s %d", MAP_Y_START + 7, DISPLAY_X_SIZE / 2 - 7, DEATH_UN, G_game_history.Death);

    if (G_game_history.win_status == GAME_WIN) {
        // write game history
        printf("\x1b[%d;%dH%s", MAP_Y_START + 3, DISPLAY_X_SIZE / 2 - strlen("You Win") / 2, "You Win");

        // create menu list
        char *menu_names[3] = {"    Next map    ", "    Play Again    ", "      Exit      "};
        struct MENU menu;
        menu.number = 3;
        menu.active = 0;
        menu.name = menu_names;

        // menu function
        cmd = Menu(&menu, DISPLAY_X_SIZE / 2, MAP_Y_START + MAP_MAX_Y_SIZE - 6, back_color, fore_color, 0);
    }
    else if (G_game_history.win_status == GAME_LOSE) {
        // write game history
        printf("\x1b[%d;%dH%s", MAP_Y_START + 3, DISPLAY_X_SIZE / 2 - strlen("You Lose") / 2, "You Lose");

        // create menu list
        char *menu_names[2] = {"    Play Again    ", "      Exit      "};
        struct MENU menu;
        menu.number = 2;
        menu.active = 0;
        menu.name = menu_names;

        // menu function
        cmd = Menu(&menu, DISPLAY_X_SIZE / 2, MAP_Y_START + MAP_MAX_Y_SIZE - 5, back_color, fore_color, 0);
    }

    return cmd;
}

void Make_It_Free()
{
    free(G_block_coin);
    free(G_mushroom);
    free(G_cactus);
    free(G_flower);
    free(G_soul);
    free(G_teleport);
    free(G_map_area);

    for (int i = 0; i < G_map_info.size.Y; i++) {
        free(G_game_map[i]);
    }

    free(G_game_map);
}


/////////////////////////////// Item Loader //////////////////////////////////////

char Space_Loader(COORD pos)
{
    return 0;
}

char Mario_Loader(COORD pos)
{
    G_mario.cur_pos = G_mario.next_pos = pos;
    return 0;
}

char Block_Indis_Loader(COORD pos)
{
    return 0;
}

char Block_dis_Loader(COORD pos)
{
    return 0;
}

char Block_White_Loader(COORD pos)
{
    return 0;
}

char Block_Green_Loader(COORD pos)
{
    return 0;
}

char Coin_Loader(COORD pos)
{
    return 0;
}

char Coin_Bag_Loader(COORD pos)
{
    return 0;
}

char Shield_Loader(COORD pos)
{
    return 0;
}

char Flag_Loader(COORD pos)
{
    return 0;
}

char Block_Coin_Loader(COORD pos)
{
    srand(clock() - G_last_id[BLOCK_COIN_TYPE]);
    G_block_coin[G_last_id[BLOCK_COIN_TYPE]].pass = 0;
    G_block_coin[G_last_id[BLOCK_COIN_TYPE]].health = rand() % 5 + 1;
    G_block_coin[G_last_id[BLOCK_COIN_TYPE]].val = 1;

    G_last_id[BLOCK_COIN_TYPE]++;
    return G_last_id[BLOCK_COIN_TYPE] - 1;
}

char Block_Mushroom_Loader(COORD pos)
{
    return Mushroom_Loader(pos);
}

char Mushroom_Loader(COORD pos)
{
    srand(clock() - G_last_id[MUSHROOM_TYPE]);
    G_mushroom[G_last_id[MUSHROOM_TYPE]].dir = rand() % 3 + 1;
    G_mushroom[G_last_id[MUSHROOM_TYPE]].pass = 0;
    G_mushroom[G_last_id[MUSHROOM_TYPE]].speedX = 1;
    G_mushroom[G_last_id[MUSHROOM_TYPE]].speedY = 1;
    G_mushroom[G_last_id[MUSHROOM_TYPE]].TX = 0;
    G_mushroom[G_last_id[MUSHROOM_TYPE]].TY = 0;
    G_mushroom[G_last_id[MUSHROOM_TYPE]].pos = pos;
    G_mushroom[G_last_id[MUSHROOM_TYPE]].health = 0;
    G_mushroom[G_last_id[MUSHROOM_TYPE]].val = 1;

    G_last_id[MUSHROOM_TYPE]++;
    return G_last_id[MUSHROOM_TYPE] - 1;
}

char Cactus_Loader(COORD pos)
{
    G_cactus[G_last_id[CACTUS_TYPE]].pass = 0;
    G_cactus[G_last_id[CACTUS_TYPE]].TD = 0;
    G_cactus[G_last_id[CACTUS_TYPE]].damage = 1;
    G_cactus[G_last_id[CACTUS_TYPE]].speedD = 1;

    G_last_id[CACTUS_TYPE]++;
    return G_last_id[CACTUS_TYPE] - 1;
}

char Flower_Loader(COORD pos)
{
    srand(clock() - G_last_id[FLOWER_TYPE]);
    G_flower[G_last_id[FLOWER_TYPE]].pass = 0;
    G_flower[G_last_id[FLOWER_TYPE]].speedY = 2;
    G_flower[G_last_id[FLOWER_TYPE]].status = rand() % 2;
    G_flower[G_last_id[FLOWER_TYPE]].TY = (float)clock()/CLOCKS_PER_SEC - (float)(rand() % 200) / 100;
    G_flower[G_last_id[FLOWER_TYPE]].pos = pos;
    G_flower[G_last_id[FLOWER_TYPE]].damage = 1;

    G_last_id[FLOWER_TYPE]++;
    return G_last_id[FLOWER_TYPE] - 1;
}

char Soul_Loader(COORD pos)
{
    G_soul[G_last_id[SOUL_TYPE]].health = 1;
    G_soul[G_last_id[SOUL_TYPE]].pass = 0;
    G_soul[G_last_id[SOUL_TYPE]].pos = pos;
    G_soul[G_last_id[SOUL_TYPE]].damage = 1;
    G_soul[G_last_id[SOUL_TYPE]].dir = LEFT;
    G_soul[G_last_id[SOUL_TYPE]].TD = 0;
    G_soul[G_last_id[SOUL_TYPE]].TX = 0;
    G_soul[G_last_id[SOUL_TYPE]].speedD = 1;
    G_soul[G_last_id[SOUL_TYPE]].speedX = 0.5;

    G_last_id[SOUL_TYPE]++;
    return G_last_id[SOUL_TYPE] - 1;
}

char Teleport_Loader(COORD pos)
{
    return G_last_id[TELEPORT_TYPE];
}


////////////////////////////// Item Updeter ///////////////////////////////////////

char Mario_X_Updater()
{
    // right and left move
    if (G_input_key == 'a' || G_input_key == 'A') {
        float cur_time = (float)clock() / CLOCKS_PER_SEC;

        if (cur_time - G_mario.TX > G_mario.speedX) {
            G_mario.next_pos.X = G_mario.cur_pos.X - 1;
            G_mario.next_pos.Y = G_mario.cur_pos.Y;
            G_mario.TX = cur_time;

            return 1;
        }
    }
    else if (G_input_key == 'd' || G_input_key == 'D') {
        float cur_time = (float)clock() / CLOCKS_PER_SEC;

        if (cur_time - G_mario.TX > G_mario.speedX) {
            G_mario.next_pos.X = G_mario.cur_pos.X + 1;
            G_mario.next_pos.Y = G_mario.cur_pos.Y;
            G_mario.TX = cur_time;

            return 1;
        }
    }

    return 0;
}

char Mario_Y_Updater()
{
    float cur_time = (float)clock() / CLOCKS_PER_SEC;

    // if w presed and mario were walking
    if (G_mario.jump_status == WALK) {
        if (G_item_pass[G_game_map[G_mario.cur_pos.Y + 1][G_mario.cur_pos.X].Type] == 1) {
            G_mario.jump_status = FALL;
            G_mario.TY = 0;
        }
        else {
            if (G_input_key == 'w' || G_input_key == 'W') {
                G_mario.jump_status = JUMP;
                G_mario.jump_step = 0;
                G_mario.TY = 0;
            }
            else if (G_input_key == 's' || G_input_key == 'S') {

            }
        }
    }

    // up and down move
    if (G_mario.jump_status == JUMP) {
        if (G_mario.jump_step < G_mario.jump_height) {
            if (cur_time - G_mario.TY > G_mario.speedY) {
                G_mario.jump_step += 1;
                G_mario.next_pos.Y = G_mario.cur_pos.Y - 1;
                G_mario.next_pos.X = G_mario.cur_pos.X;
                G_mario.TY = cur_time;

                return 1;
            }
        }
        else {
            G_mario.jump_status = FALL;
            G_mario.jump_step = 0;
            G_mario.TY = cur_time;
        }
    }
    else if (G_mario.jump_status == FALL){
        if (cur_time - G_mario.TY > G_mario.speedY) {
            G_mario.next_pos.Y = G_mario.cur_pos.Y + 1;
            G_mario.next_pos.X = G_mario.cur_pos.X;
            G_mario.TY = cur_time;

            return 1;
        }
    }

    return 0;
}

void Mushroom_Updater()
{
    // update all mushrooms
    for (int i = 0; i < G_item_number[MUSHROOM_TYPE]; i++)
    {
        if (G_mushroom[i].health > 0)
        {
            if (G_mushroom[i].dir != STOP)
            {
                float cur_time = (float)clock() / CLOCKS_PER_SEC;

                // reset cur position in map
                G_game_map[G_mushroom[i].pos.Y][G_mushroom[i].pos.X].Type = SPACE_TYPE;
                G_game_map[G_mushroom[i].pos.Y][G_mushroom[i].pos.X].ID = 0;
                G_game_map[G_mushroom[i].pos.Y][G_mushroom[i].pos.X].is_changed = 1;

                // Y move
                if (G_game_map[G_mushroom[i].pos.Y + 1][G_mushroom[i].pos.X].Type == SPACE_TYPE
                    || G_game_map[G_mushroom[i].pos.Y + 1][G_mushroom[i].pos.X].Type == MARIO_TYPE) {
                    if (cur_time - G_mushroom[i].TY > G_mushroom[i].speedY) {
                        G_mushroom[i].pos.Y += 1;
                        G_mushroom[i].TY = cur_time;
                    }
                }
                else {
                    G_mushroom[i].dir = STOP;
                }

                // X move
                if (G_mushroom[i].dir == RIGHT) {
                    if (G_game_map[G_mushroom[i].pos.Y][G_mushroom[i].pos.X + 1].Type == SPACE_TYPE) {
                        if (cur_time - G_mushroom[i].TX > G_mushroom[i].speedX) {
                            G_mushroom[i].pos.X += 1;
                            G_mushroom[i].TX = cur_time;
                        }
                    }
                    else {
                        G_mushroom[i].dir = DOWN;
                    }
                }
                else if (G_mushroom[i].dir == LEFT) {
                    if (G_game_map[G_mushroom[i].pos.Y][G_mushroom[i].pos.X - 1].Type == SPACE_TYPE) {
                        if (cur_time - G_mushroom[i].TX > G_mushroom[i].speedX) {
                            G_mushroom[i].pos.X -= 1;
                            G_mushroom[i].TX = cur_time;
                        }
                    }
                    else {
                        G_mushroom[i].dir = DOWN;
                    }
                }

                // replace new postion on map
                G_game_map[G_mushroom[i].pos.Y][G_mushroom[i].pos.X].Type = MUSHROOM_TYPE;
                G_game_map[G_mushroom[i].pos.Y][G_mushroom[i].pos.X].ID = i;
                G_game_map[G_mushroom[i].pos.Y][G_mushroom[i].pos.X].is_changed = 1;
            }
            else if (G_mushroom[i].dir == STOP) {
                if (G_game_map[G_mushroom[i].pos.Y + 1][G_mushroom[i].pos.X].Type == SPACE_TYPE
                    || G_game_map[G_mushroom[i].pos.Y + 1][G_mushroom[i].pos.X].Type == MARIO_TYPE) {
                    G_mushroom[i].dir = DOWN;
                }
                // G_game_map[G_mushroom[i].pos.Y][G_mushroom[i].pos.X].Type = MUSHROOM_TYPE;
                // G_game_map[G_mushroom[i].pos.Y][G_mushroom[i].pos.X].ID = i;
                // G_game_map[G_mushroom[i].pos.Y][G_mushroom[i].pos.X].is_changed = 1;
            }
        }
    }
}

void Flower_Updater()
{
    // update all flowers
    for (int i = 0; i < G_item_number[FLOWER_TYPE]; i++) {
        float cur_time = (float)clock() / CLOCKS_PER_SEC;

        // Y move
        if (cur_time - G_flower[i].TY > G_flower[i].speedY) {
            if (G_flower[i].status == 1) {
                G_flower[i].status = 0;
                G_game_map[G_flower[i].pos.Y][G_flower[i].pos.X].Type = SPACE_TYPE;
                G_game_map[G_flower[i].pos.Y][G_flower[i].pos.X].ID = 0;
                G_game_map[G_flower[i].pos.Y][G_flower[i].pos.X].is_changed = 1;
            }
            else {
                G_flower[i].status = 1;
                G_game_map[G_flower[i].pos.Y][G_flower[i].pos.X].Type = FLOWER_TYPE;
                G_game_map[G_flower[i].pos.Y][G_flower[i].pos.X].ID = i;
                G_game_map[G_flower[i].pos.Y][G_flower[i].pos.X].is_changed = 1;
            }

            G_flower[i].TY = cur_time;
        }
    }
}

void Soul_Updater()
{
    // update all souls
    for (int i = 0; i < G_item_number[SOUL_TYPE]; i++) {
        if (G_soul[i].health > 0) {
            float cur_time = (float)clock() / CLOCKS_PER_SEC;

            if (cur_time - G_soul[i].TX > G_soul[i].speedX) {
                G_game_map[G_soul[i].pos.Y][G_soul[i].pos.X].Type = SPACE_TYPE;
                G_game_map[G_soul[i].pos.Y][G_soul[i].pos.X].ID = 0;
                G_game_map[G_soul[i].pos.Y][G_soul[i].pos.X].is_changed = 1;

                if (G_soul[i].dir == RIGHT) {
                    if ((G_game_map[G_soul[i].pos.Y][G_soul[i].pos.X + 1].Type == SPACE_TYPE
                         || G_game_map[G_soul[i].pos.Y][G_soul[i].pos.X + 1].Type == MARIO_TYPE)
                        && G_game_map[G_soul[i].pos.Y + 1][G_soul[i].pos.X + 1].Type != SPACE_TYPE) {
                        G_soul[i].pos.X += 1;
                    }
                    else {
                        G_soul[i].dir = LEFT;
                    }
                }
                else {
                    if ((G_game_map[G_soul[i].pos.Y][G_soul[i].pos.X - 1].Type == SPACE_TYPE
                         || G_game_map[G_soul[i].pos.Y][G_soul[i].pos.X - 1].Type == MARIO_TYPE)
                        && G_game_map[G_soul[i].pos.Y + 1][G_soul[i].pos.X - 1].Type != SPACE_TYPE) {
                        G_soul[i].pos.X -= 1;
                    }
                    else {
                        G_soul[i].dir = RIGHT;
                    }
                }

                G_soul[i].TX = cur_time;

                G_game_map[G_soul[i].pos.Y][G_soul[i].pos.X].Type = SOUL_TYPE;
                G_game_map[G_soul[i].pos.Y][G_soul[i].pos.X].ID = i;
                G_game_map[G_soul[i].pos.Y][G_soul[i].pos.X].is_changed = 1;
            }
        }
    }
}


////////////////////////////// Item Hit ///////////////////////////////////////////

void Space_Hit()
{
    // reset previous position
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].Type = SPACE_TYPE;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID = 0;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].is_changed = 1;

    // set new position
    G_mario.cur_pos = G_mario.next_pos;

    // write new position
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].Type = MARIO_TYPE;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID = 0;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].is_changed = 1;
}

void Block_Indis_Hit()
{
    // if hit from bellow
    if (G_mario.cur_pos.Y > G_mario.next_pos.Y) {
        G_mario.jump_status = FALL;
        G_mario.jump_step = 0;
        G_mario.TY = 0;
    }
    // if hit from top
    else if (G_mario.cur_pos.Y < G_mario.next_pos.Y) {
        G_mario.jump_status = WALK;
        G_mario.TY = 0;
    }
    G_mario.next_pos = G_mario.cur_pos;
}

void Block_dis_Hit()
{
    // if hit from bellow
    if (G_mario.cur_pos.Y > G_mario.next_pos.Y) {
        G_mario.jump_status = FALL;
        G_mario.jump_step = 0;
        G_mario.TY = 0;
        if (G_mario.shield == 1) {
            G_mario.shield = 0;
            G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].Type = SPACE_TYPE;
            G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].ID = 0;
            G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].is_changed = 1;
            // update game info
            Print_InGame_Health_Info();
        }
    }
    // if hit from top
    else if (G_mario.cur_pos.Y < G_mario.next_pos.Y) {
        G_mario.jump_status = WALK;
        G_mario.TY = 0;
    }
    G_mario.next_pos = G_mario.cur_pos;
}

void Block_White_Hit()
{
    // if hit from bellow
    if (G_mario.cur_pos.Y > G_mario.next_pos.Y) {
        G_mario.jump_status = FALL;
        G_mario.jump_step = 0;
        G_mario.TY = 0;
    }
    // if hit from top
    else if (G_mario.cur_pos.Y < G_mario.next_pos.Y) {
        G_mario.jump_status = WALK;
        G_mario.TY = 0;
    }
    G_mario.next_pos = G_mario.cur_pos;
}

void Block_Green_Hit()
{
    // if hit from bellow
    if (G_mario.cur_pos.Y > G_mario.next_pos.Y) {
        G_mario.jump_status = FALL;
        G_mario.jump_step = 0;
        G_mario.TY = 0;
    }
    // if hit from top
    else if (G_mario.cur_pos.Y < G_mario.next_pos.Y) {
        G_mario.jump_status = WALK;
        G_mario.TY = 0;
    }
    G_mario.next_pos = G_mario.cur_pos;
}

void Coin_Hit()
{
    // add coin
    G_game_history.Game_Coins += G_coin.val;

    // reset previous position
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].Type = SPACE_TYPE;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID = 0;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].is_changed = 1;

    // set new position
    G_mario.cur_pos = G_mario.next_pos;

    // write new position
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].Type = MARIO_TYPE;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID = 0;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].is_changed = 1;

    // update game info
    Print_InGame_Coin_Info();
}

void Coin_Bag_Hit()
{
    // add coin
    G_game_history.Game_Coins += G_coin_bag.val;

    // reset previous position
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].Type = SPACE_TYPE;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID = 0;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].is_changed = 1;

    // set new position
    G_mario.cur_pos = G_mario.next_pos;

    // write new position
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].Type = MARIO_TYPE;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID = 0;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].is_changed = 1;

    // update game info
    Print_InGame_Coin_Info();
}

void Shield_Hit()
{
    // change shield status
    G_mario.shield = 1;

    // reset previous position
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].Type = SPACE_TYPE;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID = 0;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].is_changed = 1;

    // set new position
    G_mario.cur_pos = G_mario.next_pos;

    // write new position
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].Type = MARIO_TYPE;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID = 0;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].is_changed = 1;

    // update game info
    Print_InGame_Health_Info();
}

void Flag_Hit()
{
    // win
    G_game_history.win_status = GAME_WIN;
    G_game_status = END_GAME;

    // reset previous position
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].Type = SPACE_TYPE;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID = 0;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].is_changed = 1;

    // set new position
    G_mario.cur_pos = G_mario.next_pos;

    // write new position
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].Type = MARIO_TYPE;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID = 0;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].is_changed = 1;
}

void Block_Coin_Hit()
{
    // if hit from bellow
    if (G_mario.cur_pos.Y > G_mario.next_pos.Y) {
        G_mario.jump_status = FALL;
        G_mario.jump_step = 0;
        G_mario.TY = 0;

        G_block_coin[G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].ID].health -= 1;

        G_game_history.Game_Coins += G_block_coin[G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].ID].val;

        // if block coin health == 0
        if (G_block_coin[G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].ID].health == 0) {
            G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].Type = SPACE_TYPE;
            G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].ID = 0;
            G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].is_changed = 1;
        }
    }
    // if hit from top
    else if (G_mario.cur_pos.Y < G_mario.next_pos.Y) {
        G_mario.jump_status = WALK;
        G_mario.TY = 0;
    }
    G_mario.next_pos = G_mario.cur_pos;

    // update game info
    Print_InGame_Coin_Info();
}

void Block_Mushroom_Hit()
{
    // if hit from bellow
    if (G_mario.cur_pos.Y > G_mario.next_pos.Y) {
        G_mario.jump_status = FALL;
        G_mario.jump_step = 0;
        G_mario.TY = 0;

        G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].Type = MUSHROOM_TYPE;
        G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].is_changed = 1;

        G_mushroom[G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].ID].TY = G_mushroom[G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].ID].TX = (float)clock() / CLOCKS_PER_SEC;
        G_mushroom[G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].ID].health = 1;
    }
    // if hit from top
    else if (G_mario.cur_pos.Y < G_mario.next_pos.Y) {
        G_mario.jump_status = WALK;
        G_mario.TY = 0;
    }
    G_mario.next_pos = G_mario.cur_pos;
}

void Mushroom_Hit()
{
    // add health
    if (G_mushroom[G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].ID].health > 0) {
        G_mario.health += G_mushroom[G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].ID].val;
    }
    G_mushroom[G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID].health = 0;

    // reset previous position
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].Type = SPACE_TYPE;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID = 0;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].is_changed = 1;

    // set new position
    G_mario.cur_pos = G_mario.next_pos;

    // write new position
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].Type = MARIO_TYPE;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID = 0;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].is_changed = 1;

    Print_InGame_Health_Info();
}

void Cactus_Hit()
{
    float cur_time = (float)clock() / CLOCKS_PER_SEC;

    // if hit from bellow
    if (G_mario.cur_pos.Y > G_mario.next_pos.Y) {
        G_mario.jump_status = FALL;
        G_mario.jump_step = 0;
        G_mario.TY = 0;
    }
    // if hit from top
    else if (G_mario.cur_pos.Y < G_mario.next_pos.Y) {
        G_mario.jump_status = WALK;
        G_mario.TY = 0;
    }

    // decrease mario health
    if (cur_time - G_cactus[G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].ID].TD > G_cactus[G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].ID].speedD) {
        if (G_mario.shield == 1) {
            G_mario.shield = 0;
        }
        else {
            G_mario.health -= G_cactus[G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].ID].damage;
        }
        G_cactus[G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].ID].TD = cur_time;

        // update game info
        Print_InGame_Health_Info();
    }

    G_mario.next_pos = G_mario.cur_pos;
}

void Flower_Hit()
{
    // reset previous position
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].Type = SPACE_TYPE;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID = 0;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].is_changed = 1;

    // set new position
    G_mario.cur_pos = G_mario.next_pos;

    // update flower state
    G_flower[G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID].status = 0;
    G_flower[G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID].TY = (float)clock() / CLOCKS_PER_SEC;

    // decrease mario health
    if (G_mario.shield == 1) {
        G_mario.shield = 0;
    }
    else {
        G_mario.health -= G_flower[G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID].damage;
    }

    // write new position
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].Type = MARIO_TYPE;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID = 0;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].is_changed = 1;

    // update game info
    Print_InGame_Health_Info();
}

void Soul_Hit()
{
    float cur_time = (float)clock() / CLOCKS_PER_SEC;

    // if hit from up
    if (G_mario.cur_pos.Y < G_mario.next_pos.Y) {
        G_soul[G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].ID].health = 0;

        // update kill bonus
        if (cur_time - G_mario.TK <= G_mario.speedk) {
            G_mario.kill_bonus *= 2;
        }
        else {
            G_mario.kill_bonus = 1;
        }

        // add score
        G_game_history.Game_Score += G_mario.kill_bonus * KILL_SCORE;

        G_mario.TK = cur_time;

        // update game info
        Print_InGame_Score_Info();
    }
    else {
        if (cur_time - G_soul[G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].ID].TD > G_soul[G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].ID].speedD) {
            if (G_mario.shield == 1) {
                G_mario.shield = 0;
            }
            else {
                G_mario.health -= G_soul[G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].ID].damage;
            }
            G_soul[G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].ID].TD = cur_time;

            // update game info
            Print_InGame_Health_Info();
        }
    }

    // reset previous position
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].Type = SPACE_TYPE;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID = 0;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].is_changed = 1;

    // set new position
    G_mario.cur_pos = G_mario.next_pos;

    // write new position
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].Type = MARIO_TYPE;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID = 0;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].is_changed = 1;
}

void Teleport_Hit()
{
    // reset previous position
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].Type = SPACE_TYPE;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID = 0;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].is_changed = 1;

    // set new position
    G_mario.next_pos = G_teleport[G_game_map[G_mario.next_pos.Y][G_mario.next_pos.X].ID].pos_end;
    G_mario.cur_pos = G_mario.next_pos;

    // write new position
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].Type = MARIO_TYPE;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].ID = 0;
    G_game_map[G_mario.cur_pos.Y][G_mario.cur_pos.X].is_changed = 1;
}


/////////////////////////// Refresh Game //////////////////////////////////////

void Geme_Reload()
{
    int y = 0, x = 0, yend = MAP_MAX_Y_SIZE + G_map_info.start.Y, xend = MAP_MAX_X_SIZE + G_map_info.start.X;

    // set background attr
    Set_Back_Color(G_map_info.map_color.r, G_map_info.map_color.g, G_map_info.map_color.b);

    clock_t t = clock();

    // print screen
    for (y = G_map_info.start.Y; y < yend; y++) {
        printf("\x1b[%d;%dH", y + MAP_Y_START - G_map_area[G_map_info.active_area].start.Y, MAP_X_START + 1);
        for (x = G_map_info.start.X; x < xend; x++) {
            printf("%s", G_UNICODE[ G_game_map[y][x].Type ]);
        }
    }

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;

#ifdef DEBUG
    printf("\x1b[H\x1b[38;5;10m%lf", time_taken);
#endif
}

void Game_Update()
{
    int y = 0, x = 0, yend = MAP_MAX_Y_SIZE + G_map_info.start.Y
        , xend = MAP_MAX_X_SIZE + G_map_info.start.X;

    // set background attr
    Set_Back_Color(G_map_info.map_color.r, G_map_info.map_color.g, G_map_info.map_color.b);

    // print screen
    for (y = G_map_info.start.Y; y < yend; y++) {
        for (x = G_map_info.start.X; x < xend; x++) {
            if (G_game_map[y][x].is_changed == 1) {
                printf("\x1b[%d;%dH%s", y + MAP_Y_START  - G_map_area[G_map_info.active_area].start.Y, 2 * (x - G_map_info.start.X) + MAP_X_START + 1
                       , G_UNICODE[ G_game_map[y][x].Type ]);
                G_game_map[y][x].is_changed = 0;
            }
        }
    }
}

void Set_Map_Area()
{
    for (int i = 0; i < G_map_area_number; i++) {
        if (G_mario.cur_pos.X >= G_map_area[i].start.X && G_mario.cur_pos.X < G_map_area[i].end.X
            && G_mario.cur_pos.Y >= G_map_area[i].start.Y && G_mario.cur_pos.Y < G_map_area[i].end.Y) {
            G_map_info.active_area = i;
            return;
        }
    }
}

int Set_X_Start_Position()
{
    COORD next_start_position;

    if (G_map_area[G_map_info.active_area].size.X <= MAP_MAX_X_SIZE) {
        next_start_position.X = G_map_area[G_map_info.active_area].start.X;
    }
    else if (G_mario.cur_pos.X - G_map_area[G_map_info.active_area].start.X < MAP_MAX_X_SIZE / 2) {
        next_start_position.X = G_map_area[G_map_info.active_area].start.X;
    }
    else if (G_mario.cur_pos.X + MAP_MAX_X_SIZE / 2 >= G_map_area[G_map_info.active_area].end.X) {
        next_start_position.X = G_map_area[G_map_info.active_area].end.X - MAP_MAX_X_SIZE;
    }
    else {
        next_start_position.X = G_mario.cur_pos.X - MAP_MAX_X_SIZE / 2;
    }

    return next_start_position.X - G_map_info.start.X;

    // if (G_map_info.size.X <= MAP_MAX_X_SIZE) {
    //     next_start_position.X = 0;
    // }
    // else if (G_mario.cur_pos.X - MAP_MAX_X_SIZE / 2 < 0) {
    //     next_start_position.X = 0;
    // }
    // else if (G_mario.cur_pos.X + MAP_MAX_X_SIZE / 2 >= G_map_info.size.X) {
    //     next_start_position.X = G_map_info.size.X - MAP_MAX_X_SIZE;
    // }
    // else {
    //     next_start_position.X = G_mario.cur_pos.X - MAP_MAX_X_SIZE / 2;
    // }

    // return next_start_position.X - G_map_info.start.X;
}

int Set_Y_Start_Position()
{
    COORD next_start_position;

    if (G_map_area[G_map_info.active_area].size.Y <= MAP_MAX_Y_SIZE) {
        next_start_position.Y = G_map_area[G_map_info.active_area].start.Y;
    }
    else if (G_mario.cur_pos.Y - G_map_area[G_map_info.active_area].start.Y < MAP_MAX_Y_SIZE / 2) {
        next_start_position.Y = G_map_area[G_map_info.active_area].start.Y;
    }
    else if (G_mario.cur_pos.Y + MAP_MAX_Y_SIZE / 2 >= G_map_area[G_map_info.active_area].end.Y) {
        next_start_position.Y = G_map_area[G_map_info.active_area].end.Y - MAP_MAX_Y_SIZE;
    }
    else {
        next_start_position.Y = G_mario.cur_pos.Y - MAP_MAX_Y_SIZE / 2;
    }

    return next_start_position.Y - G_map_info.start.Y;

    // if (G_map_info.size.Y <= MAP_MAX_Y_SIZE) {
    //     next_start_position.Y = 0;
    // }
    // else if (G_mario.cur_pos.Y - MAP_MAX_Y_SIZE / 2 < 0) {
    //     next_start_position.Y = 0;
    // }
    // else if (G_mario.cur_pos.Y + MAP_MAX_Y_SIZE / 2 >= G_map_info.size.Y) {
    //     next_start_position.Y = G_map_info.size.Y - MAP_MAX_Y_SIZE;
    // }
    // else {
    //     next_start_position.Y = G_mario.cur_pos.Y - MAP_MAX_Y_SIZE / 2;
    // }

    // return next_start_position.Y - G_map_info.start.Y;
}

void Scroll_Map_X(int xmove)
{
    int x = 0, y = 0, yend = MAP_Y_START + MAP_MAX_Y_SIZE, xend = MAP_X_START + 2 * MAP_MAX_X_SIZE;

    Set_Back_Color(G_map_info.map_color.r, G_map_info.map_color.g, G_map_info.map_color.b);

    if (xmove > 0) {
        for (y = MAP_Y_START; y < yend; y++) {
            printf("\x1b[%d;%dH\x1b[2P", y, MAP_X_START + 1);
            printf("\x1b[%d;%dH\x1b[2@", y, xend - 1);
            printf("%s", G_UNICODE[ G_game_map[G_map_info.start.Y + y - MAP_Y_START][G_map_info.start.X + MAP_MAX_X_SIZE - 1].Type ]);
        }
    }
    else {
        for (y = MAP_Y_START; y < yend; y++) {
            printf("\x1b[%d;%dH\x1b[2P", y, xend - 1);
            printf("\x1b[%d;%dH\x1b[2@", y, MAP_X_START + 1);
            printf("%s", G_UNICODE[ G_game_map[G_map_info.start.Y + y - MAP_Y_START][G_map_info.start.X].Type ]);
        }
    }
}

void Scroll_Map_Y(int ymove)
{
    int x = 0, y = 0, yend = MAP_Y_START + MAP_MAX_Y_SIZE, xend = MAP_X_START + 2 * MAP_MAX_X_SIZE;

    Set_Back_Color(G_map_info.map_color.r, G_map_info.map_color.g, G_map_info.map_color.b);

    if (ymove > 0) {
        for (x = MAP_X_START; x < xend; x++) {
            printf("\x1b[%d;%dH\x1b[2P", MAP_Y_START, x + 1);
            printf("\x1b[%d;%dH\x1b[2@", yend , x + 1);
            printf("%s", G_UNICODE[ G_game_map[G_map_info.start.Y + MAP_MAX_Y_SIZE - 1][G_map_info.start.X + x - MAP_X_START].Type ]);
        }
    }
    else {
        for (y = MAP_Y_START; y < yend; y++) {
            printf("\x1b[%d;%dH\x1b[2P", yend, x + 1);
            printf("\x1b[%d;%dH\x1b[2@", MAP_Y_START, x + 1);
            printf("%s", G_UNICODE[ G_game_map[G_map_info.start.Y][G_map_info.start.X + x - MAP_X_START].Type ]);
        }
    }
}

void Print_InGame_Health_Info()
{
    Set_Back_Color(0, 0, 0);
    Set_Fore_Color(72, 202, 228);

    printf("\x1b[%d;%dH%*c", MAP_Y_START - 2, MAP_X_START + 3, 14, ' ');

    printf("\x1b[%d;%dH%s ", MAP_Y_START - 2, MAP_X_START + 3, G_UNICODE[MARIO_TYPE]);

    for (int i = 0; i < G_mario.health; i++) {
        printf("%s", HEART_UN);
    }

    if (G_mario.shield == 1) {
        printf("%s", G_UNICODE[SHIELD_TYPE]);
    }
}

void Print_InGame_Death_Info()
{
    Set_Back_Color(0, 0, 0);
    Set_Fore_Color(72, 202, 228);

    printf("\x1b[%d;%dH%*c", MAP_Y_START - 2, MAP_X_START + 25, 8, ' ');

    printf("\x1b[%d;%dH%s ", MAP_Y_START - 2, MAP_X_START + 25, DEATH_UN);

    printf("%d", G_game_history.Death);
}

void Print_InGame_Coin_Info()
{
    Set_Back_Color(0, 0, 0);
    Set_Fore_Color(72, 202, 228);

    printf("\x1b[%d;%dH%*c", MAP_Y_START - 2, MAP_X_START + 33, 8, ' ');

    printf("\x1b[%d;%dH%s ", MAP_Y_START - 2, MAP_X_START + 33, G_UNICODE[COIN_TYPE]);

    printf("%d", G_game_history.Game_Coins);
}

void Print_InGame_Score_Info()
{
    Set_Back_Color(0, 0, 0);
    Set_Fore_Color(72, 202, 228);

    printf("\x1b[%d;%dH%*c", MAP_Y_START - 2, MAP_X_START + 41, 10, ' ');

    printf("\x1b[%d;%dH%s ", MAP_Y_START - 2, MAP_X_START + 41, SCORE_UN);

    printf("%d", G_game_history.Game_Score);
}

void Print_InGame_Time_Info()
{
    Set_Back_Color(0, 0, 0);
    Set_Fore_Color(72, 202, 228);

    printf("\x1b[%d;%dH%*c", MAP_Y_START - 2, MAP_X_START + 2 * MAP_MAX_X_SIZE - 7, 7, ' ');

    printf("\x1b[%d;%dH%s ", MAP_Y_START - 2, MAP_X_START + 2 * MAP_MAX_X_SIZE - 7, CLOCK_UN);

    printf("%d",G_map_info.map_tima_s - (int)G_game_history.Game_Time_S);
}

void Reload(const struct WINDOW *win, int xmax, int ymax)
{
    // int x = 0, y = 0, xc = win->scrol.X, yc = win->scrol.X, dot = 0;  // current position
    // int xend = xmax + win->scrol.X, yend = ymax + win->scrol.Y;   // end of window
    // int xstart = win->start.X - win->scrol.X, ystart = win->start.Y - win->scrol.Y;

    // // set background attr
    // Set_Back_Color(win->back_color.r, win->back_color.g, win->back_color.b);

    // clock_t t = clock();

    // // print screen
    // for (y = yc; y < yend; y++) {
    //     Gotoxy(xstart + 1, y + ystart);
    //     printf("%*c", 2 * xmax, ' ');
    //     for (x = xc; x < xend; x++) {
    //         if (win->screen[y][x].Type != '.') {
    //             Print_Un(2 * x + xstart + 1, y + ystart, win->screen[y][x].Type);
    //         }
    //     }
    // }

    int y = 0, x = 0, yend = MAP_MAX_Y_SIZE + win->start.Y, xend = MAP_MAX_X_SIZE + win->start.X;

    // set background attr
    Set_Back_Color(win->back_color.r, win->back_color.g, win->back_color.b);

    clock_t t = clock();

    // print screen
    for (y = win->start.Y; y < yend; y++) {
        printf("\x1b[%d;%dH", y, MAP_X_START + 1);
        for (x = win->start.X; x < xend; x++) {
            printf("%s", G_UNICODE[ win->screen[y - MAP_Y_START][x - MAP_X_START].Type ]);
        }
    }

    // for (y = yc; y < yend; y++) {
    //     for (x = xc; x < xend; x++) {
    //         if (win->screen[y][x - 1].val == '.') {
    //             dot = 0;

    //             while (win->screen[y][x - 1].val == '.' && x < xend) {
    //                 x++;
    //                 dot++;
    //             }

    //             printf("%*c", 2 * dot, ' ');

    //             if (x != xend) {
    //                 Print_Un(2 * (x + xstart) - 1, y + ystart, win->screen[y][x - 1].val);
    //             }
    //         } else {
    //             Print_Un(2 * (x + xstart) - 1, y + ystart, win->screen[y][x - 1].val);
    //         }
    //     }
    // }

    // print screen
    // for (y = yc; y < yend; y++) {
    //     for (x = xc; x < xend; x++) {
    //         //Print_Un(pos.X + win->start.X - win->scrol.X, pos.Y + win->start.Y - win->scrol.Y, *((*((win->screen) + pos.Y * win->size.X)) + pos.X));
    //         Print_Un(2 * (x + xstart) - 1, y + ystart, win->screen[y * xend + x - 1]);
    //     }
    // }

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;

#ifdef DEBUG
    //printf("\x1b[H\x1b[38;5;10m%lf", time_taken);
#endif

    // reset background color
    Reset_Color();
}


///////////////////////////////////////////////////


int Shop(struct ACCOUNT_USER_INFO *acc_user_info, struct PERSONAL_USER_INFO *per_user_info)
{
    struct WINDOW *map = Load_Map(LOAD_SHOP_MAP);
    struct MARIO_SKIN mario_skins[MARIO_SKIN_NUMBER] = {G_Mario_Skin, G_Frunk_Skin, G_Astra_Skin, G_Omen_Skin, G_Spike_Skin, G_King_MMD_Skin};

    // define fore_color
    struct COLOR fore_color = {3, 4, 94};

    // sign up
    while (TRUE) {
        // create menu list
        char *menu_names[2] = {"    Mario Skin    ", "      Theme       "};
        struct MENU menu;
        menu.number = 2;
        menu.active = 0;
        menu.name = menu_names;

        // reload map
        Reload(map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);
        Print_Account_Info(acc_user_info, per_user_info, map->back_color, fore_color);

        // menu function
        int cmd = Menu(&menu, DISPLAY_X_SIZE / 2, MAP_Y_START + 3, map->back_color, fore_color, 1);

        switch (cmd) {
        case 0:
            Reload(map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);
            Mario_Skin_Shop(mario_skins, MARIO_SKIN_NUMBER, DISPLAY_X_SIZE / 2 - 10, MAP_Y_START + 3, map->back_color, fore_color, acc_user_info, per_user_info);
            break;
        case 1:

            break;
        default:
            return SUCCESSFUL;
            break;
        }

    }
}

int Mario_Skin_Shop(struct MARIO_SKIN *mario_skin, int skin_number, int xmiddle, int ystart, struct COLOR back_color, struct COLOR fore_color, struct ACCOUNT_USER_INFO *acc_user_info, struct PERSONAL_USER_INFO *per_user_info)
{
    char ch = 0;
    int active = 0, pa = 0, len = 0, maxlen = 16,box_xsize = 25, box_ysize = 9, box_xstart = xmiddle + 12, box_ystart = ystart;
    // set default color
    Set_Back_Color(back_color.r, back_color.g, back_color.b);
    Set_Fore_Color(fore_color.r, fore_color.g, fore_color.b);

    // print skins
    for (int i = 0; i < skin_number; i++) {
        // set cursor position
        len = strlen(mario_skin[i].name);


        if (i == active) {
            Gotoxy(xmiddle - maxlen / 2, ystart + i);
            printf("%*c", maxlen, ' ');
            Gotoxy(xmiddle - len / 2, ystart + i);
            // set active color
            Set_Fore_Color(back_color.r, back_color.g, back_color.b);
            Set_Back_Color(fore_color.r, fore_color.g, fore_color.b);

            // print
            printf("%s", mario_skin[i].name);

            // set default color
            Set_Back_Color(back_color.r, back_color.g, back_color.b);
            Set_Fore_Color(fore_color.r, fore_color.g, fore_color.b);
        }
        else {
            Gotoxy(xmiddle - maxlen / 2, ystart + i);
            printf("%*c", maxlen, ' ');
            Gotoxy(xmiddle - len / 2, ystart + i);
            // print
            printf("%s", mario_skin[i].name);
        }
    }

    // choose skin
    while (TRUE) {
        Print_Account_Info(acc_user_info, per_user_info, back_color, fore_color);
        // set cursor position
        len = strlen(mario_skin[pa].name);

        // set color
        Set_Back_Color(back_color.r, back_color.g, back_color.b);
        Set_Fore_Color(fore_color.r, fore_color.g, fore_color.b);

        Gotoxy(xmiddle - maxlen / 2, ystart + pa);
        printf("%*c", maxlen, ' ');

        Gotoxy(xmiddle - len / 2, ystart + pa);

        // print
        printf("%s", mario_skin[pa].name);

        // set color
        Set_Fore_Color(back_color.r, back_color.g, back_color.b);
        Set_Back_Color(fore_color.r, fore_color.g, fore_color.b);

        // reset info
        for (int i = box_ystart; i < box_ystart + box_ysize; i++) {
            Gotoxy(box_xstart, i);
            printf("%*c", box_xsize, ' ');
        }

        // set cursor position
        len = strlen(mario_skin[active].name);
        Gotoxy(xmiddle - len / 2, ystart + active);

        // print
        Gotoxy(xmiddle - maxlen / 2, ystart + active);
        printf("%*c", maxlen, ' ');
        Gotoxy(xmiddle - len / 2, ystart + active);
        printf("%s", mario_skin[active].name);

        // print info
        Gotoxy(box_xstart + 2, box_ystart + 1);
        printf("%s %s", mario_skin[active].skin, mario_skin[active].name);

        Gotoxy(box_xstart + 2, box_ystart + 3);
        printf("Health : ");
        for (int i = 0; i < mario_skin[active].health; i++) {
            printf(HEART_UN);
        }

        Gotoxy(box_xstart + 2, box_ystart + 4);
        printf("Speed : %d", (int)(2 / mario_skin[active].speedX));

        Gotoxy(box_xstart + 2, box_ystart + 5);
        printf("Jump Height : %d", mario_skin[active].jump_height);

        Gotoxy(box_xstart + 2, box_ystart + 7);
        printf("Price : %d", mario_skin[active].price);

        ch = getch();

        switch (ch) {
        case ESC_KEY:
            if ((ch = getch()) == 72) {
                pa = active;
                if (active > 0) {
                    active--;
                }
                else {
                    active = skin_number - 1;
                }
            }
            else if (ch == 80) {
                pa = active;
                if (active < skin_number - 1) {
                    active++;
                }
                else {
                    active = 0;
                }
            }
            break;
        case 13:
            if (Buy_Skin(acc_user_info, mario_skin[active]) == -1) {
                Set_Back_Color(back_color.r, back_color.g, back_color.b);
                Set_Fore_Color(214, 34, 70);
                Gotoxy(box_xstart, box_ystart + box_ysize + 1);
                printf("%*c", box_xsize, ' ');
                Gotoxy(box_xstart + box_xsize / 2 - strlen("You have it!") / 2, box_ystart + box_ysize + 1);
                printf("You have it!");
            }
            else if (Buy_Skin(acc_user_info, mario_skin[active]) == -2) {
                Set_Back_Color(back_color.r, back_color.g, back_color.b);
                Set_Fore_Color(214, 34, 70);
                Gotoxy(box_xstart, box_ystart + box_ysize + 1);
                printf("%*c", box_xsize, ' ');
                Gotoxy(box_xstart + box_xsize / 2 - strlen("Money is not enough") / 2, box_ystart + box_ysize + 1);
                printf("Money is not enough");
            }
            else {
                Set_Back_Color(back_color.r, back_color.g, back_color.b);
                Set_Fore_Color(214, 34, 70);
                Gotoxy(box_xstart, box_ystart + box_ysize + 1);
                printf("%*c", box_xsize, ' ');
                Gotoxy(box_xstart + box_xsize / 2 - strlen("You buy it") / 2, box_ystart + box_ysize + 1);
                printf("You buy it");
            }
            break;
        case 27:
            return EXIT;
        default:
            break;
        }
    }

    Reset_Color();
}

int Select_Mario_skin(int xmiddle, int ystart, struct COLOR back_color, struct COLOR fore_color, struct ACCOUNT_USER_INFO *acc_user_info)
{
    struct MARIO_SKIN mario_skin[MARIO_SKIN_NUMBER] = {G_Mario_Skin, G_Frunk_Skin, G_Astra_Skin, G_Omen_Skin, G_Spike_Skin, G_King_MMD_Skin};

    struct ACCOUNT_ITEM_INFO *acc_items = Get_User_Item_List(acc_user_info->User_ID);
    char ch = 0;
    int active = 0, pa = 0, len = 0, maxlen = 16,box_xsize = 25, box_ysize = 9, box_xstart = xmiddle + 12, box_ystart = ystart;
    int skin_number = MARIO_SKIN_NUMBER;

    for (int i = 0; i < acc_user_info->item_number; i++) {

    }

    // set default color
    Set_Back_Color(back_color.r, back_color.g, back_color.b);
    Set_Fore_Color(fore_color.r, fore_color.g, fore_color.b);

    // print skins
    for (int i = 0; i < skin_number; i++) {
        // set cursor position
        len = strlen(mario_skin[i].name);


        if (i == active) {
            Gotoxy(xmiddle - maxlen / 2, ystart + i);
            printf("%*c", maxlen, ' ');
            Gotoxy(xmiddle - len / 2, ystart + i);
            // set active color
            Set_Fore_Color(back_color.r, back_color.g, back_color.b);
            Set_Back_Color(fore_color.r, fore_color.g, fore_color.b);

            // print
            printf("%s", mario_skin[i].name);

            // set default color
            Set_Back_Color(back_color.r, back_color.g, back_color.b);
            Set_Fore_Color(fore_color.r, fore_color.g, fore_color.b);
        }
        else {
            Gotoxy(xmiddle - maxlen / 2, ystart + i);
            printf("%*c", maxlen, ' ');
            Gotoxy(xmiddle - len / 2, ystart + i);
            // print
            printf("%s", mario_skin[i].name);
        }
    }

    // choose skin
    while (TRUE) {
        // set cursor position
        len = strlen(mario_skin[pa].name);

        // set color
        Set_Back_Color(back_color.r, back_color.g, back_color.b);
        Set_Fore_Color(fore_color.r, fore_color.g, fore_color.b);

        Gotoxy(xmiddle - maxlen / 2, ystart + pa);
        printf("%*c", maxlen, ' ');

        Gotoxy(xmiddle - len / 2, ystart + pa);

        // print
        printf("%s", mario_skin[pa].name);

        // set color
        Set_Fore_Color(back_color.r, back_color.g, back_color.b);
        Set_Back_Color(fore_color.r, fore_color.g, fore_color.b);

        // reset info
        for (int i = box_ystart; i < box_ystart + box_ysize; i++) {
            Gotoxy(box_xstart, i);
            printf("%*c", box_xsize, ' ');
        }

        // set cursor position
        len = strlen(mario_skin[active].name);
        Gotoxy(xmiddle - len / 2, ystart + active);

        // print
        Gotoxy(xmiddle - maxlen / 2, ystart + active);
        printf("%*c", maxlen, ' ');
        Gotoxy(xmiddle - len / 2, ystart + active);
        printf("%s", mario_skin[active].name);

        // print info
        Gotoxy(box_xstart + 2, box_ystart + 1);
        printf("%s %s", mario_skin[active].skin, mario_skin[active].name);

        Gotoxy(box_xstart + 2, box_ystart + 3);
        printf("Health : ");
        for (int i = 0; i < mario_skin[active].health; i++) {
            printf(HEART_UN);
        }

        Gotoxy(box_xstart + 2, box_ystart + 4);
        printf("Speed : %d", (int)(2 / mario_skin[active].speedX));

        Gotoxy(box_xstart + 2, box_ystart + 5);
        printf("Jump Height : %d", mario_skin[active].jump_height);

        Gotoxy(box_xstart + 2, box_ystart + 7);
        printf("Price : %d", mario_skin[active].price);

        ch = getch();

        switch (ch) {
        case ESC_KEY:
            if ((ch = getch()) == 72) {
                pa = active;
                if (active > 0) {
                    active--;
                }
                else {
                    active = skin_number - 1;
                }
            }
            else if (ch == 80) {
                pa = active;
                if (active < skin_number - 1) {
                    active++;
                }
                else {
                    active = 0;
                }
            }
            break;
        case 13:
            for (int i = 0; i < acc_user_info->item_number; i++) {
                if (acc_items[i].Item_ID == mario_skin[active].ID) {
                    return acc_items[i].Item_ID;
                }
            }

            Set_Back_Color(back_color.r, back_color.g, back_color.b);
            Set_Fore_Color(214, 34, 70);
            Gotoxy(box_xstart, box_ystart + box_ysize + 1);
            printf("%*c", box_xsize, ' ');
            Gotoxy(box_xstart + box_xsize / 2 - strlen("You dont have it!") / 2, box_ystart + box_ysize + 1);
            printf("You dont have it!");

            break;
        case 27:
            return EXIT;
        default:
            break;
        }
    }

    Reset_Color();
}

int Buy_Skin(struct ACCOUNT_USER_INFO *acc_user_info, struct MARIO_SKIN mario_skin)
{
    struct ACCOUNT_ITEM_INFO *acc_items = Get_User_Item_List(acc_user_info->User_ID);

    for (int i = 0; i < acc_user_info->item_number; i++) {
        if (acc_items[i].Item_ID == mario_skin.ID) {
            return -1;
        }
    }

    if (mario_skin.price <= acc_user_info->Coins) {
        acc_user_info->Coins -= mario_skin.price;

        acc_user_info->item_number++;
        Do_Add_Item(mario_skin.ID, acc_user_info->User_ID);

        Do_Update_user_id_Info(acc_user_info);
        return SUCCESSFUL;
    }

    return -2;
}

void Print_Account_Info(struct ACCOUNT_USER_INFO *acc_user_info, struct PERSONAL_USER_INFO *per_user_info, struct COLOR back_color, struct COLOR fore_color)
{
    Reset_Color();
    Set_Fore_Color(back_color.r, back_color.g, back_color.b);
    Set_Back_Color(0, 0, 0);

    Gotoxy(MAP_X_START, MAP_Y_START - 2);
    printf("%*c", MAP_MAX_X_SIZE * 2, ' ');

    Gotoxy(MAP_X_START + 2 * MAP_MAX_X_SIZE - 25, MAP_Y_START - 2);
    printf(MARIO_UN " %s", per_user_info->NickName);

    Gotoxy(MAP_X_START + 6, MAP_Y_START - 2);
    printf(COIN_UN " %d", acc_user_info->Coins);

    Gotoxy(MAP_X_START + 16, MAP_Y_START - 2);
    printf(SCORE_UN " %d", acc_user_info->Score);

    Reset_Color();
}

void Loader_Page()
{
    struct WINDOW *map = Load_Map(LOAD_LOADING_MAP);

    // define fore_color
    struct COLOR fore_color = {3, 4, 94};

    // reload map
    Reload(map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);

    Set_Back_Color(map->back_color.r, map->back_color.g, map->back_color.b);
    Set_Fore_Color(fore_color.r, fore_color.g, fore_color.b);

    Gotoxy(DISPLAY_X_SIZE / 2 - strlen("[Press Any Key]") / 2, MAP_Y_START + 10);
    printf("[Press Any Key]");
    Gotoxy(DISPLAY_X_SIZE / 2 - strlen("to continue") / 2, MAP_Y_START + 11);
    printf("to continue");


    getch();

    Reset_Color();
}












