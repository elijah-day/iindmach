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
#include "iind_edit.h"
#include "iind_gui.h"
#include "iind_load.h"
#include "iind_render.h"
#include "iind_run.h"
#include "iind_world.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void iind_handle_sdl_event
(
	SDL_Event *iind_sdl_event,
	bool *iind_running_state,
	bool *iind_sdl_key_press,
	int *iind_sdl_mouse_state,
	int *iind_sdl_mouse_pos
)
{
	while(SDL_PollEvent(iind_sdl_event)) switch(iind_sdl_event->type)
	{
		case SDL_QUIT:
			*iind_running_state = false;
			break;
			
		case SDL_KEYDOWN:
			iind_sdl_key_press[iind_sdl_event->key.keysym.scancode] = true;
			break;
			
		case SDL_KEYUP:
			iind_sdl_key_press[iind_sdl_event->key.keysym.scancode] = false;
			break;
			
		case SDL_MOUSEMOTION:
			SDL_GetMouseState
			(
				&iind_sdl_mouse_pos[0],
				&iind_sdl_mouse_pos[1]
			);
			break;

		case SDL_MOUSEBUTTONDOWN:
			*iind_sdl_mouse_state =
			SDL_GetMouseState(NULL, NULL);
			break;
			
		case SDL_MOUSEBUTTONUP:
			*iind_sdl_mouse_state = 0;
			break;
	}
}

bool iind_handle_sdl_tick_intervals
(
	int *iind_refresh_rate,
	int *iind_sdl_tick_counter,
	int *iind_prev_sdl_ticks
)
{
	*iind_sdl_tick_counter += SDL_GetTicks() - *iind_prev_sdl_ticks;
	*iind_prev_sdl_ticks = SDL_GetTicks();

	if(*iind_sdl_tick_counter >= *iind_refresh_rate)
	{
		*iind_sdl_tick_counter = 0;
		return true;
	}

	return false;
}

