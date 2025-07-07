#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include "file_manegement.h"

//////////////////////////////// Define ////////////////////////////////////////



//////////////////////////////// USER Functions ////////////////////////////////

int Do_Load_File(enum LOAD_FILE_STATUS load_file_status, FILE **file, char *file_name)
{
    char file_address[ADDRESS_SIZE] = {};

    strcpy(file_address, INSTALLATION_ADDRESS);

    // create file address
    switch (load_file_status) {
    case LOAD_ALLUSERS:
        strcat(file_address, USERDATA_ADDRESS);
        strcat(file_address, file_name);
        break;
    case LOAD_USER_ID:
        strcat(file_address, USERDATA_ADDRESS);
        strcat(file_address, file_name);
        break;
    case LOAD_H_USER_ID:
        strcat(file_address, USERDATA_ADDRESS);
        strcat(file_address, file_name);
        break;
    default:
        break;
    }

    // open file address
    if (((*file) = fopen(file_address, "r+b")) == NULL) {


        // create file
        if (((*file) = fopen(file_address, "w+b")) == NULL) {
            return ERROR_OPENING_FILE;
        }

        // for first run
        if (load_file_status == LOAD_ALLUSERS) {
            struct ALL_USER_IFNO alluser_info = {0, 0, 0, 0};

            // write alluser for first time
            fseek(*file, 0, SEEK_SET);
            fwrite(&alluser_info, sizeof(struct ALL_USER_IFNO), 1, *file);
            fseek(*file, 0, SEEK_SET);
        }
    }

    return SUCCESSFUL;
}

long long int Do_User_Finder(enum SEARCH_STATUS search_status, void *search_info)
{
    FILE *file = NULL;
    struct PERSONAL_USER_INFO personal_user_info;
    int struct_pui_size = sizeof(struct PERSONAL_USER_INFO), acc_indicator = -1;

    // open alluser file
    if (Do_Load_File(LOAD_ALLUSERS, &file, ALLUSER_FILE_NAME) == ERROR_OPENING_FILE) {
        return ERROR_OPENING_FILE;
    }

    fseek(file, sizeof(struct ALL_USER_IFNO), SEEK_SET);

    // finding according to search status
    switch (search_status) {
    case SEARCH_BY_USERNAME:
        while (fread(&personal_user_info, struct_pui_size, 1, file) > 0) {
            if (strcmp(personal_user_info.UserName, (char *)search_info) == 0) {
                fseek(file, -struct_pui_size, SEEK_CUR);
                acc_indicator = ftell(file);
                break;
            }
        }
        break;
    case SEARCH_BY_EMAIL:
        while (fread(&personal_user_info, struct_pui_size, 1, file) > 0) {
            if (strcmp(personal_user_info.Email, (char *)search_info) == 0) {
                fseek(file, -struct_pui_size, SEEK_CUR);
                acc_indicator = ftell(file);
                break;
            }
        }
        break;
    case SEARCH_BY_USER_ID:
        while (fread(&personal_user_info, struct_pui_size, 1, file) > 0) {
            if (personal_user_info.User_ID == *((int *)search_info)) {
                fseek(file, -struct_pui_size, SEEK_CUR);
                acc_indicator = ftell(file);
                break;
            }
        }
        break;
    default:
        break;
    }

    fclose(file);

    return acc_indicator;
}

