#ifndef MARIO_GAME_H
#define MARIO_GAME_H

#endif // MARIO_GAME_H


#include "file_manegement.h"
#include "console_output.h"

//////////////////////////////// Define ////////////////////////////////////////

// #define DEBUG

#define ESC_KEY -32
#define MIN_PASSWORD_LEN 8

#define MAP_X_START 10
#define MAP_Y_START 8
#define MAP_MAX_X_SIZE 50
#define MAP_MAX_Y_SIZE 15

#define MAX_HIS_NUMBER 9

#define MAP_NUMBER 2
#define MARIO_SKIN_NUMBER 6

#define KILL_SCORE 100

#define MAX_SKIN_SIZE 8

#define ITEM_NUMBER 17

#define SUBMIT_BUTTON " SUBMIT "

#define SPACE_TYPE 0
#define MARIO_TYPE 1
#define BLOCK_INDIS_TYPE 2
#define BLOCK_DIS_TYPE 3
#define BLOCK_WHITE_TYPE 4
#define BLOCK_GREEN_TYPE 5
#define COIN_TYPE 6
#define COIN_BAG_TYPE 7
#define SHIELD_TYPE 8
#define FLAG_TYPE 9
#define BLOCK_COIN_TYPE 10
#define BLOCK_MUSHROOM_TYPE 11
#define MUSHROOM_TYPE 12
#define CACTUS_TYPE 13
#define FLOWER_TYPE 14
#define SOUL_TYPE 15
#define TELEPORT_TYPE 16

#define SPACE_CHAR '.'
#define MARIO_CHAR '@'
#define BLOCK_INDIS_CHAR '#'
#define BLOCK_DIS_CHAR 'd'
#define BLOCK_WHITE_CHAR '+'
#define BLOCK_GREEN_CHAR '-'
#define COIN_CHAR '*'
#define COIN_BAG_CHAR '$'
#define SHIELD_CHAR 's'
#define FLAG_CHAR 'f'
#define BLOCK_COIN_CHAR 'b'
#define BLOCK_MUSHROOM_CHAR '&'
#define MUSHROOM_CHAR 'm'
#define CACTUS_CHAR 'c'
#define FLOWER_CHAR 'w'
#define SOUL_CHAR 'e'

#define SPACE_UN "  "
#define MARIO_UN "🦸"
#define BLOCK_INDIS_UN "🧱"
#define BLOCK_DIS_UN "🟧"
#define BLOCK_WHITE_UN "▉▉"
#define BLOCK_GREEN_UN "▉▉"
#define COIN_UN "🪙"
#define COIN_BAG_UN "💰"
#define SHIELD_UN "🛡️"
#define FLAG_UN "🚩"
#define BLOCK_COIN_UN "🟨"
#define BLOCK_MUSHROOM_UN "🟨"
#define MUSHROOM_UN "🍄"
#define CACTUS_UN "🌵"
#define FLOWER_UN "🌱"
#define SOUL_UN "🐙"
#define TELEPORT_UN "  "

#define HEART_UN "❤️"
#define SCORE_UN "💠"
#define CLOCK_UN "⏱️"
#define DEATH_UN "💀"
#define REPEAT_UN "♻️"
#define GAME_UN "🎮"
#define GAME_WIN_UN "🏅"
#define BEST_SCORE_UN "🏆"
#define MAP_ICON_UN "🗺️"
#define SPEED_UN "👟"

#define SPACE_UN_2 "  "
#define MARIO_UN_2 "🦸"
#define BLOCK_INDIS_UN_2 "🧱"
#define BLOCK_DIS_UN_2 "🧊"
#define BLOCK_WHITE_UN_2 "▉▉"
#define BLOCK_GREEN_UN_2 "▉▉"
#define COIN_UN_2 "💵"
#define COIN_BAG_UN_2 "💴"
#define SHIELD_UN_2 "🛡️"
#define FLAG_UN_2 "🚩"
#define BLOCK_COIN_UN_2 "🟨"
#define BLOCK_MUSHROOM_UN_2 "🟨"
#define MUSHROOM_UN_2 "🍗"
#define CACTUS_UN_2 "❄️"
#define FLOWER_UN_2 "🔥"
#define SOUL_UN_2 "⛄"
#define TELEPORT_UN_2 "  "

enum VERIFICATION_ERROR {
    VERIFIED = 1,
    LEN_ERROR = -1,
    CHAR_ERROR = -2,
    REP_ERROR = -3,
    EMAIL_ERROR = -4,
    PASS_LEN_ERROR = -5
};

enum SIGN_ERROR {
    SUCCESFUL = 1,
    EXIT = -1,
    SIGNIN_ERROR = -2,
    USERNAME_NOT_FOUND = -3,
    WRONG_PASSWORD = -4
};

enum JUMP_STATUS {
    WALK = 0,
    JUMP = 1,
    FALL = 2
};

