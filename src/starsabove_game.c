#include <SDL.h>

#include "simple_logger.h"

#include "starsabove_game.h"

GameState gameState = { 0 };
Bool KEYS[322];

//Font* font;

void load_game(char* filename)
{
	SJson* game;

	if (filename == NULL)
	{
		slog("Cannot load a game with NULL filename"); return NULL;
	}

	game = sj_load(filename);

	if (!game)
	{
		slog("Cannot load the file at \"%s\"", filename); return NULL;
	}
	
	if (load_nations(game) == NULL)
	{
		slog("Was unable to load this game's nations");
		return NULL;
	}

	if (load_systems(game) == NULL)
	{
		slog("Was unable to load this game's star systems");
		return NULL;
	}

	sj_free(game);

}

void new_game()
{

}

void save_game(char* savefile_name)
{
	SJson* savefile = sj_object_new();
	SJson* systems = sj_array_new();

	char filename[10] = "jsondata/";
	char savefile_copy[256]; strcpy(savefile_copy, savefile_name);

	int i;
	EntityManager entity_manager = *entity_manager_get();
	Entity* thisEntity;

	if (savefile_name == NULL)
	{
		slog("Cannot save game to file \"%s\"!", savefile_name);
		return NULL;
	}
	
	//Go through each entity in the entity manager
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		//If the entity at index i is not in use, continue
		if (entity_manager.entity_list[i]._inuse == 0) continue;

		thisEntity = &entity_manager.entity_list[i];
		
		sj_array_append(systems, thisEntity->toJson(thisEntity));
	}

	sj_object_insert(savefile, "Systems", systems);

	sj_object_insert(savefile, "Nations", allNations_toJson());

	strcat(filename, savefile_copy);

	sj_save(savefile, filename);

}

void test() 
{

	load_game("jsondata/Test Input.json");

	save_game("TESTOUT.json");

	//test_ui();

	strcpy(gameState.playerNation, "Nation1");

	slog("Test Setup is done!");
}

void test_ui()
{
	Menu_State* testmenustate = NULL;
	UI_Element* title;
	UI_Element* beginning;

	//Make a menu
	title = textbox_init(vector2d(10, 10), vector2d(200, 50), "TITLE", font_load("resources/fonts/futur.ttf", 16));
	beginning = textbox_init(vector2d(0, 0), vector2d(200, 50), "BEGINNING", font_load("resources/fonts/futur.ttf", 12));

	testmenustate = menu_state_new(
		NULL,
		title,
		beginning,
		vector2d(10, 10),
		0,
		5
	);

	menu_addTo(
		testmenustate->current_menu,
		textbox_init(vector2d(0, 0), vector2d(200, 50), "ADDED", font_load("resources/fonts/futur.ttf", 12))
	);

	ui_addevent(beginning,
		new_gameevent(
			"System1",
			"SUB_TARGET_ID",
			"COMMAND",
			"DESCRIPTOR",
			123,
			testmenustate,
			1
		)
	);

	menu_state_hide(testmenustate);
	menu_state_show(testmenustate);
	
}

void prepare_game()
{
	int i = 0; EntityManager* entity_manager = entity_manager_get();

    // Starting the entity manager
    entity_manager_init(100);

	// Starting the ui manager
	ui_manager_init(500);

	// Starting the font manager, loading fonts
	font_init(50);
	font_load("resources/fonts/futur.ttf", 12);

	// Starting the nations list
	nations_list_init(24);

	// Initialize the player's menu state
	gameState.player_menustate = NULL;

	// Set up the debug world
	test();

	atexit(starsabove_exit);

	slog("============ GAME LOADED! ============");

}

void testcmd()
{
	/* FOR TESTING
	int i = 0; EntityManager* entity_manager = entity_manager_get();

	for (i = 0; i < entity_manager->max_entities; i++)
	{
		//If the element at index i is not in use, continue
		if (entity_manager->entity_list[i]._inuse == 0) continue;

		if (entity_manager->entity_list[i].owner != NULL) slog("Owner of %s: \"%s\"", entity_manager->entity_list[i].name, entity_manager->entity_list[i].owner->name);

		else slog("No Owner for %s", entity_manager->entity_list[i].name);

	}

	//*/

	/*

	int i; int j; Nation_List* nation_list = nation_list_get();

	for (i = 0; i < nation_list->max_nations; i++)
	{

		if (nation_list->nations[i]._inuse == 0) continue; // Someone else using this one

		slog(nation_list->nations[i].name);
	}

	//*/

}

void starsabove_loop()
{

	testcmd();

	entity_manager_update_entities();

	entity_manager_draw_entities();

	ui_manager_update();

	ui_manager_draw();
}

