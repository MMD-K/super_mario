#include "mario_game.h"
#include <conio.h>

//////////////////////////////// Define ////////////////////////////////////////



//////////////////////////////// Structures ////////////////////////////////////



//////////////////////////////// Login Functions ////////////////////////////////

int LogIn(long long int *acc_indicator)
{
    // login saved account
    struct ALL_USER_IFNO all_user_info = Get_Alluser_Info();
    if (all_user_info.Saved_UserName[0] != 0) {
        // if account finded
        if (((*acc_indicator) = Do_User_Finder(SEARCH_BY_USERNAME, all_user_info.Saved_UserName)) > 0) {
            // read user id
            struct PERSONAL_USER_INFO personal_user_info = Get_Personal_User_Info(*acc_indicator);
            // check password
            if (personal_user_info.Email[0] == 0 || personal_user_info.Password[0] == 0 ||strcmp(personal_user_info.Password, all_user_info.Saved_Password) == 0) {
                return personal_user_info.User_ID;
            }
        }
    }

    // if not login with saved account
    struct WINDOW *map = Load_Map(LOAD_LOGIN_MAP);

    // define fore_color
    struct COLOR fore_color = {3, 4, 94};

    // if you dont sign in you will continue while
    while (TRUE) {
        int user_id = 0;
        // create menu list
        char *menu_names[5] = {"     Sign In     ", "     Sign Up     ", " Forgot Password ", "      Guest      ", "      Exit       "};
        struct MENU menu;
        menu.number = 5;
        menu.active = 0;
        menu.name = menu_names;

        // reload map
        Reload(map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);

        // menu function
        int cmd = Menu(&menu, DISPLAY_X_SIZE / 2, MAP_Y_START + 3, map->back_color, fore_color, 0);

        switch (cmd) {
        case 0:
            if ((user_id = Sign_In(acc_indicator)) > 0) {
                return user_id;
            }
            break;
        case 1:
            Sign_Up();
            break;
        case 2:
            Forgot_Password();
            break;
        case 3:
            return Guest(acc_indicator);
            return 0;
            break;
        case 4:
            return EXIT;
            break;
        default:
            break;
        }

    }
}

int Sign_In(long long int *acc_indicator)
{
    char username[USERNAME_SIZE] = {}, password[PASSWORD_SIZE] = {};
    struct WINDOW *map = Load_Map(LOAD_LOGIN_MAP);
    int input_size[2] = {USERNAME_SIZE, PASSWORD_SIZE};
    struct COLOR fore_color = {3, 4, 94};   // define fore_color
    enum SIGN_ERROR error = SUCCESFUL;

    char *input_names[4] = {" User Name ", username, " Password ", password};
    struct INPUT_LIST input_list;
    input_list.number = 4;
    input_list.active = 1;
    input_list.name = input_names;

    Reload(map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);

    while (TRUE) {
        struct PERSONAL_USER_INFO personal_user_info;

        if(Input_List(&input_list, input_size, error, DISPLAY_X_SIZE / 2, MAP_Y_START + 3, map->back_color, fore_color, 3) == EXIT) {
            return SIGNIN_ERROR;
        }

        if ((*acc_indicator = Do_User_Finder(SEARCH_BY_USERNAME, username)) <= 0) {
            error = USERNAME_NOT_FOUND;
            continue;
        }

        personal_user_info = Get_Personal_User_Info(*acc_indicator);

        if (strcmp(personal_user_info.Password, password)) {
            error = WRONG_PASSWORD;
            continue;
        }

        // update save account
        struct ALL_USER_IFNO all_user_info = Get_Alluser_Info();
        strcpy(all_user_info.Saved_UserName, personal_user_info.UserName);
        strcpy(all_user_info.Saved_Password, personal_user_info.Password);

        // rewrite all user file
        Do_Update_Alluser_Info(&all_user_info);

        return SUCCESFUL;
    }
}

