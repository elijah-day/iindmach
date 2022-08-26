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
#include "iind_gui.h"
#include "iind_load.h"
#include "iind_menu.h"
#include "iind_run.h"
#include "iind_world.h"
#include <math.h>
#include "SDL2/SDL.h"
#include <stdbool.h>

void iind_handle_player_movement_controls
(
	IINDWorldMarker *iind_world_markers,
	int iind_sdl_mouse_pos[],
	int iind_sdl_mouse_state,
	int iind_render_scale,
	float iind_aspect_ratio,
	float iind_camera[]
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

bool iind_handle_navigation_controls
(
	int iind_sdl_key_bind_id,
	int *iind_dialogue_tags,
	bool *iind_menu_open_state,
	bool *iind_menu_selection_state,
	IINDMenuItem *iind_main_menu,
	IINDMenuNav *iind_menu_nav
)
{
	switch(iind_sdl_key_bind_id)
	{
		case IIND_DIALOGUE_KEY_BIND_ID:
			iind_dialogue_tags[IIND_DIALOGUE_CUR_TAG] += 1;
			break;
			
		case IIND_MENU_OPEN_KEY_BIND_ID:
			*iind_menu_open_state = !(*iind_menu_open_state);
			break;
			
		case IIND_MENU_LEFT_KEY_BIND_ID:
			iind_menu_nav->selected_menu_item -= 1;
			break;
			
		case IIND_MENU_RIGHT_KEY_BIND_ID:
			iind_menu_nav->selected_menu_item += 1;
			break;
			
		case IIND_MENU_SELECT_KEY_BIND_ID:
			if(*iind_menu_open_state)
			{
				if
				(
					(*iind_menu_nav).selected_sub_menu
					[iind_menu_nav->selected_menu_item]
					.sub_menu != NULL
				)
				{
					(*iind_menu_nav).prev_sub_menu = 
					(*iind_menu_nav).selected_sub_menu;
					
					(*iind_menu_nav).selected_sub_menu =
					(*iind_menu_nav).prev_sub_menu
					[iind_menu_nav->selected_menu_item].sub_menu;
				}
				
				*iind_menu_selection_state = true;
			}
			break;
			
		default:
			return false;
	}
	
	return true;
}

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
)
{
	switch(iind_sdl_key_bind_id)
	{
		case IIND_FULLSCREEN_KEY_BIND_ID:
		
			if(*iind_sdl_window_fullscreen_state)
			{
				SDL_SetWindowFullscreen
				(
					iind_sdl_window,
					0
				);
				
				*iind_sdl_window_fullscreen_state = false;
			}
			else
			{
				SDL_SetWindowFullscreen
				(
					iind_sdl_window,
					SDL_WINDOW_FULLSCREEN_DESKTOP
				);
				
				*iind_sdl_window_fullscreen_state = true;
			}
			
			break;
			
		case IIND_QUICKSAVE_KEY_BIND_ID:
		
			iind_save_world
			(
				iind_world_save_name,
				iind_world_tiles,
				iind_world_entities,
				iind_world_funcs,
				iind_world_tile_count,
				iind_world_entity_count,
				iind_world_func_count
			);
		
			iind_update_gui_text_element
			(
				iind_sdl_renderer,
				iind_gui_elements,
				IIND_GUI_HINT_ELEMENT,
				iind_gui_text_ttf_font,
				iind_gui_text_sdl_color,
				"Quicksaving...",
				0
			);
		
			break;
			
		default:
			return false;
	}
	
	return true;
}






