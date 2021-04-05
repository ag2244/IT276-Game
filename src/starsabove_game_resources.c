#include "starsabove_game_resources.h"
#include "starsabove_ui_textbox.h"

Game_Event* new_gameevent(char* target_id, char* sub_target_id, char* command, char* descriptor, int qty, struct Menu_State* menu_state, Bool _menubase)
{

	Game_Event* game_event = malloc(sizeof(Game_Event));

	if (target_id != NULL) strcpy(game_event->target_id, target_id);
	if (sub_target_id != NULL) strcpy(game_event->sub_target_id, sub_target_id);
	if (command != NULL) strcpy(game_event->command, command);
	if (descriptor != NULL) strcpy(game_event->descriptor, descriptor);

	game_event->qty = qty;

	game_event->menu_state = menu_state;

	game_event->_menubase = _menubase;

	return game_event;

}

void gameevent_copy(Game_Event* dst, Game_Event* src)
{
	if (src->command != NULL) { strcpy(dst->command, src->command); }
	if (src->sub_target_id != NULL) { strcpy(dst->target_id, src->target_id); }
	if (src->target_id != NULL) { strcpy(dst->sub_target_id, src->sub_target_id); }
	if (src->descriptor != NULL) { strcpy(dst->descriptor, src->descriptor); }
	dst->qty = src->qty;

	/*if (dst->menu_state != NULL)
	{
		menu_state_hide(dst->menu_state);
	}*/

	dst->menu_state = src->menu_state;

	dst->_menubase = src->_menubase;

	dst->_sent = 1;
}

void gameevent_free(Game_Event* gameevent)
{

	if (gameevent->menu_state) 
	{
		menu_state_free(gameevent->menu_state);

		gameevent->menu_state = NULL;
	}

	free(gameevent);

}