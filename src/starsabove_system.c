#pragma once
#include "simple_logger.h"
#include "gfc_vector.h"

#include "starsabove_system.h"

//Use for pointing to the data for an individual system
System_Data* system_data;

System_Data* system_data_new(Uint32 numPlanets)
{

    System_Data* systemdata;
    Planet* planets;

    if (numPlanets == NULL)
        numPlanets = 0;

    planets = gfc_allocate_array(sizeof(Planet), numPlanets);
    systemdata = malloc(sizeof(System_Data));

    systemdata->num_planets = numPlanets;
    systemdata->planets = planets;

    return systemdata;
}

int load_systems(SJson* game_json)
{
    //The star systems in this game
    SJson* systems;
    SJson* currentSystem;
    SJson* planets;

    System_Data* systemdata;

    char* name = NULL;
    char* temp = NULL;

    Nation* nation = NULL;

    SJson* positionJson;
    Vector2D pos;
    float x, y;

    int i, j;
    int numPlanets;

    //game["Systems"]
    systems = sj_object_get_value(game_json, "Systems");

    if (!systems)
    {
        slog("Save file does not have any star systems"); return NULL;
    }

    if (!sj_is_array(systems))
    {
        slog("Star Systems is not an array!"); return NULL;
    }

    //Go through the systems
    for (i = 0; i < systems->v.array->count; i++)
    {
        currentSystem = sj_array_get_nth(systems, i);

        positionJson = sj_object_get_value(currentSystem, "position");

        //Set up the position vector

        sj_get_float_value(sj_array_get_nth(positionJson, 0), &x); //float value of json object containing the value at currentSystem["position"][0]
        sj_get_float_value(sj_array_get_nth(positionJson, 1), &y); //float value of json object containing the value at currentSystem["position"][1]

        pos = vector2d(x, y);

        //Get the name

        name = sj_get_string_value(sj_object_get_value(currentSystem, "name"));

        if (name == 0) {

            slog("Cannot use '0' name!"); return NULL;

        }

        //Get the planets

        planets = sj_object_get_value(currentSystem, "planets");

        if (!sj_is_array(planets))
        {
            slog("Star System Planets is not an array!"); return NULL;
        }

        numPlanets = sj_array_get_count(planets);

        systemdata = system_data_new(numPlanets);

        for (j = 0; j < numPlanets; j++)
        {

            systemdata->planets[j] = *planet_fromJson(sj_array_get_nth(planets, j));

        }

        //Get the owner nation

        temp = sj_get_string_value(sj_object_get_value(currentSystem, "owner"));

        if (temp == 0) {

            slog("Cannot use '0' name!"); return NULL;

        }

        if (strcmp(temp, "null") != 0)
        {
            nation = get_nation_by_name(temp);

            if (nation == NULL) {

                slog("Owner nation \"%s\" does not exist!", temp); return NULL;

            }
        }

        else nation = NULL;

        system_spawn(name, pos, nation, systemdata);

    }

    slog("All systems have been spawned!");
    return 1;
}

void system_onClick(Entity* self, Game_Event* event_reciever) 
{
    if (self == NULL)
    {
        slog("Cannot click on NULL entity!");
        return NULL;
    }

    strcpy(event_reciever->command, self->clickEvent->command);
    strcpy(event_reciever->target_id, self->clickEvent->target_id);
    gfc_linestrcpy_cpy(event_reciever->descriptor, self->clickEvent->descriptor);
    event_reciever->qty = self->clickEvent->qty;
    event_reciever->menu_state = self->clickEvent->menu_state;

    event_reciever->_sent = 1;
}

SJson* system_toJson(Entity* self)
{
    SJson* systemJson;
    SJson* array_position;

    char* owner;

    systemJson = sj_object_new();
    array_position = sj_array_new();

    if (self == NULL)
    {
        slog("Cannot convert NULL system to json!");
        return NULL;
    }

    //Insert the system name
    sj_object_insert(systemJson, "name", sj_new_str(self->name));

    //Insert the name of the system's owner

    if (self->owner != NULL) owner = sj_new_str(self->owner->name);

    else owner = sj_new_str("null");

    sj_object_insert(systemJson, "owner", owner);

    //Get the position array and insert it into the system json
    sj_array_append(array_position, sj_new_float(self->position.x));
    sj_array_append(array_position, sj_new_float(self->position.y));

    sj_object_insert(systemJson, "position", array_position);

    return systemJson;

    return sj_object_new();
}

void system_reciever(Entity* self, Game_Event* event)
{
    slog(event->command);
}

Entity* system_spawn(char* name, Vector2D position, Nation* owner, System_Data* systemdata)
{

    Entity* ent = NULL;

    //Create the new entity
    ent = entity_new_name(name);

    if (!ent)
    {
        slog("Failed to spawn a system");
        //return NULL;
    }

    //Load sprite and related info
    ent->sprite = gf2d_sprite_load_all("images/gameobjects/star.png", 64, 64, 1);

    vector2d_copy(ent->position, position);

    ent->frameRate = 0;
    ent->frameCount = 1;

    //Create collider circle
    ent->collider_circle = circle_new();

    ent->collider_circle->radius = 32;

    vector2d_copy(ent->collider_circle->position, ent->position);
    

    //Center circle collider to center of sprite
    ent->collider_circle->position.x += ent->sprite->frame_w/2;
    ent->collider_circle->position.y += ent->sprite->frame_h/2;

    //Load system details
    
    ent->owner = owner; //Can be null!

    ent->data = systemdata;

    //Functions

    ent->onClick = system_onClick;
    ent->toJson = system_toJson;
    ent->reciever = system_reciever;

    //Create clickSignal

    //Done!
    slog("System \"%s\" created!", ent->name);
    return ent;
}

int system_data_numPlanets(System_Data* systemdata)
{
    return systemdata->num_planets;
}

int system_num_planets(Entity* system)
{
    return system_data_numPlanets(((struct System_Data *) system->data));
}

/*Bottom*/
