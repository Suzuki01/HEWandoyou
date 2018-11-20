#ifndef JUGDEMENT_H
#define JUGDEMENT_H
#include<d3dx9.h>
#include"mydirect3d.h"

typedef enum Direction
{
	UP,
	RIGHTUP,
	RIGHT,
	RIGHTDOWN,
	DOWN,
	LEFTDOWN,
	LEFT,
	LEFTUP,
	END =-1,
};
typedef enum indicator_type {
	DIRECITION,
	NO_DIRECITION,
	TOOFAR,
};
typedef struct SwordSlash_tag {

	bool alive;
	Direction Dir;
	int life;

}SwordSlash;
typedef struct indicator {
	float posX;
	float posY;
	indicator_type state;
	float r;
	bool alive;
};


bool Judgement(Direction input,indicator indicator);
void Jugment_Init(void);
#endif // !JUGDEMENT_H
