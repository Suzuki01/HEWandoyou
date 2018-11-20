#ifndef PLAYER_ACTION_H_
#define PLAYER_ACTION_H_

#include "judgement.h"

#define	ATTACK_INTERVAL	(20)

void PlayerAction_Initialize(void);
void PlayerAction_Update(void);
void PlayerAction_Draw(void);

bool PlayerAction_GetSlash(Direction Dir);
void PlayerAction_SetSlash(Direction Dir, bool Flag);
bool PlayerAction_GetbIsChargeAttack(void);
bool PlayerAction_GetbIsAvoidance_Left(void);
bool PlayerAction_GetbIsAvoidance_Right(void);
void PlayerAction_SetbIsAvoidance_Left(bool flag);
void PlayerAction_SetbIsAvoidance_Right(bool flag);
void PlayerAction_Draw();
#endif // PLAYER_H_