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


//③頂点データ（Cube）
static const Vertex3D v[] =
{
	{ D3DXVECTOR3(-0.5f, 0.5f, -0.5f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0, 0) },
	{ D3DXVECTOR3(0.5f, 0.5f, -0.5f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 0) },
	{ D3DXVECTOR3(-0.5f, -0.5f, -0.5f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0, 1) },
	{ D3DXVECTOR3(-0.5f, -0.5f, -0.5f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0, 1) },
	{ D3DXVECTOR3(0.5f, 0.5f, -0.5f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 0) },
	{ D3DXVECTOR3(0.5f, -0.5f, -0.5f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1, 1) },

	{ D3DXVECTOR3(-0.5f, 0.5f, 0.5f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0, 0) },
	{ D3DXVECTOR3(0.5f, 0.5f, 0.5f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 0) },
	{ D3DXVECTOR3(-0.5f, 0.5f, -0.5f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0, 1) },
	{ D3DXVECTOR3(-0.5f, 0.5f, -0.5f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0, 1) },
	{ D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 0) },
	{ D3DXVECTOR3(0.5f, 0.5f, -0.5f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 1) },

	{ D3DXVECTOR3(0.5f, 0.5f, -0.5f),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0, 0) },
	{ D3DXVECTOR3(0.5f, 0.5f, 0.5f),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 0) },
	{ D3DXVECTOR3(0.5f, -0.5f, -0.5f),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0, 1) },
	{ D3DXVECTOR3(0.5f, -0.5f, -0.5f),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0, 1) },
	{ D3DXVECTOR3(0.5f, 0.5f, 0.5f),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 0) },
	{ D3DXVECTOR3(0.5f, -0.5f, 0.5f),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 1) },

	{ D3DXVECTOR3(0.5f, 0.5f, 0.5f),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0, 0) },
	{ D3DXVECTOR3(-0.5f, 0.5f, 0.5f),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1, 0) },
	{ D3DXVECTOR3(0.5f, -0.5f, 0.5f),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0, 1) },
	{ D3DXVECTOR3(0.5f, -0.5f, 0.5f),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0, 1) },
	{ D3DXVECTOR3(-0.5f, 0.5f, 0.5f),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1, 0) },
	{ D3DXVECTOR3(-0.5f, -0.5f, 0.5f),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 1) },

	{ D3DXVECTOR3(0.5f, -0.5f, 0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0, 0) },
	{ D3DXVECTOR3(-0.5f, -0.5f, 0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1, 0) },
	{ D3DXVECTOR3(0.5f, -0.5f, -0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0, 1) },
	{ D3DXVECTOR3(0.5f, -0.5f, -0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0, 1) },
	{ D3DXVECTOR3(-0.5f, -0.5f, 0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 0) },
	{ D3DXVECTOR3(-0.5f, -0.5f, -0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1, 1) },

	{ D3DXVECTOR3(-0.5f, 0.5f, 0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0, 0) },
	{ D3DXVECTOR3(-0.5f, 0.5f, -0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 0) },
	{ D3DXVECTOR3(-0.5f, -0.5f, 0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0, 1) },
	{ D3DXVECTOR3(-0.5f, -0.5f, 0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0, 1) },
	{ D3DXVECTOR3(-0.5f, 0.5f, -0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1, 0) },
	{ D3DXVECTOR3(-0.5f, -0.5f, -0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1, 1) },
};

#define CUBE_LENTH		(0.5f)

void Cube_Initialize(void)
{
	

}

void Cube_Update(void)
{

}

void Cube_Draw(TextureIndex texture_index, float dx, float dy, float dz,float sx, float sy, float sz,float angleX, float angleY, float angleZ)
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


	//描画
	D3DXMatrixTranslation(&mtxTranslation, dx+ CUBE_LENTH, dy+ CUBE_LENTH, dz+CUBE_LENTH);
	D3DXMatrixScaling(&mtxScaling, sx, sy, sz);
	//D3DXMatrixRotationX(&mtxRotation, angleX);
	//D3DXMatrixRotationY(&mtxRotation, angleY);
	mtxWorld = mtxScaling*mtxTranslation;
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, v, sizeof(Vertex3D));
}
