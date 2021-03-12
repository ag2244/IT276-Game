#include <SDL.h>

#include "simple_logger.h"

#include "starsabove_game.h"
GameState gameState = { 0 };

int load_systems(SJson* game)
{
	//The star systems in this game
	SJson* systems;
	SJson* currentSystem;

	char* name = NULL, temp = NULL;

	SJson* positionJson;
	Vector2D pos;
	float x, y;

	int i;

	//game["Systems"]
	systems = sj_object_get_value(game, "Systems");

	if (!systems)
	{
		slog("Save file does not have any star systems"); return NULL;
	}

	if (!sj_is_array(systems))
	{
		slog("Star Systems is not an array!"); return NULL;
	}

	//Go through the systems
	for (i = 0; i < systems->v.array->count; i++)
	{
		currentSystem = sj_array_get_nth(systems, i);

		positionJson = sj_object_get_value(currentSystem, "position");
		
		//Set up the position vector

		sj_get_float_value(sj_array_get_nth(positionJson, 0), &x); //float value of json object containing the value at currentSystem["position"][0]
		sj_get_float_value(sj_array_get_nth(positionJson, 1), &y); //float value of json object containing the value at currentSystem["position"][1]

		pos = vector2d(x, y);


		/*slog("ASD");
		//Get the name: string value of json object containing the value at currentSystem["name"]
		temp = sj_get_string_value(sj_object_get_value(currentSystem, "name"));
		slog("QWE");

		strcpy(name, temp);
		slog(name);*/

		name = sj_get_string_value(sj_object_get_value(currentSystem, "name"));

		if (name == 0) {

			slog("Cannot use '0' name!"); return NULL;

		}

		system_spawn(name, pos);

	}

	slog("All systems have been spawned!");
	return 1;
}

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

	strcat(filename, savefile_copy);

	sj_save(savefile, filename);

}

void test() 
{

	load_game("jsondata/Test Input.json");

	save_game("TESTOUT.json");

	if (get_entity_by_name("System1"))
	{
		slog("Entity exists with name \"System1\"!");
	}

	if (get_entity_by_name("System2"))
	{
		slog("Entity exists with name \"System2\"!");
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
	nation_new("Testistan1", 50);

	gametext_init(vector2d(0, 700));

	// Set up the debug world
	test();

	slog("============ GAME LOADED! ============");

}

void starsabove_loop()
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