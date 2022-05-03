/*
IINDMACH
Copyright (C) 2022  Elijah Day

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef IIND_WORLD_H
#define IIND_WORLD_H

/*
=================
MISC. DEFINITIONS
=================
*/

#define IIND_PI 3.14159

#define IIND_WORLD_PLAYER_ENTITY 0
#define IIND_WORLD_ENTITY_MOVEMENT_HEALTH_DRAIN_RATE 0.01

#define IIND_WORLD_ENTITY_MAX_HEALTH 100
#define IIND_DEFAULT_WORLD_ENTITY_MOVEMENT_SPEED 0.015

#define IIND_DEFAULT_WORLD_DOOR_FUNC_RADIUS 0.5

/*
========================
WORLD MARKER DEFINITIONS
========================
*/

#define IIND_WORLD_PLAYER_MOVEMENT_MARKER 0
#define IIND_WORLD_EDIT_MODE_MARKER 1

/*
========================
FUNC TYPE ID DEFINITIONS
========================
*/

#define IIND_WORLD_DOOR_FUNC 0
#define IIND_WORLD_SWITCH_FUNC 1
#define IIND_WORLD_TIMER_FUNC 2
#define IIND_WORLD_DIALOGUE_FUNC 3
#define IIND_WORLD_GATE_FUNC 4
#define IIND_WORLD_INVERTER_FUNC 5
#define IIND_WORLD_CHARGER_FUNC 6

/*
=======================
FUNC TAG ID DEFINITIONS
=======================
*/

/*"LH" meaning "low-high."*/
#define IIND_WORLD_FUNC_LH_TAG 0

#define IIND_WORLD_FUNC_INPUT_TAG 2
#define IIND_WORLD_FUNC_OUTPUT_TAG 3

#define IIND_WORLD_DOOR_FUNC_WORLD_TAG 0
#define IIND_WORLD_DOOR_FUNC_X_TAG 1
#define IIND_WORLD_DOOR_FUNC_Y_TAG 2

#define IIND_WORLD_SWITCH_FUNC_TOGGLE_TAG 1

/*
Make sure the timer's "current time" tag matches the value of the
generic "low-high" tag.  It doubles as the indicator of weather or
not the timer is powered.
*/
#define IIND_WORLD_FUNC_TIMER_CUR_TAG 0
#define IIND_WORLD_FUNC_TIMER_MAX_TAG 1

#define IIND_WORLD_DIALOGUE_FUNC_MIN_TAG 0
#define IIND_WORLD_DIALOGUE_FUNC_MAX_TAG 1
#define IIND_WORLD_DIALOGUE_FUNC_PORTRAIT_TAG 2

#define IIND_WORLD_GATE_FUNC_TOGGLE_TAG 3

/*
===============================
STRUCT AND FUNCTION DEFINITIONS
===============================
*/

#include <stdbool.h>

typedef struct IINDWorldMarker
{
	float x, y;
	int sdl_texture_id;
	bool active;
}
IINDWorldMarker;

typedef struct IINDWorldTile
{
	int x, y, sdl_texture_id;
}
IINDWorldTile;

typedef struct IINDWorldEntity
{
	float x, y, speed, angle, health;
	int sdl_texture_id, anim_col, anim_row;
	bool collision;
}
IINDWorldEntity;

typedef struct IINDWorldFunc
{
	int bind_id, type, tags[4];
}
IINDWorldFunc;

bool iind_test_world_entity_distance
(
	IINDWorldEntity *iind_world_entities,
	int iind_world_entity_id,
	float iind_world_entity_distance,
	float iind_x,
	float iind_y
);

bool iind_move_world_entity
(
	IINDWorldTile *iind_world_tiles,
	IINDWorldEntity *iind_world_entities,
	int iind_world_tile_count,
	int iind_world_entity_count,
	int iind_target_world_entity_id,
	float iind_world_entity_speed,
	float iind_world_entity_direction
);

void iind_handle_world_funcs
(
	IINDWorldEntity *iind_world_entities,
	IINDWorldFunc *iind_world_funcs,
	int iind_world_entity_count,
	int iind_world_func_count,
	int *iind_new_world_id,
	IINDWorldEntity *iind_world_trans_entity,
	bool *iind_world_trans_state,
	int *iind_dialogue_tags
);

void iind_update_world_entity_anims
(
	IINDWorldEntity *iind_world_entities,
	int iind_world_entity_count,
	int iind_anim_counter
);

void iind_handle_player_movement
(
	IINDWorldMarker *iind_world_markers,
	IINDWorldTile *iind_world_tiles,
	IINDWorldEntity *iind_world_entities,
	int iind_world_tile_count,
	int iind_world_entity_count
);

#endif
