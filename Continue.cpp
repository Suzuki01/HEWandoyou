#include"Continue.h"
#include "sprite.h"
#include "joycon.h"
#include "input.h"
#include "scene.h"

void Continue_Init()
{

}
void Continue_Fin()
{

}
void Continue_Update()
{
	if (Keyboard_IsTrigger(DIK_1))
	{
		Scene_Change(SCENE_INDEX_GAME);
	}
	if (Keyboard_IsTrigger(DIK_2))
	{
		Scene_Change(SCENE_INDEX_RANKING);
	}
}
void Continue_Draw()
{
	Sprite_Draw(TEXTURE_INDEX_CONTINUE, 0.0f, 0.0f);
}