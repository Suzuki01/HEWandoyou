#include "opening.h"
#include "sprite.h"
#include "joycon.h"
#include "input.h"
#include "scene.h"

void Opening_Init()
{

}
void Opening_Fin()
{

}
void Opening_Update()
{
	if (Keyboard_IsTrigger(DIK_1))
	{
		Scene_Change(SCENE_INDEX_GAME_TUTORIAL);
	}
	if (Keyboard_IsTrigger(DIK_2))
	{
		Scene_Change(SCENE_INDEX_BOSS_SELECT);
	}
	if (Keyboard_IsTrigger(DIK_3))
	{
		Scene_Change(SCENE_INDEX_RANKING);
	}
}
void Opening_Draw()
{
	Sprite_Draw(TEXTURE_INDEX_OPENING, 0.0f, 0.0f);
}
