/*==============================================================================

スプライト描画モジュール [sprite.cpp]
Author : Youhei Sato
Date   : 2018/06/17
--------------------------------------------------------------------------------

==============================================================================*/
#include <d3dx9.h>
#include <math.h>
#include "mydirect3d.h"
#include "texture.h"


/*------------------------------------------------------------------------------
構造体宣言
------------------------------------------------------------------------------*/
// ２Dポリゴン頂点構造体
typedef struct Vertex2D_tag
{
	D3DXVECTOR4 position; // 頂点座標（座標変換済み頂点）
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;
} Vertex2D;
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) // ２Dポリゴン頂点フォーマット
#define THRESHOLD_VALUE	(10)


/*------------------------------------------------------------------------------
グローバル変数宣言
------------------------------------------------------------------------------*/
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL; // 頂点バッファインターフェース
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;   // インデックスバッファインターフェース
static D3DCOLOR g_Color = 0xffffffff;


/*------------------------------------------------------------------------------
関数定義
------------------------------------------------------------------------------*/

// スプライト描画モジュールの初期化
void Sprite_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	// 頂点バッファの確保（頂点４つ分）
	pDevice->CreateVertexBuffer(sizeof(Vertex2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_DEFAULT, &g_pVertexBuffer, NULL);
	// インデックスバッファの確保（インデックス６個分）
	pDevice->CreateIndexBuffer(sizeof(WORD) * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIndexBuffer, NULL);

	// 頂点インデックスバッファへ頂点インデックスの書き込み
	LPWORD pIndex; // 仮想アドレス
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0);
	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;
	pIndex[3] = 1;
	pIndex[4] = 3;
	pIndex[5] = 2;
	g_pIndexBuffer->Unlock();
}

// スプライト描画モジュールの終了処理
void Sprite_Finalize(void)
{
	// インデックスバッファの解放
	if (g_pIndexBuffer) {
		g_pIndexBuffer->Release();
		g_pIndexBuffer = NULL;
	}

	// 頂点バッファの解放
	if (g_pVertexBuffer) {
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
}

// スプライトポリゴンの頂点カラー設定
void Sprite_SetColor(D3DCOLOR color)
{
	g_Color = color;
}

// スプライト描画
// ※テクスチャ解像度と同じ大きさのスプライトを指定座標に描画する
void Sprite_Draw(TextureIndex texture_index, float dx, float dy)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	// テクスチャの幅、高さ取得
	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	// 頂点バッファへの頂点情報の書き込み

	Vertex2D* pV; // 仮想アドレス
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);

	pV[0].position = D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f);
	pV[1].position = D3DXVECTOR4(dx + w - 0.5f, dy - 0.5f, 0.0f, 1.0f);
	pV[2].position = D3DXVECTOR4(dx - 0.5f, dy + h - 0.5f, 0.0f, 1.0f);
	pV[3].position = D3DXVECTOR4(dx + w - 0.5f, dy + h - 0.5f, 0.0f, 1.0f);

	for (int i = 0; i < 4; i++) {
		pV[i].color = g_Color;
	}

	pV[0].texcoord = D3DXVECTOR2(0.0f, 0.0f);
	pV[1].texcoord = D3DXVECTOR2(1.0f, 0.0f);
	pV[2].texcoord = D3DXVECTOR2(0.0f, 1.0f);
	pV[3].texcoord = D3DXVECTOR2(1.0f, 1.0f);

	g_pVertexBuffer->Unlock();

	pDevice->SetFVF(FVF_VERTEX2D);                                     // デバイスに頂点の構成要素を設定
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));         // デバイスにテクスチャを設定
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex2D)); // 頂点バッファの設定
	pDevice->SetIndices(g_pIndexBuffer);                               // インデックスバッファの設定

																	   // プリミティブの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

