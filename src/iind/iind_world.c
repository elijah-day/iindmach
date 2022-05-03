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
#include "iind_world.h"
#include <math.h>
#include <stdbool.h>

bool iind_test_world_entity_distance
(
	IINDWorldEntity *iind_world_entities,
	int iind_world_entity_id,
	float iind_world_entity_distance,
	float iind_x,
	float iind_y
)
{
	if
	(
		iind_world_entities[iind_world_entity_id].x + 0.5 >
		iind_x - iind_world_entity_distance + 1 &&
				
		iind_world_entities[iind_world_entity_id].x + 0.5 <
		iind_x + iind_world_entity_distance &&
				
		iind_world_entities[iind_world_entity_id].y + 0.5 >
		iind_y - iind_world_entity_distance + 1 &&
				
		iind_world_entities[iind_world_entity_id].y + 0.5 <
		iind_y + iind_world_entity_distance
	)
	{
		return true;
	}
	
	else
	{
		return false;
	}
}

bool iind_move_world_entity
(
	IINDWorldTile *iind_world_tiles,
	IINDWorldEntity *iind_world_entities,
	int iind_world_tile_count,
	int iind_world_entity_count,
	int iind_target_world_entity_id,
	float iind_world_entity_speed,
	float iind_world_entity_direction
)
{
	float iind_prev_x = iind_world_entities[iind_target_world_entity_id].x;
	float iind_prev_y = iind_world_entities[iind_target_world_entity_id].y;

	iind_world_entities[iind_target_world_entity_id].x +=
	iind_world_entity_speed * cos(iind_world_entity_direction * IIND_PI / 180);
			
	iind_world_entities[iind_target_world_entity_id].y +=
	iind_world_entity_speed * sin(iind_world_entity_direction * IIND_PI / 180);

	bool iind_collision_state = true;
	
	for(int i = 0; i < iind_world_tile_count; i++)
	{
		if
		(
			iind_test_world_entity_distance
			(
				iind_world_entities,
				iind_target_world_entity_id,
				1,
				iind_world_tiles[i].x,
				iind_world_tiles[i].y
			)
		)
		{
			iind_collision_state = false;
			break;
		}
	}
	
	for(int i = 0; i < iind_world_entity_count; i++)
	{
		if(iind_world_entities[i].collision)
		{
			if
			(
				iind_test_world_entity_distance
				(
					iind_world_entities,
					iind_target_world_entity_id,
					1,
					iind_world_entities[i].x,
					iind_world_entities[i].y
				) &&
				i != iind_target_world_entity_id
			)
			{
				iind_collision_state = true;
				break;
			}
		}
	}
	
	if(iind_collision_state)
	{
		iind_world_entities[iind_target_world_entity_id].x = iind_prev_x;
		iind_world_entities[iind_target_world_entity_id].y = iind_prev_y;
	}
	
	return iind_collision_state;
}

