#pragma once
#include "simple_logger.h"
#include "gfc_vector.h"

#include "starsabove_system.h"

//Use for pointing to the data for an individual system
System_Data* system_data;

System_Data* system_data_new()
{
    return malloc(sizeof(System_Data));
}

System_Transmission* system_transmission_new()
{
    return malloc(sizeof(System_Transmission));
}

System_Transmission* system_transmission_get(void* transmission)
{
    return (System_Transmission*)transmission;
}

int load_systems(SJson* game_json)
{
    //The star systems in this game
    SJson* systems;
    SJson* currentSystem;

    char* name = NULL;
    char* temp = NULL;

    Nation* nation = NULL;

    SJson* positionJson;
    Vector2D pos;
    float x, y;

    int i;

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

        //Get the owner

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

        system_spawn(name, pos, nation);

    }

    slog("All systems have been spawned!");
    return 1;
}

void system_onClick(Entity* self, Game_Event* event_reciever) {
    if (self == NULL)
    {
        slog("Cannot click on NULL entity!");
        return NULL;
    }

    /*if (self->name != NULL)
        slog("Clicked on STAR SYSTEM \"%s\"", self->name);*/
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

Entity* system_spawn(char* name, Vector2D position, Nation* owner)
{

    Entity* ent = NULL;
    System_Data* thisSystem = system_data_new(name);

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

    (struct System_Data*)ent->data = thisSystem;

    //Functions

    ent->onClick = system_onClick;
    ent->toJson = system_toJson;
    ent->reciever = system_reciever;

    //Done!
    //slog("System \"%s\" created!", ent->name);
    return ent;
}

/*Bottom*/