int Do_Add_Account(struct PERSONAL_USER_INFO *personal_user_info)
{
    char file_name[FILE_NAME_SIZE] = {};
    FILE *file = NULL;
    struct ACCOUNT_FILE_HISTORY account_file_history = {personal_user_info->User_ID, 0, 0, 0, 0};
    struct ACCOUNT_USER_INFO account_user_info = {personal_user_info->User_ID, 0, 0, 0};
    time_t cur_time = 0;

    // open alluser file
    if (Do_Load_File(LOAD_ALLUSERS, &file, ALLUSER_FILE_NAME) == ERROR_OPENING_FILE) {
        return ERROR_OPENING_FILE;
    }

    // write personal user info in alluser file
    fseek(file, 0, SEEK_END);
    fwrite(personal_user_info, sizeof(struct PERSONAL_USER_INFO), 1, file);
    fclose(file);

    // get time
    time(&cur_time);
    account_file_history.Time_SignUp = *localtime(&cur_time);

    // create file name
    strcpy(file_name, HISTORY_FILE_NAME);
    itoa(personal_user_info->User_ID, file_name + 1, 10);
    strcat(file_name, USER_FILE_FORMAT);

    // create H_user_id file
    if (Do_Load_File(LOAD_H_USER_ID, &file, file_name) == ERROR_OPENING_FILE) {
        return ERROR_OPENING_FILE;
    }

    // write account_file_history in user_id file
    fseek(file, 0, SEEK_SET);
    fwrite(&account_file_history, sizeof(struct ACCOUNT_FILE_HISTORY), 1, file);
    fclose(file);

    // create user_id file
    if (Do_Load_File(LOAD_USER_ID, &file, file_name + 1) == ERROR_OPENING_FILE) {
        return ERROR_OPENING_FILE;
    }

    Do_Update_user_id_Info(&account_user_info);

    Do_Add_Item(1001, account_user_info.User_ID);

    fclose(file);

    return SUCCESSFUL;
}

int Do_Delete_Account(int user_id)
{
    char file_address[ADDRESS_SIZE] = {}, file_name[FILE_NAME_SIZE] = {}, *backup_file;
    FILE *file = NULL;
    struct PERSONAL_USER_INFO temp_user_info;
    int struct_pui_size= sizeof(struct PERSONAL_USER_INFO);
    long long int indicator = 0, newfile_size = 0;

    // create file address
    strcpy(file_address, INSTALLATION_ADDRESS);
    strcat(file_address, USERDATA_ADDRESS);
    strcpy(file_name, HISTORY_FILE_NAME);
    itoa(user_id, file_name + 1, 10);
    strcat(file_name, USER_FILE_FORMAT);
    strcat(file_address, file_name);

    // delete H_user_id file
    if (remove(file_address) != 0) {
        return ERROR_REMOVING_FILE;
    }

    // create file address
    strcpy(file_address, INSTALLATION_ADDRESS);
    strcat(file_address, USERDATA_ADDRESS);
    strcat(file_address, file_name + 1);
    strcat(file_name, USER_FILE_FORMAT);

    // delete user_id file
    if (remove(file_address) != 0) {
        return ERROR_REMOVING_FILE;
    }

    // open alluser file
    if (Do_Load_File(LOAD_ALLUSERS, &file, ALLUSER_FILE_NAME) == ERROR_OPENING_FILE) {
        return ERROR_OPENING_FILE;
    }

    // find user indicator
    if ((indicator = Do_User_Finder(SEARCH_BY_USER_ID, &user_id)) <= 0) {
        return ERROR_REMOVING_FILE;
    }

    // backup last user
    fseek(file, -struct_pui_size, SEEK_END);
    fread(&temp_user_info, struct_pui_size, 1, file);

    // find new alluser size
    newfile_size = ftell(file) - struct_pui_size;

    // write temp instead of deleted account
    fseek(file, indicator, SEEK_SET);
    fwrite(&temp_user_info, struct_pui_size, 1, file);

    // backup whole alluser file
    backup_file = (char *)malloc(newfile_size);
    fseek(file, 0, SEEK_SET);
    fread(backup_file, newfile_size, 1, file);

    fclose(file);

    // create file address
    strcpy(file_address, INSTALLATION_ADDRESS);
    strcat(file_address, USERDATA_ADDRESS);
    strcat(file_address, ALLUSER_FILE_NAME);

    // rewrite alluser file
    if ((file = fopen(file_address, "w+")) == NULL) {
        return ERROR_REMOVING_FILE;
    }
    fseek(file, 0, SEEK_SET);
    fwrite(backup_file, newfile_size, 1, file);

    fclose(file);

    return SUCCESSFUL;
}

