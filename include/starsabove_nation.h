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

//asd

#endif