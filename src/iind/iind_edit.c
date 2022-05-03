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

#include "iind_edit.h"
#include "iind_world.h"
#include <stdlib.h>
#include <stdio.h>

void iind_create_world_tile
(
	IINDWorldTile **iind_world_tiles,
	int *iind_world_tile_count,
	int iind_edit_x,
	int iind_edit_y,
	int iind_edit_id
)
{
	*iind_world_tiles = realloc
	(
		*iind_world_tiles,
		(*iind_world_tile_count + 1) * sizeof(IINDWorldTile)
	);
	
	(*iind_world_tiles)[*iind_world_tile_count].x =
	iind_edit_x;
	
	(*iind_world_tiles)[*iind_world_tile_count].y =
	iind_edit_y;
	
	(*iind_world_tiles)[*iind_world_tile_count].sdl_texture_id =
	iind_edit_id;
	
	*iind_world_tile_count += 1;
}

void iind_delete_world_tile
(
	IINDWorldTile **iind_world_tiles,
	int *iind_world_tile_count,
	int iind_edit_x,
	int iind_edit_y
)
{
	int iind_delete_index = -1;

	for(int i = 0; i < *iind_world_tile_count; i++)
	{
		if
		(
			(*iind_world_tiles)[i].x == iind_edit_x &&
			(*iind_world_tiles)[i].y == iind_edit_y
		)
		{
			iind_delete_index = i;
			break;
		}
	}
	
	if(iind_delete_index > -1)
	{
		for(int i = iind_delete_index; i < *iind_world_tile_count - 1; i++)
		{
			(*iind_world_tiles)[i].x =
			(*iind_world_tiles)[i + 1].x;
			
			(*iind_world_tiles)[i].y =
			(*iind_world_tiles)[i + 1].y;
			
			(*iind_world_tiles)[i].sdl_texture_id =
			(*iind_world_tiles)[i + 1].sdl_texture_id;
		}
		
		*iind_world_tiles = realloc
		(
			*iind_world_tiles,
			(*iind_world_tile_count - 1) * sizeof(IINDWorldTile)
		);
		
		*iind_world_tile_count -= 1;
	}
	else printf("iind_delete_index invalid: %d\n", iind_delete_index);
}

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
)
{
	bool iind_entity_overlap = false;

	for(int i = 0; i < *iind_world_entity_count; i++)
	{
		if
		(
			(*iind_world_entities)[i].x == *iind_edit_x &&
			(*iind_world_entities)[i].y == *iind_edit_y
		)
		iind_entity_overlap = true;
	}

	if(!iind_entity_overlap)
	{
		*iind_world_entities = realloc
		(
			*iind_world_entities,
			(*iind_world_entity_count + 1) * sizeof(IINDWorldEntity)
		);
		
		(*iind_world_entities)[*iind_world_entity_count].x =
		*iind_edit_x;
		
		(*iind_world_entities)[*iind_world_entity_count].y =
		*iind_edit_y;
		
		(*iind_world_entities)[*iind_world_entity_count].speed = 0;
		
		(*iind_world_entities)[*iind_world_entity_count].angle =
		*iind_edit_angle;
		
		(*iind_world_entities)[*iind_world_entity_count].health =
		IIND_WORLD_ENTITY_MAX_HEALTH;
		
		(*iind_world_entities)[*iind_world_entity_count].sdl_texture_id =
		*iind_edit_id;
		
		/*
		Initialized to true because collision should only
		be false if a func allows it.
		*/
		(*iind_world_entities)[*iind_world_entity_count].collision =
		true;
		
		*iind_world_entity_count += 1;
		
		if(*iind_edit_type > -1)
		{
			*iind_world_funcs = realloc
			(
				*iind_world_funcs,
				(*iind_world_func_count + 1) * sizeof(IINDWorldFunc)
			);
			
			(*iind_world_funcs)[*iind_world_func_count].bind_id =
			*iind_world_entity_count - 1;
			
			(*iind_world_funcs)[*iind_world_func_count].type =
			*iind_edit_type;
			
			(*iind_world_funcs)[*iind_world_func_count].tags[0] =
			iind_edit_tags[0];
			
			(*iind_world_funcs)[*iind_world_func_count].tags[1] =
			iind_edit_tags[1];
			
			(*iind_world_funcs)[*iind_world_func_count].tags[2] =
			iind_edit_tags[2];
			
			(*iind_world_funcs)[*iind_world_func_count].tags[3] =
			iind_edit_tags[3];
			
			*iind_world_func_count += 1;
		}
	}
	else printf("iind_entity_overlap true\n");
}

