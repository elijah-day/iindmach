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
#include "iind_load.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <stdlib.h>

void iind_update_gui_text_element
(
	SDL_Renderer *iind_sdl_renderer,
	IINDGUIElement *iind_gui_elements,
	int iind_gui_element_index,
	TTF_Font *iind_gui_text_ttf_font,
	SDL_Color iind_gui_text_sdl_color,
	const char *iind_gui_text,
	int iind_portrait_texture_id
)
{
	iind_gui_elements[iind_gui_element_index].ttf_font =
	iind_gui_text_ttf_font;
	
	iind_gui_elements[iind_gui_element_index].sdl_color =
	iind_gui_text_sdl_color;
	
	strcpy(iind_gui_elements[iind_gui_element_index].text, iind_gui_text);
	
	iind_gui_elements[iind_gui_element_index].text_len =
	strlen(iind_gui_text);
	
	iind_gui_elements[iind_gui_element_index].portrait_texture_id =
	iind_portrait_texture_id;
	
	/*
	This is what was causing the memory leak (or, rather, the lack of
	this piece of code).  Every time a new texture was created
	for the updated GUI element, the previous one wasn't actually freed.
	That's why memory usage was stacking up.
	*/
	if(iind_gui_elements[iind_gui_element_index].sdl_texture != NULL)
	{
		SDL_DestroyTexture(iind_gui_elements[iind_gui_element_index].sdl_texture);
	}
	
	iind_gui_elements[iind_gui_element_index].sdl_texture =
	iind_load_sdl_texture_from_font
	(
		iind_sdl_renderer,
		iind_gui_elements[iind_gui_element_index].ttf_font,
		iind_gui_elements[iind_gui_element_index].sdl_color,
		iind_gui_elements[iind_gui_element_index].text
	);
	
	if(strcmp(iind_gui_text, " ") != 0)
	{
		if(iind_gui_element_index == IIND_GUI_HINT_ELEMENT)
		{
			iind_gui_elements[iind_gui_element_index].display_counter = 
			IIND_GUI_DISPLAY_COUNTER_START;
		}
		else if(iind_gui_element_index == IIND_GUI_DIALOGUE_ELEMENT)
		{
			iind_gui_elements[iind_gui_element_index].display_counter = 
			iind_gui_elements[iind_gui_element_index].text_len *
			IIND_GUI_ELEMENT_DISPLAY_COUNTER_MULTIPLIER;
		}
	}
}

void iind_handle_gui_text_element_fade
(
	SDL_Renderer *iind_sdl_renderer,
	IINDGUIElement *iind_gui_elements,
	int iind_gui_element_index
)
{
	if
	(
		iind_gui_elements[iind_gui_element_index].display_counter <=
		IIND_GUI_DISPLAY_COUNTER_START &&
		iind_gui_elements[iind_gui_element_index].display_counter > 0
	)
	{
		if
		(
			iind_gui_elements[iind_gui_element_index].display_counter <
			IIND_GUI_DISPLAY_COUNTER_START / 2
		)
		{
			SDL_SetTextureAlphaMod
			(
				iind_gui_elements[iind_gui_element_index].sdl_texture,
				iind_gui_elements[iind_gui_element_index].display_counter * 2
			);
		}
	
		iind_gui_elements[iind_gui_element_index].display_counter -= 1;
	}
	else if(iind_gui_elements[iind_gui_element_index].display_counter == 0)
	{
		iind_update_gui_text_element
		(
			iind_sdl_renderer,
			iind_gui_elements,
			iind_gui_element_index,
			iind_gui_elements[iind_gui_element_index].ttf_font,
			iind_gui_elements[iind_gui_element_index].sdl_color,
			" ",
			0
		);
		
		/*Prevents updating the element unnecessarily.*/
		iind_gui_elements[iind_gui_element_index].display_counter -= 1;
	}
}

void iind_update_dialogue
(
	SDL_Renderer *iind_sdl_renderer,
	IINDGUIElement *iind_gui_elements,
	int iind_gui_dialogue_element_index,
	TTF_Font *iind_gui_text_ttf_font,
	SDL_Color iind_gui_text_sdl_color,
	char *iind_dialogue_strings[],
	int *iind_dialogue_tags
)
{
	if
	(
		iind_dialogue_tags[IIND_DIALOGUE_CUR_TAG] ==
		iind_dialogue_tags[IIND_DIALOGUE_MIN_TAG]
	)
	{
		iind_update_gui_text_element
		(
			iind_sdl_renderer,
			iind_gui_elements,
			iind_gui_dialogue_element_index,
			iind_gui_text_ttf_font,
			iind_gui_text_sdl_color,
			iind_dialogue_strings[iind_dialogue_tags[IIND_DIALOGUE_CUR_TAG]],
			iind_dialogue_tags[IIND_DIALOGUE_PORTRAIT_TAG]
		);
					
		iind_dialogue_tags[IIND_DIALOGUE_MIN_TAG] += 1;
	}
	else if
	(
		iind_dialogue_tags[IIND_DIALOGUE_CUR_TAG] ==
		iind_dialogue_tags[IIND_DIALOGUE_MAX_TAG]
	)
	{
		iind_update_gui_text_element
		(
			iind_sdl_renderer,
			iind_gui_elements,
			iind_gui_dialogue_element_index,
			iind_gui_text_ttf_font,
			iind_gui_text_sdl_color,
			" ",
			0
		);
					
		iind_dialogue_tags[IIND_DIALOGUE_MIN_TAG] = -1;
		iind_dialogue_tags[IIND_DIALOGUE_CUR_TAG] = -1;
		iind_dialogue_tags[IIND_DIALOGUE_MAX_TAG] = -1;
	}
}
	
