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

#ifndef IIND_INIT_H
#define IIND_INIT_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdbool.h>

bool iind_init_sdl(SDL_Window **iind_sdl_window, SDL_Renderer **iind_sdl_renderer);
void iind_terminate_sdl(SDL_Window *iind_sdl_window, SDL_Renderer *iind_sdl_renderer);

#endif
