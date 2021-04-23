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
	SHIP_DISABLED
} 
shipStatus;

static char ship_status_names[4][128] =
{
	"Unbuilt",
	"Constructing",
	"Active",
	"Disabled"
};

typedef struct
{
	char type[128];

	int status;
	int health;

	float* maintenance;

	struct Fleet* fleet;

	Bool _inuse;

} Ship;

typedef struct
{

	char name[128];

	Ship* ships;

	char location[128];

	Bool _inuse;

} Fleet;

Fleet* fleet_fromlist(Fleet* fleet, int i);

void shiptemplates_load(SJson* ships_json);

void shiptemplates_free();

Ship* getshipbyname(char shiptype[128]);

Ship* ship_copy(Ship* src, Fleet* fleet);

Ship* ship_fromJson(SJson* ship_json, Fleet* fleet);

Ship* ship_init(char shiptype[128], float* costs, int health, int status, Fleet* fleet);

Fleet* fleet_fromjson(SJson* fleet_json);

Fleet* fleetlist_fromJson(SJson* fleets_json);

Fleet* fleet_init(char name[128], Entity* location);

Menu_State* fleet_menustate(Fleet* fleet);

void ship_free(Ship* self);

SJson* ship_toJson(Ship* self);

void fleet_free(Fleet* self);

SJson* fleet_toJson(Fleet* self);

SJson* fleetlist_toJson(Fleet* fleetlist);

#endif