void event_relay()
{
	int i;
	EntityManager* entity_manager;

	if (gameState.player_menustate) { slog(gameState.player_menustate->current_menu->title->text); } else { slog("NULL"); }

	//If the new event's menu state isn't null
	if (gameState.frame_event.menu_state != NULL)
	{
		if ((gameState.frame_event._menubase == 1) && (gameState.player_menustate))
		{
			menu_state_free(menu_state_root(gameState.player_menustate));
		}

		//Set the player's menu state to that of the new event and show it
		gameState.player_menustate = gameState.frame_event.menu_state;

		menu_state_show(gameState.player_menustate);
	}

	entity_manager = entity_manager_get();

	if (!entity_manager) 
	{
		return NULL;
	}

	//Go through each entity in the entity manager
	for (i = 0; i < entity_manager->max_entities; i++)
	{
		//If the entity at index i is not in use, continue
		if (entity_manager->entity_list[i]._inuse == 0) continue;

		if (strcmp(entity_manager->entity_list[i].name, gameState.frame_event.target_id) == 0)
		{
			if (entity_manager->entity_list[i].reciever)
			{
				entity_manager->entity_list[i].reciever(&entity_manager->entity_list[i], &gameState.frame_event);
			}
		}
	}
}

Bool starsabove_hoverDetection(float mX, float mY) {

    int i;
	UI_Manager* ui_manager;
	EntityManager* entity_manager;
	
	//Get the UI Manager
	ui_manager = ui_manager_get();

	if (!ui_manager) {
		return NULL;
	}

	//Go through each element in the UI
	for (i = 0; i < ui_manager->max_elements; i++)
	{
		//If the element at index i is not in use, continue
		if (ui_manager->element_list[i]._inuse == 0) continue;

		//If the element is a clickable and the mouse is hovering over it, set the gameState.currentClickable_ui to point to it and return True.
		if (ui_clickable(&ui_manager->element_list[i], mX, mY)) 
		{
			gameState.currentClickable_ui = &ui_manager->element_list[i];
			gameState.currentClickable_entity = NULL;
			return 1;
		}
			
	}

	//Now to do the same but for the entity manager
	entity_manager = entity_manager_get();

	if (!entity_manager) {
		return NULL;
	}

	//Go through each entity in the entity manager
	for (i = 0; i < entity_manager->max_entities; i++)
	{
		//If the entity at index i is not in use, continue
		if (entity_manager->entity_list[i]._inuse == 0) continue;

		//If the entity is a clickable and the mouse is hovering over it, set the gameState.currentClickable_entity to point to it and return True.
		if (entity_clickable(&entity_manager->entity_list[i], mX, mY))
		{
			gameState.currentClickable_entity = &entity_manager->entity_list[i];
			gameState.currentClickable_ui = NULL;
			return 1;
		}
	}

	//No clickable is being hovered over
	gameState.currentClickable_ui = NULL;
	gameState.currentClickable_entity = NULL;
	return 0;

}

void onClick_left() 
{
	if (gameState.currentClickable_ui) {
		ui_onClick(gameState.currentClickable_ui, &gameState.frame_event);

		if (gameState.frame_event._sent == 1)
		{
			event_relay();

			gameState.frame_event._sent = 0;
		}

		return;

	};
	
	if (gameState.currentClickable_entity) {
		entity_onClick(gameState.currentClickable_entity, &gameState.frame_event);

		if (gameState.frame_event._sent == 1)
		{
			event_relay();
			
			gameState.frame_event._sent = 0;
		}

		return;
	};
}

void processKeys(Uint8 keys, Uint32 mouse) {

	SDL_Event frame_event;

	Menu_State* temp;

	//Process the frame event itself
	while (SDL_PollEvent(&frame_event)) {

		switch (frame_event.type) {

		case SDL_MOUSEBUTTONDOWN:

			if (frame_event.button.button == SDL_BUTTON_LEFT)
				onClick_left();

			break;

		case SDL_KEYDOWN:

			if (frame_event.key.keysym.sym == SDLK_BACKSPACE)
			{
				if (gameState.player_menustate != NULL)
				{

					//Go back one menu_state
					gameState.player_menustate = menu_state_back(gameState.player_menustate);

					//If the new menu_state exists, show it
					if (gameState.player_menustate != NULL)
					{
						menu_state_show(gameState.player_menustate);
					}

					//Otherwise, set our current menu_state to NULL

				}
			}

			else
				KEYS[frame_event.key.keysym.sym] = 1;

			break;

		case SDL_KEYUP:
			KEYS[frame_event.key.keysym.sym] = 0;

			break;

		default:
			break;
		}
	}

}

void starsabove_exit()
{
	gameState.currentClickable_entity = NULL;
	gameState.currentClickable_ui = NULL;

	slog("Freed game metadata");
}