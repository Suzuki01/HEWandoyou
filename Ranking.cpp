#include"Ranking.h"
#include "sprite.h"
#include "joycon.h"
#include "input.h"
#include "scene.h"

void Ranking_Init()
{

}
void Ranking_Fin()
{

}
void Ranking_Update()
{
	if (Keyboard_IsTrigger(DIK_2))
	{
		Scene_Change(SCENE_INDEX_OPENING);
	}
}
void Ranking_Draw()
{
	Sprite_Draw(TEXTURE_INDEX_RANKING, 0.0f, 0.0f);
}
