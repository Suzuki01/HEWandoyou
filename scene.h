#ifndef SCENE_H_
#define SCENE_H_

typedef enum tag_SCENE_INDEX
{
	SCENE_INDEX_TITLE = 0,
	SCENE_INDEX_OPENING,
	SCENE_INDEX_GAME_TUTORIAL,
	SCENE_INDEX_BOSS_SELECT,
	SCENE_INDEX_GAME,
	SCENE_INDEX_ENDING,
	SCENE_INDEX_RANKING,
	SCENE_INDEX_CONTINUE,
	SCENE_INDEX_MAX,

}SCENE_INDEX;

void  Scene_Init(SCENE_INDEX index);//
void  Scene_Fin();
void  Scene_Update();
void  Scene_Draw();
void  Scene_Check();
void  Scene_Change(SCENE_INDEX index);

#endif // !SCENE_H_
#pragma once