void iind_delete_world_entity
(
	IINDWorldEntity **iind_world_entities,
	IINDWorldFunc **iind_world_funcs,
	int *iind_world_entity_count,
	int *iind_world_func_count,
	float *iind_edit_x,
	float *iind_edit_y
)
{
	int iind_delete_index = -1;
	
	bool iind_delete_has_func = false;

	for(int i = 0; i < *iind_world_entity_count; i++)
	{
		if
		(
			*iind_edit_x > (*iind_world_entities)[i].x - 0.1 &&
			*iind_edit_x < (*iind_world_entities)[i].x + 0.1 &&
			
			*iind_edit_y > (*iind_world_entities)[i].y - 0.1 &&
			*iind_edit_y < (*iind_world_entities)[i].y + 0.1
		)
		{
			iind_delete_index = i;
			break;
		}
	}
	
	if(iind_delete_index > -1)
	{
		for(int i = iind_delete_index; i < *iind_world_entity_count - 1; i++)
		{
			(*iind_world_entities)[i].x =
			(*iind_world_entities)[i + 1].x;
			
			(*iind_world_entities)[i].y =
			(*iind_world_entities)[i + 1].y;
			
			(*iind_world_entities)[i].angle =
			(*iind_world_entities)[i + 1].angle;
			
			(*iind_world_entities)[i].sdl_texture_id =
			(*iind_world_entities)[i + 1].sdl_texture_id;
		}
		
		*iind_world_entities = realloc
		(
			*iind_world_entities,
			(*iind_world_entity_count - 1) * sizeof(IINDWorldEntity)
		);
		
		*iind_world_entity_count -= 1;
		
		for(int i = 0; i < *iind_world_func_count; i++)
		{	
			if((*iind_world_funcs)[i].bind_id == iind_delete_index)
			{
				for(int j = i; j < *iind_world_func_count - 1; j++)
				{
					(*iind_world_funcs)[j].bind_id =
					(*iind_world_funcs)[j + 1].bind_id - 1;
					
					(*iind_world_funcs)[j].type =
					(*iind_world_funcs)[j + 1].type;
					
					(*iind_world_funcs)[j].tags[0] =
					(*iind_world_funcs)[j + 1].tags[0];
					
					(*iind_world_funcs)[j].tags[1] =
					(*iind_world_funcs)[j + 1].tags[1];
					
					(*iind_world_funcs)[j].tags[2] =
					(*iind_world_funcs)[j + 1].tags[2];
					
					(*iind_world_funcs)[j].tags[3] =
					(*iind_world_funcs)[j + 1].tags[3];
				}
				
				*iind_world_funcs = realloc
				(
					*iind_world_funcs,
					(*iind_world_func_count - 1) * sizeof(IINDWorldFunc)
				);
				
				*iind_world_func_count -= 1;
				
				iind_delete_has_func = true;
				
				break;
			}
		}
		
		if(!iind_delete_has_func)
		{
			for(int i = 0; i < *iind_world_func_count; i++)
			{
				if((*iind_world_funcs)[i].bind_id > iind_delete_index)
				{
					(*iind_world_funcs)[i].bind_id -= 1;
				}
			}
		}
	}
	else printf("iind_delete_index invalid: %d\n", iind_delete_index);
}

void iind_create_world_func
(
	IINDWorldFunc **iind_world_funcs,
	int *iind_world_func_count,
	float *iind_edit_x,
	float *iind_edit_y,
	int *iind_edit_type,
	int *iind_edit_tags
)
{
	*iind_world_funcs = realloc
	(
		*iind_world_funcs,
		(*iind_world_func_count + 1) * sizeof(IINDWorldFunc)
	);
	
	(*iind_world_funcs)[*iind_world_func_count].type =
	*iind_edit_type;
	
	(*iind_world_funcs)[*iind_world_func_count].tags[0] =
	iind_edit_tags[0];
	
	(*iind_world_funcs)[*iind_world_func_count].tags[1] =
	iind_edit_tags[1];
	
	(*iind_world_funcs)[*iind_world_func_count].tags[2] =
	iind_edit_tags[2];
	
	(*iind_world_funcs)[*iind_world_func_count].tags[3] =
	iind_edit_tags[3];
	
	*iind_world_func_count += 1;
}