int Do_Change_User_Info(enum CHANGE_USER_INFO_STATUS change_status, char *change_info, long long int Account_Indicator)
{
    FILE *file;
    struct PERSONAL_USER_INFO personal_user_info;
    int struct_pui_size= sizeof(struct PERSONAL_USER_INFO);

    // open alluser file
    if (Do_Load_File(LOAD_ALLUSERS, &file, ALLUSER_FILE_NAME) == ERROR_OPENING_FILE) {
        return ERROR_OPENING_FILE;
    }

    //
    fseek(file, Account_Indicator, SEEK_SET);
    fread(&personal_user_info, struct_pui_size, 1, file);

    // change user info
    switch (change_status) {
    case CHANGE_USERNAME:
        strcpy(personal_user_info.UserName, change_info);
        break;
    case CHANGE_EMAIL:
        strcpy(personal_user_info.Email, change_info);
        break;
    case CHANGE_NICKNAME:
        strcpy(personal_user_info.NickName, change_info);
        break;
    case CHANGE_PASSWORD:
        strcpy(personal_user_info.Password, change_info);
        break;
    default:
        break;
    }

    // rewrite changed info in the file
    fseek(file, Account_Indicator, SEEK_SET);
    fwrite(&personal_user_info, struct_pui_size, 1, file);

    fclose(file);

    return SUCCESSFUL;
}

int Do_Add_Item(int Item_ID, int user_id)
{
    FILE *file;
    char file_name[FILE_NAME_SIZE] = {};
    struct ACCOUNT_ITEM_INFO item_info = {Item_ID};
    struct ACCOUNT_USER_INFO acc_user_info;

    // create file name
    itoa(user_id, file_name, 10);
    strcat(file_name, USER_FILE_FORMAT);

    // open user_id file
    if (Do_Load_File(LOAD_USER_ID, &file, file_name) == ERROR_OPENING_FILE) {
        return ERROR_OPENING_FILE;
    }

    // ++item_number
    fseek(file, 0, SEEK_SET);
    fread(&acc_user_info, sizeof(struct ACCOUNT_USER_INFO), 1, file);

    acc_user_info.item_number++;

    fseek(file, 0, SEEK_SET);
    fwrite(&acc_user_info, sizeof(struct ACCOUNT_USER_INFO), 1, file);

    // add item at the end of file
    fseek(file, 0, SEEK_END);
    fwrite(&item_info, sizeof(struct ACCOUNT_ITEM_INFO), 1, file);

    fclose(file);

    return SUCCESSFUL;
}

int Do_Add_History(struct ACCOUNT_GAME_HISTORY *game_history, int user_id)
{
    FILE *file;
    char file_name[FILE_NAME_SIZE] = {};

    // create file name
    strcpy(file_name, HISTORY_FILE_NAME);
    itoa(user_id, file_name + 1, 10);
    strcat(file_name, USER_FILE_FORMAT);

    // open H_user_id file
    if (Do_Load_File(LOAD_H_USER_ID, &file, file_name) == ERROR_OPENING_FILE) {
        return ERROR_REMOVING_FILE;
    }

    // add history add the end of file
    fseek(file, 0, SEEK_END);
    fwrite(game_history, sizeof(struct ACCOUNT_GAME_HISTORY), 1, file);

    fclose(file);

    return SUCCESSFUL;
}