void iind_handle_world_funcs
(
	IINDWorldEntity *iind_world_entities,
	IINDWorldFunc *iind_world_funcs,
	int iind_world_entity_count,
	int iind_world_func_count,
	int *iind_new_world_id,
	IINDWorldEntity *iind_world_trans_entity,
	bool *iind_world_trans_state,
	int *iind_dialogue_tags
)
{	
	for(int i = 0; i < iind_world_func_count; i++)
	{
		
		/*
		=======================
		COMPONENT POWER UPDATES
		=======================
		*/
		
		for(int j = 0; j < iind_world_func_count; j++)
		{
			if
			(	
				iind_world_funcs[i].tags[IIND_WORLD_FUNC_OUTPUT_TAG] ==
				iind_world_funcs[j].tags[IIND_WORLD_FUNC_INPUT_TAG]
			)
			{
				if(iind_world_funcs[i].tags[IIND_WORLD_FUNC_LH_TAG] > 0)
				switch(iind_world_funcs[j].type)
				{
					case IIND_WORLD_TIMER_FUNC:
					
						/*TODO: USE A MACRO HERE.*/
						iind_world_funcs[j].tags[IIND_WORLD_FUNC_TIMER_CUR_TAG] =
						256;
						
						break;
						
					case IIND_WORLD_GATE_FUNC:
						
						if(!iind_world_funcs[j].tags[IIND_WORLD_GATE_FUNC_TOGGLE_TAG])
						{
							iind_world_funcs[j].tags[IIND_WORLD_GATE_FUNC_TOGGLE_TAG] =
							1;
							
							iind_world_entities[iind_world_funcs[j].bind_id].angle =
							0;
							
							iind_world_entities[iind_world_funcs[j].bind_id].collision =
							false;
						}
						
						break;
						
					case IIND_WORLD_INVERTER_FUNC:
						
						iind_world_funcs[j].tags[IIND_WORLD_FUNC_LH_TAG] =
						0;
						
						iind_world_entities[iind_world_funcs[j].bind_id].angle =
						300;
						
						break;
						
				}
				
				else switch(iind_world_funcs[j].type)
				{
					case IIND_WORLD_GATE_FUNC:
						
						if(iind_world_funcs[j].tags[IIND_WORLD_GATE_FUNC_TOGGLE_TAG])
						{
							iind_world_funcs[j].tags[IIND_WORLD_GATE_FUNC_TOGGLE_TAG] =
							0;
							
							iind_world_entities[iind_world_funcs[j].bind_id].angle =
							300;
						}
						
						/*
						Here to prevent trapping the player in gate collision.
						Only re-enables collision if the player has left the gate.
						*/
						if(!iind_world_entities[iind_world_funcs[j].bind_id].collision)
						{
							if
							(
								!iind_test_world_entity_distance
								(
									iind_world_entities,
									IIND_WORLD_PLAYER_ENTITY,
									1,
									iind_world_entities[iind_world_funcs[j].bind_id].x,
									iind_world_entities[iind_world_funcs[j].bind_id].y
								)
							)
							
							iind_world_entities[iind_world_funcs[j].bind_id].collision =
							true;
						}
						
						break;
						
					case IIND_WORLD_INVERTER_FUNC:
						
						iind_world_funcs[j].tags[IIND_WORLD_FUNC_LH_TAG] =
						1;
						
						iind_world_entities[iind_world_funcs[j].bind_id].angle =
						0;
						
						break;
				}
			}
		}
	
		/*
		=========================
		COMPONENT PASSIVE UPDATES
		=========================
		*/
		
		/*
		TODO: Put a switch-case/mathematical solution (?) here instead of
		using if-else statements.  Thanks, Noor.
		*/
		switch(iind_world_funcs[i].type)
		{
			case IIND_WORLD_TIMER_FUNC:
			
				if(iind_world_funcs[i].tags[IIND_WORLD_FUNC_TIMER_CUR_TAG] > 0)
				{
					iind_world_funcs[i].tags[IIND_WORLD_FUNC_TIMER_CUR_TAG] -= 1;
					
					if
					(
						iind_world_funcs[i].tags[IIND_WORLD_FUNC_TIMER_CUR_TAG] == 0
					)
					{
						iind_world_entities[iind_world_funcs[i].bind_id].angle = 300;
					}
					else if
					(
						iind_world_funcs[i].tags[IIND_WORLD_FUNC_TIMER_CUR_TAG] ==
						256 / 3
					)
					{
						iind_world_entities[iind_world_funcs[i].bind_id].angle = 0;
					}
					else if
					(
						iind_world_funcs[i].tags[IIND_WORLD_FUNC_TIMER_CUR_TAG] ==
						256 * 2 / 3
					)
					{
						iind_world_entities[iind_world_funcs[i].bind_id].angle = 60;
					}
					else if
					(
						iind_world_funcs[i].tags[IIND_WORLD_FUNC_TIMER_CUR_TAG] ==
						256 - 1
					)
					{
						iind_world_entities[iind_world_funcs[i].bind_id].angle = 90;
					}
				}
				
				break;
		}
	
		/*
		========================
		FUNC INTERACTION UPDATES
		========================
		*/
	
		if
		(
			iind_test_world_entity_distance
			(
				iind_world_entities,
				IIND_WORLD_PLAYER_ENTITY,
				1.25,
				iind_world_entities[iind_world_funcs[i].bind_id].x,
				iind_world_entities[iind_world_funcs[i].bind_id].y
			)
		)
		{
			switch(iind_world_funcs[i].type)
			{
				case IIND_WORLD_DOOR_FUNC:
				
					*iind_new_world_id =
					iind_world_funcs[i].tags[IIND_WORLD_DOOR_FUNC_WORLD_TAG];
					
					iind_world_trans_entity->x =
					iind_world_funcs[i].tags[IIND_WORLD_DOOR_FUNC_X_TAG];
					
					iind_world_trans_entity->y =
					iind_world_funcs[i].tags[IIND_WORLD_DOOR_FUNC_Y_TAG];
						
					iind_world_trans_entity->angle = 
					iind_world_entities[IIND_WORLD_PLAYER_ENTITY].angle;
						
					*iind_world_trans_state = true;
					
					break;

				case IIND_WORLD_SWITCH_FUNC:
						
					if(iind_world_funcs[i].tags[IIND_WORLD_SWITCH_FUNC_TOGGLE_TAG])
					{
						if(iind_world_funcs[i].tags[IIND_WORLD_FUNC_LH_TAG] == 0)
						{
							iind_world_entities[iind_world_funcs[i].bind_id].angle =
							300;
							
							iind_world_funcs[i].tags[IIND_WORLD_FUNC_LH_TAG] = 1;
						}
						else
						{
							iind_world_entities[iind_world_funcs[i].bind_id].angle = 0;
							iind_world_funcs[i].tags[IIND_WORLD_FUNC_LH_TAG] = 0;
						}
							
						iind_world_funcs[i].tags[IIND_WORLD_SWITCH_FUNC_TOGGLE_TAG] = 0;
					}
					
					break;
					
				case IIND_WORLD_DIALOGUE_FUNC:
				
					/*
					The only reason a tag system is being used here
					(dialogue tags, not func tags) is to avoid passing
					the entire list of dialogue strings through the
					function every time it's executed.  While it is a
					bit redundant, it should be somewhat more efficient.
					*/
					if(!iind_dialogue_tags[IIND_DIALOGUE_ACTIVE_TAG])
					{
						iind_dialogue_tags[IIND_DIALOGUE_MIN_TAG] =
						iind_world_funcs[i].tags[IIND_WORLD_DIALOGUE_FUNC_MIN_TAG];
						
						iind_dialogue_tags[IIND_DIALOGUE_CUR_TAG] =
						iind_world_funcs[i].tags[IIND_WORLD_DIALOGUE_FUNC_MIN_TAG];
						
						iind_dialogue_tags[IIND_DIALOGUE_MAX_TAG] =
						iind_world_funcs[i].tags[IIND_WORLD_DIALOGUE_FUNC_MAX_TAG];
						
						iind_dialogue_tags[IIND_DIALOGUE_PORTRAIT_TAG] =
						iind_world_funcs[i].tags[IIND_WORLD_DIALOGUE_FUNC_PORTRAIT_TAG];
						
						iind_dialogue_tags[IIND_DIALOGUE_ACTIVE_TAG] = 1;
						iind_dialogue_tags[IIND_DIALOGUE_FUNC_ID_TAG] = i;
					}
				
					break;
					
				case IIND_WORLD_CHARGER_FUNC:
					
					iind_world_entities[IIND_WORLD_PLAYER_ENTITY].health += 0.1;
					
					break;
			}
		}
		
		/*
		Here to prevent switches from being rapidly toggled on and off.
		It only allows the switch to become usable after the player has left
		its range.
		*/
		else if
		(
			iind_world_funcs[i].type == IIND_WORLD_SWITCH_FUNC &&
			iind_world_funcs[i].tags[IIND_WORLD_SWITCH_FUNC_TOGGLE_TAG] == 0
		)
		{
			iind_world_funcs[i].tags[IIND_WORLD_SWITCH_FUNC_TOGGLE_TAG] = 1;
		}
		
		/*
		Here to reset dialogue triggers after leaving the range
		of the func that initiated the last used dialogue.
		*/
		else if
		(
			i == iind_dialogue_tags[IIND_DIALOGUE_FUNC_ID_TAG] &&
			iind_dialogue_tags[IIND_DIALOGUE_CUR_TAG] ==
			iind_dialogue_tags[IIND_DIALOGUE_MAX_TAG]
		)
		{
			iind_dialogue_tags[IIND_DIALOGUE_ACTIVE_TAG] = 0;
		}
	}
}

