
#include "simple_logger.h"

#include "starsabove_ui_mainmenu.h"

Menu_State* savefiles_menustate(Menu_State* previous)
{

	// Thanks to CodeVault:
	// https://www.youtube.com/watch?v=j9yL30R6npk

	Menu_State* savefiles_menustate = NULL;

	UI_Element* savefile_button;
	Game_Event* load_event;

	struct dirent* entity;

	char savefile_name[128];

	//New menustate
	savefiles_menustate = menu_state_new
	(
		NULL,
		textbox_init_clear
		(
			vector2d(10, 10), 
			vector2d(300, 150), 
			"LOAD A SAVE", 
			font_load("resources/fonts/futur.ttf", 72)
		),
		NULL,
		vector2d(10, 10),
		0,
		15
	);

	//Open the savegames directory
	DIR* dir = opendir("savegames");

	if (dir == NULL)
	{
		slog("COULD NOT READ SAVEGAMES DIRECTORY"); return NULL;
	}

	entity = readdir(dir);

	//Keep going through the filenames
	for (NULL; entity != NULL; entity = readdir(dir))
	{
		//Get the savefile name
		if ((strcmp(entity->d_name, ".") != 0) && (strcmp(entity->d_name, "..") != 0))
		{
			savefile_button = textbox_init_clear(
				vector2d(0, 0),
				vector2d(200, 50),
				entity->d_name,
				font_load("resources/fonts/futur.ttf", 20)
			);

			savefile_button->signal = new_gameevent(
				"STARSABOVE",
				NULL,
				"LOAD GAME",
				entity->d_name,
				0,
				NULL,
				0
			);

			menu_addTo(
				savefiles_menustate->current_menu,
				savefile_button
			);
		}

	}

	closedir(dir);

	menu_state_hide(savefiles_menustate);

	return savefiles_menustate;
}

Menu_State* main_menu()
{
	Menu_State* title_menustate = NULL;
	UI_Element* title;
	UI_Element* load_game_button;

	//Make a menu with a title
	title = textbox_init_clear(vector2d(10, 10), vector2d(300, 150), "STARS ABOVE", font_load("resources/fonts/futur.ttf", 72));

	title_menustate = menu_state_new(
		NULL,
		title,
		NULL,
		vector2d(10, 10),
		0,
		15
	);

	load_game_button = textbox_init_clear(vector2d(0, 0), vector2d(200, 50), "LOAD A SAVE FILE", font_load("resources/fonts/futur.ttf", 20));

	menu_addTo(
		title_menustate->current_menu,
		load_game_button
	);

	load_game_button->signal = new_gameevent(
			"STARSABOVE",
			NULL,
			"SHOWSAVES",
			NULL,
			0,
			savefiles_menustate(title_menustate),
			0
		);

	return title_menustate;

	//menu_state_hide(testmenustate);
	//menu_state_show(testmenustate);

}