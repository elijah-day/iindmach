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

#include "iind_controls.h"
#include "iind_world.h"
#include "SDL2/SDL.h"
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
)
{
	if(iind_sdl_mouse_state == 1)
	{	
		iind_world_markers[0].active = true;
	
		/*TODO: This can be simplified.*/	
		
		iind_world_markers[0].x =
		((float)iind_sdl_mouse_pos[0] / iind_render_scale /
		iind_aspect_ratio + iind_camera[0]) / 2 -
		((float)iind_sdl_mouse_pos[1] /
		iind_render_scale - iind_camera[1] / 2) * -1 - 1;
		
		iind_world_markers[0].y =
		((float)iind_sdl_mouse_pos[1] /
		iind_render_scale - iind_camera[1] / 2) * -1 +
		((float)iind_sdl_mouse_pos[0] / iind_render_scale /
		iind_aspect_ratio + iind_camera[0]) / 2;
	}
}