void iind_run(SDL_Window *iind_sdl_window, SDL_Renderer *iind_sdl_renderer)
{
	bool iind_running_state = true;

	/*
	========
	FPS VARS
	========
	*/

	int iind_fps = IIND_DEFAULT_FPS;
	int iind_fps_refresh_rate = 1000 / iind_fps;
	int iind_fps_sdl_tick_counter = IIND_DEFAULT_COUNTER_VAL;
	int iind_prev_fps_sdl_ticks = IIND_DEFAULT_COUNTER_VAL;
	
	/*
	=====================
	WORLD TIME SCALE VARS
	=====================
	*/
	
	int iind_world_time_scale = IIND_DEFAULT_WORLD_TIME_SCALE;
	int iind_world_refresh_rate = 1000 / iind_world_time_scale;
	int iind_world_sdl_tick_counter = IIND_DEFAULT_COUNTER_VAL;
	int iind_prev_world_sdl_ticks = IIND_DEFAULT_COUNTER_VAL;
	
	/*
	===========
	RENDER VARS
	===========
	*/
	
	int iind_render_scale = IIND_DEFAULT_RENDER_SCALE;
	float iind_aspect_ratio = IIND_DEFAULT_ASPECT_RATIO;
	
	float iind_camera[2] = {0, 0};
	
	int iind_sdl_window_size[2];
	bool iind_sdl_window_fullscreen_state = false;
	
	int iind_anim_counter = IIND_DEFAULT_COUNTER_VAL;
	
	SDL_GetWindowSize
	(
		iind_sdl_window,
		&iind_sdl_window_size[0],
		&iind_sdl_window_size[1]
	);
	
	/*
	==========
	WORLD VARS
	==========
	*/
	
	int iind_cur_world_id = IIND_DEFAULT_WORLD_ID;
	int iind_new_world_id = IIND_DEFAULT_WORLD_ID;
	
	IINDWorldEntity iind_world_trans_entity;
	bool iind_world_trans_state = false;
	
	int iind_world_count =
	iind_get_text_file_line_count(IIND_DEFAULT_MODULE_WORLD_ID_PATH);
	
	char *iind_world_names[iind_world_count];
	
	for(int i = 0; i < iind_world_count; i++)
	{
		iind_world_names[i] =
		calloc(IIND_WORLD_NAME_MAX_LEN, sizeof(char));
		
		if(!iind_world_names[i])
		{
			printf("calloc error\n");
			iind_running_state = false;
		}
	}
	
	if(!iind_running_state) for(int i = 0; i < iind_world_count; i++)
	{
		if(iind_world_names[i]) free(iind_world_names[i]);
	}
	
	iind_read_list_file
	(
		iind_world_names,
		iind_world_count,
		IIND_DEFAULT_MODULE_WORLD_ID_PATH
	);
	
	/*
	================
	SDL TEXTURE VARS
	================
	*/
	
	const int iind_sdl_texture_count = 
	iind_get_text_file_line_count(IIND_DEFAULT_SDL_TEXTURE_LIST_PATH);
	
	SDL_Texture *iind_sdl_textures[iind_sdl_texture_count];
	iind_load_sdl_textures
	(
		iind_sdl_renderer,
		iind_sdl_textures,
		iind_sdl_texture_count,
		IIND_DEFAULT_SDL_TEXTURE_LIST_PATH
	);
	
	/*
	========
	GUI VARS
	========
	*/
	
	TTF_Font *iind_gui_text_ttf_font = TTF_OpenFont
	(
		IIND_DEFAULT_SDL_GUI_FONT_PATH,
		IIND_DEFAULT_SDL_GUI_FONT_SIZE
	);
	
	SDL_Color iind_gui_text_sdl_color = {255, 255, 255, 255};
	
	IINDGUIElement iind_gui_elements[IIND_GUI_ELEMENT_COUNT];
	
	/*
	Initialize all textures to NULL here so that they are not
	destroyed prematurely (later segmentation fault).
	*/
	for(int i = 0; i < IIND_GUI_ELEMENT_COUNT; i++)
	{
		iind_gui_elements[i].sdl_texture = NULL;
	}
	
	for(int i = 0; i < IIND_GUI_ELEMENT_COUNT; i++)
	{
		iind_update_gui_text_element
		(
			iind_sdl_renderer,
			iind_gui_elements,
			i,
			iind_gui_text_ttf_font,
			iind_gui_text_sdl_color,
			" ",
			0
		);
		
		SDL_SetTextureBlendMode
		(
			iind_gui_elements[i].sdl_texture,
			SDL_BLENDMODE_BLEND
		);
	}
	
	/*
	=============
	DIALOGUE VARS
	=============
	*/ 
	
	/*
	This shouldn't (?) need to use heap memory since it doesn't
	change from the time it's loaded in.  Rework later.  Probably.
	*/
	
	int iind_dialogue_tags[6] = {-1, -1, -1, -1, 0, -1};
	
	int iind_dialogue_count =
	iind_get_text_file_line_count(IIND_DEFAULT_DIALOGUE_LIST_PATH);
	
	char *iind_dialogue_strings[iind_dialogue_count];
	
	for(int i = 0; i < iind_dialogue_count; i++)
	{
		iind_dialogue_strings[i] =
		calloc(IIND_DIALOGUE_MAX_LEN, sizeof(char));
		
		if(!iind_dialogue_strings[i])
		{
			printf("calloc error\n");
			iind_running_state = false;
		}
	}
	
	if(!iind_running_state) for(int i = 0; i < iind_dialogue_count; i++)
	{
		if(iind_dialogue_strings[i]) free(iind_dialogue_strings[i]);
	}
	
	iind_read_list_file
	(
		iind_dialogue_strings,
		iind_dialogue_count,
		IIND_DEFAULT_DIALOGUE_LIST_PATH
	);
	
	/*
	==============
	SDL INPUT VARS
	==============
	*/
	
	SDL_Event iind_sdl_event;
	
	int iind_sdl_mouse_state = 0;
	int iind_sdl_mouse_pos[2];
	
	bool iind_sdl_key_presses[IIND_SDL_KEY_COUNT];
	bool iind_sdl_key_hold_states[IIND_SDL_KEY_COUNT];
	int iind_sdl_key_bind_ids[IIND_SDL_KEY_COUNT];
	
	for(int i = 0; i < IIND_SDL_KEY_COUNT; i++)
	{
		iind_sdl_key_presses[i] = false;
		iind_sdl_key_hold_states[i] = false;
		iind_sdl_key_bind_ids[i] = -1;
	}
	
	bool iind_input_hold_state = false;
	
	/*
	==============
	EDIT MODE VARS
	==============
	*/

	bool iind_edit_mode = true;
	
	float iind_edit_x = 0;
	float iind_edit_y = 0;
	float iind_edit_angle = 300;
	
	int iind_edit_id = 0;
	int iind_edit_type = -1;
	int iind_edit_tags[4] = {0, 0, 0, 0};
	
	int iind_edit_world_element_type = 0;
	
	char iind_edit_info_str[IIND_EDIT_INFO_STR_MAX_LEN];
	
	/*
	==============
	GAME LOAD LOOP
	==============
	*/
	
	while(iind_running_state)
	{	
		/*
		==============
		WORLD ID CHECK
		==============
		*/
	
		if(iind_new_world_id >= iind_world_count)
		{
			printf("iind_new_world_id invalid: %d\n", iind_new_world_id);
			break;
		}
		
		/*
		===================
		WORLD ELEMENT COUNT
		===================
		*/
		
		int iind_world_tile_count = 0;
		int iind_world_entity_count = 0;
		int iind_world_func_count = 0;
		
		iind_load_world_element_counts
		(
			iind_world_names[iind_new_world_id],
			&iind_world_tile_count,
			&iind_world_entity_count,
			&iind_world_func_count
		);
		
		/*
		=====================
		WORLD ELEMENT LOADING
		=====================
		*/
	
		IINDWorldMarker *iind_world_markers =
		calloc(IIND_WORLD_MARKER_COUNT, sizeof(IINDWorldMarker));
		
		IINDWorldTile *iind_world_tiles =
		calloc(iind_world_tile_count, sizeof(IINDWorldTile));
		
		IINDWorldEntity *iind_world_entities =
		calloc(iind_world_entity_count, sizeof(IINDWorldEntity));
		
		IINDWorldFunc *iind_world_funcs =
		calloc(iind_world_func_count, sizeof(IINDWorldFunc));
		
		if
		(
			iind_world_markers == NULL ||
			iind_world_tiles == NULL ||
			iind_world_entities == NULL ||
			iind_world_funcs == NULL
		)
		{
			printf("calloc error\n");
			if(iind_world_markers) free(iind_world_markers);
			if(iind_world_tiles) free(iind_world_tiles);
			if(iind_world_entities) free(iind_world_entities);
			if(iind_world_funcs) free(iind_world_funcs);
			break;
		}
		
		/*
		=============
		WORLD LOADING
		=============
		*/
		
		iind_cur_world_id = iind_new_world_id;
		
		bool iind_load_success = iind_load_world
		(
			iind_world_names[iind_new_world_id],
			iind_world_tiles,
			iind_world_entities,
			iind_world_funcs,
			iind_world_tile_count,
			iind_world_entity_count,
			iind_world_func_count
		);
		
		/*
		===================
		WORLD ELEMENT SETUP
		===================
		*/
		
		/*Here so entities don't go flying all over the place upon loading.*/
		for(int i = 0; i < iind_world_entity_count; i++)
		{
			iind_world_entities[i].speed = 0;
		}
		
		iind_world_markers[IIND_WORLD_PLAYER_MOVEMENT_MARKER].sdl_texture_id = 0;
		iind_world_markers[IIND_WORLD_PLAYER_MOVEMENT_MARKER].x = 0.0;
		iind_world_markers[IIND_WORLD_PLAYER_MOVEMENT_MARKER].y = 0.0;
		iind_world_markers[IIND_WORLD_PLAYER_MOVEMENT_MARKER].active = false;
	
		iind_world_markers[IIND_WORLD_EDIT_MODE_MARKER].sdl_texture_id = 7;
		iind_world_markers[IIND_WORLD_EDIT_MODE_MARKER].x = 0.0;
		iind_world_markers[IIND_WORLD_EDIT_MODE_MARKER].y = 0.0;
		if(iind_edit_mode)
		{
			iind_world_markers[IIND_WORLD_EDIT_MODE_MARKER].active = true;
		}		
		
		/*
		===================
		WORLD TRANSITIONING
		===================
		*/
		
		if(iind_world_trans_state)
		{
			iind_world_entities[IIND_WORLD_PLAYER_ENTITY].x =
			iind_world_trans_entity.x;
			
			iind_world_entities[IIND_WORLD_PLAYER_ENTITY].y =
			iind_world_trans_entity.y;
			
			iind_world_entities[IIND_WORLD_PLAYER_ENTITY].angle =
			iind_world_trans_entity.angle;
			
			iind_world_trans_state = false;
		}
		
		/*
		==============
		GAME RUN LOOP
		==============
		*/
		
		if(iind_load_success) while(iind_new_world_id == iind_cur_world_id)
		{
			/*
			==================
			SDL EVENT HANDLING
			==================
			*/
		
			/*Here to exit this loop upon setting the running state to false.*/
			if(!iind_running_state) iind_new_world_id -= 1;
			
			iind_handle_sdl_event
			(
				&iind_sdl_event,
				&iind_running_state,
				&iind_sdl_key_presses,
				&iind_sdl_mouse_state,
				&iind_sdl_mouse_pos
			);
			
			/*
			==============
			WORLD UPDATING
			==============
			*/
			
			if
			(
				iind_handle_sdl_tick_intervals
				(
					&iind_world_refresh_rate,
					&iind_world_sdl_tick_counter,
					&iind_prev_world_sdl_ticks
				)
			)
			{
				/*TEMPORARY KEY BIND ASSIGNMENT:*/
				iind_sdl_key_bind_ids[SDL_SCANCODE_F11] =
				IIND_FULLSCREEN_KEY_BIND_ID;
				
				iind_sdl_key_bind_ids[SDL_SCANCODE_F5] =
				IIND_QUICKSAVE_KEY_BIND_ID;
				
				iind_sdl_key_bind_ids[SDL_SCANCODE_SPACE] =
				IIND_DIALOGUE_KEY_BIND_ID;
				
				iind_sdl_key_bind_ids[SDL_SCANCODE_ESCAPE] =
				IIND_MENU_KEY_BIND_ID;
				/*END*/
				
				iind_handle_player_movement_controls
				(
					iind_world_markers,
					iind_sdl_mouse_pos,
					iind_sdl_mouse_state,
					iind_render_scale,
					iind_aspect_ratio,
					iind_camera
				);
				
				for(int i = 0; i < IIND_SDL_KEY_COUNT; i++)
				{
					if(iind_sdl_key_presses[i])
					{	
						if(iind_sdl_key_hold_states[i]) break;
						iind_sdl_key_hold_states[i] = true;
						
						if
						(
							iind_handle_navigation_controls
							(
								iind_sdl_key_bind_ids[i],
								iind_dialogue_tags
							)
						)
						{
							break;
						}
						else if
						(
							iind_handle_misc_controls
							(
								iind_sdl_key_bind_ids[i],
								iind_sdl_window,
								iind_sdl_renderer,
								iind_world_names[iind_cur_world_id],
								iind_world_tiles,
								iind_world_entities,
								iind_world_funcs,
								iind_world_tile_count,
								iind_world_entity_count,
								iind_world_func_count,
								iind_gui_elements,
								iind_gui_text_ttf_font,
								iind_gui_text_sdl_color,
								&iind_sdl_window_fullscreen_state
							)
						)
						{
							break;
						}
					}
					else
					{
						iind_sdl_key_hold_states[i] = false;
					}
				}
				
				
				/*
				BEGIN TEMP.
				EVERYTHING PAST THIS POINT SHOULD BE REWORKED INTO
				THE CONTROLS FUNCTION.
				*/
					
				if(iind_sdl_key_presses[SDL_SCANCODE_E] && !iind_input_hold_state)
				{		
					if(iind_edit_world_element_type == 0) iind_create_world_tile
					(
						&iind_world_tiles,
						&iind_world_tile_count,
						(int)iind_edit_x,
						(int)iind_edit_y,
						iind_edit_id
					);
					
					if(iind_edit_world_element_type == 1) iind_create_world_entity
					(
						&iind_world_entities,
						&iind_world_funcs,
						&iind_world_entity_count,
						&iind_world_func_count,
						&iind_edit_x,
						&iind_edit_y,
						&iind_edit_angle,
						&iind_edit_id,
						&iind_edit_type,
						iind_edit_tags
					);
					
					iind_input_hold_state = true;
				}
				
				if(iind_sdl_key_presses[SDL_SCANCODE_R] && !iind_input_hold_state)
				{		
					if(iind_edit_world_element_type == 0) iind_delete_world_tile
					(
						&iind_world_tiles,
						&iind_world_tile_count,
						(int)iind_edit_x,
						(int)iind_edit_y
					);
					
					if(iind_edit_world_element_type == 1) iind_delete_world_entity
					(
						&iind_world_entities,
						&iind_world_funcs,
						&iind_world_entity_count,
						&iind_world_func_count,
						&iind_edit_x,
						&iind_edit_y
					);
					
					iind_input_hold_state = true;
				}
				
				if(iind_sdl_key_presses[SDL_SCANCODE_DOWN] && !iind_input_hold_state)
				{		
					iind_edit_y -= 0.5;
				}
				
				if(iind_sdl_key_presses[SDL_SCANCODE_UP] && !iind_input_hold_state)
				{		
					iind_edit_y += 0.5;
				}
				
				if(iind_sdl_key_presses[SDL_SCANCODE_LEFT] && !iind_input_hold_state)
				{		
					iind_edit_x -= 0.5;
				}
				
				if(iind_sdl_key_presses[SDL_SCANCODE_RIGHT] && !iind_input_hold_state)
				{		
					iind_edit_x += 0.5;
				}
				
				if(iind_sdl_key_presses[SDL_SCANCODE_1] && !iind_input_hold_state)
				{		
					iind_edit_world_element_type = 0;
				}
				if(iind_sdl_key_presses[SDL_SCANCODE_2] && !iind_input_hold_state)
				{		
					iind_edit_world_element_type = 1;
				}
				
				if
				(
					iind_sdl_key_presses[SDL_SCANCODE_SEMICOLON] &&
					!iind_input_hold_state
				)
				{		
					iind_edit_type -= 1;
				}
				
				if
				(
					iind_sdl_key_presses[SDL_SCANCODE_APOSTROPHE] &&
					!iind_input_hold_state
				)
				{		
					iind_edit_type += 1;
				}
				
				if(iind_sdl_key_presses[SDL_SCANCODE_COMMA] && !iind_input_hold_state)
				{		
					iind_edit_id -= 1;
				}
				
				if(iind_sdl_key_presses[SDL_SCANCODE_PERIOD] && !iind_input_hold_state)
				{		
					iind_edit_id += 1;
				}
				
				if(iind_sdl_key_presses[SDL_SCANCODE_H] && !iind_input_hold_state)
				{		
					iind_edit_tags[0] -= 1;
				}
				if(iind_sdl_key_presses[SDL_SCANCODE_Y] && !iind_input_hold_state)
				{		
					iind_edit_tags[0] += 1;
				}
				
				if(iind_sdl_key_presses[SDL_SCANCODE_J] && !iind_input_hold_state)
				{		
					iind_edit_tags[1] -= 1;
				}
				if(iind_sdl_key_presses[SDL_SCANCODE_U] && !iind_input_hold_state)
				{		
					iind_edit_tags[1] += 1;
				}
				
				if(iind_sdl_key_presses[SDL_SCANCODE_K] && !iind_input_hold_state)
				{		
					iind_edit_tags[2] -= 1;
				}
				if(iind_sdl_key_presses[SDL_SCANCODE_I] && !iind_input_hold_state)
				{		
					iind_edit_tags[2] += 1;
				}
				
				if(iind_sdl_key_presses[SDL_SCANCODE_L] && !iind_input_hold_state)
				{		
					iind_edit_tags[3] -= 1;
				}
				if(iind_sdl_key_presses[SDL_SCANCODE_O] && !iind_input_hold_state)
				{		
					iind_edit_tags[3] += 1;
				}
				
				/*Health bar testing.*/
				
				if(iind_sdl_key_presses[SDL_SCANCODE_Z])
				{		
					iind_world_entities[0].health -= 0.5;
				}
				if(iind_sdl_key_presses[SDL_SCANCODE_X])
				{		
					iind_world_entities[0].health += 0.5;
				}
				if(iind_sdl_key_presses[SDL_SCANCODE_C])
				{		
					iind_world_entities[0].health = 100;
				}
				
				/*Prevents holding down certain keys.*/
				for(int i = 0; i < IIND_SDL_KEY_COUNT; i++)
				{
					iind_input_hold_state = false;
					
					if(iind_sdl_key_presses[i] == true)
					{
						iind_input_hold_state = true;
						break;
					}
				}
				
				if(iind_edit_mode && iind_input_hold_state)
				{
					sprintf
					(
						iind_edit_info_str,
						"WET: %d | ID: %d | TYPE: %d | TAG1: %d | TAG2: %d | TAG3: %d | TAG4: %d",
						iind_edit_world_element_type,
						iind_edit_id,
						iind_edit_type,
						iind_edit_tags[0],
						iind_edit_tags[1],
						iind_edit_tags[2],
						iind_edit_tags[3]
					);
					
					iind_update_gui_text_element
					(
						iind_sdl_renderer,
						iind_gui_elements,
						IIND_GUI_HINT_ELEMENT,
						iind_gui_text_ttf_font,
						iind_gui_text_sdl_color,
						iind_edit_info_str,
						0
					);
				}
				/*
				END TEMP.
				EVERYTHING BEFORE THIS POINT SHOULD BE REWORKED INTO
				THE CONTROLS FUNCTION.
				*/
				
				if(iind_edit_mode)
				{
					iind_world_markers[IIND_WORLD_EDIT_MODE_MARKER].x =
					iind_edit_x;
					
					iind_world_markers[IIND_WORLD_EDIT_MODE_MARKER].y =
					iind_edit_y;
				}
				
				iind_update_world_entity_anims
				(
					iind_world_entities,
					iind_world_entity_count,
					iind_anim_counter
				);
				
				iind_handle_player_movement
				(
					iind_world_markers,
					iind_world_tiles,
					iind_world_entities,
					iind_world_tile_count,
					iind_world_entity_count
				);
				
				if(iind_dialogue_tags[IIND_DIALOGUE_MIN_TAG] > -1)
				{
					iind_update_dialogue
					(
						iind_sdl_renderer,
						iind_gui_elements,
						IIND_GUI_DIALOGUE_ELEMENT,
						iind_gui_text_ttf_font,
						iind_gui_text_sdl_color,
						iind_dialogue_strings,
						iind_dialogue_tags
					);
				}
				
				iind_handle_world_funcs
				(
					iind_world_entities,
					iind_world_funcs,
					iind_world_entity_count,
					iind_world_func_count,
					&iind_new_world_id,
					&iind_world_trans_entity,
					&iind_world_trans_state,
					&iind_dialogue_tags
				);
				
				iind_anim_counter += 1;
				if(iind_anim_counter > IIND_DEFAULT_ANIM_SPEED)
				{
					iind_anim_counter = 0;
				}
				
				/*
				I'm not sure where else to put this at the moment.
				Possibly add a misc. updating function for the animations,
				health values, etc.
				*/
				if(iind_world_entities[IIND_WORLD_PLAYER_ENTITY].health < 0)
				{
					iind_world_entities[IIND_WORLD_PLAYER_ENTITY].health = 0;
				}
				if
				(
					iind_world_entities[IIND_WORLD_PLAYER_ENTITY].health >
					IIND_WORLD_ENTITY_MAX_HEALTH
				)
				{
					iind_world_entities[IIND_WORLD_PLAYER_ENTITY].health = 100;
				}
			}
			
			/*
			===============
			RENDER UPDATING
			===============
			*/
			
			else if
			(
				iind_handle_sdl_tick_intervals
				(
					&iind_fps_refresh_rate,
					&iind_fps_sdl_tick_counter,
					&iind_prev_fps_sdl_ticks
				)
			)
			{	
				SDL_GetWindowSize
				(
					iind_sdl_window,
					&iind_sdl_window_size[0],
					&iind_sdl_window_size[1]
				);
			
				iind_camera[0] =
				iind_world_entities[IIND_WORLD_PLAYER_ENTITY].x + 0.75 +
				iind_world_entities[IIND_WORLD_PLAYER_ENTITY].y -
				iind_sdl_window_size[0] / iind_render_scale / iind_aspect_ratio / 2;
					
				iind_camera[1] =
				iind_sdl_window_size[1] / iind_render_scale +
				iind_world_entities[IIND_WORLD_PLAYER_ENTITY].y + 0.75 -
				iind_world_entities[IIND_WORLD_PLAYER_ENTITY].x;
			
				iind_handle_gui_text_element_fade
				(
					iind_sdl_renderer,
					iind_gui_elements,
					IIND_GUI_HINT_ELEMENT
				);
				
				if(iind_gui_elements[IIND_GUI_DIALOGUE_ELEMENT].display_counter > 0)
				iind_gui_elements[IIND_GUI_DIALOGUE_ELEMENT].display_counter -= 1;
			
				iind_render_world
				(
					iind_sdl_renderer,
					iind_sdl_textures,
					iind_world_markers,
					iind_world_tiles,
					iind_world_entities,
					iind_world_tile_count,
					iind_world_entity_count,
					iind_render_scale,
					iind_aspect_ratio,
					iind_camera
				);
				
				iind_render_gui
				(
					iind_sdl_renderer,
					iind_world_entities,
					iind_gui_elements,
					iind_render_scale,
					iind_aspect_ratio,
					iind_sdl_window_size,
					iind_sdl_textures
				);
						
				SDL_RenderPresent(iind_sdl_renderer);
			}
			
			/*
			=====
			DELAY
			=====
			*/
			
			/*Should probably make this an adjustable in-game setting later on.*/
			SDL_Delay(4);
		}
		else
		{
			printf("iind_load_world error\n");
			iind_running_state = false;
		}
		
		/*
		=======
		CLEANUP
		=======
		*/
		
		if(iind_world_markers) free(iind_world_markers);
		if(iind_world_tiles) free(iind_world_tiles);
		if(iind_world_entities) free(iind_world_entities);
		if(iind_world_funcs) free(iind_world_funcs);
		
		if(iind_sdl_textures) SDL_DestroyTexture(iind_sdl_textures);
		
		for(int i = 0; i < IIND_GUI_ELEMENT_COUNT; i++)
		{
			if(iind_gui_elements[i].sdl_texture)
			{
				SDL_DestroyTexture(iind_gui_elements[i].sdl_texture);
			}
		}
	}
	
	/*
	=======
	CLEANUP
	=======
	*/
	
	for(int i = 0; i < iind_world_count; i++)
	{
		if(iind_world_names[i]) free(iind_world_names[i]);
	}
	
	for(int i = 0; i < iind_dialogue_count; i++)
	{
		if(iind_dialogue_strings[i]) free(iind_dialogue_strings[i]);
	}
}
