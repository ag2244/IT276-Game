#pragma once

#ifndef __UNITS_SPY_H
#define __UNITS_SPY_H

#include "starsabove_units.h"

#include "starsabove_ui_textbox.h"

typedef struct Nation;

static const int max_national_spies = 20;

typedef enum spy_status
{
	SPY_TEMPLATE = 0,
	SPY_TRAINING,
	SPY_TURNEND,
	SPY_ACTIVE,
	SPY_SABOTAGING,
	SPY_STEALING,
	SPY_UNCOVERING
}
spy_status;

static char spy_status_names[7][128] =
{
	"Unbuilt",
	"Training",
	"Turn Over",
	"Active",
	"Sabotaging",
	"Stealing",
	"Uncovering"
};

typedef struct Spy_s
{
	int status;

	int buildtime;

	float* maintenance;
	float* costs;

	char location[128];
	char sublocation[128];

	Nation* owner;

	Bool _inuse;

} Spy;

Spy* spyfromlist(Spy* spylist, int i);

float* spy_onNewTurn(Spy* self);

Menu_State* spy_menustate(Spy* self, Menu_State* previous, Bool forconstruction);

Menu_State* spies_menustate(Spy* spies, int numspies, Menu_State* previous, Bool forconstruction, Bool inlocation, char location[128], char sublocation[128]);

Spy* spy_fromJson(SJson* spy_json);
Spy* spies_fromJson(SJson* spies_array);

Spy* spy_init(int status, int buildtime, float* maintenance, float* costs, char location[128], char sublocation[128]);

SJson* spy_toJson(Spy* self);
SJson* spies_toJson(Spy* spies_array);

void spy_free(Spy* self);

#endif
