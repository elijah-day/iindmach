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

#include "iind_gui.h"
#include "iind_render.h"
#include "iind_run.h"
#include "iind_world.h"
#include <math.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <stdio.h>

void iind_render_gui
(
	SDL_Renderer *iind_sdl_renderer,
	IINDGUIElement *iind_gui_elements,
	int iind_render_scale,
	float iind_aspect_ratio,
	int iind_sdl_window_size[],
	SDL_Texture *iind_sdl_textures[]
)
{
	/*
	=====
	SETUP
	=====
	*/
	
	SDL_Rect iind_sdl_srcrect;
	SDL_Rect iind_sdl_destrect;
	
	/*
	==========================
	GUI HINT ELEMENT RENDERING
	==========================
	*/
	
	/*
	TODO: Should probably find a way to replace these
	strcmps with int comparison or similar.
	*/
	if(strcmp(iind_gui_elements[IIND_GUI_HINT_ELEMENT].text, " ") != 0)
	{	
		iind_sdl_destrect.x = iind_render_scale * iind_aspect_ratio / 5;
		
		iind_sdl_destrect.y = iind_render_scale * iind_aspect_ratio / 10;
		
		iind_sdl_destrect.w =
		iind_gui_elements[IIND_GUI_HINT_ELEMENT].text_len *
		iind_render_scale * iind_aspect_ratio / 5;
		
		iind_sdl_destrect.h = iind_render_scale / 2;

		if
		(
			SDL_RenderCopy
			(
				iind_sdl_renderer,
				iind_gui_elements[IIND_GUI_HINT_ELEMENT].sdl_texture,
				NULL,
				&iind_sdl_destrect
			)
			!= 0
		)
		{
			printf("SDL_RenderCopy error: %s\n", SDL_GetError());
		}
	}
	
	/*
	==============================
	GUI DIALOGUE ELEMENT RENDERING
	==============================
	*/
	
	if(strcmp(iind_gui_elements[IIND_GUI_DIALOGUE_ELEMENT].text, " ") != 0)
	{
		/*
		==============
		TEXT RENDERING
		==============
		*/
	
		/*Dest*/
	
		iind_sdl_destrect.x =
		iind_sdl_window_size[0] / 2 -
		iind_gui_elements[IIND_GUI_DIALOGUE_ELEMENT].text_len *
		iind_render_scale * iind_aspect_ratio / 10;
		
		iind_sdl_destrect.y = iind_sdl_window_size[1] - iind_render_scale;
		
		iind_sdl_destrect.w =
		(iind_gui_elements[IIND_GUI_DIALOGUE_ELEMENT].text_len -
		iind_gui_elements[IIND_GUI_DIALOGUE_ELEMENT].display_counter /
		IIND_GUI_ELEMENT_DISPLAY_COUNTER_MULTIPLIER) *
		iind_render_scale * iind_aspect_ratio / 5;
		
		iind_sdl_destrect.h = iind_render_scale / 2;
		
		/*Src*/
		
		iind_sdl_srcrect.x = 0;
		iind_sdl_srcrect.y = 0;
		
		iind_sdl_srcrect.w =
		(IIND_DEFAULT_SDL_GUI_FONT_SIZE / 2 + 1) *
		(iind_gui_elements[IIND_GUI_DIALOGUE_ELEMENT].text_len -
		iind_gui_elements[IIND_GUI_DIALOGUE_ELEMENT].display_counter /
		IIND_GUI_ELEMENT_DISPLAY_COUNTER_MULTIPLIER);
		
		iind_sdl_srcrect.h = IIND_DEFAULT_SDL_GUI_FONT_SIZE * 2;
		
		/*Rendering*/
		
		SDL_SetRenderDrawColor
		(
			iind_sdl_renderer,
			IIND_GUI_TEXT_BOX_R,
			IIND_GUI_TEXT_BOX_G,
			IIND_GUI_TEXT_BOX_B,
			IIND_GUI_TEXT_BOX_A
		);
		
		if
		(
			SDL_RenderFillRect(iind_sdl_renderer, &iind_sdl_destrect)
			!= 0
		)
		{
			printf("SDL_RenderFillRect error: %s\n", SDL_GetError());
		}
		
		if
		(
			SDL_RenderCopy
			(
				iind_sdl_renderer,
				iind_gui_elements[IIND_GUI_DIALOGUE_ELEMENT].sdl_texture,
				&iind_sdl_srcrect,
				&iind_sdl_destrect
			)
			!= 0
		)
		{
			printf("SDL_RenderCopy error: %s\n", SDL_GetError());
		}
		
		/*
		==================
		PORTRAIT RENDERING
		==================
		*/
		
		iind_sdl_destrect.x = iind_sdl_window_size[0] / 2 -
		iind_render_scale * iind_aspect_ratio / 2;
		
		iind_sdl_destrect.y = iind_sdl_window_size[1] - iind_render_scale * 2.1;
		
		iind_sdl_destrect.w = iind_render_scale * iind_aspect_ratio;
		iind_sdl_destrect.h = iind_render_scale;
		
		if
		(
			SDL_RenderFillRect(iind_sdl_renderer, &iind_sdl_destrect)
			!= 0
		)
		{
			printf("SDL_RenderFillRect error: %s\n", SDL_GetError());
		}
		
		if
		(
			SDL_RenderCopy
			(
				iind_sdl_renderer,
				iind_sdl_textures[iind_gui_elements[IIND_GUI_DIALOGUE_ELEMENT].portrait_texture_id],
				NULL,
				&iind_sdl_destrect
			)
			!= 0
		)
		{
			printf("SDL_RenderCopy error: %s\n", SDL_GetError());
		}
	}
	
	/*
	================================
	GUI STATUS BAR ELEMENT RENDERING
	================================
	*/
	
	{
		/*
		====================
		ENERGY BAR RENDERING
		====================
		*/
		
		SDL_SetRenderDrawColor
		(
			iind_sdl_renderer,
			IIND_GUI_ENERGY_BAR_R,
			IIND_GUI_ENERGY_BAR_G,
			IIND_GUI_ENERGY_BAR_B,
			IIND_GUI_ENERGY_BAR_A
		);
		
		iind_sdl_destrect.x =
		iind_sdl_window_size[0] - 3.25 * iind_render_scale * iind_aspect_ratio;
			
		iind_sdl_destrect.y = 0.2 * iind_render_scale;
			
		iind_sdl_destrect.w =
		3 * iind_render_scale * iind_aspect_ratio;
		
		iind_sdl_destrect.h =
		0.5 * iind_render_scale;
	
		if
		(
			SDL_RenderFillRect(iind_sdl_renderer, &iind_sdl_destrect)
			!= 0
		)
		{
			printf("SDL_RenderFillRect error: %s\n", SDL_GetError());
		}
	
		/*
		=================
		OVERLAY RENDERING
		=================
		*/
		
		/*
		This isn't commented anywhere else in the project, but this is as good
		of a place as any to do it.  Some of the weird hardcoded numbers for
		the SDL rect x, y, w, and h values are based on the width-to-height ratio
		of certain textures.  (e.g. the status bar is 4:1 at the time of writing
		this, which is why the x and w values are multiplied by 4 and h is
		multiplied by 1.)
		*/
		
		iind_sdl_destrect.x =
		iind_sdl_window_size[0] - 4 * iind_render_scale * iind_aspect_ratio;
			
		iind_sdl_destrect.y = 0;
			
		iind_sdl_destrect.w =
		4 * iind_render_scale * iind_aspect_ratio;
		
		iind_sdl_destrect.h =
		iind_render_scale;

		if
		(
			SDL_RenderCopy
			(
				iind_sdl_renderer,
				iind_sdl_textures[IIND_GUI_STATUS_BAR_TEXTURE_ID],
				NULL,
				&iind_sdl_destrect
			)
			!= 0
		)
		{
			printf("SDL_RenderCopy error: %s\n", SDL_GetError());
		}
	}

}

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
)
{
	/*
	=====
	SETUP
	=====
	*/

	if(SDL_SetRenderDrawColor(iind_sdl_renderer, 0, 0, 0, 255) != 0)
	{
		printf("SDL_SetRenderDrawColor error: %s\n", SDL_GetError());
	}
	
	SDL_RenderClear(iind_sdl_renderer);
	SDL_Rect iind_sdl_destrect;
	
	/*
	====================
	WORLD TILE RENDERING
	====================
	*/
	
	for(int i = 0; i < iind_world_tile_count; i++)
	{
		iind_sdl_destrect.x =
		(iind_world_tiles[i].x + iind_world_tiles[i].y - iind_camera[0]) *
		iind_render_scale * iind_aspect_ratio;
			
		iind_sdl_destrect.y =
		(iind_world_tiles[i].x - iind_world_tiles[i].y + iind_camera[1]) *
		iind_render_scale / 2;
			
		iind_sdl_destrect.w = 2 * iind_render_scale * iind_aspect_ratio;
		iind_sdl_destrect.h = iind_render_scale;
		
		if
		(
			SDL_RenderCopy
			(
				iind_sdl_renderer,
				iind_sdl_textures[iind_world_tiles[i].sdl_texture_id],
				NULL,
				&iind_sdl_destrect
			)
			!= 0
		)
		{
			printf("SDL_RenderCopy error: %s\n", SDL_GetError());
		}
	}
	
	/*
	======================
	WORLD MARKER RENDERING
	======================
	*/
	
	for(int i = 0; i < IIND_WORLD_MARKER_COUNT; i++)
	{
		if(iind_world_markers[i].active)
		{
			iind_sdl_destrect.x =
			(iind_world_markers[i].x + iind_world_markers[i].y - iind_camera[0]) *
			iind_render_scale * iind_aspect_ratio;
					
			iind_sdl_destrect.y =
			(iind_world_markers[i].x - iind_world_markers[i].y + iind_camera[1]) *
			iind_render_scale / 2 - iind_render_scale;
					
			iind_sdl_destrect.w = 2 * iind_render_scale * iind_aspect_ratio;
			iind_sdl_destrect.h = 2 * iind_render_scale;
			
			if
			(
				SDL_RenderCopy
				(
					iind_sdl_renderer,
					iind_sdl_textures[iind_world_markers[i].sdl_texture_id],
					NULL,
					&iind_sdl_destrect
				)
				!= 0
			)
			{
				printf("SDL_RenderCopy error: %s\n", SDL_GetError());
			}
		}
	}
	
	/*
	============
	ENTITY QUEUE
	============
	*/
		
	int iind_world_entity_render_queue[iind_world_entity_count];
	
	for(int i = 0; i < iind_world_entity_count; i++)
	{
		int iind_render_queue_pos = 0;
	
		for(int j = 0; j < iind_world_entity_count; j++)
		{	
			if
			(
				i != j &&
				(iind_world_entities[i].x - iind_world_entities[i].y) >
				(iind_world_entities[j].x - iind_world_entities[j].y)
			)
			{
				iind_render_queue_pos += 1;
			}
			else if
			(
				i != j &&
				(iind_world_entities[i].x - iind_world_entities[i].y) ==
				(iind_world_entities[j].x - iind_world_entities[j].y)
			)
			{
				iind_world_entities[i].x += IIND_WORLD_ENTITY_OVERLAP_SHIFT;
			}
		}
		
		iind_world_entity_render_queue[iind_render_queue_pos] = i;
	}
	
	SDL_Rect iind_sdl_srcrect;
	
	/*
	======================
	WORLD ENTITY RENDERING
	======================
	*/
	
	for(int i = 0; i < iind_world_entity_count; i++)
	{
		int j = iind_world_entity_render_queue[i];
		
		if(j < iind_world_entity_count && j > -1)
		{
			iind_sdl_srcrect.x =
			iind_world_entities[j].anim_col * IIND_DEFAULT_WORLD_ENTITY_ANIM_W;
			
			iind_sdl_srcrect.y =
			iind_world_entities[j].anim_row * IIND_DEFAULT_WORLD_ENTITY_ANIM_W;
			
			iind_sdl_srcrect.w = IIND_DEFAULT_WORLD_ENTITY_ANIM_W;
			iind_sdl_srcrect.h = iind_sdl_srcrect.w;
			
			iind_sdl_destrect.x =
			(iind_world_entities[j].x + iind_world_entities[j].y - iind_camera[0]) *
			iind_render_scale * iind_aspect_ratio;
					
			iind_sdl_destrect.y =
			(iind_world_entities[j].x - iind_world_entities[j].y + iind_camera[1]) *
			iind_render_scale / 2 - iind_render_scale;
					
			iind_sdl_destrect.w = 2 * iind_render_scale * iind_aspect_ratio;
			iind_sdl_destrect.h = 2 * iind_render_scale;
			
			if
			(
				SDL_RenderCopy
				(
					iind_sdl_renderer,
					iind_sdl_textures[iind_world_entities[j].sdl_texture_id],
					&iind_sdl_srcrect,
					&iind_sdl_destrect
				)
				!= 0
			)
			{
					printf("SDL_RenderCopy error: %s\n", SDL_GetError());
			}
		}
		else
		{
			printf("iind_world_render_queue[%d] invalid: %d\n", i, j);
		}
	}
}
