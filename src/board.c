#include <string.h>
#include "pacman.h"
#include "board.h"
#include "player.h"
#include "ghosts.h"
#include "pills.h"

/* TODO: Store this data in an external text file and read it in

   This will involve changing the (numerous) hard-coded constants
   throughout the game :( 
*/

char *g_pLevel1Map = {"\
############################\
#............##............#\
#.####.#####.##.#####.####.#\
#P####.#####.##.#####.####P#\
#..........................#\
#.####.##.########.##.####.#\
#......##....##....##......#\
######.##### ## #####.######\
     #.##          ##.#     \
     #.## ###--### ##.#     \
######.## # HHHH # ##.######\
      .   # HHHH #   .      \
######.## # HHHH # ##.######\
     #.## ######## ##.#     \
     #.##    $     ##.#     \
######.## ######## ##.######\
#............##............#\
#.####.#####.##.#####.####.#\
#P..##................##..P#\
###.##.##.########.##.##.###\
#......##....##....##......#\
#.##########.##.##########.#\
#..........................#\
############################"};

void Pac_InitialiseGame(GAME_STATE *ptr)
{
	ptr->iLevel = 0;
	Pac_RepopulateBoard(ptr);
	Pac_InitialisePlayer(ptr);
	Pac_InitialiseGhosts(ptr);
	Pac_InitialisePills(ptr);
}

void Pac_ReinitialiseGame(GAME_STATE *ptr)
{
	Pac_RepopulateBoard(ptr);
	Pac_ReinitialisePlayer(ptr);
	Pac_InitialiseGhosts(ptr);
	Pac_InitialisePills(ptr);
}

void Pac_SetMap(GAME_STATE *ptr, int w, int h, char c)
{
	if (w<0 || w>=ptr->iMapWidth)	return;
	if (h<0 || h>=ptr->iMapHeight)	return;
	ptr->Map[w + h*ptr->iMapWidth] = c;
}

BOOL Pac_GetMap(GAME_STATE *ptr, int w, int h, char *c)
{
	if (w<0 || w>=ptr->iMapWidth)	return FALSE;
	if (h<0 || h>=ptr->iMapHeight)	return FALSE;
	*c =  ptr->Map[w + h*ptr->iMapWidth];
	return TRUE;
} 

BOOL Pac_IsOpenPos(GAME_STATE *ptr, int x, int y)
{
char c;

	if (Pac_GetMap(ptr, x, y, &c))
		return Pac_IsOpenArea(c);
	return FALSE;
}

BOOL Pac_IsOpenArea(char c)
{
	if (c=='P' || c==' ' || c=='.' || c=='$')
		return TRUE;
	return FALSE;
}


void Pac_RepopulateBoard(GAME_STATE *ptr)
{
int i;

	ptr->iMapWidth = 28;
	ptr->iMapHeight = 24;
	memcpy(ptr->Map, g_pLevel1Map,ptr->iMapWidth*ptr->iMapHeight); 
	ptr->iDotsLeft = 0;
	for(i=ptr->iMapWidth*ptr->iMapHeight;i>=0;)
		if (ptr->Map[--i] == '.')
			ptr->iDotsLeft++;
}