int Sign_Up()
{
    struct WINDOW *map = Load_Map(LOAD_LOGIN_MAP);
    int input_size[4] = {USERNAME_SIZE, NICKNAME_SIZE, EMAIL_SIZE, PASSWORD_SIZE};
    struct COLOR fore_color = {3, 4, 94};   // define fore_color
    enum VERIFICATION_ERROR error = VERIFIED;
    struct PERSONAL_USER_INFO personal_user_info = {0, 0, 0, 0, 0, 0};

    char *input_names[8] = {" User Name ", personal_user_info.UserName," Nick Name ", personal_user_info.NickName
                            , " email ", personal_user_info.Email, " Password ", personal_user_info.Password};
    struct INPUT_LIST input_list;
    input_list.number = 8;
    input_list.active = 1;
    input_list.name = input_names;

    Reload(map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);

    while (TRUE) {
        if(Input_List(&input_list, input_size, error, DISPLAY_X_SIZE / 2, MAP_Y_START + 2, map->back_color, fore_color, 7) == EXIT) {
            return SIGNIN_ERROR;
        }

        if ((error = Verification_Username(personal_user_info.UserName)) != VERIFIED) {
            continue;
        }

        if ((error = Verification_Email(personal_user_info.Email)) != VERIFIED) {
            continue;
        }

        if ((error = Verification_Passwors(personal_user_info.Password)) != VERIFIED) {
            continue;
        }

        // generate new user id
        struct ALL_USER_IFNO all_user_info = Get_Alluser_Info();
        all_user_info.Last_ID++;
        all_user_info.Number_Of_Users++;

        personal_user_info.User_ID = all_user_info.Last_ID;

        // rewrite all user file
        Do_Update_Alluser_Info(&all_user_info);

        // create new account
        Do_Add_Account(&personal_user_info);

        return SUCCESFUL;
    }
}

int Sign_Up_Guest(struct PERSONAL_USER_INFO *per_user_info)
{
    struct WINDOW *map = Load_Map(LOAD_LOGIN_MAP);
    int input_size[4] = {USERNAME_SIZE, NICKNAME_SIZE, EMAIL_SIZE, PASSWORD_SIZE}, acc_indicator = 0;
    struct COLOR fore_color = {3, 4, 94};   // define fore_color
    enum VERIFICATION_ERROR error = VERIFIED;

    char *input_names[8] = {" User Name ", per_user_info->UserName," Nick Name ", per_user_info->NickName
                            , " email ", per_user_info->Email, " Password ", per_user_info->Password};
    struct INPUT_LIST input_list;
    input_list.number = 8;
    input_list.active = 1;
    input_list.name = input_names;

    Reload(map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);

    while (TRUE) {
        if(Input_List(&input_list, input_size, error,  DISPLAY_X_SIZE / 2, MAP_Y_START + 2, map->back_color, fore_color, 7) == EXIT) {
            return SIGNIN_ERROR;
        }

        if ((error = Verification_Username(per_user_info->UserName)) != VERIFIED) {
            continue;
        }

        if ((error = Verification_Email(per_user_info->Email)) != VERIFIED) {
            continue;
        }

        if ((error = Verification_Passwors(per_user_info->Password)) != VERIFIED) {
            continue;
        }

        // find guest user
        acc_indicator = Do_User_Finder(SEARCH_BY_USER_ID, &per_user_info->User_ID);

        // change account info
        Do_Change_User_Info(CHANGE_USERNAME, per_user_info->UserName, acc_indicator);
        Do_Change_User_Info(CHANGE_NICKNAME, per_user_info->NickName, acc_indicator);
        Do_Change_User_Info(CHANGE_EMAIL, per_user_info->Email, acc_indicator);
        Do_Change_User_Info(CHANGE_PASSWORD, per_user_info->Password, acc_indicator);

        return SUCCESFUL;
    }
}