int Do_Update_Alluser_Info(struct ALL_USER_IFNO *new_alluser_info)
{
    FILE *file;

    // load alluser file
    if (Do_Load_File(LOAD_ALLUSERS, &file, ALLUSER_FILE_NAME) == ERROR_OPENING_FILE) {
        return ERROR_OPENING_FILE;
    }

    // rewrite new info
    fseek(file, 0, SEEK_SET);
    fwrite(new_alluser_info, sizeof(struct ALL_USER_IFNO), 1, file);

    fclose(file);

    return SUCCESSFUL;
}

int Do_Update_user_id_Info(struct ACCOUNT_USER_INFO *new_user_info)
{
    FILE *file;
    char file_name[FILE_NAME_SIZE] = {};

    // create file name
    itoa(new_user_info->User_ID, file_name, 10);
    strcat(file_name, USER_FILE_FORMAT);

    // load alluser file
    if (Do_Load_File(LOAD_USER_ID, &file, file_name) == ERROR_OPENING_FILE) {
        return ERROR_OPENING_FILE;
    }

    // rewrite new info
    fseek(file, 0, SEEK_SET);
    fwrite(new_user_info, sizeof(struct ACCOUNT_USER_INFO), 1, file);

    fclose(file);

    return SUCCESSFUL;
}

int Do_Update_History_Info(struct ACCOUNT_FILE_HISTORY *new_history_info)
{
    FILE *file;
    char file_name[FILE_NAME_SIZE] = {};

    // create file name
    strcpy(file_name, HISTORY_FILE_NAME);
    itoa(new_history_info->User_ID, file_name + 1, 10);
    strcat(file_name, USER_FILE_FORMAT);

    // load alluser file
    if (Do_Load_File(LOAD_H_USER_ID, &file, file_name) == ERROR_OPENING_FILE) {
        return ERROR_OPENING_FILE;
    }

    // rewrite new info
    fseek(file, 0, SEEK_SET);
    fwrite(new_history_info, sizeof(struct ACCOUNT_FILE_HISTORY), 1, file);

    fclose(file);

    return SUCCESSFUL;
}

struct ALL_USER_IFNO Get_Alluser_Info()
{
    FILE *file;
    struct ALL_USER_IFNO alluser_info;

    // open alluser file
    Do_Load_File(LOAD_ALLUSERS, &file, ALLUSER_FILE_NAME);

    // read alluser info file
    fseek(file, 0, SEEK_SET);
    fread(&alluser_info, sizeof(struct ALL_USER_IFNO), 1, file);

    fclose(file);

    return alluser_info;
}

struct PERSONAL_USER_INFO Get_Personal_User_Info(long long int indicattor)
{
    FILE *file;
    struct PERSONAL_USER_INFO personal_user_info;

    // open alluser file
    Do_Load_File(LOAD_ALLUSERS, &file, ALLUSER_FILE_NAME);

    // read alluser info file
    fseek(file, indicattor, SEEK_SET);
    fread(&personal_user_info, sizeof(struct PERSONAL_USER_INFO), 1, file);

    fclose(file);

    return personal_user_info;
}

struct ACCOUNT_USER_INFO Get_Account_info(int user_id)
{
    FILE *file;
    char file_name[FILE_NAME_SIZE] = {};
    struct ACCOUNT_USER_INFO acc_user_info;

    // create file_name
    itoa (user_id, file_name, 10);
    strcat(file_name, USER_FILE_FORMAT);

    // open user_id file
    Do_Load_File(LOAD_USER_ID, &file, file_name);

    // read account user info
    fseek(file, 0, SEEK_SET);
    fread(&acc_user_info, sizeof(struct ACCOUNT_USER_INFO), 1, file);

    fclose(file);

    return acc_user_info;
}

struct ACCOUNT_ITEM_INFO *Get_User_Item_List(int user_id)
{
    FILE *file;
    struct ACCOUNT_ITEM_INFO *item_list;
    char file_name[FILE_NAME_SIZE] = {};
    int item_numbers = 0, struct_item_size = sizeof(struct ACCOUNT_ITEM_INFO);

