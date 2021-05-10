#pragma once

#include <SDL.h>
#include "gfc_types.h"
#include "gfc_vector.h"

#include "gf2d_sprite.h"

#include "simple_json.h"
#include "simple_logger.h"

#include "starsabove_nation.h"

#include "starsabove_resources.h"
#include "starsabove_entity.h"
#include "starsabove_ui_textbox.h"

#include "starsabove_fleet.h"
#include "starsabove_units_spy.h"

Nation_List nation_list = { 0 };

int load_nations(SJson* game_json)
{
	Nation* nation = NULL;

	//The nations in this game
	SJson* nations;
	SJson* currentNation;
	SJson* controlled_systems;

	char* name = NULL;
	
	int i; int j;

	Fleet* thisfleet;

	//game["Nations"]
	nations = sj_object_get_value(game_json, "Nations");

	if (!nations)
	{
		slog("Save file does not have any nations"); return NULL;
	}

	if (!sj_is_array(nations))
	{
		slog("Nations is not an array!"); return NULL;
	}

	//Go through the nations
	for (i = 0; i < nations->v.array->count; i++)
	{
		currentNation = sj_array_get_nth(nations, i);

		//Get the name
		name = sj_get_string_value(sj_object_get_value(currentNation, "name"));

		if (name == 0) {

			slog("Cannot use '0' name!"); return NULL;

		}

		nation = nation_add(
			name, 
			resources_fromJson(sj_object_get_value(currentNation, "resources")), 
			fleetlist_fromJson(sj_object_get_value(currentNation, "fleets")),
			spies_fromJson(sj_object_get_value(currentNation, "spies"))
		);

		if (!nation)
		{
			slog("Could not add nation \"%s\"", name); return NULL;
		}

		for (j = 0; j < max_national_fleets; j++)
		{
			thisfleet = fleet_fromlist(nation->fleets, i);

			if (!thisfleet) { continue; }

			thisfleet->owner = nation;
		}


	}

	slog("All nations have been loaded!");
	return 1;
}

void nations_list_init(Uint32 max_nations)
{
	if (max_nations == 0) {
		slog("Cannot allocate 0 nations!");
		return;
	}

	if (nation_list.nations != NULL) {
		nations_list_free();
	}

	nation_list.nations = (Nation*)gfc_allocate_array(sizeof(Nation), max_nations);

	if (nation_list.nations == NULL) {
		slog("Failed to allocate entity list!");
		return;
	}

	nation_list.max_nations = max_nations;

	atexit(nations_list_free);
	slog("Nations list initialized");
}

Nation* get_nation_by_name(char* name)
{
	int i;
	Nation* current_nation;

	if (nation_list.nations == NULL) {
		slog("Nations list does not exist!");
		return;
	}

	if (name == NULL) {
		slog("Cannot get an nation with NULL name");
		return;
	}

	for (i = 0; i < nation_list.max_nations; i++)
	{
		current_nation = &nation_list.nations[i];

		if (current_nation->_inuse && current_nation->name)
		{
			if (strcmp(current_nation->name, name) == 0)
				return &nation_list.nations[i];
		}
	}

	slog("No Nation with the name: \"%s\"", name);
	return NULL;
}

Nation_List* nation_list_get()
{

	if (nation_list.nations == NULL) {
		slog("Cannot get an empty nations list");
		return;
	}

	return &nation_list;
}

void nations_list_free()
{
	if (nation_list.nations != NULL) {
		free(nation_list.nations);
	}

	memset(&nation_list, 0, sizeof(Nation_List));
	slog("Nations List freed");
}

void nation_list_reciever(Game_Event* event)
{
	int i;
	Nation* current_nation;

	if (nation_list.nations == NULL) {
		slog("NATIONS LIST DOES NOT EXIST!");
		return;
	}

	if (event == NULL) {
		slog("CANNOT RECIEVE A NULL EVENT");
		return;
	}

	for (i = 0; i < nation_list.max_nations; i++)
	{
		current_nation = &nation_list.nations[i];

		if (!current_nation->_inuse)
		{
			continue;
		}

		if (strcmp(current_nation->name, event->target_id) == 0)
		{
			nation_reciever(current_nation, event);
		}
	}
}

