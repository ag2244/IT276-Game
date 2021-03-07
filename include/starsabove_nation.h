#pragma once

#ifndef __NATION_H__
#define __NATION_H__

#include "starsabove_entity.h"
#include "starsabove_system.h"
#include "starsabove_nation.h"

typedef struct Nation_s
{
	char* name;
	Entity* controlled_systems;

} Nation;

typedef struct Nations_s
{
	Nation* nations;
	Uint32 max_nations;

} Nation_List;


void nations_list_init(Uint32 max_entities);

Nation* get_nation_by_name(char* name);

void nations_list_free();

void nation_init(char* name, Uint32 max_systems);

void nation_free(Nation* nation);

#endif