int Forgot_Password()
{
    char username[USERNAME_SIZE] = {}, password[PASSWORD_SIZE] = {}, email[EMAIL_SIZE] = {};
    struct WINDOW *map = Load_Map(LOAD_LOGIN_MAP);
    int input_size[4] = {USERNAME_SIZE, EMAIL_SIZE, PASSWORD_SIZE}, acc_indicator = 0;
    struct COLOR fore_color = {3, 4, 94};   // define fore_color
    enum VERIFICATION_ERROR error = VERIFIED;
    struct PERSONAL_USER_INFO personal_user_info;

    char *input_names[6] = {" User Name ", username, " email ", email, " Password ", password};
    struct INPUT_LIST input_list;
    input_list.number = 6;
    input_list.active = 1;
    input_list.name = input_names;

    Reload(map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);

    while (TRUE) {
        if(Input_List(&input_list, input_size, error, DISPLAY_X_SIZE / 2, MAP_Y_START + 3, map->back_color, fore_color, 5) == EXIT) {
            return SIGNIN_ERROR;
        }

        // check username
        if ((acc_indicator = Do_User_Finder(SEARCH_BY_USERNAME, username)) <= 0) {
            error = USERNAME_NOT_FOUND;
            continue;
        }

        // check email
        personal_user_info = Get_Personal_User_Info(acc_indicator);

        if (strcmp(personal_user_info.Email, email) != 0) {
            error = EMAIL_ERROR;
            continue;
        }

        // check password
        if ((error = Verification_Passwors(password)) != VERIFIED) {
            continue;
        }

        // change password
        Do_Change_User_Info(CHANGE_PASSWORD, password, acc_indicator);

        return SUCCESFUL;
    }
}

int Guest(long long int *acc_indicator)
{
    int user_id = 0;
    struct ALL_USER_IFNO all_user_info = Get_Alluser_Info();
    struct PERSONAL_USER_INFO per_user_info;

    // create personal guest info
    user_id = per_user_info.User_ID = all_user_info.Last_ID + 1;
    strcpy(per_user_info.UserName, GUEST_NAME);
    itoa(user_id, per_user_info.UserName + strlen(per_user_info.UserName), 10);
    strcpy(per_user_info.NickName, per_user_info.UserName);
    per_user_info.Password[0] = 0;
    per_user_info.Email[0] = 0;

    // add guest account
    Do_Add_Account(&per_user_info);

    // generate new user id
    *acc_indicator = Do_User_Finder(SEARCH_BY_USER_ID, &user_id);

    // update all user file
    all_user_info.Number_Of_Users++;
    all_user_info.Last_ID++;
    strcpy(all_user_info.Saved_UserName, per_user_info.UserName);
    all_user_info.Saved_Password[0] = 0;

    // rewrite all user file
    Do_Update_Alluser_Info(&all_user_info);

    return user_id;
}

void Load_Account(long long int acc_indicator,struct PERSONAL_USER_INFO *per_user_info, struct ACCOUNT_USER_INFO *acc_user_info)
{
    *per_user_info = Get_Personal_User_Info(acc_indicator);
    *acc_user_info = Get_Account_info(per_user_info->User_ID);
}

void Logout(int user_id, struct PERSONAL_USER_INFO *per_user_info,int start_time)
{
    struct ACCOUNT_FILE_HISTORY acc_his_info = Get_Histroy_Info(user_id);
    struct ALL_USER_IFNO all_user_info = Get_Alluser_Info();

    // update history
    acc_his_info.Time_Play_S += (int)clock() / CLOCKS_PER_SEC - start_time;

    Do_Update_History_Info(&acc_his_info);


    // if current account is guest
    if (per_user_info->Password[0] == 0 || per_user_info->Email[0] == 0) {
        struct WINDOW *map = Load_Map(LOAD_LOGIN_MAP);

        // define fore_color
        struct COLOR fore_color = {3, 4, 94};

        // sign up
        while (TRUE) {
            // create menu list
            char *menu_names[2] = {"     Sign Up     ", "     Logout      "};
            struct MENU menu;
            menu.number = 2;
            menu.active = 0;
            menu.name = menu_names;

            // reload map
            Reload(map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);

            // menu function
            int cmd = Menu(&menu, DISPLAY_X_SIZE / 2, MAP_Y_START + 3, map->back_color, fore_color, 1);

            switch (cmd) {
            case 0:
                if (Sign_Up_Guest(per_user_info) != SIGNIN_ERROR) {
                    return;
                }
                break;
            case 1:
                Do_Delete_Account(user_id);
                return;
                break;
            default:
                return;
                break;
            }

        }
    }

    // update saved account
    all_user_info.Saved_UserName[0] = 0;
    all_user_info.Saved_Password[0] = 0;

    Do_Update_Alluser_Info(&all_user_info);
}

