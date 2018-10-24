#include"title.h"
#include "sprite.h"
#include "joycon.h"
#include "input.h"
#include "scene.h"
 
void Title_Init(void)
{

}
void Title_Fin(void)
{

}
void Title_Update(void)
{
	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		Scene_Change(SCENE_INDEX_OPENING);
	}
}
void Title_Draw(void)
{
	Sprite_Draw(TEXTURE_INDEX_TITLE, 0.0f, 0.0f);
}