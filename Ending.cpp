#include"Ending.h"
#include "sprite.h"
#include "joycon.h"
#include "input.h"
#include "scene.h"

void Ending_Init()
{

}
void Ending_Fin()
{

}
void Ending_Update()
{
	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		Scene_Change(SCENE_INDEX_RANKING);
	}
}
void Ending_Draw()
{
	Sprite_Draw(TEXTURE_INDEX_ENDING, 0.0f, 0.0f);
}