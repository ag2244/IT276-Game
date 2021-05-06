#pragma once
#include "simple_logger.h"
#include "gfc_vector.h"

#include "starsabove_system.h"

#include "starsabove_resources.h"

#include "starsabove_buildable.h"
#include "starsabove_fleet.h"

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

void load_system_neighbors(Entity* system, SJson* system_json)
{
    int i;

    SJson* neighbors_json;
    Entity* neighbors;

    if (!system_json)
    {
        slog("CANNOT LOAD EDGES FOR NULL SYSTEM JSON"); return;
    }

    neighbors_json = sj_object_get_value(system_json, "edges");

    if ((!neighbors_json) || (!sj_is_array(neighbors_json)))
    {
        slog("Could not load this system's edges"); return;
    }

    neighbors = malloc(sizeof(Entity) * sj_array_get_count(neighbors_json));

    for (i = 0; i < sj_array_get_count(neighbors_json); i++)
    {
        neighbors[i] = *get_entity_by_name(sj_get_string_value(sj_array_get_nth(neighbors_json, i)));
    }

    system->num_neighbors = sj_array_get_count(neighbors_json);
    system->neighbors = neighbors;

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

        else 
        {
            nation = NULL;
        }

        system_spawn(name, pos, nation, systemdata);

    }

    slog("All systems have been spawned!");

    for (i = 0; i < systems->v.array->count; i++)
    {
        currentSystem = sj_array_get_nth(systems, i);

        load_system_neighbors(&entity_manager_get()->entity_list[i], currentSystem);
    }

    slog("All systems have been connected with their neighbors!");

    return 1;
}

void system_onClick(Entity* self, Game_Event* event_reciever, Bool playerowned)
{
    if (self == NULL)
    {
        slog("Cannot click on NULL entity!");
        return NULL;
    }

    if (event_reciever == NULL)
    {
        slog("Cannot pass signal to on NULL event!");
        return NULL;
    }

    system_gameevent_init(self, playerowned);

    gameevent_copy(event_reciever, self->clickEvent);
}

void system_update_fleetbutton(UI_Element* self)
{
    int i;
    Entity* this_entity;

    Nation* thisNation;

    if (self == NULL)
    {
        slog("CANNOT UPDATE NULL BUTTON!"); return;
    }

    this_entity = (Entity*)self->data;

    for (i = 0; i < nation_list_get()->max_nations; i++)
    {
        thisNation = &nation_list_get()->nations[i];

        if (!thisNation->_inuse) { continue; }

        if ((nation_fleetbylocation(thisNation, this_entity, 0)) && (thisNation->_is_player))
        {
            self->hidden = 0;
        }

        else
        {
            self->hidden = 1;
        }
    }
}

void system_update(Entity* self)
{

}

float* system_onNewTurn(Entity* self)
{

    int i;
    float* resources_total = malloc(6 * sizeof(float));

    for (i = 0; i < 6; i++) { resources_total[i] = 0; }

    System_Data* systemdata = self->data;

    for (i = 0; i < system_num_planets(self); i++)
    {
        resources_total = resourcelist_add(resources_total, systemdata->planets[i].onNewTurn(&systemdata->planets[i]));
    }

    return resources_total;

}

void system_gameevent_init(Entity* ent, Bool playerOwned)
{
    int i = 0;

    char temp[128];

    System_Data* systemdata = ent->data;

    UI_Element* planet_textbox;

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

    ent->clickEvent->eventsound = gfc_sound_load("audio/System.mp3", 0.5, 1);

    for (i = 0; i < systemdata->num_planets; i++)
    {

        //Planet textbox name
        sprintf(temp, "> %s", systemdata->planets[i].name);

        //Create the planet title textbox for the system menu
        planet_textbox = textbox_init
        (
            vector2d(10, 10),
            vector2d(200, 50),
            temp,
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
                planet_menustate_init(&systemdata->planets[i], ent->clickEvent->menu_state, ent->name, playerOwned),
                0
            )
        );

        //ent->clickEvent->menu_state = planet_menustate_init(&systemdata->planets[i], ent->clickEvent->menu_state, ent->name);
    }

    menu_state_hide(ent->clickEvent->menu_state);
}

