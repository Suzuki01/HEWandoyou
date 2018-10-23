#include <d3dx9.h>
#include "mydirect3d.h"
#include "common.h"
#include "debug_font.h"
#include "input.h"
#include "texture.h"

//①頂点構造体
typedef struct Vertex3D_tag
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;
}Vertex3D;


//②FVFの定義
#define FVF_VERTEX3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)		// 3Dポリゴン頂点フォーマット
#define CUBE_LENTH		(0.5f)

//③頂点データ（Cube）
//static const Vertex3D v[] =
//{
//	{ D3DXVECTOR3(-0.5f, 0.5f, -0.5f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0, 0) },
//	{ D3DXVECTOR3(0.5f, 0.5f, -0.5f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 0) },
//	{ D3DXVECTOR3(-0.5f, -0.5f, -0.5f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0, 1) },
//	{ D3DXVECTOR3(-0.5f, -0.5f, -0.5f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0, 1) },
//	{ D3DXVECTOR3(0.5f, 0.5f, -0.5f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 0) },
//	{ D3DXVECTOR3(0.5f, -0.5f, -0.5f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1, 1) },
//
//	{ D3DXVECTOR3(-0.5f, 0.5f, 0.5f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0, 0) },
//	{ D3DXVECTOR3(0.5f, 0.5f, 0.5f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 0) },
//	{ D3DXVECTOR3(-0.5f, 0.5f, -0.5f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0, 1) },
//	{ D3DXVECTOR3(-0.5f, 0.5f, -0.5f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0, 1) },
//	{ D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 0) },
//	{ D3DXVECTOR3(0.5f, 0.5f, -0.5f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 1) },
//
//	{ D3DXVECTOR3(0.5f, 0.5f, -0.5f),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0, 0) },
//	{ D3DXVECTOR3(0.5f, 0.5f, 0.5f),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 0) },
//	{ D3DXVECTOR3(0.5f, -0.5f, -0.5f),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0, 1) },
//	{ D3DXVECTOR3(0.5f, -0.5f, -0.5f),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0, 1) },
//	{ D3DXVECTOR3(0.5f, 0.5f, 0.5f),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 0) },
//	{ D3DXVECTOR3(0.5f, -0.5f, 0.5f),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 1) },
//
//	{ D3DXVECTOR3(0.5f, 0.5f, 0.5f),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0, 0) },
//	{ D3DXVECTOR3(-0.5f, 0.5f, 0.5f),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1, 0) },
//	{ D3DXVECTOR3(0.5f, -0.5f, 0.5f),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0, 1) },
//	{ D3DXVECTOR3(0.5f, -0.5f, 0.5f),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0, 1) },
//	{ D3DXVECTOR3(-0.5f, 0.5f, 0.5f),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1, 0) },
//	{ D3DXVECTOR3(-0.5f, -0.5f, 0.5f),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 1) },
//
//	{ D3DXVECTOR3(0.5f, -0.5f, 0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0, 0) },
//	{ D3DXVECTOR3(-0.5f, -0.5f, 0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1, 0) },
//	{ D3DXVECTOR3(0.5f, -0.5f, -0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0, 1) },
//	{ D3DXVECTOR3(0.5f, -0.5f, -0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0, 1) },
//	{ D3DXVECTOR3(-0.5f, -0.5f, 0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 0) },
//	{ D3DXVECTOR3(-0.5f, -0.5f, -0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1, 1) },
//
//	{ D3DXVECTOR3(-0.5f, 0.5f, 0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0, 0) },
//	{ D3DXVECTOR3(-0.5f, 0.5f, -0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 0) },
//	{ D3DXVECTOR3(-0.5f, -0.5f, 0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0, 1) },
//	{ D3DXVECTOR3(-0.5f, -0.5f, 0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0, 1) },
//	{ D3DXVECTOR3(-0.5f, 0.5f, -0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1, 0) },
//	{ D3DXVECTOR3(-0.5f, -0.5f, -0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 1) },
//};



static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL; // 頂点バッファインターフェース
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;   // インデックスバッファインターフェース
static D3DCOLOR g_Color = 0xffffffff;


