#pragma once

#ifndef __UNITS_H__
#define __UNITS_H__

#include "simple_json.h"

#include "gfc_types.h";

#include "starsabove_resources.h"
#include "starsabove_ui_textbox.h"

typedef struct Nation;

static const int max_national_units = 1000;

typedef enum unit_type
{
	UNIT_TEMPLATE = 0,
	UNIT_MARINE,
	UNIT_SPY,
	UNIT_COLONIST
}
unit_type;

typedef enum location_type
{
	LOCATION_TEMPLATE = 0,
	LOCATION_STRING,
	LOCATION_FLEET,
	LOCATION_PLANET
}
location_type;

typedef struct Unit_s
{
	unit_type type;
	int health;
	int status;

	int buildtime;

	float* maintenance;
	float* costs;

	void* location;
	location_type locationtype;

	struct Unit_s* (*unit_copy) (struct Unit_s* self);

	float* (*onNewTurn)(struct Unit_s* self);

	Menu_State* (*unit_menustate)(struct Unit_s* self, Menu_State* previous, Bool forconstruction);

	struct Unit_s* (*fromJson) (SJson* unit_json);
	SJson* (*toJson) (struct Unit_s* self);

	void (*free) (struct Unit_s* self);

	Bool _inuse;

} Unit;

Unit* unit_fromJson(SJson* unit_json);

Unit* unit_new(int type, int health, int status, int buildtime, float* maintenance, float* costs);

Unit* unit_init();

Menu_State* unit_menustate(Unit* self, Menu_State* previous, Bool forconstruction);

float* unit_onNewTurn(Unit* self);

SJson* unit_toJson(Unit* self);

Unit* unit_free();

#endif