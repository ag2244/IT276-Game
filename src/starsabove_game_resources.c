#include "starsabove_game_resources.h"
#include "starsabove_ui_textbox.h"

Game_Event* new_gameevent(char* target_id, char* sub_target_id, char* command, char* descriptor, int qty, struct Menu_State* menu_state)
{

	Game_Event* game_event = malloc(sizeof(Game_Event));

	strcpy(game_event->command, command);
	strcpy(game_event->target_id, target_id);
	strcpy(game_event->descriptor, descriptor);

	game_event->qty = qty;

	game_event->menu_state = menu_state;

	return game_event;

}