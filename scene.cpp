#include"scene.h"
#include"title.h"
#include"opening.h"
#include"tutorial.h"
#include"BossSelect.h"
#include"Ending.h"
#include"Ranking.h"
#include"Continue.h"
#include"game.h"
#include"sprite.h"
#include "enemy.h"

static SCENE_INDEX g_SceneIndex;
static SCENE_INDEX g_SceneNextIndex;
typedef void(*SceneFunc)(void);//ä÷êîÉ|ÉCÉìÉ^

static const SceneFunc Init[] =
{
	Title_Init,
	Opening_Init,
	Tutorial_Init,
	BossSelect_Init,
	Game_Initialize,
	Ending_Init,
	Ranking_Init,
	Continue_Init,
};

//scene ä«óù
void  Scene_Init(SCENE_INDEX index)//
{
	
	g_SceneIndex = index;
	g_SceneNextIndex = index;
	Init[g_SceneNextIndex]();

}
void  Scene_Fin()
{
	switch (g_SceneIndex)
	{
	case SCENE_INDEX_TITLE:
		Title_Fin();
		break;
	case SCENE_INDEX_OPENING:
		Opening_Fin();
		break;
	case SCENE_INDEX_GAME_TUTORIAL:
		Tutorial_Fin();
		break;
	case SCENE_INDEX_BOSS_SELECT:
		BossSelect_Fin();
		break;
	case SCENE_INDEX_GAME:
		Game_Finalize();
		break;
	case SCENE_INDEX_ENDING:
		Ending_Fin();
		break;
	case SCENE_INDEX_RANKING:
		Ranking_Fin();
		break;
	case SCENE_INDEX_CONTINUE:
		Continue_Fin();
		break;
	}
}
void  Scene_Update()
{
	switch (g_SceneIndex)
	{
	case SCENE_INDEX_TITLE:
		Title_Update();
		break;
	case SCENE_INDEX_OPENING:
		Opening_Update();
		break;
	case SCENE_INDEX_GAME_TUTORIAL:
		Tutorial_Update();
		break;
	case SCENE_INDEX_BOSS_SELECT:
		BossSelect_Update();
		break;
	case SCENE_INDEX_GAME:
		Game_Update();
		break;
	case SCENE_INDEX_ENDING:
		Ending_Update();
		break;
	case SCENE_INDEX_RANKING:
		Ranking_Update();
		break;
	case SCENE_INDEX_CONTINUE:
		Continue_Update();
		break;
	}
}
void  Scene_Draw()
{
	switch (g_SceneIndex)
	{
	case SCENE_INDEX_TITLE:
		Title_Draw();
		break;
	case SCENE_INDEX_OPENING:
		Opening_Draw();
		break;
	case SCENE_INDEX_GAME_TUTORIAL:
		Tutorial_Draw();
		break;
	case SCENE_INDEX_BOSS_SELECT:
		BossSelect_Draw();
		break;
	case SCENE_INDEX_GAME:
		Game_Draw();
		break;
	case SCENE_INDEX_ENDING:
		Ending_Draw();
		break;
	case SCENE_INDEX_RANKING:
		Ranking_Draw();
		break;
	case SCENE_INDEX_CONTINUE:
		Continue_Draw();
		break;
	}
}
void  Scene_Check(void)
{
	if (g_SceneIndex != g_SceneNextIndex)
	{
		Scene_Fin();
		Scene_Init(g_SceneNextIndex);
	}

}
void Scene_Change(SCENE_INDEX index)
{
	g_SceneNextIndex = index;
}
