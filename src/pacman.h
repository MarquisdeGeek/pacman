#ifndef _PACMAN_H
#define _PACMAN_H

#include <stdlib.h>

/* 
** Curses already defines these, but if we ever change the renderer
** we'd have to go back and add these.
*/
#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

/* Pacman limits */
#define MAX_PILLS		4
#define MAX_GHOSTS		4
#define MAX_PACMAN_LIVES	6

/* curses already have KEY_UP, so we prefix with PAC */
#define PACKEY_UP		'k'
#define PACKEY_DOWN		'm'
#define PACKEY_LEFT		'x'
#define PACKEY_RIGHT		'c'

#define RND(__x)	(int)((double)rand()/((double)RAND_MAX+1)*__x)

typedef int BOOL;

typedef enum {
	ePAC_LifeLost,
	ePAC_SheetComplete,
	ePAC_UserQuit,
	} tGameEnd;

typedef enum {
	eDIR_Left,
	eDIR_Right,
	eDIR_Up,
	eDIR_Down,
	} tDir;

typedef struct sGPOS {
	int x, y;
	} GPOS;

typedef struct sGPILL {
	GPOS	Pos;
	int	bEaten;
	float	fDuration;
	} G_PEPPILL;

typedef struct sGHOST {
	GPOS	Pos;
	int	bActive;
	int	iInHome;
	float	fEdibleDuration;	/* rem, power ups only affect ghosts outside their home */
	tDir	Direction;
	} G_GHOST;

typedef struct sGPLAYER {
	GPOS	Pos;
	int	bDead;
	int	iScore;
	int	iLives;
	int	iLastValidDir;
	float	fTimecum;
	int	iAnimFrame;
	int	iNextGhostScore;
	} G_PLAYER;

typedef struct sGAMESTATE {
	/* Power pills */
	float 		iPP_Timecum;
	int		iPP_Flash;
	G_PEPPILL	Pills[MAX_PILLS];
	float		fTimeBeforeSpecial;
	float		fSpecialDuration;
	int		iSpecialChar;
	GPOS		SpecialPos;
	
	/* Ghosts */
	G_GHOST		Ghosts[MAX_GHOSTS];

	/* Player */
	G_PLAYER	Player;

	/* Map */
	int		iLevel;
	int		iDotsLeft;
	int		iMapWidth, iMapHeight;
	char		Map[672];

	/* Other */
	int		iHighScore;
	char		*pMarquee;
	long		iMarqueeSize;

	} GAME_STATE;

#endif	/* _PACMAN_H */

