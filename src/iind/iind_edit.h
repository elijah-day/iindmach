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

#ifndef IIND_EDIT_H
#define IIND_EDIT_H

#include "iind_world.h"

void iind_create_world_tile
(
	IINDWorldTile **iind_world_tiles,
	int *iind_world_tile_count,
	int iind_edit_x,
	int iind_edit_y,
	int iind_edit_id
);

void iind_delete_world_tile
(
	IINDWorldTile **iind_world_tiles,
	int *iind_world_tile_count,
	int iind_edit_x,
	int iind_edit_y
);

void iind_create_world_entity
(
	IINDWorldEntity **iind_world_entities,
	IINDWorldFunc **iind_world_funcs,
	int *iind_world_entity_count,
	int *iind_world_func_count,
	float *iind_edit_x,
	float *iind_edit_y,
	float *iind_edit_angle,
	int *iind_edit_id,
	int *iind_edit_type,
	int *iind_edit_tags
);

void iind_delete_world_entity
(
	IINDWorldEntity **iind_world_entities,
	IINDWorldFunc **iind_world_funcs,
	int *iind_world_entity_count,
	int *iind_world_func_count,
	float *iind_edit_x,
	float *iind_edit_y
);

void iind_create_world_func
(
	IINDWorldFunc **iind_world_funcs,
	int *iind_world_func_count,
	float *iind_edit_x,
	float *iind_edit_y,
	int *iind_edit_type,
	int *iind_edit_tags
);

#endif
