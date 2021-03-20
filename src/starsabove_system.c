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

    if (self == NULL)
    {
        slog("Cannot pass signal to on NULL event!");
        return NULL;
    }

    gameevent_copy(event_reciever, self->clickEvent);
}

SJson* system_toJson(Entity* self)
{
    int i;

    SJson* systemJson;

    SJson* array_position;
    SJson* array_planets;

    char* owner;

    system_data = (struct System_Data*)self->data;

    systemJson = sj_object_new();

    array_position = sj_array_new();
    array_planets = sj_array_new();

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

    //Add planets
    
    for (i = 0; i < system_data->num_planets; i++)
        sj_array_append(array_planets, planet_toJson(&system_data->planets[i]));

    sj_object_insert(systemJson, "planets", array_planets);

    return systemJson;

    return sj_object_new();
}

void system_reciever(Entity* self, Game_Event* event)
{
    slog(event->target_id);
}

Entity* system_spawn(char* name, Vector2D position, Nation* owner, System_Data* systemdata)
{
    int i = 0;

    Entity* ent = NULL;

    UI_Element* planet_textbox;

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
    ent->clickEvent = new_gameevent(
        "GAME UI",
        NULL,
        NULL,
        NULL,
        0,
        menu_state_new(
            NULL,
            textbox_init
            (
                vector2d(10, 10),
                vector2d(200, 50),
                ent->name,
                font_load("resources/fonts/futur.ttf", 16)
            ),
            NULL,
            vector2d(10, 10),
            0,
            5
        ),
        1
    );

    for (i = 0; i < systemdata->num_planets; i++)
    {

        //Create the planet title textbox for the system menu
        planet_textbox = textbox_init
        (
            vector2d(10, 10),
            vector2d(200, 50),
            systemdata->planets[i].name,
            font_load("resources/fonts/futur.ttf", 12)
        );

        //Add this planet as a textbox to the system menu
        menu_addTo(
            menu_state_getsafe(ent->clickEvent->menu_state)->current_menu,
            planet_textbox
        );

        //Add a game_event to planet_textbox when its clicked to initiate the planet menu_state
        ui_addevent(planet_textbox,
            new_gameevent(
                ent->name,
                systemdata->planets[i].name,
                "COMMAND",
                "DESCRIPTOR",
                123,
                //Get the menustate for this planet
                planet_menustate_init(&systemdata->planets[i], ent->clickEvent->menu_state, ent->name),
                0
            )
        );

        //ent->clickEvent->menu_state = planet_menustate_init(&systemdata->planets[i], ent->clickEvent->menu_state, ent->name);
    }

    menu_state_hide(ent->clickEvent->menu_state);

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
    return system_data_numPlanets((struct System_Data *) system->data);
}

/*Bottom*/