SJson* system_toJson(Entity* self)
{
    int i;

    SJson* systemJson;

    SJson* array_position;
    SJson* array_planets;
    SJson* array_edges;

    char* owner;
    char nullstr[128] = "null";

    system_data = (struct System_Data*)self->data;

    systemJson = sj_object_new();

    array_position = sj_array_new();
    array_planets = sj_array_new();
    array_edges = sj_array_new();

    if (self == NULL)
    {
        slog("Cannot convert NULL system to json!");
        return NULL;
    }

    //Insert the system name
    sj_object_insert(systemJson, "name", sj_new_str(self->name));

    //Insert the name of the system's owner

    if (self->owner != NULL) { owner = sj_new_str(self->owner->name); }

    else { owner = sj_new_str(nullstr); }

    sj_object_insert(systemJson, "owner", owner);

    //Get the position array and insert it into the system json
    sj_array_append(array_position, sj_new_float(self->position.x));
    sj_array_append(array_position, sj_new_float(self->position.y));

    sj_object_insert(systemJson, "position", array_position);

    //Add planets
    
    for (i = 0; i < system_data->num_planets; i++)
    {
        sj_array_append(array_planets, planet_toJson(&system_data->planets[i]));
    }

    sj_object_insert(systemJson, "planets", array_planets);

    //Add edges

    for (i = 0; i < self->num_neighbors; i++)
    {
        sj_array_append(array_edges, sj_new_str( self->neighbors[i].name) );
    }

    if (sj_array_get_count(array_edges) > 0) {sj_object_insert(systemJson, "edges", array_edges);}

    return systemJson;

    return sj_object_new();
}

void system_reciever(Entity* self, Game_Event* event)
{
    int i;

    if (!self)
    {
        slog("NULL ENTITY CANNOT RECIEVE EVENT"); return;
    }

    if (!self)
    {
        slog("ENTITY CANNOT RECIEVE NULL EVENT"); return;
    }
    //If the command is to construct a building
    if (strcmp(event->command, "CONSTRUCT_BUILDING") == 0)
    {
        Buildable* bldng = buildable_copy(buildable_get_byname(event->descriptor));

        float* new_resources = resourcelist_subtract(self->owner->resources_total, bldng->costs);

        if (resourcelist_checkdeficit(new_resources))
        {
            planet_construct(system_planet_byname(self, event->sub_target_id), bldng);

            free(self->owner->resources_total);
            self->owner->resources_total = new_resources;
        }

        else { slog("CANNOT AFFORD THIS BUILDING"); }
    }

    //If the command is to construct a building
    if (strcmp(event->command, "CONSTRUCT_SHIP") == 0)
    {
        Fleet* fleet;

        Ship* ship = ship_copy(getshipbyname(event->descriptor), NULL);

        float* new_resources = resourcelist_subtract(self->owner->resources_total, ship->costs);

        if (resourcelist_checkdeficit(new_resources))
        {
            slog("ENOUGH RESOURCES FOR \"%s\"", ship->type); 

            fleet = nation_fleetbylocation(self->owner, self, 1);

            if (fleet)
            {

                ship->status = (int)SHIP_CONSTRUCTING;

                fleet_addShip(fleet, ship);

                //Fleet* thisfleet; for (i = 0; i < max_national_fleets; i++) { thisfleet = fleet_fromlist(self->owner->fleets, i); if (thisfleet) { slog("%s", thisfleet->name); } }

                return;
            }
        }

        else { slog("CANNOT AFFORD THIS SHIP"); }
    }
}

void system_fleetbutton_onClick(UI_Element* self, Game_Event* event_reciever, Bool playerowned)
{
    int i;

    Entity* this_entity;
    Nation* thisnation;

    Fleet* local_fleet = NULL;

    if (self == NULL)
    {
        slog("Cannot click on NULL button!");
        return NULL;
    }

    if (self == NULL)
    {
        slog("Cannot pass signal to on NULL event!");
        return NULL;
    }

    this_entity = (Entity*) self->data;

    for (i = 0; i < nation_list_get()->max_nations; i++)
    {
        thisnation = &nation_list_get()->nations[i];

        if (!thisnation->_inuse) { continue; }

        local_fleet = nation_fleetbylocation(thisnation, this_entity, 0);

        if (thisnation->_is_player && (local_fleet))
        {
            break;
        }
    }

    if (!local_fleet) { slog("NO SELECTABLE FLEET HERE!"); return; }

    gameevent_copy(
        event_reciever, 
        new_gameevent
        (
            self->text,
            "FLEETS",
            "SHOW LOCAL FLEETS",
            NULL,
            0,
            fleet_menustate(
                local_fleet, 
                NULL
            ),
            1
        )
    );

    event_reciever->eventsound = gfc_sound_load("audio/Ship.mp3", 0.5, 1);
}

