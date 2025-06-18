#include <ctype.h>
#include "pacman.h"
#include "player.h"
#include "board.h"
#include "ghosts.h"
#include "pills.h"

static G_PEPPILL *GotPills(GAME_STATE *ptr, int x, int y)
{
int i;
	
	for(i=0;i<MAX_PILLS;i++)
		if (ptr->Pills[i].Pos.x == x && ptr->Pills[i].Pos.y == y && !ptr->Pills[i].bEaten)
			return &ptr->Pills[i];
	return 0;
}

void Pac_InitialisePlayer(GAME_STATE *ptr)
{
	ptr->Player.iScore = 0;
	ptr->Player.iLives = 3;
	Pac_ReinitialisePlayer(ptr);
}

void Pac_ReinitialisePlayer(GAME_STATE *ptr)
{
G_PLAYER *pPlayer=&ptr->Player;

	pPlayer->bDead = 0;
	pPlayer->Pos.x = 14;
	pPlayer->Pos.y = 18;
	pPlayer->iLastValidDir = PACKEY_LEFT;
	pPlayer->fTimecum = 0;
	pPlayer->iAnimFrame = 0;
}

void Pac_CheckPlayerVsGhosts(GAME_STATE *ptr)
{
G_GHOST *pG = &ptr->Ghosts[0];
int i;

	for(i=0;i<MAX_GHOSTS;i++,pG++)
		if (pG->bActive && 
			pG->Pos.x == ptr->Player.Pos.x &&
			pG->Pos.y == ptr->Player.Pos.y)
			{ 
			if (pG->fEdibleDuration == 0)
				{
				ptr->Player.bDead = 1;
				}
			else
				{
				ptr->Player.iScore += ptr->Player.iNextGhostScore;
				ptr->Player.iNextGhostScore *= 2;
				/* TODO: if anyone wants to trace the 'eyes' 
			   	back to the home, be my guest... */
				ptr->Ghosts[i].bActive = 0;
				}
			}
}

void Pac_UpdatePlayer(GAME_STATE *ptr, float telaps, int key)
{
int x = ptr->Player.Pos.x;
int y = ptr->Player.Pos.y;
int iPrevScore = ptr->Player.iScore;
G_PEPPILL *pGot;
char c;

	ptr->Player.fTimecum += telaps;
	while (ptr->Player.fTimecum > 0.1f)
		{
		ptr->Player.fTimecum -= 0.1f;
		if (++ptr->Player.iAnimFrame == 6)
			ptr->Player.iAnimFrame = 0;
		}

	key = tolower(key);
	x+=(key==PACKEY_RIGHT?1:(key==PACKEY_LEFT?-1:0));
	y+=(key==PACKEY_DOWN?1:(key==PACKEY_UP?-1:0));
	if (!Pac_IsOpenPos(ptr, x,y) || (x==ptr->Player.Pos.x && y==ptr->Player.Pos.y))
		{
		key = ptr->Player.iLastValidDir;
		x = ptr->Player.Pos.x;
		y = ptr->Player.Pos.y;
		x+=(key==PACKEY_RIGHT?1:(key==PACKEY_LEFT?-1:0));
		y+=(key==PACKEY_DOWN?1:(key==PACKEY_UP?-1:0));
		}

	if (Pac_GetMap(ptr, x, y, &c))
		{
		if (Pac_IsOpenArea(c))
			{
			if (key==PACKEY_LEFT || key==PACKEY_RIGHT || key==PACKEY_UP || key==PACKEY_DOWN)
				ptr->Player.iLastValidDir = key;
			ptr->Player.Pos.x = x;
			ptr->Player.Pos.y = y;
			if (c == '.')
				{
				ptr->Player.iScore += 10;
				ptr->iDotsLeft--;
				Pac_SetMap(ptr, x, y, ' ');
				}

			if (c == 'P' && (pGot = GotPills(ptr, x,y)))
				{
				Pac_GhostsAreVunerable(ptr, pGot->fDuration);
				pGot->bEaten = 1;
				ptr->Player.iScore += 50;
				ptr->Player.iNextGhostScore = 200;
				}
			}
		}
	else if (x < 0)	/* must be the tunnel! */
		{
		ptr->Player.Pos.x = ptr->iMapWidth-1;
		}
	else if (x >= ptr->iMapWidth)
		{
		ptr->Player.Pos.x = 0;
		}

	/* Bonus at 10K */
	if (iPrevScore < 10000 && ptr->Player.iScore >= 10000)
		if (++ptr->Player.iLives > MAX_PACMAN_LIVES)
			ptr->Player.iLives = MAX_PACMAN_LIVES;

	/* Special (i.e. cherries, et al) eaten? */
	if (ptr->Player.Pos.x == ptr->SpecialPos.x &&
		ptr->Player.Pos.y == ptr->SpecialPos.y &&
		ptr->fSpecialDuration)
		{
		ptr->Player.iScore += 100 + ptr->iSpecialChar*100;
		if (++ptr->iSpecialChar >= ptr->iLevel)
			ptr->iSpecialChar = 0;
		Pac_RespawnSpecial(ptr);
		}

	/* High? */
	if (ptr->Player.iScore > ptr->iHighScore)
		ptr->iHighScore = ptr->Player.iScore;
}

