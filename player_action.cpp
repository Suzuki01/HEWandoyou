#include "joycon.h"
#include "judgement.h"
#include "debug_font.h"
#include "sword_effect.h"
#include "common.h"
#include "input.h"
#include "sprite.h"
#include "player_action.h"

#define CHARGE_MAX			(30)
#define EFFECT_START		(100)
#define INDICATOR_DRAW_TIME	(30)

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

static bool bIsAttackIndicator;
static int indicatorCnt; //攻撃インジケーターの表示を管理する

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
		SwordEffectCreate(SCREEN_WIDTH * 0.5 - EFFECT_START, SCREEN_HEIGHT * 0.5 - EFFECT_START, D3DXToRadian(135), TEXTURE_INDEX_SWORD_EFFECT, 5, 5);
	}
	//下
	if (Keyboard_IsTrigger(DIK_G))
	{
		bIsSlash_Down = true;
		SwordEffectCreate(SCREEN_WIDTH * 0.5 - EFFECT_START, SCREEN_HEIGHT * 0.5 - EFFECT_START, D3DXToRadian(-45), TEXTURE_INDEX_SWORD_EFFECT, 5, 5);
	}
	//左
	if (Keyboard_IsTrigger(DIK_F))
	{
		bIsSlash_Left = true;
		SwordEffectCreate(SCREEN_WIDTH * 0.5 - EFFECT_START, SCREEN_HEIGHT * 0.5 - EFFECT_START, D3DXToRadian(45), TEXTURE_INDEX_SWORD_EFFECT, 5, 5);
	}
	//右
	if (Keyboard_IsTrigger(DIK_H))
	{
		bIsSlash_Right = true;
		SwordEffectCreate(SCREEN_WIDTH * 0.5 - EFFECT_START, SCREEN_HEIGHT * 0.5 - EFFECT_START, D3DXToRadian(-135), TEXTURE_INDEX_SWORD_EFFECT, 5, 5);
	}
	//左上
	if (Keyboard_IsTrigger(DIK_R))
	{
		bIsSlash_LeftUp = true;
		SwordEffectCreate(SCREEN_WIDTH * 0.5 - EFFECT_START, SCREEN_HEIGHT * 0.5 - EFFECT_START, D3DXToRadian(90), TEXTURE_INDEX_SWORD_EFFECT, 5, 5);
	}
	//右上
	if (Keyboard_IsTrigger(DIK_Y))
	{
		bIsSlash_RightUp = true;
		SwordEffectCreate(SCREEN_WIDTH * 0.5 - EFFECT_START, SCREEN_HEIGHT * 0.5 - EFFECT_START, D3DXToRadian(180), TEXTURE_INDEX_SWORD_EFFECT, 5, 5);
	}
	//左下
	if (Keyboard_IsTrigger(DIK_V))
	{
		bIsSlash_LeftDown = true;
		SwordEffectCreate(SCREEN_WIDTH * 0.5 - EFFECT_START, SCREEN_HEIGHT * 0.5 - EFFECT_START, D3DXToRadian(0), TEXTURE_INDEX_SWORD_EFFECT, 5, 5);
	}
	//右下
	if (Keyboard_IsTrigger(DIK_B))
	{
		bIsSlash_RightDown = true;
		SwordEffectCreate(SCREEN_WIDTH * 0.5 - EFFECT_START, SCREEN_HEIGHT * 0.5 - EFFECT_START, D3DXToRadian(-90), TEXTURE_INDEX_SWORD_EFFECT, 5, 5);
	}
	//チャージ攻撃
	//チャージ開始
	if (Keyboard_IsPress(DIK_E))
	{
		PlayerAction_Charge++;
	}
	if (PlayerAction_Charge > CHARGE_MAX)
	{
		PlayerAction_Charge = CHARGE_MAX;
	}
	//必殺技
	if (PlayerAction_Charge == CHARGE_MAX && Keyboard_IsTrigger(DIK_A))
	{
		bIsChargeAttack = true;
		PlayerAction_Charge = 0;
		SwordEffectCreate(SCREEN_WIDTH * 0.5 - EFFECT_START, SCREEN_HEIGHT * 0.5 - EFFECT_START, 0, TEXTURE_INDEX_SPECIAL_ATTACK, 15,5);
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
	if (/*JoyconVector(1) < 250 && GetAccel(1).x < -3.0 && GetJyro(1).x > 1.0*/Keyboard_IsTrigger(DIK_P))
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
	//攻撃のインジケーターの描画
	if(bIsSlash_Up){
		Sprite_Draw(TEXTURE_INDEX_ATTACK_UP, INDICATOR_POSX, INDICATOR_POSY, 0, 0, 1950, 1950, 975, 975, 0.1, 0.1, 0);
		if (indicatorCnt > 30) {
			bIsSlash_Up = false;
			indicatorCnt = 0;
		}
		indicatorCnt++;
	}
	if (bIsSlash_Down) {
		Sprite_Draw(TEXTURE_INDEX_ATTACK_DOWN, INDICATOR_POSX, INDICATOR_POSY, 0, 0, 1950, 1950, 975, 975, 0.1, 0.1, 0);
		if (indicatorCnt > 30) {
			bIsSlash_Down = false;
			indicatorCnt = 0;
		}
		indicatorCnt++;
	}
	if (bIsSlash_Left) {
		Sprite_Draw(TEXTURE_INDEX_ATTACK_LEFT, INDICATOR_POSX, INDICATOR_POSY, 0, 0, 1950, 1950, 975, 975, 0.1, 0.1, 0);
		if (indicatorCnt > 30) {
			bIsSlash_Left = false;
			indicatorCnt = 0;
		}
		indicatorCnt++;
	}
	if (bIsSlash_Right) {
		Sprite_Draw(TEXTURE_INDEX_ATTACK_RIGHT, INDICATOR_POSX, INDICATOR_POSY, 0, 0, 1950, 1950, 975, 975, 0.1, 0.1, 0);
		if (indicatorCnt > 30) {
			bIsSlash_Right = false;
			indicatorCnt = 0;
		}
		indicatorCnt++;
	}
	if (bIsSlash_LeftUp) {
		Sprite_Draw(TEXTURE_INDEX_ATTACK_LEFT_UP, INDICATOR_POSX, INDICATOR_POSY, 0, 0, 1950, 1950, 975, 975, 0.1, 0.1, 0);
		if (indicatorCnt > 30) {
			bIsSlash_LeftUp = false;
			indicatorCnt = 0;
		}
		indicatorCnt++;
	}
	if (bIsSlash_LeftDown) {
		Sprite_Draw(TEXTURE_INDEX_ATTACK_LEFT_DOWN, INDICATOR_POSX, INDICATOR_POSY, 0, 0, 1950, 1950, 975, 975, 0.1, 0.1, 0);
		if (indicatorCnt > 30) {
			bIsSlash_LeftDown = false;
			indicatorCnt = 0;
		}
		indicatorCnt++;
	}
	if (bIsSlash_RightUp) {
		Sprite_Draw(TEXTURE_INDEX_ATTACK_RIGHT_UP, INDICATOR_POSX, INDICATOR_POSY, 0, 0, 1950, 1950, 975, 975, 0.1, 0.1, 0);
		if (indicatorCnt > 30) {
			bIsSlash_RightUp = false;
			indicatorCnt = 0;
		}
		indicatorCnt++;
	}
	if (bIsSlash_RightDown) {
		Sprite_Draw(TEXTURE_INDEX_ATTACK_RIGHT_DOWN, INDICATOR_POSX, INDICATOR_POSY, 0, 0, 1950, 1950, 975, 975, 0.1, 0.1, 0);
		if (indicatorCnt > 30) {
			bIsSlash_RightDown = false;
			indicatorCnt = 0;
		}
		indicatorCnt++;
	}
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
