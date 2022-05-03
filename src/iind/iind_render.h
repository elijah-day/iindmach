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

#ifndef IIND_RENDER_H
#define IIND_RENDER_H

#define IIND_WORLD_ENTITY_OVERLAP_SHIFT 0.0001

#define IIND_GUI_TEXT_BOX_R 15
#define IIND_GUI_TEXT_BOX_G 15
#define IIND_GUI_TEXT_BOX_B 15
#define IIND_GUI_TEXT_BOX_A 127

#define IIND_GUI_HEALTH_BAR_R 240
#define IIND_GUI_HEALTH_BAR_G 240
#define IIND_GUI_HEALTH_BAR_B 190
#define IIND_GUI_HEALTH_BAR_A 255

#include "iind_gui.h"
#include "iind_world.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

void iind_render_gui
(
	SDL_Renderer *iind_sdl_renderer,
	IINDWorldEntity *iind_world_entities,
	IINDGUIElement *iind_gui_elements,
	int iind_render_scale,
	float iind_aspect_ratio,
	int iind_sdl_window_size[],
	SDL_Texture *iind_sdl_textures[]
);

void iind_render_world
(
	SDL_Renderer *iind_sdl_renderer,
	SDL_Texture *iind_sdl_textures[],
	IINDWorldMarker *iind_world_markers,
	IINDWorldTile *iind_world_tiles,
	IINDWorldEntity *iind_world_entities,
	int iind_world_tile_count,
	int iind_world_entity_count,
	int iind_render_scale,
	float iind_aspect_ratio,
	float iind_camera[]
);

#endif
