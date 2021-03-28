#pragma once

#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "simple_json.h"

#include "starsabove_resources.h"

enum buildableStatus
{
	BLD_CONSTRUCTING = -1,
	BLD_ACTIVE,
	BLD_DISABLED
};

typedef struct
{

	int status;

	char name[128];
	float* resource_input;
	float* resource_output;

} Buildable;

Buildable* buildable_fromJson(SJson* buildable_json);

Buildable* buildable_new(int status, char* name, float* input, float* output);

void buildable_free(Buildable* buildable);

SJson* buildable_toJson(Buildable* buildable);

#endif