enum DIRECTION {
    STOP = 0,
    RIGHT = 1,
    LEFT = 2,
    DOWN = 3,
    UP = 4
};

enum GAME_STATUS {
    END_GAME = 0,
    IN_GAME = 1,
    GAME_PAUSE = 2
};

//////////////////////////////// Structures ////////////////////////////////////

struct MAP {
    COORD start;
    COORD size;
    struct COLOR map_color;
    int map_tima_s;
    char active_area;
};

struct MAP_AREA {
    COORD size;
    COORD start;
    COORD end;
};

struct MENU {
    int number;
    char **name;
    int active;
};

struct INPUT_LIST {
    int number;
    char **name;
    int active;
};

struct SPACE {  // type 0
    char pass;
};

struct MARIO {  // type 1
    char skin[MAX_SKIN_SIZE];
    COORD cur_pos;
    COORD next_pos;
    char health;
    char shield;
    enum JUMP_STATUS jump_status;
    char jump_height;
    char jump_step;
    float speedY;
    float TY;     // last y position update
    enum DIRECTION dir;
    float speedX;
    float TX;     // last X position update
    float speedk;
    float TK;     // last kill time
    char kill_bonus;
};

struct BLOCK_INDIS {    // type 2
    char pass;
};

struct BLOCK_DIS {   // type 3
    char pass;
};

struct BLOCK_WHITE {    // type 4
    char pass;
};

struct BLOCK_GREEN {    // type 5
    char pass;
};

struct COIN {   // type 6
    char pass;
    char val;
};

struct COIN_BAG {   // type 7
    char pass;
    char val;
};

struct SHIELD {     // type 8
    char pass;
};

struct FLAG {   // type 9
    char pass;
};

////// need id
struct BLOCK_COIN {     // type 10
    char pass;
    char health;
    char val;
};

struct BLOCK_MUSHROOM {     // type 11
    char pass;
};

////// need id
struct MUSHROOM {    // type 12
    char pass;
    char health;
    COORD pos;
    enum DIRECTION dir;
    float speedY;
    float TY;     // last y position update
    float speedX;
    float TX;     // last x position update
    char val;
};

////// need id
struct CACTUS {     // type 13
    char pass;
    char damage;
    float speedD;
    float TD;      // last damage time
};

////// need id
struct FLOWER {     // type 14
    char pass;
    COORD pos;
    char status;
    float speedY;
    float TY;       // last y update
    char damage;
};

////// need id
struct SOUL {   // type 15
    char pass;
    char health;
    enum DIRECTION dir;
    COORD pos;
    float speedX;
    float TX;
    char damage;
    float speedD;
    float TD;       // last damage time
};

////// need id
struct TELEPORT {   // type 16
    char pass;
    COORD pos_end;
};

////////////////////////// skin struct /////////////////////////

struct MARIO_SKIN {
    int ID;
    char name[NICKNAME_SIZE];
    char skin[MAX_SKIN_SIZE];
    char health;
    char jump_height;
    float speedX;
    int price;
};

//////////////////////////////// Main Functions ////////////////////////////////

void Start_Game();
int Main_Game();
int End_Game();

//////////////////////////////// Menu Functions ////////////////////////////////

int Menu(struct MENU *menu, int xmiddle, int ystart, struct COLOR back_color, struct COLOR fore_color, char esc_check);

int LogIn(long long int *acc_indicator);
void Logout(int user_id, struct PERSONAL_USER_INFO *per_user_info, int start_time);
int Account_Management(struct PERSONAL_USER_INFO *per_user_inf, long long int acc_indicator, struct ACCOUNT_USER_INFO *acc_user_info);
int Change_User_Info(enum CHANGE_USER_INFO_STATUS change_status, char *change_info, long long int acc_indicator);

int Select_Map();
int Select_Player(struct ACCOUNT_USER_INFO *acc_user_info);

void Load_Account(long long int acc_indicator,struct PERSONAL_USER_INFO *per_user_info, struct ACCOUNT_USER_INFO *acc_user_info);

void Exit(int user_id, int start_time);

//////////////////////////////// verification Functions ////////////////////////

int Verification_Username(char *username);
int Verification_Email(char *email);
int Verification_Passwors(char *password);
void Print_Error();

//////////////////////////////// Load Functions ////////////////////////////////

struct WINDOW *Load_Map(enum LOAD_MAP_STATUS load_map_status);

//////////////////////////////// Input Function ////////////////////////////////

int Sign_In(long long int *acc_indicator);
int Sign_Up();
int Sign_Up_Guest(struct PERSONAL_USER_INFO *per_user_info);
int Forgot_Password();
int Guest(long long int *acc_indicator);

int Input_List(struct INPUT_LIST *input_list, int *input_size, int error,int xmiddle, int ystart, struct COLOR back_color, struct COLOR fore_color, int check_pass);

