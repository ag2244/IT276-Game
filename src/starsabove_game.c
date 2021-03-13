#include <SDL.h>

#include "simple_logger.h"

#include "starsabove_game.h"
GameState gameState = { 0 };

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
	
	if (load_systems(game) == NULL)
	{
		slog("Was unable to load this game's star systems");
		return NULL;
	}

	if (load_nations(game) == NULL)
	{
		slog("Was unable to load this game's nations");
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

	if (get_nation_by_name("Nation1"))
	{
		slog("Nation exists with name \"Nation1\"!");
	}
}

void prepare_game()
{
	int i = 0; EntityManager* entity_manager = entity_manager_get();

    /* Starting the entity manager */
    entity_manager_init(100);

	/* Starting the ui manager */
	ui_manager_init(50);

	/* Starting the nations list */
	nations_list_init(24);

	gametext_init(vector2d(0, 700));

	// Set up the debug world
	test();

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

		printf("NAME: \"%s\"\n\n", entity_manager->entity_list[i].name);

	}

	//*/

	/*

	int i = 0; Nation_List* nation_list = nation_list_get();

	for (i = 0; i < nation_list->max_nations; i++)
	{

		if (nation_list->nations[i]._inuse == 0) continue; // Someone else using this one

		slog("NAME: \"%s\"\n\n", nation_list->nations[i].name);
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
	if (gameState.currentClickable_entity) {
		entity_onClick(gameState.currentClickable_entity);
	};
}

void processKeys(Uint8 keys, Uint32 mouse) {

	SDL_Event frame_event;

	while (SDL_PollEvent(&frame_event)) {

		switch (frame_event.type) {

		case SDL_MOUSEBUTTONDOWN:

			if (frame_event.button.button == SDL_BUTTON_LEFT)
				onClick_left();

			break;

		default:
			break;
		}
	}

}