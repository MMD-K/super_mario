#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include "mario_game.h"

//////////////////////////////// Define ////////////////////////////////////////



//////////////////////////////// Menu Functions ////////////////////////////////



//////////////////////////////// verification Functions ////////////////////////

int Verification_Username(char *username)
{
    int i = 0;

    // check username length
    if (strlen(username) >= USERNAME_SIZE) {
        return LEN_ERROR;
    }

    // check char
    while (username[i]) {
        if (!((username[i] >= 'a' && username[i] <= 'z') ||
              (username[i] >= 'A' && username[i] <= 'Z') ||
              (username[i] >= '0' && username[i] <= '9') ||
              username[i] == '_')) {

            return CHAR_ERROR;
        }
        i++;
    }

    // check other usernames
    if (Do_User_Finder(SEARCH_BY_USERNAME, username) > 0) {
        return REP_ERROR;
    }

    return VERIFIED;
}

int Verification_Email(char *email)
{
    int i = 0, len = 0;

    // check username length
    if ((len = strlen(email)) >= EMAIL_SIZE) {
        return LEN_ERROR;
    }

    // check email
    while (email[i] && email[i] != '@') {
        i++;
    }

    if (i + 3 >= len) {
        return EMAIL_ERROR;
    }

    while (email[i] && email[i] != '.') {
        i++;
    }

    if (i + 1 >= len) {
        return EMAIL_ERROR;
    }

    // check other emails
    if (Do_User_Finder(SEARCH_BY_EMAIL, email) > 0) {
        return REP_ERROR;
    }

    return VERIFIED;
}

int Verification_Passwors(char *password)
{
    int i = 0, len = 0, num_check = 0, c_check = 0, C_check = 0, sign_check = 0;

    // check max password len
    if ((len = strlen(password)) >= PASSWORD_SIZE) {
        return LEN_ERROR;
    }

    // check min password len
    if (len < MIN_PASSWORD_LEN) {
        return PASS_LEN_ERROR;
    }

    // check char
    while (password[i]) {
        if (password[i] >= 'a' && password[i] <= 'z') {
            c_check = 1;
        }
        else if (password[i] >= 'A' && password[i] <= 'Z') {
            C_check = 1;
        }
        else if (password[i] >= '0' && password[i] <= '9') {
            num_check = 1;
        }
        else {
            sign_check = 1;
        }
        i++;
    }

    if (num_check && c_check && C_check && sign_check) {
        return VERIFIED;
    }
    return CHAR_ERROR;
}
