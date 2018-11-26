#include "bill_board.h"
#include "mydirect3d.h"
#include "texture.h"
#include "camera.h"
#include <d3dx9.h>

//②FVFの定義
#define FVF_BILL_BOARD_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL)
#define CUBE_VERTEX_COUNT (4)
#define CUBE_DRAW_MAX (4)
#define THRESHOLD_VALUE	(60)

typedef struct {
	D3DXVECTOR3 position;//VECTOR4は座標変換済み頂点rhw = 1.0が2dの時に入っていた
	D3DXVECTOR3 normal; //法線ベクトル
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;
}BillBoardVertex;

static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static D3DXMATRIX mtxInvV;



void BillBoard_Initialize() {
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	pDevice->CreateVertexBuffer(sizeof(BillBoardVertex) * 4, D3DUSAGE_WRITEONLY, FVF_BILL_BOARD_VERTEX, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);
}

void BillBoard_Finalize() {

	if (g_pVertexBuffer) {
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
}

void BillBoard_Draw(TextureIndex index,float posX,float posY,float posZ,float scaleX, float scaleY, float scaleZ) {
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	//④ワールド座標変換行列の作成と設定
	D3DXMATRIX mtxWorld; //4*4
	D3DXMATRIX mtxScale;
	D3DXMATRIX mtxTranslation;

	BillBoardVertex* pBillBoardVertex = NULL;
	g_pVertexBuffer->Lock(0, 0, (void**)&pBillBoardVertex, 0);

	//頂点データ
	static const BillBoardVertex v[] = {
		{ D3DXVECTOR3(-0.5f, 0.5f,0.0f), D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0,0) },
		{ D3DXVECTOR3(0.5f, 0.5f,0.0f), D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1,0) },
		{ D3DXVECTOR3(-0.5f,-0.5f,0.0f), D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0,1) },
		{ D3DXVECTOR3(0.5f,-0.5f,0.0f), D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1,1) },
	};

	memcpy(&pBillBoardVertex[0], &v[0], sizeof(v[0]) * 4);
	g_pVertexBuffer->Unlock();

	D3DXMatrixTranslation(&mtxTranslation, posX, posY, posZ); //行列の平行移動
	D3DXMatrixScaling(&mtxScale, scaleX, scaleY, scaleZ); //行列の平行移動
	//D3DXMatrixInverse(&mtxInvV, NULL, &GetViewMatrix()); //逆行列の作成
	D3DXMatrixTranspose(&mtxInvV,&Camera_GetView());	//転置行列の作成
	mtxInvV._14 = 0.0f; //せん断の部分の値を0に
	mtxInvV._24 = 0.0f;
	mtxInvV._34 = 0.0f;

	mtxWorld = mtxScale * mtxInvV * mtxTranslation;
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld); //第1引数：どういう変換タイプ？第2引数：その行列のアドレスは？

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE); //ライトを使うか使わないか
												   //アンビエントの設定
	
	//zテスト
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	//テクスチャの設定
	pDevice->SetTexture(0, Texture_GetTexture(index));

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(BillBoardVertex));
	pDevice->SetFVF(FVF_BILL_BOARD_VERTEX);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

}