Nation* nation_new(Nation* nation, char* name, float* resources, Fleet* fleets, Spy* spies)
{

	gfc_line_cpy(nation->name, name);

	nation->resources_total = malloc(6 * sizeof(float));

	for (int i = 0; i < numresources; i++)
	{
		nation->resources_total[i] = resources[i];
	}

	nation->fleets = fleets;
	nation->spies = spies;

	nation->toJson = nation_toJson;

	nation->_is_player = 0;

	return nation;
}

Nation* nation_add(char* name, float* resources, Fleet* fleets, Spy* spies)
{
	int i;

	if (nation_list.nations == NULL) {
		slog("Nations list does not exist!");
		return;
	}

	for (i = 0; i < nation_list.max_nations; i++)
	{

		if (nation_list.nations[i]._inuse) continue; // Someone else using this one
		memset(&nation_list.nations[i], 0, sizeof(Nation));

		nation_list.nations[i]._inuse = 1;

		nation_new(&nation_list.nations[i], name, resources, fleets, spies);

		return &nation_list.nations[i];
	}

	slog("No free nations available");
	return NULL;
}

void nation_onNewTurn(Nation* nation)
{
	int i;

	EntityManager entitymgr = *entity_manager_get();
	Entity_p* thisSystem;

	Fleet* thisfleet = NULL;

	float* resources = malloc(numresources * sizeof(float));

	for (i = 0; i < 6; i++) { resources[i] = 0; }

	for (i = 0; i < entitymgr.max_entities; i++)
	{

		if (entitymgr.entity_list[i]._inuse == 0) {continue; }

		thisSystem = &entitymgr.entity_list[i];

		if (thisSystem->owner == nation)
		{
			resources = resourcelist_add(thisSystem->onNewTurn(thisSystem), resources);
		}

	}

	for (i = 0; i < max_national_fleets; i++)
	{
		thisfleet = fleet_fromlist(nation->fleets, i);

		if (!thisfleet) { continue; }

		resources = resourcelist_add (fleet_onNewTurn(thisfleet), resources);

	}

	resources = resourcelist_add(nation->resources_total, resources);

	for (i = 0; i < 6; i++)
	{
		nation->resources_total[i] = resources[i];
	}
}

void nations_list_onNewTurn()
{
	int i;

	//Go through each nation and perform their onNewTurn() function
	for (i = 0; i < nation_list.max_nations; i++)
	{
		if (nation_list.nations[i]._inuse)
		{
			nation_onNewTurn(&nation_list.nations[i]);
		}
	}
}

void nation_reciever(Nation* nation, Game_Event* event)
{

	if (nation == NULL)
	{
		slog("CANNOT RECIEVE EVENT FOR NULL NATION"); return;
	}

	if (event == NULL)
	{
		slog("CANNOT RECIEVE NULL EVENT FOR NATION"); return;
	}

	//If the command is to move a fleet
	if (strcmp(event->command, "MOVETO") == 0)
	{
		Fleet* thisfleet;

		thisfleet = nation_fleetbyname(nation, event->sub_target_id, 0);

		if (!thisfleet)
		{
			slog("UNABLE TO GET FLEET FOR REQUESTED MOVE ACTION"); return;
		}

		if ((thisfleet->status != (int)SHIP_ACTIVE) && (thisfleet->status != (int)SHIP_PARTIALLY_ACTIVE)) { slog("Fleet cannot move"); return; }

		strcpy(thisfleet->location, event->descriptor);

		thisfleet->status = (int)SHIP_TURNEND;
	}

}

Fleet* nation_new_fleet(Nation* self, char location_name[128])
{
	int i;
	int fleetnum = 1;
	char fleet_name[128];

	Fleet* thisfleet;
	Fleet* newFleet;

	sprintf(fleet_name, "%s Fleet %i", self->name, fleetnum);

	for (i = 0; i < max_national_fleets; i++)
	{
		thisfleet = fleet_fromlist(self->fleets, i);

		if (!thisfleet) { continue; }

		if (strcmp(thisfleet->name, fleet_name) == 0)
		{
			fleetnum++;

			sprintf(fleet_name, "%s Fleet %i", self->name, fleetnum);
		}
	}

	newFleet = fleet_init(fleet_name, 0, location_name);
	newFleet->owner = self;

	newFleet->status = (int)SHIP_CONSTRUCTING;

	if (fleetlist_addFleet(self->fleets, newFleet) == 1)
	{
		return newFleet;
	}

	return NULL;
}

