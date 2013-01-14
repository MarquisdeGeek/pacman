#ifndef _GHOSTS_H
#define _GHOSTS_H

/* telaps is in seconds */
void Pac_InitialiseGhosts(GAME_STATE *ptr);
void Pac_UpdateGhosts(GAME_STATE *ptr, float telaps);
void Pac_ActivateGhost(G_GHOST *pGh, int x, int y, tDir eDir);
void Pac_GhostsAreVunerable(GAME_STATE *ptr, float fDuration);

#endif	/* _GHOSTS_H */

