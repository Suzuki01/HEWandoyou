/*==============================================================================

ゲーム制御 [game.cpp]
Author : Youhei Sato
Date   : 2018/09/05
--------------------------------------------------------------------------------

==============================================================================*/

#include "cube.h"
#include "grid.h"
#include "light.h"
#include "camera.h"
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "debug_font.h"
#include "input.h"
#include "joycon.h"
#include "fade.h"
#include "explosion.h"
#include "scene.h"
#include "BossSelect.h"

static bool bGameOver;
static bool bGameWin;

/*------------------------------------------------------------------------------
関数定義
------------------------------------------------------------------------------*/

// ゲームの初期化
void Game_Initialize(void)
{
	bGameOver = false;
	bGameWin = false;

	Cube_Initialize();
	Grid_Initialize();
	Light_Initialize();
	Camera_Initialize();
	Enemy_Initialize();
	Player_Initialize();
	Bullet_Initialize();
	Explosion_Initialize();
	Fade_Initialize();
}

// ゲームの終了処理
void Game_Finalize(void)
{
	Cube_Finalize();
}

// ゲームの更新
void Game_Update(void)
{

	Camera_Update();
	Cube_Update();
	Enemy_Update();
	Player_Update();
	Bullet_Update();
	pollLoop();
	Explosion_Update();
	Fade_Update();
	
	//playerHP =0;
	if (Keyboard_IsTrigger(DIK_1))
	{
		Scene_Change(SCENE_INDEX_CONTINUE);
	}
	//EnemyHp =0;
	if (Keyboard_IsTrigger(DIK_2))
	{
		Scene_Change(SCENE_INDEX_ENDING);
	}

	//ゲーム終了
	if (Player_Get_HP() == 0|| Enemy_Get_HP() == 0)
	{
		Scene_Change(SCENE_INDEX_CONTINUE);
	}

	if (Enemy_Get_HP() == 0)
	{
		Scene_Change(SCENE_INDEX_GAME);
	}
}

// ゲームの描画
void Game_Draw(void)
{
	Light_SetLight();
	Enemy_Draw();
	Player_Draw();
	Bullet_Draw();
	Explosion_Draw();

	DebugFont_Draw(50, 100,  "1で　playerHP=0　 test");
	DebugFont_Draw(50, 200,  "2で　EnemyHP=0    test");

	Grid_Draw();
	Fade_Draw();
}