void system_spawn_initbuttons_fleets(Entity* self)
{
    UI_Element* fleet_button;

    if (!self)
    {
        slog("CANNOT INIT FLEET BUTTON FOR NULL ENTITY"); return;
    }

    fleet_button = &self->shortcut_buttons[0];

    fleet_button->spriteMain = gf2d_sprite_load_all("images/ui/fleet_button.png", 24, 24, 1);
    fleet_button->hidden = 0;
    fleet_button->position = vector2d(
        0,
        (float)fleet_button->spriteMain->frame_w + (float)self->sprite->frame_h - 10
    );
    // fleet_button->position = vector2d(-1 * (float)fleet_button->spriteMain->frame_w, (-1 * (float)fleet_button->spriteMain->frame_h))

    fleet_button->onClick = system_fleetbutton_onClick;
    fleet_button->update = system_update_fleetbutton;

    fleet_button->data = self;

    fleet_button->collider_box = box_new();

    vector2d_copy(fleet_button->collider_box->position, fleet_button->position);
    fleet_button->collider_box->size = vector2d((float)fleet_button->spriteMain->frame_w, (float)self->sprite->frame_h);
    fleet_button->collider_box->is_clickable = box_clickable;
}

void system_spawn_initbuttons(Entity* self)
{

    if (!self)
    {
        slog("CANNOT INIT BUTTONS FOR NULL ENTITY"); return;
    }

    self->num_buttons = 1;

    self->shortcut_buttons = malloc(sizeof(UI_Element) * self->num_buttons);

    system_spawn_initbuttons_fleets(self);
}

Entity* system_spawn(char* name, Vector2D position, Nation* owner, System_Data* systemdata)
{
    int i = 0;

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
    ent->onNewTurn = system_onNewTurn;
    //ent->update = system_update;

    system_spawn_initbuttons(ent);

    //Done!
    slog("System \"%s\" created!", ent->name);
    return ent;
}

Menu_State* system_neighbors_menustate(Entity* self, Menu_State* previous, char title[128], Game_Event* template)
{
    int i;

    Menu_State* neighbors_menustate;
    UI_Element* neighbor_textbox;

    if (!self)
    {
        slog("CANNOT CREATE NEIGHBORS MENU FOR NULL SYSTEM"); return NULL;
    }

    if (!previous)
    {
        slog("CANNOT USE NULL PREVIOUS MENU FOR NEIGHBORS MENU"); return NULL;
    }

    if (!title)
    {
        slog("CANNOT USE NULL NAME FOR NEIGHBORS MENU"); return NULL;
    }

    neighbors_menustate = menu_state_new(
        previous,
        textbox_init
        (
            vector2d(10, 10),
            vector2d(250, 50),
            title,
            font_load("resources/fonts/futur.ttf", 16)
        ),
        NULL,
        vector2d(10, 10),
        0,
        5
    );

    for (i = 0; i < self->num_neighbors; i++)
    {
        neighbor_textbox = textbox_init(
            vector2d(10, 10),
            vector2d(250, 50),
            self->neighbors[i].name,
            font_load("resources/fonts/futur.ttf", 12)
        );

        neighbor_textbox->signal = malloc(sizeof(Game_Event));

        gameevent_copy(neighbor_textbox->signal, template);
        strcpy(neighbor_textbox->signal->descriptor, self->neighbors[i].name);

        menu_addTo(neighbors_menustate->current_menu, neighbor_textbox);
    }

    menu_state_hide(neighbors_menustate);

    return neighbors_menustate;
}

//Get number of planets
int system_data_numPlanets(System_Data* systemdata)
{
    return systemdata->num_planets;
}

int system_num_planets(Entity* system)
{
    return system_data_numPlanets((struct System_Data *) system->data);
}


//Get a specific planet
Planet* system_data_planet_byname(System_Data* systemdata, char* planet_name)
{
    int i;

    for (i = 0; i < systemdata->num_planets; i++)
    {
        if (strcmp(systemdata->planets[i].name, planet_name) == 0)
        {
            return &systemdata->planets[i];
        }
    }

    slog("Planet not found!");
    return NULL;
}

Planet* system_planet_byname(Entity* system, char* planet_name)
{
    return system_data_planet_byname((struct System_Data*)system->data, planet_name);
}

/*Bottom*/
