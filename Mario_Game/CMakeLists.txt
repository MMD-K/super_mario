cmake_minimum_required(VERSION 3.16)

project(Mario_Game LANGUAGES C)

add_executable(Mario_Game main.c
    file_manegement.h
    mario_game.h
    file_management.c
    account_management.c
    console_output.h
    console_output.c
    mario_game.c
    login_functions.c
    main_menu_functions.c)

include(GNUInstallDirs)
install(TARGETS Mario_Game
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
