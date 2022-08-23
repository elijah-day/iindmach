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

#ifndef IIND_RUN_H
#define IIND_RUN_H

#define IIND_DEFAULT_COUNTER_VAL 0
#define IIND_DEFAULT_FPS 64
#define IIND_DEFAULT_WORLD_TIME_SCALE 128
#define IIND_DEFAULT_RENDER_SCALE 80
#define IIND_DEFAULT_ASPECT_RATIO 16 / 9
#define IIND_DEFAULT_ANIM_SPEED 28
#define IIND_DEFAULT_WORLD_ID 0
#define IIND_DEFAULT_SDL_GUI_FONT_SIZE 16

#define IIND_DEFAULT_WORLD_ENTITY_ANIM_W 32

#define IIND_DEFAULT_SDL_TEXTURE_LIST_PATH "assets/images.list"
#define IIND_DEFAULT_MODULE_WORLD_ID_PATH "modules/iind/world_id.list"
#define IIND_DEFAULT_DIALOGUE_LIST_PATH "dialogue.list"
#define IIND_DEFAULT_SDL_GUI_FONT_PATH "assets/fonts/mononoki-Regular.ttf"

#define IIND_WORLD_NAME_MAX_LEN 128
#define IIND_DIALOGUE_MAX_LEN 64
#define IIND_EDIT_INFO_STR_MAX_LEN 128
#define IIND_WORLD_MARKER_COUNT 2

#define IIND_MENU_ITEM_COUNT 2
#define IIND_MENU_ITEM_STR_MAX_LEN 16
#define IIND_MENU_ITEM_1_STR "< QUIT >"
#define IIND_MENU_ITEM_2_STR "< SAVE >"

#include "SDL2/SDL.h"
#include <stdbool.h>

void iind_handle_sdl_event
(
	SDL_Event *iind_sdl_event,
	bool *iind_running_state,
	bool *iind_sdl_key_press,
	int *iind_sdl_mouse_state,
	int *iind_sdl_mouse_pos
);

bool iind_handle_sdl_tick_intervals
(
	int *iind_refresh_rate,
	int *iind_sdl_tick_counter,
	int *iind_prev_sdl_ticks
);

void iind_run(SDL_Window *iind_sdl_window, SDL_Renderer *iind_sdl_renderer);

#endif