void Exit(int user_id, int start_time)
{
    if (user_id > 0) {
       // update history
        struct ACCOUNT_FILE_HISTORY acc_his_file = Get_Histroy_Info(user_id);

        acc_his_file.Time_Play_S += (int)clock() / CLOCKS_PER_SEC - start_time;

        Do_Update_History_Info(&acc_his_file);
    }
}

int Account_Management(struct PERSONAL_USER_INFO *per_user_info, long long int acc_indicator, struct ACCOUNT_USER_INFO *acc_user_info)
{
    // if not login with saved account
    struct WINDOW *map = Load_Map(LOAD_LOGIN_MAP);

    // define fore_color
    struct COLOR fore_color = {3, 4, 94};

    // if you dont sign in you will continue while
    while (TRUE) {
        // create menu list
        char *menu_names[6] = {" Change User Name ", " Change Nick Name ", "   Change Email   ", " Change Password  ", "  Delete Account  ", "       Back       "};
        struct MENU menu;
        menu.number = 6;
        menu.active = 0;
        menu.name = menu_names;

        // reload map
        Reload(map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);
        Print_Account_Info(acc_user_info, per_user_info, map->back_color, fore_color);

        // menu function
        int cmd = Menu(&menu, DISPLAY_X_SIZE / 2, MAP_Y_START + 3, map->back_color, fore_color, 1);

        switch (cmd) {
        case 0:
            Change_User_Info(CHANGE_USERNAME, per_user_info->UserName, acc_indicator);
            break;
        case 1:
            Change_User_Info(CHANGE_NICKNAME, per_user_info->NickName, acc_indicator);
            break;
        case 2:
            Change_User_Info(CHANGE_EMAIL, per_user_info->Email, acc_indicator);
            break;
        case 3:
            Change_User_Info(CHANGE_PASSWORD, per_user_info->Password, acc_indicator);
            break;
        case 4:
            if (Delete_Account(per_user_info->User_ID) == EXIT) {
                return EXIT;
            }
            break;
        case 5:
            return SUCCESFUL;
            break;
        default:
            return -1;
            break;
        }
    }
}