/////////////////////////////// Game ////////////////////////////////////////////

void Game(int user_id, struct ACCOUNT_USER_INFO *acc_user_info);
struct ACCOUNT_GAME_HISTORY In_Game();
void *Game_Process(void *arg);
void *Game_Input(void *arg);

void Load_Game(int selected_map, int selected_skin);
void Load_Map_Game(FILE *map_file);
void UnLoad_Game(int user_id, struct ACCOUNT_USER_INFO *acc_user_info);

int Set_X_Start_Position();
int Set_Y_Start_Position();
void Scroll_Map_X(int xmove);
void Scroll_Map_Y(int ymove);
void Set_Map_Area();

int Select_Mario_skin(int xmiddle, int ystart, struct COLOR back_color, struct COLOR fore_color, struct ACCOUNT_USER_INFO *acc_user_info);

void Print_InGame_Health_Info();
void Print_InGame_Coin_Info();
void Print_InGame_Score_Info();
void Print_InGame_Time_Info();
void Print_InGame_Death_Info();

char Pause_Game();
char End_In_Game();

void Make_It_Free();

/////////////////////////////// Item Loader //////////////////////////////////////

char Space_Loader(COORD pos);   // type 0
char Mario_Loader(COORD pos);   // type 1
char Block_Indis_Loader(COORD pos);   // type 2
char Block_dis_Loader(COORD pos);   // type 3
char Block_White_Loader(COORD pos);   // type 4
char Block_Green_Loader(COORD pos);   // type 5
char Coin_Loader(COORD pos);   // type 6
char Coin_Bag_Loader(COORD pos);   // type 7
char Shield_Loader(COORD pos);   // type 8
char Flag_Loader(COORD pos);   // type 9
char Block_Coin_Loader(COORD pos);   // type 10
char Block_Mushroom_Loader(COORD pos);   // type 11
char Mushroom_Loader(COORD pos);   // type 12
char Cactus_Loader(COORD pos);   // type 13
char Flower_Loader(COORD pos);   // type 14
char Soul_Loader(COORD pos);   // type 15
char Teleport_Loader(COORD pos);   // type 16

////////////////////////////// Item Updeter ///////////////////////////////////////

char Mario_X_Updater();   // type 1
char Mario_Y_Updater();   // type 1
void Mushroom_Updater();   // type 12
void Flower_Updater();   // type 14
void Soul_Updater();   // type 15

////////////////////////////// Item Hit ///////////////////////////////////////////

void Space_Hit();   // type 0
void Block_Indis_Hit();   // type 2
void Block_dis_Hit();   // type 3
void Block_White_Hit();   // type 4
void Block_Green_Hit();   // type 5
void Coin_Hit();   // type 6
void Coin_Bag_Hit();   // type 7
void Shield_Hit();   // type 8
void Flag_Hit();   // type 9
void Block_Coin_Hit();   // type 10
void Block_Mushroom_Hit();   // type 11
void Mushroom_Hit();   // type 12
void Cactus_Hit();   // type 13
void Flower_Hit();   // type 14
void Soul_Hit();   // type 15
void Teleport_Hit();   // type 16

////////////////////////////////////// main menu functions /////////////////////////////////

void Show_History(int user_id, struct ACCOUNT_USER_INFO acc_user_info);
int Delete_Account(int user_id);
int Shop(struct ACCOUNT_USER_INFO *acc_user_info, struct PERSONAL_USER_INFO *per_user_info);
int Mario_Skin_Shop(struct MARIO_SKIN *mario_skin, int skin_number, int xmiddle, int ystart, struct COLOR back_color, struct COLOR fore_color, struct ACCOUNT_USER_INFO *acc_user_info, struct PERSONAL_USER_INFO *per_user_info);
int Buy_Skin(struct ACCOUNT_USER_INFO *acc_user_info, struct MARIO_SKIN mario_skin);
void Print_Account_Info(struct ACCOUNT_USER_INFO *acc_user_info, struct PERSONAL_USER_INFO *per_user_info, struct COLOR back_color, struct COLOR fore_color);
void Creator();
void Setting();

void Loader_Page();

/*

. -> "  "                           -> 0
@ -> 🦸 🦹 👮 🧙 🧝 🤴            -> 1
# -> 🧱                             -> 2
d -> 🟧                             -> 3
+ -> ▉ white                        -> 4
- -> ▉ green                        -> 5
* -> 🪙                             -> 6
$ -> 💰                             -> 7
s -> 🛡️                             -> 8
f -> 🚩                             -> 9
b -> 🟨 coin                        -> 10
& -> 🟨 mushroom                    -> 11
m -> 🍄                             -> 12
c -> 🌵                             -> 13
w -> 🌱                             -> 14
e -> 🐙                             -> 15
number -> invisible


*/







