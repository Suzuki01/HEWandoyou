#include "tutorial.h"
#include "sprite.h"
#include "joycon.h"
#include "input.h"
#include "scene.h"
void Tutorial_Init()
{

}
void Tutorial_Fin()
{

}
void Tutorial_Update()
{
	if (Keyboard_IsTrigger(DIK_3))
	{
		Scene_Change(SCENE_INDEX_OPENING);
	}
}
void Tutorial_Draw()
{
	Sprite_Draw(TEXTURE_INDEX_TUTORIAL, 0.0f, 0.0f);
}