int Change_User_Info(enum CHANGE_USER_INFO_STATUS change_status, char *change_info, long long int acc_indicator)
{
    struct WINDOW *map = Load_Map(LOAD_LOGIN_MAP);
    int input_size[5] = {USERNAME_SIZE, NICKNAME_SIZE,EMAIL_SIZE, PASSWORD_SIZE, PASSWORD_SIZE};
    struct COLOR fore_color = {3, 4, 94};   // define fore_color


    while (TRUE) {
        char username[USERNAME_SIZE] = {}, nickname[NICKNAME_SIZE] = {}, email[EMAIL_SIZE] = {}, pass[PASSWORD_SIZE] = {}, newpass[PASSWORD_SIZE] = {};
        char *input_names[10] = {" New User Name ", username," New Nick Name ", nickname, " New Email ", email
                                 , " Current Password ", pass, " New Password ", newpass};
        struct INPUT_LIST input_list;
        enum VERIFICATION_ERROR error = VERIFIED;

        Reload(map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);

        switch (change_status) {
        case CHANGE_USERNAME:
            input_list.number = 2;
            input_list.active = 1;
            input_list.name = &input_names[0];

            while (TRUE) {
                if(Input_List(&input_list, &input_size[0], error, DISPLAY_X_SIZE / 2, MAP_Y_START + 3, map->back_color, fore_color, -1) == EXIT) {
                    return SIGNIN_ERROR;
                }

                if ((error = Verification_Username(username)) != VERIFIED) {
                    continue;
                }

                // change username
                Do_Change_User_Info(CHANGE_USERNAME, username, acc_indicator);
                strcpy(change_info, username);

                return SUCCESFUL;
            }
            break;
        case CHANGE_NICKNAME:
            input_list.number = 2;
            input_list.active = 1;
            input_list.name = &input_names[2];

            while (TRUE) {
                if(Input_List(&input_list, &input_size[1], error, DISPLAY_X_SIZE / 2, MAP_Y_START + 3, map->back_color, fore_color, -1) == EXIT) {
                    return SIGNIN_ERROR;
                }

                // change username
                Do_Change_User_Info(CHANGE_NICKNAME, nickname, acc_indicator);
                strcpy(change_info, nickname);

                return SUCCESFUL;
            }
            break;
        case CHANGE_EMAIL:
            input_list.number = 2;
            input_list.active = 1;
            input_list.name = &input_names[4];

            while (TRUE) {
                if(Input_List(&input_list, &input_size[2], error, DISPLAY_X_SIZE / 2, MAP_Y_START + 3, map->back_color, fore_color, -1) == EXIT) {
                    return SIGNIN_ERROR;
                }

                if ((error = Verification_Email(email)) != VERIFIED) {
                    continue;
                }

                // change username
                Do_Change_User_Info(CHANGE_EMAIL, email, acc_indicator);
                strcpy(change_info, email);

                return SUCCESFUL;
            }
            break;
        case CHANGE_PASSWORD:
            input_list.number = 4;
            input_list.active = 1;
            input_list.name = &input_names[6];

            while (TRUE) {
                if(Input_List(&input_list, &input_size[3], error, DISPLAY_X_SIZE / 2, MAP_Y_START + 3, map->back_color, fore_color, -2) == EXIT) {
                    return SIGNIN_ERROR;
                }

                struct PERSONAL_USER_INFO personal_user_info  = Get_Personal_User_Info(acc_indicator);

                // check previous password
                if (strcmp (pass, personal_user_info.Password) != 0) {
                    error = SIGNIN_ERROR;
                    continue;
                }

                if ((error = Verification_Passwors(newpass)) != VERIFIED) {
                    continue;
                }

                // change username
                Do_Change_User_Info(CHANGE_PASSWORD, newpass, acc_indicator);
                strcpy(change_info, newpass);

                return SUCCESFUL;
            }
            break;
        default:
            break;
        }
    }
}

int Delete_Account(int user_id)
{
    struct WINDOW *map = Load_Map(LOAD_LOGIN_MAP);

    // define fore_color
    struct COLOR fore_color = {3, 4, 94};

    // sign up
    while (TRUE) {
        // create menu list
        char *menu_names[2] = {"     Delete     ", "      Back      "};
        struct MENU menu;
        menu.number = 2;
        menu.active = 0;
        menu.name = menu_names;

        // reload map
        Reload(map, MAP_MAX_X_SIZE, MAP_MAX_Y_SIZE);

        // menu function
        int cmd = Menu(&menu, DISPLAY_X_SIZE / 2, MAP_Y_START + 3, map->back_color, fore_color, 1);

        switch (cmd) {
        case 0:
            Do_Delete_Account(user_id);
            return EXIT;
            break;
        case 1:
            return SUCCESFUL;
            break;
        default:
            return SUCCESFUL;
            break;
        }

    }
}

//////////////////////////////// Menu Functions ////////////////////////////////

