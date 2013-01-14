
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>		/* we still use the getch */
#include "pacman.h"
#include "gameloop.h"
#include "player.h"
#include "ghosts.h"
#include "board.h"
#include "render.h"
#include "pills.h"

/*
  I got a little bored I'm afraid writing bits of this, which is why
  the animated death and flashing screen happen syncronously. To be done 
  correctly, they should be pseudo-event driven like the rest of the program.
*/
static void DrawDynamic(void *pCtx, GAME_STATE *ptr)
{
	Pac_DrawBoard(pCtx, ptr);
	Pac_DrawPills(pCtx, ptr);
	Pac_DrawPlayer(pCtx, ptr);
	Pac_DrawGhosts(pCtx, ptr);
	Pac_RenderRHS(pCtx, ptr);
}

tGameEnd Pac_GameLoop(void *pCtx, GAME_STATE *ptr)
{
int c,keypress;
float telaps;
struct timeval iTk, iTk2, tvTelaps;

do {
keypress = ptr->Player.iLastValidDir;
gettimeofday(&iTk, 0);

/* Wait 1/10 sec , getting whatever key's been pressed */

do {
	c = getch();
	if (c != EOF)
if (c != ptr->Player.iLastValidDir)
		keypress = c;

	if (c=='q' || c=='Q')
		return ePAC_UserQuit;

	gettimeofday(&iTk2, 0);
	timersub(&iTk2, &iTk, &tvTelaps);
	telaps = tvTelaps.tv_sec + tvTelaps.tv_usec/1000;
	telaps /= 1000.0f;


#if _DEBUG
move(21,30); printw(" clock=%ld  telaps=%f key=%c last=%c", clock(), telaps, keypress, ptr->Player.iLastValidDir);
move(23,30); printw("  pills=%d", ptr->iDotsLeft);
printw("tv %d + %d/1000", tvTelaps.tv_sec, tvTelaps.tv_usec);
#endif
	if (telaps < 0) { fprintf(stderr, "how?????"); break; }
	} while(telaps < 0.1f);

	/* scroll marquee */
	if (ptr->pMarquee)
		{
		c = *ptr->pMarquee;
		memmove(ptr->pMarquee, ptr->pMarquee+1, ptr->iMarqueeSize-1);
		ptr->pMarquee[ptr->iMarqueeSize-1] = c;
		}

	/* Update all game components */
	Pac_UpdatePlayer(ptr, telaps, keypress);
	Pac_CheckPlayerVsGhosts(ptr);
	Pac_UpdateGhosts(ptr, telaps);
	Pac_CheckPlayerVsGhosts(ptr);
	Pac_UpdatePills(ptr, telaps);

	/* Draw everything */
	DrawDynamic(pCtx, ptr);

	/* Make it visible */
	Pac_Blit(pCtx);

	/* Has an end game condition been reached? */
	if (ptr->iDotsLeft == 0)	return ePAC_SheetComplete;
	else if (ptr->Player.bDead)	return ePAC_LifeLost;

	} while(1);
}

void Pac_MainGame(void *pCtx, GAME_STATE *ptr)
{
tGameEnd iGS;

	Pac_InitialiseGame(ptr);
	Pac_RenderGameInfo(pCtx);
	Pac_RenderRHS(pCtx, ptr);
	do {
		/* Start of sheet/new life */
		DrawDynamic(pCtx, ptr);
		Pac_Blit(pCtx);
		sleep(2);

		iGS = Pac_GameLoop(pCtx, ptr);
		switch(iGS) {
			case	ePAC_SheetComplete: 
					Pac_FlashBoard(pCtx, ptr); 
					ptr->iLevel++;
					Pac_ReinitialiseGame(ptr);
					break;
			case	ePAC_LifeLost:	 
					--ptr->Player.iLives;
					Pac_RenderRHS(pCtx, ptr);
					Pac_AnimateDeadPlayer(pCtx, ptr); 
					if (ptr->Player.iLives < 0)
						return;
					sleep(1);
					Pac_InitialiseGhosts(ptr);
					Pac_ReinitialisePlayer(ptr);
					break;
			default:
					break;
			}
	
	} while(iGS != ePAC_UserQuit);
}


