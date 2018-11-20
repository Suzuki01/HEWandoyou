#include "joycon.h"
#include "judgement.h"
#include "debug_font.h"
#include "sword_effect.h"
#include "common.h"
#include "input.h"

#define CHARGE_MAX		(30)
#define EFFECT_START	(100)

//一定方向斬撃
static bool bIsSlash_Up;
static bool bIsSlash_Down;
static bool bIsSlash_Left;
static bool bIsSlash_Right;
static bool bIsSlash_LeftUp;
static bool bIsSlash_RightUp;
static bool bIsSlash_LeftDown;
static bool bIsSlash_RightDown;

//チャージ攻撃
static bool bIsChargeAttack;
static int PlayerAction_Charge;

//回避
static bool bIsAvoidance_Left;
static bool bIsAvoidance_Right;

void PlayerAction_Initialize(void)
{
	bIsSlash_Up = false;
	bIsSlash_Down = false;
	bIsSlash_Left = false;
	bIsSlash_Right = false;
	bIsSlash_LeftUp = false;
	bIsSlash_RightUp = false;
	bIsSlash_LeftDown = false;
	bIsSlash_RightDown = false;

	bIsChargeAttack = false;
	PlayerAction_Charge = 0;
	SwordEffectInit();
}
void PlayerAction_Update(void)
{
	//一定方向斬撃
	//上
	if (Keyboard_IsTrigger(DIK_T))
	{
		bIsSlash_Up = true;
		SwordEffectCreate(SCREEN_WIDTH * 0.5 - EFFECT_START,SCREEN_HEIGHT * 0.5 - EFFECT_START,D3DXToRadian(-45));
	}
	//下
	if (Keyboard_IsTrigger(DIK_G))
	{
		bIsSlash_Down = true;
		SwordEffectCreate(SCREEN_WIDTH * 0.5 - EFFECT_START, SCREEN_HEIGHT * 0.5 - EFFECT_START, D3DXToRadian(135));
	}
	//左
	if (Keyboard_IsTrigger(DIK_F))
	{
		bIsSlash_Left = true;
		SwordEffectCreate(SCREEN_WIDTH * 0.5 - EFFECT_START, SCREEN_HEIGHT * 0.5 - EFFECT_START, D3DXToRadian(-135));
	}
	//右
	if (Keyboard_IsTrigger(DIK_H))
	{
		bIsSlash_Right = true;
		SwordEffectCreate(SCREEN_WIDTH * 0.5 - EFFECT_START, SCREEN_HEIGHT * 0.5 - EFFECT_START, D3DXToRadian(45));
	}
	//左上
	if (Keyboard_IsTrigger(DIK_R))
	{
		bIsSlash_LeftUp = true;
		SwordEffectCreate(SCREEN_WIDTH * 0.5 - EFFECT_START, SCREEN_HEIGHT * 0.5 - EFFECT_START, D3DXToRadian(-90));
	}
	//右上
	if (Keyboard_IsTrigger(DIK_Y))
	{
		bIsSlash_RightUp = true;
		SwordEffectCreate(SCREEN_WIDTH * 0.5 - EFFECT_START, SCREEN_HEIGHT * 0.5 - EFFECT_START, D3DXToRadian(0));
	}
	//左下
	if (Keyboard_IsTrigger(DIK_V))
	{
		bIsSlash_LeftDown = true;
		SwordEffectCreate(SCREEN_WIDTH * 0.5 - EFFECT_START, SCREEN_HEIGHT * 0.5 - EFFECT_START, D3DXToRadian(180));
	}
	//右下
	if (Keyboard_IsTrigger(DIK_B))
	{
		bIsSlash_RightDown = true;
		SwordEffectCreate(SCREEN_WIDTH * 0.5 - EFFECT_START, SCREEN_HEIGHT * 0.5 - EFFECT_START, D3DXToRadian(90));
	}

	//チャージ攻撃
	//チャージ開始
	if (1)
	{
		PlayerAction_Charge++;
	}
	if (PlayerAction_Charge > CHARGE_MAX)
	{
		PlayerAction_Charge = CHARGE_MAX;
	}
	//必殺技
	if (PlayerAction_Charge == CHARGE_MAX && 1)
	{
		bIsChargeAttack = true;
		PlayerAction_Charge = 0;
	}
	if (PlayerAction_Charge == 0)
	{
		bIsChargeAttack = false;
	}


	//回避
	if (JoyconVector(1) < 250 && GetAccel(1).x > 3.0 && GetJyro(1).x < -1.0)
	{
		bIsAvoidance_Left = true;
	}
	if (JoyconVector(1) < 250 && GetAccel(1).x < -3.0 && GetJyro(1).x > 1.0)
	{
		bIsAvoidance_Right = true;
	}

	SwordEffectUpdate();
}
void PlayerAction_Draw(void)
{
	DebugFont_Draw(500, 150, "ax: %f", GetAccel(1).x);
	DebugFont_Draw(500, 200, "ay: %f", GetAccel(1).y);
	DebugFont_Draw(500, 250, "az: %f", GetAccel(1).z);
	DebugFont_Draw(500, 300, "g1: %f", GetJyro(1).x);
	DebugFont_Draw(500, 350, "g2: %f", GetJyro(1).y);
	DebugFont_Draw(500, 400, "g3: %f", GetJyro(1).z);

	DebugFont_Draw(500, 500, "bIsAvoidance_Right: %d", bIsAvoidance_Right);
	SwordEffectDraw();
}


void PlayerAction_SetSlash(Direction Dir,bool Flag)
{
	switch (Dir)
	{
	case UP:
		bIsSlash_Up = Flag;
		break;
	case RIGHTUP:
		bIsSlash_RightUp = Flag;
		break;
	case RIGHT:
		bIsSlash_Right = Flag;
		break;
	case RIGHTDOWN:
		bIsSlash_RightDown = Flag;
		break;
	case DOWN:
		bIsSlash_Down = Flag;
		break;
	case LEFTDOWN:
		bIsSlash_LeftDown = Flag;
		break;
	case LEFT:
		bIsSlash_Left = Flag;
		break;
	case LEFTUP:
		bIsSlash_LeftUp = Flag;
		break;
	default:
		break;
	}
}

bool PlayerAction_GetSlash(Direction Dir)
{
	switch (Dir)
	{
	case UP:
		return bIsSlash_Up;
		break;
	case RIGHTUP:
		return bIsSlash_RightUp;
		break;
	case RIGHT:
		return bIsSlash_Right;
		break;
	case RIGHTDOWN:
		return bIsSlash_RightDown;
		break;
	case DOWN:
		return bIsSlash_Down;
		break;
	case LEFTDOWN:
		return bIsSlash_LeftDown ;
		break;
	case LEFT:
		return bIsSlash_Left;
		break;
	case LEFTUP:
		return bIsSlash_LeftUp;
		break;
	default:
		break;
	}
}

bool PlayerAction_GetbIsChargeAttack(void)
{
	return bIsChargeAttack;
}

bool PlayerAction_GetbIsAvoidance_Left(void)
{
	return bIsAvoidance_Left;
}

bool PlayerAction_GetbIsAvoidance_Right(void)
{
	return bIsAvoidance_Right;
}

void PlayerAction_SetbIsAvoidance_Left(bool flag)
{
	bIsAvoidance_Left = flag;
}

void PlayerAction_SetbIsAvoidance_Right(bool flag)
{
	bIsAvoidance_Right = flag;
}
