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

#include "iind/iind_init.h"
#include "iind/iind_run.h"
#include "SDL2/SDL.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
	SDL_Window *iind_sdl_window = NULL;
	SDL_Renderer *iind_sdl_renderer = NULL;

	if(iind_init_sdl(&iind_sdl_window, &iind_sdl_renderer))
	{
		iind_run
		(
			argc,
			argv,
			iind_sdl_window,
			iind_sdl_renderer
		);
	}

	iind_terminate_sdl(iind_sdl_window, iind_sdl_renderer);

	return 0;
}
