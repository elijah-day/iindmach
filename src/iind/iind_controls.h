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

#define IIND_SDL_KEY_COUNT 322

#define IIND_FULLSCREEN_KEY_BIND_ID 0
#define IIND_QUICKSAVE_KEY_BIND_ID 1
#define IIND_DIALOGUE_KEY_BIND_ID 2
#define IIND_MENU_KEY_BIND_ID 3
#define IIND_MENU_LEFT_KEY_BIND_ID 4
#define IIND_MENU_RIGHT_KEY_BIND_ID 5

#include "iind_render.h"
#include "iind_world.h"
#include <stdbool.h>

void iind_handle_player_movement_controls
(
	IINDWorldMarker *iind_world_markers,
	int iind_sdl_mouse_pos[],
	int iind_sdl_mouse_state,
	int iind_render_scale,
	float iind_aspect_ratio,
	float iind_camera[]
);

bool iind_handle_navigation_controls
(
	int iind_sdl_key_bind_id,
	int *iind_dialogue_tags,
	bool *iind_menu_state,
	int *iind_selected_menu_item
);

bool iind_handle_misc_controls
(
	int iind_sdl_key_bind_id,
	SDL_Window *iind_sdl_window,
	SDL_Renderer *iind_sdl_renderer,
	char *iind_world_save_name,
	IINDWorldTile *iind_world_tiles,
	IINDWorldEntity *iind_world_entities,
	IINDWorldFunc *iind_world_funcs,
	int iind_world_tile_count,
	int iind_world_entity_count,
	int iind_world_func_count,
	IINDGUIElement *iind_gui_elements,
	TTF_Font *iind_gui_text_ttf_font,
	SDL_Color iind_gui_text_sdl_color,
	bool *iind_sdl_window_fullscreen_state
);

#endif