void iind_update_world_entity_anims
(
	IINDWorldEntity *iind_world_entities,
	int iind_world_entity_count,
	int iind_anim_counter
)
{
	for(int i = 0; i < iind_world_entity_count; i++)
	{
		if(iind_world_entities[i].speed == 0)
		{
			iind_world_entities[i].anim_row = 0;
		}
		else if(iind_world_entities[i].speed > 0 && iind_anim_counter == 25)
		{
			iind_world_entities[i].anim_row += 1;
			if(iind_world_entities[i].anim_row > 4)
			{
				iind_world_entities[i].anim_row = 1;
			}
		}
		
		/*
		======================
		DEATH ANIMATION UPDATE
		======================
		*/
		
		if(iind_world_entities[i].health <= 0)
		{
			iind_world_entities[i].anim_row = 5;
		}
	
		/*
		========================
		WALKING ANIMATION UPDATE
		========================
		*/
	
		if
		(
			iind_world_entities[i].angle > -67.5 &&
			iind_world_entities[i].angle < -22.5
		)
		{
			iind_world_entities[i].anim_col = 0;
		}
		else if
		(
			iind_world_entities[i].angle > -22.5 &&
			iind_world_entities[i].angle < 22.5
		)
		{
			iind_world_entities[i].anim_col = 1;
		}
		else if
		(
			iind_world_entities[i].angle > 22.5 &&
			iind_world_entities[i].angle < 67.5
		)
		{
			iind_world_entities[i].anim_col = 2;
		}
		else if
		(
			iind_world_entities[i].angle > 67.5 &&
			iind_world_entities[i].angle < 112.5
		)
		{
			iind_world_entities[i].anim_col = 3;
		}
		else if
		(
			iind_world_entities[i].angle > 112.5 &&
			iind_world_entities[i].angle < 157.5
		)
		{
			iind_world_entities[i].anim_col = 4;
		}
		else if
		(
			iind_world_entities[i].angle > 157.5 &&
			iind_world_entities[i].angle < 202.5
		)
		{
			iind_world_entities[i].anim_col = 5;
		}
		else if
		(
			iind_world_entities[i].angle > 202.5 &&
			iind_world_entities[i].angle < 247.5
		)
		{
			iind_world_entities[i].anim_col = 6;
		}
		else if
		(
			iind_world_entities[i].angle > 247.5 &&
			iind_world_entities[i].angle < 292.5
		)
		{
			iind_world_entities[i].anim_col = 7;
		}
		else if
		(
			iind_world_entities[i].angle > 292.5 &&
			iind_world_entities[i].angle < 337.5
		)
		{
			iind_world_entities[i].anim_col = 0;
		}
	}
}

