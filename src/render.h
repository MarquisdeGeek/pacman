
#ifndef _RENDER_H
#define _RENDER_H

/* 
** The rendered returns a handle to the specific renderer we're using.
** With curses, this isn't an issue (there's only one terminal we can render
** to), but could be used if someone every got around to re-writing it for
** GTK et al. I term this handle a 'context', and is always the first parameter
** Within our curses implementation this is always NULL.
*/

void *Pac_InitRender(void);
BOOL Pac_CloseRender(void *);
void Pac_Blit(void *);

void Pac_DrawBoard(void *, GAME_STATE *ptr);
void Pac_DrawPlayer(void *, GAME_STATE *ptr);
void Pac_DrawGhosts(void *, GAME_STATE *ptr);
void Pac_DrawPills(void *, GAME_STATE *ptr);
void Pac_FlashBoard(void *, GAME_STATE *ptr);
void Pac_DrawEndGame(void *, GAME_STATE *ptr);
BOOL Pac_AnotherGame(void *);
void Pac_AnimateDeadPlayer(void *, GAME_STATE *ptr);

void Pac_RenderGameInfo(void *);
void Pac_RenderRHS(void *, GAME_STATE *ptr);

#endif	/* _RENDER_H */

