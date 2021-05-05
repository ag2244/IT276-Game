#include "starsabove_ui_mainmenu.h"

Menu_State* savefiles_menustate(Menu_State* previous)
{
	Menu_State* savefiles_menustate = NULL;

	UI_Element* savefile_button;
	Game_Event* load_event;



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

	ui_addevent(load_game_button,
		new_gameevent(
			"STARSABOVE",
			NULL,
			"SHOWSAVES",
			NULL,
			0,
			savefiles_menustate(title_menustate),
			0
		)
	);

	//menu_state_hide(testmenustate);
	//menu_state_show(testmenustate);

}