void Cube_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	// 頂点バッファの確保（頂点24つ分）
	pDevice->CreateVertexBuffer(sizeof(Vertex3D) * 24, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_DEFAULT, &g_pVertexBuffer, NULL);
	// インデックスバッファの確保（インデックス36個分）
	pDevice->CreateIndexBuffer(sizeof(WORD) * 36, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIndexBuffer, NULL);

	// 頂点インデックスバッファへ頂点インデックスの書き込み
	LPWORD pIndex; // 仮想アドレス
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0);
	for (int i = 0; i < 6; i++)
	{
		pIndex[i * 6 + 0] = i * 4;
		pIndex[i * 6 + 1] = i * 4 + 1;
		pIndex[i * 6 + 2] = i * 4 + 2;
		pIndex[i * 6 + 3] = i * 4 + 2;
		pIndex[i * 6 + 4] = i * 4 + 1;
		pIndex[i * 6 + 5] = i * 4 + 3;
	}
	g_pIndexBuffer->Unlock();

	// 頂点バッファへの頂点情報の書き込み

	Vertex3D* pV; // 仮想アドレス
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);

	//position
	pV[0].position = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pV[1].position = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pV[2].position = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pV[3].position = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pV[4].position = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pV[5].position = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pV[6].position = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pV[7].position = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pV[8].position = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pV[9].position = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pV[10].position = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pV[11].position = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pV[12].position = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pV[13].position = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pV[14].position = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pV[15].position = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	pV[16].position = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pV[17].position = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	pV[18].position = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pV[19].position = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pV[20].position = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pV[21].position = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pV[22].position = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	pV[23].position = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);

	//normal
	for (int i = 0; i < 4; i++)
	{
		pV[i].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pV[i + 4].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pV[i + 8].normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		pV[i + 12].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pV[i + 16].normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pV[i + 20].normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	}

	//color
	for (int i = 0; i < 24; i++) {
		pV[i].color = g_Color;
	}

	//texcoord
	for (int i = 0; i < 5; i++)
	{
		pV[i * 4 + 0].texcoord = D3DXVECTOR2(0.0f, 0.0f);
		pV[i * 4 + 1].texcoord = D3DXVECTOR2(1.0f, 0.0f);
		pV[i * 4 + 2].texcoord = D3DXVECTOR2(0.0f, 1.0f);
		pV[i * 4 + 3].texcoord = D3DXVECTOR2(1.0f, 1.0f);

	}

	g_pVertexBuffer->Unlock();

}

void Cube_Finalize(void)
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

void Cube_Update(void)
{

}

void Cube_Draw(TextureIndex texture_index, float dx, float dy, float dz, float sx, float sy, float sz, float angleX, float angleY, float angleZ)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//④ワールド座標変換行列の作成と設定
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxTranslation;
	D3DXMATRIX mtxScaling;
	D3DXMATRIX mtxRotation;
	D3DXMatrixIdentity(&mtxWorld);		//単位行列を作る

	/////////ワールド変換行列演習（平行・回転・拡大）/////////
	/*******平行*******/
	//D3DXMatrixTranslation(&mtxTranslation, 1, 0, 0);			//各方向(x,y,z)に移動する距離
	/*******拡大*******////(あまり使わないように)
	//D3DXMatrixScaling(&mtxScaling, 1, 1, 1);					//各方向(x,y,z)に拡大する倍率			/←←←←←法線ベクトルも拡大縮小したせいで、光線ベクトルの長さが1じゃなくなる可能性ある←←←←←/
	/*******回転*******/
	//D3DXMatrixRotationX(&mtxRotation, angleX);
	//D3DXMatrixRotationY(&mtxRotation, angleY);
	//D3DXMatrixRotationZ(&mtxRotation, angleZ);									//この三つの関数は一つだけ（一番下に書いたやつ）しか実行しない
	//D3DXMatrixRotationYawPitchRoll(&mtxRotation, angleY, angleX, angleZ);			//Y X Zの順番に全方向回転する
	//D3DXMatrixRotationAxis(&mtxRotation,軸ベクトルのアドレス,angle)				//特定の軸を中心に回る（斜めに回転するの可能）

	/*******行列の合成*******/
	//方法①：D3DXMatrixMultiply(行列のアドレス,合成A行列のアドレス,合成A行列のアドレス)	//AとBの合成結果を保存する、複数の行列を合成する時この関数何回も使わなければいけない、Wii （ゴミだから）は主にこれ使う
	//方法②：mtxWorld=mtxRotation*mtxTranslation;		//複数の行列合成可能、「*=」も使える



	pDevice->SetFVF(FVF_VERTEX3D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));         // デバイスにテクスチャを設定

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex3D));
	pDevice->SetIndices(g_pIndexBuffer);


	//描画
	D3DXMatrixTranslation(&mtxTranslation, dx + CUBE_LENTH, dy + CUBE_LENTH, dz + CUBE_LENTH);
	D3DXMatrixScaling(&mtxScaling, sx, sy, sz);
	//D3DXMatrixRotationX(&mtxRotation, angleX);
	//D3DXMatrixRotationY(&mtxRotation, angleY);
	mtxWorld = mtxScaling*mtxTranslation;
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);
}
