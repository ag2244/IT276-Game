#pragma once

#ifndef __FLEET_H__
#define __FLEET_H__

#include "simple_json.h"

#include "starsabove_entity.h"

#include "starsabove_resources.h"
#include "starsabove_ui_textbox.h"

typedef struct Nation;

typedef struct Fleet;

static const int max_ships = 30;
static const int max_national_fleets = 100;

typedef enum shipStatus
{
	SHIP_TEMPLATE = -1,
	SHIP_CONSTRUCTING,
	SHIP_ACTIVE,
	SHIP_MOVING,
	SHIP_DISABLED
} 
shipStatus;

static const int num_ship_statuses = 5;
static char ship_status_names[5][128] =
{
	"Unbuilt",
	"Constructing",
	"Active",
	"Moving",
	"Disabled"
};

typedef struct
{
	char type[128];

	int status;
	int health;

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

Fleet* fleet_fromlist(Fleet* fleet, int i);

void shiptemplates_load(SJson* ships_json);

void shiptemplates_free();

Ship* getshipbyname(char shiptype[128]);

Ship* ship_copy(Ship* src, Fleet* fleet);

Ship* ship_fromJson(SJson* ship_json, Fleet* fleet);

Ship* ship_init(char shiptype[128], float* maintenance, float* costs, int health, int status, Fleet* fleet);

Fleet* fleet_fromjson(SJson* fleet_json);

Fleet* fleetlist_fromJson(SJson* fleets_json);

Fleet* fleet_init(char name[128], int status, char location[128]);

Menu_State* ship_menustate(Ship* ship, Menu_State* previous, Bool forconstruction, Game_Event* event_template);

Menu_State* ships_menustate(Ship* ships, Menu_State* previous, Bool in_fleet, Game_Event* event_template);

Menu_State* fleet_menustate(Fleet* fleet, Menu_State* previous);

float* fleet_totalmaintenance(Fleet* fleet);

int fleet_addShip(Fleet* fleet, Ship* ship);

int fleetlist_addFleet(Fleet* fleetlist, Fleet* fleet);

void ship_free(Ship* self);

SJson* ship_toJson(Ship* self);

void fleet_free(Fleet* self);

SJson* fleet_toJson(Fleet* self);

SJson* fleetlist_toJson(Fleet* fleetlist);

#endif