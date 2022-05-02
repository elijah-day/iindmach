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

#include "iind_load.h"
#include "iind_render.h"
#include "iind_run.h"
#include "iind_world.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <stdio.h>
#include <stdlib.h>

int iind_get_text_file_line_count(const char *iind_text_file_path)
{
	FILE *iind_load_fp = fopen(iind_text_file_path, "r");
	int iind_line_count_return = 0;
	
	if(iind_load_fp)
	{
		char c = fgetc(iind_load_fp);
		
		while(c != EOF)
		{
			if(c == '\n') iind_line_count_return++;
			c = fgetc(iind_load_fp);
		}
		
		fclose(iind_load_fp);
	}
	else printf("fopen error: %s\n", iind_text_file_path);
	
	return iind_line_count_return;
}

void iind_load_sdl_textures
(
	SDL_Renderer *iind_sdl_renderer,
	SDL_Texture *iind_sdl_textures[],
	const int iind_sdl_texture_count,
	const char *iind_sdl_texture_list_path
)
{
	FILE *iind_load_fp = fopen(iind_sdl_texture_list_path, "r");
	
	char iind_load_buffer[IIND_LOAD_BUFFER_MAX_LEN];
	
	if(iind_load_fp)
	{
		for(int i = 0; i < iind_sdl_texture_count; i++)
		{
			fgets(iind_load_buffer, IIND_LOAD_BUFFER_MAX_LEN, iind_load_fp);
			for(int j = 0; j < IIND_LOAD_BUFFER_MAX_LEN; j++)
			{
				if(iind_load_buffer[j] == '\n')
				{
					iind_load_buffer[j] = '\0';
					break;
				}
			}
			
			iind_sdl_textures[i] = iind_load_sdl_texture_from_image
			(
				iind_sdl_renderer,
				iind_load_buffer
			);
		}
		
		fclose(iind_load_fp);
	}
	else printf("fopen error: %s\n", iind_sdl_texture_list_path);
}

void iind_read_list_file
(
	char *iind_list_strings[],
	const int iind_string_count,
	const char *iind_list_path
)
{
	FILE *iind_load_fp = fopen(iind_list_path, "r");
	
	char iind_load_buffer[IIND_LOAD_BUFFER_MAX_LEN];
	
	if(iind_load_fp)
	{
		for(int i = 0; i < iind_string_count; i++)
		{
			fgets(iind_load_buffer, IIND_LOAD_BUFFER_MAX_LEN, iind_load_fp);
			for(int j = 0; j < IIND_LOAD_BUFFER_MAX_LEN; j++)
			{
				if(iind_load_buffer[j] == '\n')
				{
					iind_load_buffer[j] = '\0';
					break;
				}
			}
			
			strcpy(iind_list_strings[i], iind_load_buffer);
		}
		
		fclose(iind_load_fp);
	}
	else printf("fopen error: %s\n", iind_list_path);
}

