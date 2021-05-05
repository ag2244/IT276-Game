#pragma once

#ifndef __UI_MAINMENU__
#define __UI_MAINMENU__

#include "starsabove_ui_base.h"
#include "starsabove_ui_textbox.h"

/*
* @brief Get the menustate listing all the loadable save files
* @param previous The previous menustate
* @return A new menustate
*/
Menu_State* savefiles_menustate(Menu_State* previous);

/*
* @brief Get the menustate used for the main menu of the game
* @param previous The previous menustate
* @return A new menustate
*/
Menu_State* main_menu();

#endif
