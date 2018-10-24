#include"BossSelect.h"
#include "sprite.h"
#include "joycon.h"
#include "input.h"
#include "scene.h"

static int BossA_Flag, BossB_Flag; 

void BossSelect_Init()
{
	BossA_Flag = 0;
	BossB_Flag = 0;
}
void BossSelect_Fin()
{

}
void BossSelect_Update()
{
	if (Keyboard_IsTrigger(DIK_1))
	{
		BossA_Flag = 1;
		Scene_Change(SCENE_INDEX_GAME);
	}
	if (Keyboard_IsTrigger(DIK_2))
	{
		BossB_Flag = 1;
		Scene_Change(SCENE_INDEX_GAME);
	}
	if (Keyboard_IsTrigger(DIK_3))
	{
		Scene_Change(SCENE_INDEX_OPENING);
	}
}
void BossSelect_Draw()
{
	Sprite_Draw(TEXTURE_INDEX_BOSS_SELECT, 0.0f, 0.0f);
}

int Get_BossA_Flag()
{
	return BossA_Flag;
}

int Get_BossB_Flag()
{
	return BossB_Flag;
}