    // create file_name
    itoa (user_id, file_name, 10);
    strcat(file_name, USER_FILE_FORMAT);

    // open user_id file
    Do_Load_File(LOAD_USER_ID, &file, file_name);

    // calculate number of items
    fseek(file, 0, SEEK_END);
    item_numbers = (ftell(file) - sizeof(struct ACCOUNT_USER_INFO)) / struct_item_size;

    // create enough space for item list
    item_list = (struct ACCOUNT_ITEM_INFO *)malloc(item_numbers * struct_item_size);

    // read item list
    fseek(file, sizeof(struct ACCOUNT_USER_INFO), SEEK_SET);
    fread(item_list, struct_item_size, item_numbers, file);

    fclose(file);

    return item_list;
}

struct ACCOUNT_FILE_HISTORY Get_Histroy_Info(int user_id)
{
    FILE *file;
    struct ACCOUNT_FILE_HISTORY acc_file_history;
    char file_name[FILE_NAME_SIZE] = {};

    // create file name
    strcpy(file_name, HISTORY_FILE_NAME);
    itoa(user_id, file_name + 1, 10);
    strcat(file_name, USER_FILE_FORMAT);

    // open H_user_id file
    Do_Load_File(LOAD_H_USER_ID, &file, file_name);

    // read history info
    fseek(file, 0, SEEK_SET);
    fread(&acc_file_history, sizeof(struct ACCOUNT_FILE_HISTORY), 1, file);

    fclose(file);

    return acc_file_history;
}

struct ACCOUNT_GAME_HISTORY *Get_Game_History_List(int user_id)
{
    FILE *file;
    struct ACCOUNT_GAME_HISTORY *history_list;
    char file_name[FILE_NAME_SIZE] = {};
    int history_numbers = 0, struct_history_size = sizeof(struct ACCOUNT_GAME_HISTORY);

    // create file name
    strcpy(file_name, HISTORY_FILE_NAME);
    itoa(user_id, file_name + 1, 10);
    strcat(file_name, USER_FILE_FORMAT);

    // open H_user_id file
    Do_Load_File(LOAD_H_USER_ID, &file, file_name);

    // calculate number of items
    fseek(file, 0, SEEK_END);
    history_numbers = (ftell(file) - sizeof(struct ACCOUNT_FILE_HISTORY)) / struct_history_size;

    // create enough space for history list
    history_list = (struct ACCOUNT_GAME_HISTORY *)malloc(history_numbers * struct_history_size);

    // read history list
    fseek(file, sizeof(struct ACCOUNT_FILE_HISTORY), SEEK_SET);
    fread(history_list, struct_history_size, history_numbers, file);

    fclose(file);

    return history_list;
}


FILE *Do_Load_Map(enum LOAD_MAP_STATUS load_map_status)
{
    char file_address[ADDRESS_SIZE] = {};

    strcpy(file_address, INSTALLATION_ADDRESS);
    strcat(file_address, GAMEDATA_ADDRESS);

    // create file address
    switch (load_map_status) {
    case LOAD_LOGIN_MAP:
        strcat(file_address, LOGIN_MAP_NAME);
        break;
    case LOAD_MAIN_MENU_MAP:
        strcat(file_address, MAIN_MENU_MAP_NAME);
        break;
    case LOAD_HISTORY_MAP:
        strcat(file_address, HISTORY_MAP_NAME);
        break;
    case LOAD_GAME_MAP_1:
        strcat(file_address, GAME_MAP_1_NAME);
        break;
    case LOAD_GAME_MAP_2:
        strcat(file_address, GAME_MAP_2_NAME);
        break;
    case LOAD_SHOP_MAP:
        strcat(file_address, SHOP_MAP_NAME);
        break;
    case LOAD_LOADING_MAP:
        strcat(file_address, LOADING_MAP_NAME);
        break;
    default:
        break;
    }

    // open file address
    return fopen(file_address, "r");
}












