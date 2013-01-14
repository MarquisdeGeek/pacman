#include <stdlib.h>
#include "pacman.h"
#include "board.h"
#include "ghosts.h"

#define GIH_SHUFFLE	1
#define GIH_GATEWAY	2
#define GIH_OUTSIDE	0

static char GetNextChar(GAME_STATE *ptr, G_GHOST *pGhost)
{
int x, y;
char c;

	x = pGhost->Pos.x; y = pGhost->Pos.y;
	switch(pGhost->Direction)
		{
		case	eDIR_Left:	x--; break;
		case	eDIR_Right:	x++; break;
		case	eDIR_Up:	y--; break;
		case	eDIR_Down:	y++; break;
		}
	if (Pac_GetMap(ptr, x, y, &c))
		return c;
	else
		return '\0';
}

static int IsNextMoveValid(GAME_STATE *ptr, G_GHOST *pGhost)
{
char c;

	c = GetNextChar(ptr, pGhost);
	return Pac_IsOpenArea(c);
}

static void HomedGhost(GAME_STATE *ptr, G_GHOST *pGhost)
{
char c;

	switch(pGhost->iInHome)
		{
		case GIH_SHUFFLE:
				/* Move up into gateway?? */
				if (RND(2) == 0 && Pac_GetMap(ptr,pGhost->Pos.x,pGhost->Pos.y-1,&c) && c=='-')
					{
					pGhost->iInHome = GIH_GATEWAY;
					pGhost->Pos.y--;
					}
				else
					{
					c = RND(3);
					if (c == 0 && Pac_GetMap(ptr,pGhost->Pos.x-1,pGhost->Pos.y,&c) && c=='H')
						pGhost->Pos.x--;
					else if (c == 1 && Pac_GetMap(ptr,pGhost->Pos.x+1,pGhost->Pos.y,&c) && c=='H')
						pGhost->Pos.x++;
					}
				break;
		case GIH_GATEWAY:
				pGhost->Pos.y--; /* ASSUME: gate opens upwards */
				pGhost->iInHome = GIH_OUTSIDE;
				if (RND(2) == 0)
					pGhost->Direction = eDIR_Left;
				else
					pGhost->Direction = eDIR_Right;
				break;
		}
}

static void MoveGhost(GAME_STATE *ptr, G_GHOST *pGhost)
{
int tries=0;

	if (!IsNextMoveValid(ptr, pGhost))
		{ /* must be either 1 or 2 valid directions */
		tDir w1,w2;

		switch(pGhost->Direction) 
			{
			case eDIR_Right:
			case eDIR_Left:	w1=eDIR_Up, w2=eDIR_Down; break;
			case eDIR_Up:
			case eDIR_Down:	w1=eDIR_Left, w2=eDIR_Right; break;
			}
		if (RND(2)==0)	/* try w1 first, then w2 */
			{
			pGhost->Direction = w1;
			if (!IsNextMoveValid(ptr, pGhost))
				pGhost->Direction = w2;
			}
		else
			{
			pGhost->Direction = w2;
			if (!IsNextMoveValid(ptr, pGhost))
				pGhost->Direction = w1;
			}
		}

	if (RND(34) == 0 && pGhost->Pos.x != ptr->Player.Pos.x && pGhost->Pos.y != ptr->Player.Pos.y)
		{
		pGhost->Direction = RND(4);	/* a little bit bad! */
		while(++tries < 4)
			{
			if (IsNextMoveValid(ptr, pGhost))
				break;/* found a new dir */
			else
				if (++pGhost->Direction == 4)
					pGhost->Direction = 0;
			}
		}

	/* Move the ghost */
	switch(pGhost->Direction)
		{
		case eDIR_Left:		pGhost->Pos.x--; break;
		case eDIR_Right:	pGhost->Pos.x++; break;
		case eDIR_Up:		pGhost->Pos.y--; break;
		case eDIR_Down:		pGhost->Pos.y++; break;
		}
	/* Check for tunnel */
	if (pGhost->Pos.x < 0)	pGhost->Pos.x = ptr->iMapWidth-1;
	if (pGhost->Pos.x >= ptr->iMapWidth) pGhost->Pos.x = 0;
}

static void DormantGhost(GAME_STATE *ptr, G_GHOST *pGhost)
{
	/* Wake him up?? */
	if (RND(30) < 2)
		Pac_ActivateGhost(pGhost, 12,10,eDIR_Left);
	
}

void Pac_InitialiseGhosts(GAME_STATE *ptr)
{
int i;

	for(i=0;i<MAX_GHOSTS;i++)
		ptr->Ghosts[i].bActive = 0;
	Pac_ActivateGhost(&ptr->Ghosts[0], 12,10, eDIR_Left);
	Pac_ActivateGhost(&ptr->Ghosts[1], 14,10, eDIR_Left);
	Pac_ActivateGhost(&ptr->Ghosts[2], 16,10, eDIR_Left);
}

void Pac_ActivateGhost(G_GHOST *pGhost, int x, int y, tDir eDir)
{
	if (pGhost->bActive)	return;
	pGhost->Pos.x = x;
	pGhost->Pos.y = y;
	pGhost->Direction = eDir;
	pGhost->bActive = 1;
	pGhost->fEdibleDuration = 0;
	pGhost->iInHome = GIH_SHUFFLE;
}

void Pac_UpdateGhosts(GAME_STATE *ptr, float telaps)
{
int i;
	for(i=0;i<MAX_GHOSTS;i++)
		{
		ptr->Ghosts[i].fEdibleDuration -= telaps;
		if (ptr->Ghosts[i].fEdibleDuration < 0)
			ptr->Ghosts[i].fEdibleDuration = 0;

		if (ptr->Ghosts[i].bActive == 0)
			DormantGhost(ptr, &ptr->Ghosts[i]);
		else if (ptr->Ghosts[i].iInHome)
			HomedGhost(ptr, &ptr->Ghosts[i]);
		else
			MoveGhost(ptr, &ptr->Ghosts[i]);
		}

}

void Pac_GhostsAreVunerable(GAME_STATE *ptr, float fDuration)
{
int i;

	for(i=0;i<MAX_GHOSTS;i++)
		if (ptr->Ghosts[i].bActive && ptr->Ghosts[i].iInHome==GIH_OUTSIDE)
			ptr->Ghosts[i].fEdibleDuration += fDuration;
}

