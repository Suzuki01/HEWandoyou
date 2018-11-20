/*==============================================================================

スプライト描画モジュール [sprite.h]
Author : Youhei Sato
Date   : 2018/06/17
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef SPRITE_H_
#define SPRITE_H_


#include <d3d9.h>    // D3DCOLOR
#include "texture.h" // TextureIndex


/*------------------------------------------------------------------------------
関数のプロトタイプ宣言
------------------------------------------------------------------------------*/

// スプライト描画モジュールの初期化
void Sprite_Initialize(void);

// スプライト描画モジュールの終了処理
void Sprite_Finalize(void);

// スプライトポリゴンの頂点カラー設定
// ※4頂点すべてに同じカラーを設定する
//
// 引数 ... color 設定する色
void Sprite_SetColor(D3DCOLOR color);

// スプライト描画
// ※テクスチャ解像度と同じ大きさのスプライトを指定座標に描画する
//
// 引数:texture_index ... テクスチャ管理番号
//      dx            ... 描画座標x
//      dy            ... 描画座標y
void Sprite_Draw(TextureIndex texture_index, float dx, float dy);

// スプライト描画
// ※テクスチャ切り取り幅、高さと同じ大きさのスプライトを指定座標に描画する
//
// 引数:texture_index ... テクスチャ管理番号
//      dx            ... 描画座標x
//      dy            ... 描画座標y
//      tx            ... テクスチャ切り取り座標x
//      ty            ... テクスチャ切り取り座標y
//      tw            ... テクスチャ切り取り幅
//      th            ... テクスチャ切り取り高さ
void Sprite_Draw(TextureIndex texture_index, float dx, float dy, int tx, int ty, int tw, int th);

// スプライト描画
// ※テクスチャ切り取り幅、高さと同じ大きさのスプライトを指定座標に配置してスプライトの指定座標を中心に拡大、回転描画する
//
// 引数:texture_index ... テクスチャ管理番号
//      dx            ... 描画座標x
//      dy            ... 描画座標y
//      tx            ... テクスチャ切り取り座標x
//      ty            ... テクスチャ切り取り座標y
//      tw            ... テクスチャ切り取り幅
//      th            ... テクスチャ切り取り高さ
//      cx            ... 中心座標x
//      cy            ... 中心座標y
//      sx            ... 拡大率x方向
//      sy            ... 拡大率y方向
//      rotation      ... 回転角度（ラジアン）
void Sprite_Draw(TextureIndex texture_index, float dx, float dy, int tx, int ty, int tw, int th, float cx, float cy, float sx, float sy, float rotation);

#endif // SPRITE_H_