void iind_handle_player_movement
(
	IINDWorldMarker *iind_world_markers,
	IINDWorldTile *iind_world_tiles,
	IINDWorldEntity *iind_world_entities,
	int iind_world_tile_count,
	int iind_world_entity_count
)
{
	if
	(
		iind_world_markers[IIND_WORLD_PLAYER_MOVEMENT_MARKER].active &&
		iind_world_entities[IIND_WORLD_PLAYER_ENTITY].health > 0
	)
	{
		iind_world_entities[0].angle = atan
		(
			((iind_world_markers[IIND_WORLD_PLAYER_MOVEMENT_MARKER].y - 
			iind_world_entities[IIND_WORLD_PLAYER_MOVEMENT_MARKER].y) /
			
			(iind_world_markers[IIND_WORLD_PLAYER_MOVEMENT_MARKER].x - 
			iind_world_entities[IIND_WORLD_PLAYER_MOVEMENT_MARKER].x))
		)
		* 180 / IIND_PI;
			
		if
		(
			iind_world_markers[IIND_WORLD_PLAYER_MOVEMENT_MARKER].x - 
			iind_world_entities[IIND_WORLD_PLAYER_ENTITY].x
			<
			0
		)
		{
			iind_world_entities[IIND_WORLD_PLAYER_ENTITY].angle *= -1;
			
			iind_world_entities[IIND_WORLD_PLAYER_ENTITY].angle =
			180 - iind_world_entities[IIND_WORLD_PLAYER_ENTITY].angle;
		}
		
		iind_world_entities[IIND_WORLD_PLAYER_ENTITY].speed =
		IIND_DEFAULT_WORLD_ENTITY_MOVEMENT_SPEED;
		
		iind_move_world_entity
		(
			iind_world_tiles,
			iind_world_entities,
			iind_world_tile_count,
			iind_world_entity_count,
			IIND_WORLD_PLAYER_ENTITY,
			iind_world_entities[IIND_WORLD_PLAYER_ENTITY].speed,
			iind_world_entities[IIND_WORLD_PLAYER_ENTITY].angle
		);
		
		iind_world_entities[IIND_WORLD_PLAYER_ENTITY].health -=
		IIND_WORLD_ENTITY_MOVEMENT_HEALTH_DRAIN_RATE;
	}
	else iind_world_entities[IIND_WORLD_PLAYER_ENTITY].speed = 0;
	
	if
	(
		iind_world_entities[IIND_WORLD_PLAYER_ENTITY].x <
		iind_world_markers[IIND_WORLD_PLAYER_ENTITY].x + 0.01 &&
		
		iind_world_entities[IIND_WORLD_PLAYER_ENTITY].x >
		iind_world_markers[IIND_WORLD_PLAYER_ENTITY].x - 0.01 &&
			
		iind_world_entities[IIND_WORLD_PLAYER_ENTITY].y <
		iind_world_markers[IIND_WORLD_PLAYER_ENTITY].y + 0.01 &&
		
		iind_world_entities[IIND_WORLD_PLAYER_ENTITY].y >
		iind_world_markers[IIND_WORLD_PLAYER_ENTITY].y - 0.01
	)
	iind_world_markers[IIND_WORLD_PLAYER_MOVEMENT_MARKER].active = false;
}


