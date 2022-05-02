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

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <stdbool.h>
#include <stdio.h>

bool iind_init_sdl(SDL_Window **iind_sdl_window, SDL_Renderer **iind_sdl_renderer)
{
	bool iind_init_success = false;

	if(SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		if(TTF_Init() == 0)
		{
			if(IMG_Init(IMG_INIT_PNG))
			{
				*iind_sdl_window = SDL_CreateWindow
				(
					"IINDMACH",
					SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED,
					720,
					360,
					SDL_WINDOW_SHOWN
				);

				if(*iind_sdl_window)
				{
					*iind_sdl_renderer = SDL_CreateRenderer
					(
						*iind_sdl_window,
						-1,
						SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
					);

					if(*iind_sdl_renderer) iind_init_success = true;
					else printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
					
					if
					(
						SDL_SetRenderDrawBlendMode
						(
							*iind_sdl_renderer,
							SDL_BLENDMODE_BLEND
						)
						!= 0
					)
					{
						printf("SDL_SetRenderDrawBlendMode error: %s\n", SDL_GetError());
					}
				}
				else printf("SDL_CreateWindow error: %s\n", SDL_GetError());
			}
			else printf("IMG_Init error: %s\n", SDL_GetError());
		}
		else printf("TTF_Init error: %s\n", SDL_GetError());
	}
	else printf("SDL_Init error: %s\n", SDL_GetError());

	return iind_init_success;
}

void iind_terminate_sdl(SDL_Window *window, SDL_Renderer *renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
