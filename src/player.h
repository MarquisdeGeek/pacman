#ifndef _PLAYER_H
#define _PLAYER_H

void Pac_InitialisePlayer(GAME_STATE *ptr);
void Pac_ReinitialisePlayer(GAME_STATE *ptr);
void Pac_UpdatePlayer(GAME_STATE *ptr, float telaps, int key);
void Pac_CheckPlayerVsGhosts(GAME_STATE *ptr);

#endif	/* _PLAYER_H */

