/*==============================================================================

ゲーム制御 [game.h]
Author : Youhei Sato
Date   : 2018/09/05
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef GAME_H_
#define GAME_H_


/*------------------------------------------------------------------------------
関数のプロトタイプ宣言
------------------------------------------------------------------------------*/

// ゲームの初期化
void Game_Initialize(void);

// ゲームの終了処理
void Game_Finalize(void);

// ゲームの更新
void Game_Update(void);

// ゲームの描画
void Game_Draw(void);

#endif // GAME_H_