struct Fleet* nation_fleetbylocation(Nation* self, Entity_p* location, Bool createnew)
{
	int i;

	Fleet* thisfleet = NULL;

	if (!self)
	{
		slog("CANNOT RETRIEVE FLEET FROM NULL NATION"); return NULL;
	}

	if (!location)
	{
		slog("CANNOT RETRIEVE FLEET FROM NULL LOCATION"); return NULL;
	}

	for (i = 0; i < max_national_fleets; i++)
	{
		thisfleet = fleet_fromlist(self->fleets, i);

		if (!thisfleet) { continue; }

		if (strcmp(thisfleet->location, location->name) == 0)
		{
			return thisfleet;
		}
	}

	if (createnew == 1)
	{
		thisfleet = nation_new_fleet(self, location->name);
	}

	return thisfleet;
}

struct Fleet* nation_fleetbyname(Nation* self, char name[128], Bool createnew)
{
	int i;

	Fleet* thisfleet = NULL;

	if (!self)
	{
		slog("CANNOT RETRIEVE FLEET FROM NULL NATION"); return NULL;
	}

	if (!name)
	{
		slog("CANNOT RETRIEVE FLEET WITH NULL NAME"); return NULL;
	}

	for (i = 0; i < max_national_fleets; i++)
	{
		thisfleet = fleet_fromlist(self->fleets, i);

		if (!thisfleet) { continue; }

		if (strcmp(thisfleet->name, name) == 0)
		{
			return thisfleet;
		}
	}

	if (createnew == 1)
	{
		//thisfleet = nation_new_fleet(self, location->name);
	}

	if (!thisfleet)
	{
		slog("COULD NOT FIND FLEET WITH NAME \"%s\"", name);
	}

	return thisfleet;
}

void nation_menustate_national_fleets(Nation* self, Menu_State* nation_menustate)
{
	char i;

	UI_Element* all_fleets_textbox;
	UI_Element* single_fleet_textbox;

	Menu_State* all_fleets_menustate;

	Fleet* thisfleet;

	all_fleets_textbox = textbox_init
	(
		vector2d(10, 10),
		vector2d(250, 50),
		"National Fleets",
		font_load("resources/fonts/futur.ttf", 12)
	);

	all_fleets_menustate = menu_state_new(
		nation_menustate,
		textbox_init
		(
			vector2d(10, 10),
			vector2d(250, 50),
			"National Fleets",
			font_load("resources/fonts/futur.ttf", 16)
		),
		NULL,
		vector2d(10, 10),
		0,
		5
	);

	all_fleets_textbox->signal = new_gameevent(
		self->name,
		"FLEETS",
		"SHOW_ALL",
		NULL,
		NULL,
		all_fleets_menustate,
		0
		);

	for (i = 0; i < max_national_fleets; i++)
	{
		thisfleet = fleet_fromlist(self->fleets, i);

		if (!thisfleet) { continue; }

		single_fleet_textbox = textbox_init
		(
			vector2d(10, 10),
			vector2d(250, 50),
			thisfleet->name,
			font_load("resources/fonts/futur.ttf", 12)
		);

		single_fleet_textbox->signal = new_gameevent(
			self->name,
			"FLEETS",
			"SHOW_ALL",
			NULL,
			NULL,
			fleet_menustate(thisfleet, all_fleets_menustate),
			0
		);

		single_fleet_textbox->signal->eventsound = gfc_sound_load("audio/Ship.mp3", 0.5, 1);

		menu_addTo(all_fleets_menustate->current_menu, single_fleet_textbox);
	}

	menu_state_hide(all_fleets_menustate);

	menu_addTo(nation_menustate->current_menu, all_fleets_textbox);

}

