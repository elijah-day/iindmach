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

#ifndef IIND_CONTROLS_H
#define IIND_CONTROLS_H

#define IIND_DEFAULT_CAMERA_MOVEMENT_SPEED 0.04

#include "iind_render.h"
#include "iind_world.h"
#include <stdbool.h>

void iind_handle_controls
(
	int iind_render_scale,
	float iind_aspect_ratio,
	float *iind_camera,
	bool iind_sdl_key_presses[],
	int iind_sdl_mouse_state,
	int iind_sdl_mouse_pos[],
	IINDWorldMarker *iind_world_markers,
	IINDWorldTile *iind_world_tiles,
	IINDWorldEntity *iind_world_entities,
	int iind_world_tile_count,
	int iind_world_entity_count
);

#endif
