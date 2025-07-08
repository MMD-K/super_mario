#ifndef FILE_MANEGEMENT_H
#define FILE_MANEGEMENT_H

#endif // FILE_MANEGEMENT_H


#include <stdio.h>
#include <time.h>
// #include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>

//////////////////////////////// Define ////////////////////////////////////////

#define ADDRESS_SIZE 500

//char G_location[ADDRESS_SIZE];
// #define INSTALLATION_ADDRESS "C:\\games\\Mario_Game\\"
// #define GAMEDATA_ADDRESS "gamedata\\"
// #define USERDATA_ADDRESS "usersdata\\"

#define INSTALLATION_ADDRESS "./data/"
#define GAMEDATA_ADDRESS "gamedata/"
#define USERDATA_ADDRESS "usersdata/"

#define LOGIN_MAP_NAME "login_map.dat"
#define MAIN_MENU_MAP_NAME "main_menu_map.dat"
#define HISTORY_MAP_NAME "history_map.dat"
#define SHOP_MAP_NAME "shop_map.dat"
#define GAME_MAP_1_NAME "map_1.dat"
#define GAME_MAP_2_NAME "map_2.dat"
#define LOADING_MAP_NAME "loading_map.dat"


#define ALLUSER_FILE_NAME "alluser.dat"
#define HISTORY_FILE_NAME "h"
#define USER_FILE_FORMAT ".dat"

#define GUEST_NAME "Guest_"


#define FILE_NAME_SIZE 20
#define USERNAME_SIZE 20
#define NICKNAME_SIZE 20
#define EMAIL_SIZE 30
#define PASSWORD_SIZE 20

enum ERROR_TYPES {
    SUCCESSFUL = 1,
    ERROR = -1,
    ERROR_OPENING_FILE = -2,
    ERROR_REMOVING_FILE = -3,
    ERROR_ADDING_ACCOUNT = -4
};

enum LOAD_FILE_STATUS {
    LOAD_ALLUSERS = 0,
    LOAD_USER_ID = 1,
    LOAD_H_USER_ID = 2
};

enum LOAD_MAP_STATUS {
    LOAD_GAME_MAP_1 = 0,
    LOAD_GAME_MAP_2 = 1,
    LOAD_LOGIN_MAP = 10,
    LOAD_MAIN_MENU_MAP = 11,
    LOAD_HISTORY_MAP = 12,
    LOAD_SHOP_MAP = 13,
    LOAD_LOADING_MAP = 14
};

enum CHANGE_USER_INFO_STATUS {
    CHANGE_USERNAME = 0,
    CHANGE_NICKNAME = 1,
    CHANGE_EMAIL = 2,
    CHANGE_PASSWORD = 3
};

enum WIN_STATUS {
    GAME_LOSE = 0,
    GAME_WIN = 1
};

enum SEARCH_STATUS {
    SEARCH_BY_USERNAME = 0,
    SEARCH_BY_EMAIL = 1,
    SEARCH_BY_USER_ID = 2
};

//////////////////////////////// Structures ////////////////////////////////////

// allusers.dat
struct ALL_USER_IFNO{
    int Number_Of_Users;
    int Last_ID;
    char Saved_UserName[USERNAME_SIZE];
    char Saved_Password[PASSWORD_SIZE];
};

struct PERSONAL_USER_INFO {
    int User_ID;
    char UserName[USERNAME_SIZE];
    char NickName[NICKNAME_SIZE];
    char Email[EMAIL_SIZE];
    char Password[PASSWORD_SIZE];
};

// userID.dat
struct ACCOUNT_USER_INFO {
    int User_ID;
    int Coins;
    long long int Score;
    int item_number;
};

struct ACCOUNT_ITEM_INFO {
    int Item_ID;
};

// H_userID.dat
struct ACCOUNT_FILE_HISTORY {
    int User_ID;
    struct tm Time_SignUp;
    int Time_Play_S;
    int Games_Played;
    int Games_Win;
    int Best_Score;
};

struct ACCOUNT_GAME_HISTORY {
    int Map_ID;
    struct tm Time_Played;
    enum WIN_STATUS win_status;
    int Death;
    int Repeat;
    int Game_Score;
    int Game_Coins;
    float Game_Time_S;
};

//////////////////////////////// USER Functions ////////////////////////////////

int create_dir(const char *path);
bool directory_exists(const char *path);
int Do_Load_File(enum LOAD_FILE_STATUS load_file_status, FILE **file, char *file_name);
long long int Do_User_Finder(enum SEARCH_STATUS search_status, void *search_info);
int Do_Add_Account(struct PERSONAL_USER_INFO *personal_user_info);
int Do_Delete_Account(int user_id);
int Do_Change_User_Info(enum CHANGE_USER_INFO_STATUS change_status, char *change_info, long long int Account_Indicator);
int Do_Add_Item(int Item_ID, int user_id);
int Do_Add_History(struct ACCOUNT_GAME_HISTORY *game_history, int user_id);
int Do_Update_Alluser_Info(struct ALL_USER_IFNO *new_alluser_info);
int Do_Update_user_id_Info(struct ACCOUNT_USER_INFO *new_user_info);
int Do_Update_History_Info(struct ACCOUNT_FILE_HISTORY *new_history_info);

struct ALL_USER_IFNO Get_Alluser_Info();
struct PERSONAL_USER_INFO Get_Personal_User_Info(long long int indicattor);
struct ACCOUNT_USER_INFO Get_Account_info(int user_id);
struct ACCOUNT_ITEM_INFO *Get_User_Item_List(int user_id);
struct ACCOUNT_FILE_HISTORY Get_Histroy_Info(int user_id);
struct ACCOUNT_GAME_HISTORY *Get_Game_History_List(int user_id);

FILE *Do_Load_Map(enum LOAD_MAP_STATUS load_map_status);