void nation_menustate_spies(Nation* self, Menu_State* nation_menustate)
{
	char i;

	UI_Element* all_spies_textbox;
	UI_Element* single_spy_textbox;

	Menu_State* all_spies_menustate;

	Spy* thisSpy;

	char spyname[128];

	all_spies_textbox = textbox_init
	(
		vector2d(10, 10),
		vector2d(250, 50),
		"National Spies",
		font_load("resources/fonts/futur.ttf", 12)
	);

	all_spies_menustate = menu_state_new(
		nation_menustate,
		textbox_init
		(
			vector2d(10, 10),
			vector2d(250, 50),
			"National Spies",
			font_load("resources/fonts/futur.ttf", 16)
		),
		NULL,
		vector2d(10, 10),
		0,
		5
	);

	all_spies_textbox->signal = new_gameevent(
		self->name,
		"SPIES",
		"SHOW_ALL",
		NULL,
		NULL,
		all_spies_menustate,
		0
	);

	for (i = 0; i < max_national_fleets; i++)
	{
		thisSpy = spyfromlist(self->spies, i);

		if (!thisSpy) { continue; }

		sprintf(spyname, "%s | %s, %s", spy_status_names[thisSpy->status], thisSpy->sublocation, thisSpy->location);

		single_spy_textbox = textbox_init
		(
			vector2d(10, 10),
			vector2d(250, 50),
			spyname,
			font_load("resources/fonts/futur.ttf", 10)
		);

		single_spy_textbox->signal = new_gameevent(
			self->name,
			"Spies",
			"SHOW_ALL",
			NULL,
			NULL,
			spy_menustate(thisSpy, all_spies_menustate, 0),
			0
		);

		single_spy_textbox->signal->eventsound = gfc_sound_load("audio/Spy.mp3", 0.5, 1);

		menu_addTo(all_spies_menustate->current_menu, single_spy_textbox);
	}

	menu_state_hide(all_spies_menustate);

	menu_addTo(nation_menustate->current_menu, all_spies_textbox);
}

struct Menu_State* nation_menustate(Nation* nation, Bool _isPlayer)
{

	int i;

	char temp[128];

	UI_Element* system_textbox;
	UI_Element* resources_textbox;

	Menu_State* nation_menustate;

	nation_menustate = menu_state_new(
		NULL,
		textbox_init
		(
			vector2d(10, 10),
			vector2d(200, 50),
			nation->name,
			font_load("resources/fonts/futur.ttf", 20)
		),
		NULL,
		vector2d(10, 10),
		0,
		5
	);

	/*
	menu_addTo(
		nation_menustate->current_menu,
		textbox_init
		(
			vector2d(10, 10),
			vector2d(250, 50),
			"Owned Systems",
			font_load("resources/fonts/futur.ttf", 12)
		)
	);*/

	//Add resources

	resources_textbox = textbox_init
	(
		vector2d(10, 10),
		vector2d(250, 50),
		"Total Resources",
		font_load("resources/fonts/futur.ttf", 12)
	);

	resources_textbox->signal = new_gameevent(
		nation->name,
		NULL,
		"GETRESOURCES",
		NULL,
		0,
		resources_menustate_init(nation->resources_total, nation_menustate, "Total Resources", 1),
		0
	);

	menu_addTo(
		nation_menustate->current_menu,
		resources_textbox
	);

	nation_menustate_national_fleets(nation, nation_menustate);

	nation_menustate_spies(nation, nation_menustate);

	menu_state_show(nation_menustate);

	return nation_menustate;
}

SJson* nation_toJson(Nation* self)
{
	int i;
	SJson* fleetlist;
	SJson* spieslist;
	SJson* nation_json = sj_object_new();

	sj_object_insert(nation_json, "name", sj_new_str(self->name));

	sj_object_insert(nation_json, "resources", resources_toJson(self->resources_total));

	fleetlist = fleetlist_toJson(self->fleets); 
	if (sj_array_get_count(fleetlist) > 0) { sj_object_insert(nation_json, "fleets", fleetlist); }

	spieslist = spies_toJson(self->spies);
	if (sj_array_get_count(spieslist) > 0) { sj_object_insert(nation_json, "spies", spieslist); }

	return nation_json;
}

SJson* allNations_toJson()
{
	SJson* nations = sj_array_new();

	int i;
	Nation* thisNation;

	//Go through each nation in the nation list
	for (i = 0; i < nation_list.max_nations; i++)
	{
		//If the nation at index i is not in use, continue
		if (nation_list.nations[i]._inuse == 0) continue;

		thisNation = &nation_list.nations[i];

		sj_array_append(nations, thisNation->toJson(thisNation));
	}

	return nations;
}

void nation_free(Nation* nation)
{
	if (!nation)
	{
		slog("Cannot free a NULL nation");
		return;
	}

	//free(nation->controlled_systems);
	nation->_inuse = 0;
	//nation->name = NULL;
}