#pragma once

#ifndef __FLEET_H__
#define __FLEET_H__

#include "simple_json.h"

#include "starsabove_entity.h"

#include "starsabove_resources.h"
#include "starsabove_ui_textbox.h"

typedef struct Nation;
typedef struct Unit;

typedef struct Fleet;

static const int max_ships = 30;
static const int max_national_fleets = 100;

typedef enum shipStatus
{
	SHIP_TEMPLATE = 0,
	SHIP_CONSTRUCTING,
	SHIP_ACTIVE,
	SHIP_PARTIALLY_ACTIVE,
	SHIP_TURNEND,
	SHIP_DISABLED
} 
shipStatus;

static const int num_ship_statuses = 6;
static char ship_status_names[6][128] =
{
	"Unbuilt",
	"Constructing",
	"Active",
	"Partially Active",
	"Turn Over",
	"Disabled"
};

typedef struct
{
	char type[128];

	int status;
	int health;

	int buildtime;

	int transport_capacity;
	struct Unit* embarked_units;

	float* supply_capacity;
	float* supply;

	float* maintenance;

	float* costs;

	struct Fleet* fleet;

	Bool _inuse;

} Ship;

typedef struct
{

	char name[128];
	int status;

	Ship* ships;

	char location[128];
	Nation* owner;

	Bool _inuse;

} Fleet;

/*
* @brief get a fleet from a list of fleets at index i
* @return A fleet, or NULL on error
*/
Fleet* fleet_fromlist(Fleet* fleetlist, int i);

/*
* @brief load all the ships from a gamedict's list of ships as templates
*/
void shiptemplates_load(SJson* ships_json);

/*
* @brief free the list of ship templates
*/
void shiptemplates_free();

/*
* @brief Get a ship from the list of ship templates by name string
* @return NULL on error, or a ship with the specified name
*/
Ship* getshipbyname(char shiptype[128]);

/*
* @brief Get a new ship with all the same information as an already existing one
* @param fleet The fleet that the ship will be part of
* @return NULL on error, or a new ship
*/
Ship* ship_copy(Ship* src, Fleet* fleet);

/*
* @brief Get a new ship from a json object
* @param fleet The fleet that the ship will be part of
* @return NULL on error, or a new ship
*/
Ship* ship_fromJson(SJson* ship_json, Fleet* fleet);

/*
* @brief Get a new ship from ship params
* @return NULL on error, or a new ship
*/
Ship* ship_init(char shiptype[128], float* maintenance, float* costs, float* supply, float* supply_cap, int health, int status, int buildtime, Fleet* fleet);

/*
* @brief Get a new fleet from a json object
* @return NULL on error, or a new fleet
*/
Fleet* fleet_fromjson(SJson* fleet_json);

/*
* @brief Get a list of fleets from a json object
* @return NULL on error, or an array of fleets
*/
Fleet* fleetlist_fromJson(SJson* fleets_json);

/*
* @brief Initialize a new fleet from fleet params
* @return NULL on error, or a new fleet
*/
Fleet* fleet_init(char name[128], int status, char location[128]);

/*
* @brief Create a new menustate for a ship
* @param forconstruction If this menustate is for a construction option or not
* @return NULL on error, or a new menu state
*/
Menu_State* ship_menustate(Ship* ship, Menu_State* previous, Bool forconstruction, Game_Event* event_template);

/*
* @brief Create a new menustate for a list of ships
* @param in_fleet whether or not these ships are in a fleet
* @return NULL on error, or a new menu state
*/
Menu_State* ships_menustate(Ship* ships, Menu_State* previous, Bool in_fleet, Game_Event* event_template);

/*
* @brief Create a new menustate for a fleet
* @return NULL on error, or a new menu state
*/
Menu_State* fleet_menustate(Fleet* fleet, Menu_State* previous);

/*
* @brief Get the total resource maintenance of a fleet
* @return NULL on error, or a new array of fleets
*/
float* fleet_totalmaintenance(Fleet* fleet);

/*
* @brief Do the new turn operations for a ship
* @return NULL on error, or a new array of floats
*/
float* ship_onNewTurn(Ship* self);

/*
* @brief Do the new turn operations for a fleet
* @return NULL on error, or a new array of floats
*/
float* fleet_onNewTurn(Fleet* self);

/*
* @brief Add a ship to a fleet
* @return 0 if we cannot add the ship, 1 if we can
*/
int fleet_addShip(Fleet* fleet, Ship* ship);

/*
* @brief Add a fleet to a fleet lst
* @return 0 if we cannot add the fleet, 1 if we can
*/
int fleetlist_addFleet(Fleet* fleetlist, Fleet* fleet);

/*
* @brief Free a ship
*/
void ship_free(Ship* self);

/*
* @brief Save a ship to a json object
* @return NULL on error, or a ship JSON object
*/
SJson* ship_toJson(Ship* self);

/*
* @brief Free a fleet
*/
void fleet_free(Fleet* self);

/*
* @brief Save a fleet to a json object
* @return NULL on error, or a fleet JSON object
*/
SJson* fleet_toJson(Fleet* self);

/*
* @brief Save a fleet list to a json object
* @return NULL on error, or a fleet list JSON object
*/
SJson* fleetlist_toJson(Fleet* fleetlist);

#endif