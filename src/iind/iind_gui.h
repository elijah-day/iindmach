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

#ifndef IIND_GUI_H
#define IIND_GUI_H

#define IIND_GUI_ELEMENT_COUNT 3

#define IIND_GUI_ELEMENT_MESSAGE_MAX_LEN 128
#define IIND_GUI_DISPLAY_COUNTER_START 255

#define IIND_GUI_HINT_ELEMENT 0
#define IIND_GUI_DIALOGUE_ELEMENT 1
#define IIND_GUI_STATUS_BAR_ELEMENT 2

#define IIND_GUI_STATUS_BAR_TEXTURE_ID 21

#define IIND_GUI_ELEMENT_DISPLAY_COUNTER_MULTIPLIER 2

#define IIND_DIALOGUE_MIN_TAG 0
#define IIND_DIALOGUE_CUR_TAG 1
#define IIND_DIALOGUE_MAX_TAG 2
#define IIND_DIALOGUE_PORTRAIT_TAG 3
#define IIND_DIALOGUE_ACTIVE_TAG 4
#define IIND_DIALOGUE_FUNC_ID_TAG 5

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

typedef struct IINDGUIElement
{
	TTF_Font *ttf_font;
	SDL_Color sdl_color;
	char text[IIND_GUI_ELEMENT_MESSAGE_MAX_LEN];
	int text_len;
	int display_counter;
	SDL_Texture *sdl_texture;
	int portrait_texture_id;
}
IINDGUIElement;

void iind_update_gui_text_element
(
	SDL_Renderer *iind_sdl_renderer,
	IINDGUIElement *iind_gui_elements,
	int iind_gui_element_index,
	TTF_Font *iind_gui_text_ttf_font,
	SDL_Color iind_gui_text_sdl_color,
	const char *iind_gui_text,
	int iind_portrait_texture_id
);

void iind_handle_gui_text_element_fade
(
	SDL_Renderer *iind_sdl_renderer,
	IINDGUIElement *iind_gui_elements,
	int iind_gui_element_index
);

#endif