int Menu(struct MENU *menu, int xmiddle, int ystart, struct COLOR back_color, struct COLOR fore_color, char esc_check)
{
    char ch = 0;
    int pa = menu->active, len = 0;
    // set default color
    Set_Back_Color(back_color.r, back_color.g, back_color.b);
    Set_Fore_Color(fore_color.r, fore_color.g, fore_color.b);

    // print menu
    for (int i = 0; i < menu->number; i++) {
        // set cursor position
        len = strlen(menu->name[i]);
        Gotoxy(xmiddle - len / 2, ystart + i);

        if (i == menu->active) {
            // set active color
            Set_Fore_Color(back_color.r, back_color.g, back_color.b);
            Set_Back_Color(fore_color.r, fore_color.g, fore_color.b);

            // print
            printf("%s", menu->name[i]);

            // set default color
            Set_Back_Color(back_color.r, back_color.g, back_color.b);
            Set_Fore_Color(fore_color.r, fore_color.g, fore_color.b);
        }
        else {
            // print
            printf("%s", menu->name[i]);
        }
    }

    // choose menu
    while (TRUE) {
        // set cursor position
        len = strlen(menu->name[pa]);
        Gotoxy(xmiddle - len / 2, ystart + pa);

        // set color
        Set_Back_Color(back_color.r, back_color.g, back_color.b);
        Set_Fore_Color(fore_color.r, fore_color.g, fore_color.b);

        // print
        printf("%s", menu->name[pa]);

        // set cursor position
        len = strlen(menu->name[menu->active]);
        Gotoxy(xmiddle - len / 2, ystart + menu->active);

        // set color
        Set_Fore_Color(back_color.r, back_color.g, back_color.b);
        Set_Back_Color(fore_color.r, fore_color.g, fore_color.b);

        // print
        printf("%s", menu->name[menu->active]);

        ch = getch();

        switch (ch) {
        case ESC_KEY:
            if ((ch = getch()) == 72) {
                pa = menu->active;
                if (menu->active > 0) {
                    menu->active--;
                }
                else {
                    menu->active = menu->number - 1;
                }
            }
            else if (ch == 80) {
                pa = menu->active;
                if (menu->active < menu->number - 1) {
                    menu->active++;
                }
                else {
                    menu->active = 0;
                }
            }
            break;
        case 13:
            return menu->active;     ///////// return
        case 27:
            if (esc_check) {
                return EXIT;
            }
        default:
            break;
        }
    }

    Reset_Color();
}

//////////////////////////////// Input Function ////////////////////////////////

