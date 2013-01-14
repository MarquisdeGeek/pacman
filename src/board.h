#ifndef _BOARD_H
#define _BOARD_H

void Pac_InitialiseGame(GAME_STATE *ptr);
void Pac_ReinitialiseGame(GAME_STATE *ptr);
void Pac_RepopulateBoard(GAME_STATE *ptr);
void Pac_SetMap(GAME_STATE *ptr, int w, int h, char c);
BOOL Pac_GetMap(GAME_STATE *ptr, int w, int h, char *c);
BOOL Pac_IsOpenArea(char c);
BOOL Pac_IsOpenPos(GAME_STATE *ptr, int x, int y);

#endif	/* _BOARD_H */