// スプライト描画
// ※テクスチャ切り取り幅、高さと同じ大きさのスプライトを指定座標に描画する
void Sprite_Draw(TextureIndex texture_index, float dx, float dy, int tx, int ty, int tw, int th)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	// UV座標計算
	float u[2], v[2];
	u[0] = (float)tx / w;
	v[0] = (float)ty / h;
	u[1] = (float)(tx + tw) / w;
	v[1] = (float)(ty + th) / h;

	Vertex2D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);

	pV[0].position = D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f);
	pV[1].position = D3DXVECTOR4(dx + tw - 0.5f, dy - 0.5f, 0.0f, 1.0f);
	pV[2].position = D3DXVECTOR4(dx - 0.5f, dy + th - 0.5f, 0.0f, 1.0f);
	pV[3].position = D3DXVECTOR4(dx + tw - 0.5f, dy + th - 0.5f, 0.0f, 1.0f);

	for (int i = 0; i < 4; i++) {
		pV[i].color = g_Color;
	}

	pV[0].texcoord = D3DXVECTOR2(u[0], v[0]);
	pV[1].texcoord = D3DXVECTOR2(u[1], v[0]);
	pV[2].texcoord = D3DXVECTOR2(u[0], v[1]);
	pV[3].texcoord = D3DXVECTOR2(u[1], v[1]);

	g_pVertexBuffer->Unlock();

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex2D));
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

// スプライト描画
// ※テクスチャ切り取り幅、高さと同じ大きさのスプライトを指定座標に配置してスプライトの指定座標を中心に拡大、回転描画する
void Sprite_Draw(TextureIndex texture_index, float dx, float dy, int tx, int ty, int tw, int th, float cx, float cy, float sx, float sy, float rotation)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	float u[2], v[2];
	u[0] = (float)tx / w;
	v[0] = (float)ty / h;
	u[1] = (float)(tx + tw) / w;
	v[1] = (float)(ty + th) / h;

	// 座標変換
	float px[4], py[4];
	px[0] = -cx        * sx * cosf(rotation) - -cy        * sy * sinf(rotation);
	py[0] = -cx        * sx * sinf(rotation) + -cy        * sy * cosf(rotation);
	px[1] = (-cx + tw) * sx * cosf(rotation) - -cy        * sy * sinf(rotation);
	py[1] = (-cx + tw) * sx * sinf(rotation) + -cy        * sy * cosf(rotation);
	px[2] = -cx        * sx * cosf(rotation) - (-cy + th) * sy * sinf(rotation);
	py[2] = -cx        * sx * sinf(rotation) + (-cy + th) * sy * cosf(rotation);
	px[3] = (-cx + tw) * sx * cosf(rotation) - (-cy + th) * sy * sinf(rotation);
	py[3] = (-cx + tw) * sx * sinf(rotation) + (-cy + th) * sy * cosf(rotation);

	Vertex2D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);

	pV[0].position = D3DXVECTOR4(px[0] + dx + cx - 0.5f, py[0] + dy + cy - 0.5f, 0.0f, 1.0f);
	pV[1].position = D3DXVECTOR4(px[1] + dx + cx - 0.5f, py[1] + dy + cy - 0.5f, 0.0f, 1.0f);
	pV[2].position = D3DXVECTOR4(px[2] + dx + cx - 0.5f, py[2] + dy + cy - 0.5f, 0.0f, 1.0f);
	pV[3].position = D3DXVECTOR4(px[3] + dx + cx - 0.5f, py[3] + dy + cy - 0.5f, 0.0f, 1.0f);

	for (int i = 0; i < 4; i++) {
		pV[i].color = g_Color;
	}

	pV[0].texcoord = D3DXVECTOR2(u[0], v[0]);
	pV[1].texcoord = D3DXVECTOR2(u[1], v[0]);
	pV[2].texcoord = D3DXVECTOR2(u[0], v[1]);
	pV[3].texcoord = D3DXVECTOR2(u[1], v[1]);

	g_pVertexBuffer->Unlock();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE); //αテスト有効　デバイス作成時はFALSE
	pDevice->SetRenderState(D3DRS_ALPHAREF, THRESHOLD_VALUE); // 第2引数にしきい値の値を入れる
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex2D));
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

