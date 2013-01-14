/*
** Pacman - In curses
*/
#include <stdio.h>
#include <ctype.h>
#include "pacman.h"
#include "render.h"
#include "gameloop.h"

void GetMarqueeText(GAME_STATE *ptr)
{
FILE *fp = fopen("pactext", "r");
int i;
char *pc;

	ptr->pMarquee = 0;
	if (fp)		
		{
		fseek(fp, 0, SEEK_END);
		ptr->iMarqueeSize = ftell(fp);
		if ((ptr->pMarquee = malloc(ptr->iMarqueeSize)))
			{
			fseek(fp,0,SEEK_SET);
			fread(ptr->pMarquee, 1, ptr->iMarqueeSize, fp);
			pc = ptr->pMarquee;
			for(i=0;i<ptr->iMarqueeSize;i++,pc++)
				if (isalnum(*pc) || ispunct(*pc))
					;/*empty*/
				else
					*pc = ' ';
			}
		fclose(fp);
		}
}	
		
int main(int argc, char **argv)
{
GAME_STATE game; 
void *pRender;

	game.iHighScore = 9710;
	GetMarqueeText(&game);
	pRender = Pac_InitRender();
	do {
		Pac_MainGame(pRender, &game);
		Pac_DrawEndGame(pRender, &game);
	} while (Pac_AnotherGame(pRender));
	Pac_CloseRender(pRender);

	return 0;
}

