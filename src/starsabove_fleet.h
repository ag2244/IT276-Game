#pragma once

#ifndef __FLEET_H__
#define __FLEET_H__

#include "simple_json.h"

#include "starsabove_entity.h"

#include "starsabove_resources.h"
#include "starsabove_ui_textbox.h"

typedef struct Fleet;

static const int num_Ship_types = 5;
static const int max_Ships = 50;

typedef enum shipStatus
{
	SHIP_TEMPLATE,
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
	shipStatus status;

	float* maintenance;

	struct Fleet* fleet;

} Ship;

typedef struct Fleet_s
{

	char name[128];

	Ship* Ships;

	Entity* location;

} Fleet;

void shiptemplates_load(SJson* ships_json);

void shiptemplates_free();

Ship* getshipbyname(char shiptype[128]);

Ship* ship_copy(Ship* src, Fleet* fleet);

Ship* ship_fromJson(SJson* ship_json, Fleet* fleet);

Ship* ship_init(char shiptype[128], float* costs, Fleet* fleet);

Fleet* fleet_fromjson(SJson* fleet_json);

Fleet* fleet_init(char name[128]);

Menu_State* fleet_menustate(Fleet* fleet);

void ship_free(Ship* self);

SJson* ship_toJson(Ship* self);

void fleet_free(Fleet* self);

SJson* fleet_toJson(Fleet* self);

#endif