SDL_Texture *iind_load_sdl_texture_from_image
(
	SDL_Renderer *iind_sdl_renderer,
	const char *iind_image_path
)
{
	if(IMG_Load(iind_image_path))
	{
		SDL_Surface *iind_temp_surface = IMG_Load(iind_image_path);
		SDL_Texture *iind_return_texture = SDL_CreateTextureFromSurface
		(
			iind_sdl_renderer,
			iind_temp_surface
		);
		
		SDL_FreeSurface(iind_temp_surface);
		
		if(iind_return_texture)
		{
			return iind_return_texture;
		}
		else printf("SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
	}
	else printf("IMG_Load error: %s\n", SDL_GetError());
	
	return NULL;	
}

SDL_Texture *iind_load_sdl_texture_from_font
(
	SDL_Renderer *iind_sdl_renderer,
	TTF_Font *iind_sdl_font,
	SDL_Color iind_sdl_font_color,
	const char *iind_font_text
)
{
	if(iind_sdl_font)
	{
		SDL_Surface *iind_temp_surface =
		TTF_RenderText_Solid
		(
			iind_sdl_font,
			iind_font_text,
			iind_sdl_font_color
		);
		
		SDL_Texture *iind_return_texture = SDL_CreateTextureFromSurface
		(
			iind_sdl_renderer,
			iind_temp_surface
		);
		
		SDL_FreeSurface(iind_temp_surface);
		
		if(iind_return_texture)
		{
			return iind_return_texture;
		}
		else printf("SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
	}
	else printf("iind_sdl_font null\n");
	
	return NULL;	
}

void iind_load_world_element_counts
(
	const char *iind_world_file_path,
	int *iind_world_tile_count,
	int *iind_world_entity_count,
	int *iind_world_func_count
)
{	
	FILE *iind_load_fp = fopen(iind_world_file_path, "rb");
	
	if(iind_load_fp)
	{
		size_t iind_read_size = 0; 
		
		iind_read_size = fread
		(
			iind_world_tile_count,
			sizeof(int),
			1,
			iind_load_fp
		);
		if(iind_read_size == 0)
		{
			printf("fread error: iind_world_tile_count: %s\n", iind_world_file_path);
		}
		
		iind_read_size = fread
		(
			iind_world_entity_count,
			sizeof(int),
			1,
			iind_load_fp
		);
		if(iind_read_size == 0)
		{
			printf("fread error: iind_world_entity_count: %s\n", iind_world_file_path);
		}
		
		iind_read_size = fread
		(
			iind_world_func_count,
			sizeof(int),
			1,
			iind_load_fp
		);
		if(iind_read_size == 0)
		{
			printf("fread error: iind_world_func_count: %s\n", iind_world_file_path);
		}
		
		fclose(iind_load_fp);
	}
	else printf("fopen error: %s\n", iind_world_file_path);
}

void iind_save_world
(
	const char *iind_world_file_path,
	IINDWorldTile *iind_world_tiles,
	IINDWorldEntity *iind_world_entities,
	IINDWorldFunc *iind_world_funcs,
	int iind_world_tile_count,
	int iind_world_entity_count,
	int iind_world_func_count
)
{
	FILE *iind_save_fp = fopen(iind_world_file_path, "wb");
	
	if(iind_save_fp)
	{	
		size_t iind_written_size = 0;
	
		iind_written_size = fwrite
		(
			&iind_world_tile_count,
			sizeof(int),
			1,
			iind_save_fp
		);
		if(iind_written_size == 0)
		{
			printf("fwrite error: iind_world_tile_count: %s\n", iind_world_file_path);
		}
		
		iind_written_size = fwrite
		(
			&iind_world_entity_count,
			sizeof(int),
			1,
			iind_save_fp
		);
		if(iind_written_size == 0)
		{
			printf("fwrite error: iind_world_entity_count: %s\n", iind_world_file_path);
		}
		
		iind_written_size = fwrite
		(
			&iind_world_func_count,
			sizeof(int),
			1,
			iind_save_fp
		);
		if(iind_written_size == 0)
		{
			printf("fwrite error: iind_world_func_count: %s\n", iind_world_file_path);
		}
	
		iind_written_size = fwrite
		(
			iind_world_tiles,
			sizeof(IINDWorldTile),
			iind_world_tile_count,
			iind_save_fp
		);
		if(iind_written_size == 0)
		{
			printf("fwrite error: iind_world_tiles: %s\n", iind_world_file_path);
		}
		
		iind_written_size = fwrite
		(
			iind_world_entities,
			sizeof(IINDWorldEntity),
			iind_world_entity_count,
			iind_save_fp
		);
		if(iind_written_size == 0)
		{
			printf("fwrite error: iind_world_entities: %s\n", iind_world_file_path);
		}
		
		iind_written_size = fwrite
		(
			iind_world_funcs,
			sizeof(IINDWorldFunc),
			iind_world_func_count,
			iind_save_fp
		);
		if(iind_written_size == 0)
		{
			printf("fwrite error: iind_world_funcs: %s\n", iind_world_file_path);
		}
		
		fclose(iind_save_fp);
	}
	else printf("fopen error: %s\n", iind_world_file_path);
}

bool iind_load_world
(
	const char *iind_world_file_path,
	IINDWorldTile *iind_world_tiles,
	IINDWorldEntity *iind_world_entities,
	IINDWorldFunc *iind_world_funcs,
	int iind_world_tile_count,
	int iind_world_entity_count,
	int iind_world_func_count
)
{
	FILE *iind_load_fp = fopen(iind_world_file_path, "rb");
	
	bool iind_load_success = true;
	
	if(iind_load_fp)
	{
		size_t iind_read_size = 0;
		
		fseek(iind_load_fp, IIND_WORLD_ELEMENT_TYPE_COUNT * sizeof(int), SEEK_SET);
		
		iind_read_size = fread
		(
			iind_world_tiles,
			sizeof(IINDWorldTile),
			iind_world_tile_count,
			iind_load_fp
		);
		if(iind_read_size == 0)
		{
			printf("fread error: iind_world_tiles: %s\n", iind_world_file_path);
		}
		
		iind_read_size = fread
		(
			iind_world_entities,
			sizeof(IINDWorldEntity),
			iind_world_entity_count,
			iind_load_fp
		);
		if(iind_read_size == 0)
		{
			printf("fread error: iind_world_entities: %s\n", iind_world_file_path);
		}

		iind_read_size = fread
		(
			iind_world_funcs,
			sizeof(IINDWorldFunc),
			iind_world_func_count,
			iind_load_fp
		);
		if(iind_read_size == 0)
		{
			printf("fread error: iind_world_funcs: %s\n", iind_world_file_path);
		}
		
		fclose(iind_load_fp);
	}
	else
	{
		printf("fopen error: %s\n", iind_world_file_path);
		iind_load_success = false;
	}
	
	return iind_load_success;
}




