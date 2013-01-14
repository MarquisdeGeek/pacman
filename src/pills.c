#include <stdio.h>
#include "pacman.h"
#include "pills.h"
#include "board.h"

void Pac_InitialisePills(GAME_STATE *ptr)
{
int x,y;
int i;
char c;

	ptr->iPP_Flash = ptr->iPP_Timecum = 0;
	ptr->iSpecialChar = ptr->iLevel;
	ptr->SpecialPos.x = ptr->SpecialPos.y = 14;
	Pac_RespawnSpecial(ptr);
	
	i=0;
	for(y=0;y<ptr->iMapHeight;y++)
		for(x=0;x<ptr->iMapWidth;x++)
			if (Pac_GetMap(ptr, x, y, &c) && c=='P')
				{
				if (i < MAX_PILLS)
					{
					ptr->Pills[i].bEaten = 0;
					ptr->Pills[i].Pos.x = x;
					ptr->Pills[i].Pos.y = y;
					ptr->Pills[i].fDuration = 6;
					i++;
					}
				else
					{
					fprintf(stderr, "Too many power pills in this level\n");
					}
				}
}

void Pac_UpdatePills(GAME_STATE *ptr, float telaps)
{
	ptr->iPP_Timecum += telaps;
	if (ptr->iPP_Timecum > 0.5f) 
		{
		ptr->iPP_Timecum = 0;
		ptr->iPP_Flash ^= 1;
		}

	if (ptr->fSpecialDuration)
		{
		ptr->fSpecialDuration -= telaps;
		if (ptr->fSpecialDuration < 0)
			Pac_RespawnSpecial(ptr);
		}
	else
		{
		ptr->fTimeBeforeSpecial -= telaps;
		if (ptr->fTimeBeforeSpecial < 0)
			{
			ptr->fTimeBeforeSpecial = 0;
			ptr->fSpecialDuration = 10 + RND(3);
			}
		}

}

void Pac_RespawnSpecial(GAME_STATE *ptr)
{
	ptr->fTimeBeforeSpecial = 10 + RND(4)*(5+ptr->iLevel);
	ptr->fSpecialDuration = 0;
}

