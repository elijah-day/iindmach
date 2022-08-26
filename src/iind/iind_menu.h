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

#ifndef IIND_MENU_H
#define IIND_MENU_H

#define IIND_MENU_ITEM_STR_MAX_LEN 16

typedef struct IINDMenuItem
{
	char item_string[IIND_MENU_ITEM_STR_MAX_LEN];
	struct IINDMenuItem *sub_menu;
}
IINDMenuItem;

typedef struct IINDMenuNav
{
	struct IINDMenuItem *prev_sub_menu;
	struct IINDMenuItem *selected_sub_menu;
	int menu_item_max_count;
	int selected_menu_item;
}
IINDMenuNav;

#endif
