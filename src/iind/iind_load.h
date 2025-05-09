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

#ifndef IIND_LOAD_H
#define IIND_LOAD_H

#define IIND_LOAD_BUFFER_MAX_LEN 128

#define IIND_WORLD_ELEMENT_TYPE_COUNT 3

#include "iind_world.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

int iind_get_text_file_line_count(const char *iind_text_file_path);

void iind_read_list_file
(
	char *iind_list_strings[],
	const int iind_string_count,
	const char *iind_list_path
);

SDL_Texture *iind_load_sdl_texture_from_image
(
	SDL_Renderer *iind_sdl_renderer,
	const char *iind_image_path
);

SDL_Texture *iind_load_sdl_texture_from_font
(
	SDL_Renderer *iind_sdl_renderer,
	TTF_Font *iind_sdl_font,
	SDL_Color iind_sdl_font_color,
	const char *iind_font_text
);

void iind_load_world_element_counts
(
	const char *iind_world_file_path,
	int *iind_world_tile_count,
	int *iind_world_entity_count,
	int *iind_world_func_count
);

void iind_save_world
(
	const char *iind_world_file_path,
	IINDWorldTile *iind_world_tiles,
	IINDWorldEntity *iind_world_entities,
	IINDWorldFunc *iind_world_funcs,
	int iind_world_tile_count,
	int iind_world_entity_count,
	int iind_world_func_count
);

bool iind_load_world
(
	const char *iind_world_file_path,
	IINDWorldTile *iind_world_tiles,
	IINDWorldEntity *iind_world_entities,
	IINDWorldFunc *iind_world_funcs,
	int iind_world_tile_count,
	int iind_world_entity_count,
	int iind_world_func_count
);

void iind_load_sdl_textures
(
	SDL_Renderer *iind_sdl_renderer,
	SDL_Texture *iind_sdl_textures[],
	const int iind_sdl_texture_count,
	const char *iind_sdl_texture_list_path
);

#endif