int Input_List(struct INPUT_LIST *input_list, int *input_size, int error, int xmiddle, int ystart, struct COLOR back_color, struct COLOR fore_color,int check_pass)
{
    char ch = 0;
    int pa = input_list->active, len = 0, i = 0;
    // set default color
    Set_Back_Color(back_color.r, back_color.g, back_color.b);
    Set_Fore_Color(fore_color.r, fore_color.g, fore_color.b);

    // print boxes
    for (i = 0; i < input_list->number; i++) {
        // set cursor position
        len = strlen(input_list->name[i]);
        Gotoxy(xmiddle - len / 2, ystart + i);

        // print name box
        printf("%s", input_list->name[i]);

        i++;

        // set cursor position
        Gotoxy(xmiddle - input_size[i / 2] / 2, ystart + i);

        len = strlen(input_list->name[i]);

        if (i == input_list->active) {
            // set active color
            Set_Fore_Color(back_color.r, back_color.g, back_color.b);
            Set_Back_Color(fore_color.r, fore_color.g, fore_color.b);

            // print input box
            printf("%*c", input_size[i / 2], ' ');
            Gotoxy(xmiddle - len / 2, ystart + i);  // set cursor position
            if (check_pass == -2 || check_pass == i) {
                for (int count = 0; count < len; count++) {
                    printf("*");
                }
            }
            else {
                printf("%*s", len, input_list->name[i]);
            }

            // set default color
            Set_Back_Color(back_color.r, back_color.g, back_color.b);
            Set_Fore_Color(fore_color.r, fore_color.g, fore_color.b);
        }
        else {
            // print input box
            printf("%*c", input_size[i / 2], ' ');
            Gotoxy(xmiddle - len / 2, ystart + i);  // set cursor position

            if (check_pass == -2 || check_pass == i) {
                for (int count = 0; count < len; count++) {
                    printf("*");
                }
            }
            else {
                printf("%*s", len, input_list->name[i]);
            }
        }
    }

    // print submit
    len = strlen(SUBMIT_BUTTON);
    Gotoxy(xmiddle - len / 2, ystart + input_list->number);
    Set_Fore_Color(back_color.r, back_color.g, back_color.b);
    Set_Back_Color(fore_color.r, fore_color.g, fore_color.b);
    printf("%s", SUBMIT_BUTTON);

    // print error
    if (error < 1) {
        Set_Back_Color(back_color.r, back_color.g, back_color.b);
        Set_Fore_Color(214, 34, 70);
        len = strlen(" ERROR ");
        Gotoxy(xmiddle - len / 2, ystart + input_list->number + 2);
        printf(" ERROR ");
    }



    // start Inputing!
    while (TRUE) {
        // print previous box
        if (pa != input_list->number) {
            len = strlen(input_list->name[pa]);
            Gotoxy(xmiddle - input_size[pa / 2] / 2, ystart + pa);     // set cursor position
            Set_Back_Color(back_color.r, back_color.g, back_color.b);   // set foreground color
            Set_Fore_Color(fore_color.r, fore_color.g, fore_color.b);   // set background color
            // print input box
            printf("%*c", input_size[pa / 2], ' ');
            Gotoxy(xmiddle - len / 2, ystart + pa);  // set cursor position

            if (check_pass == -2 || check_pass == pa) {
                for (int count = 0; count < len; count++) {
                    printf("*");
                }
            }
            else {
                printf("%*s", len, input_list->name[pa]);
            }
        }
        else {  // if pa == button
            // print submit
            len = strlen(SUBMIT_BUTTON);
            Gotoxy(xmiddle - len / 2, ystart + input_list->number);
            Set_Fore_Color(back_color.r, back_color.g, back_color.b);
            Set_Back_Color(fore_color.r, fore_color.g, fore_color.b);
            printf("%s", SUBMIT_BUTTON);
        }

        if (input_list->active != input_list->number) {
            // print active box
            len = strlen(input_list->name[input_list->active]);
            Gotoxy(xmiddle - input_size[input_list->active / 2] / 2, ystart + input_list->active);     // set cursor position
            Set_Fore_Color(back_color.r, back_color.g, back_color.b);   // set foreground color
            Set_Back_Color(fore_color.r, fore_color.g, fore_color.b);   // set background color
            // print input box
            printf("%*c", input_size[input_list->active / 2], ' ');
            Gotoxy(xmiddle - len / 2, ystart + input_list->active);  // set cursor position

            if (check_pass == -2 || check_pass == input_list->active) {
                for (int count = 0; count < len; count++) {
                    printf("*");
                }
            }
            else {
                printf("%*s", len, input_list->name[input_list->active]);
            }
        }
        else {  // if active == button
            // print submit
            len = strlen(SUBMIT_BUTTON);
            Gotoxy(xmiddle - len / 2, ystart + input_list->number);
            Set_Fore_Color(back_color.r, back_color.g, back_color.b);
            Set_Back_Color(fore_color.r, fore_color.g, fore_color.b);
            printf("%s", SUBMIT_BUTTON);
        }

        ch = getch();

        switch (ch) {
        case ESC_KEY:
            if ((ch = getch()) == 72) {     // page up
                pa = input_list->active;
                if (input_list->active == input_list->number) {
                    input_list->active--;
                }
                else if (input_list->active > 1) {
                    input_list->active -= 2;
                }
                else {
                    input_list->active = input_list->number - 1;
                }
            }
            else if (ch == 80) {    // page down
                pa = input_list->active;
                if (input_list->active < input_list->number - 1) {
                    input_list->active += 2;
                }
                else if (input_list->active == input_list->number - 1) {
                    input_list->active++;
                }
                else {
                    input_list->active = 1;
                }
            }
            break;
        case 13:    // enter
            if (input_list->active < input_list->number) {
                pa = input_list->active;
                if (input_list->active < input_list->number - 1) {
                    input_list->active += 2;
                }
                else {
                    input_list->active++;
                }
            }
            else {
                return SUCCESFUL;
            }
            break;
        case 8:     // backspace (del)
            if (input_list->active != input_list->number) {
                len = strlen(input_list->name[input_list->active]);
                input_list->name[input_list->active][len - 1] = 0;
            }
            break;
        case 27:    // ESC
            for (int i = 1; i < input_list->number; i += 2) {
                input_list->name[i][0] = 0;
            }
            return EXIT;
            break;
        default:
            if (input_list->active != input_list->number) {     // check submit
                if ((len = strlen(input_list->name[input_list->active])) < input_size[input_list->active / 2] - 1) {
                    input_list->name[input_list->active][len] = ch;
                    input_list->name[input_list->active][len + 1] = 0;
                }
            }
            break;
        }
    }
}






