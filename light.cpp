#include <d3dx9.h>
#include "mydirect3d.h"

D3DLIGHT9 light;

void Light_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);				//強制的光線ベクトルの長さを1にする（拡大縮小による不具合を防ぐ）
}


void Light_SetLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//ライティング設定
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						//ライティング有効
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;									//平行光源
	D3DXVECTOR3 vecDirLight(0.0f, 0.0f, 1.0f);							//ライトベクトル
	D3DXVec3Normalize(&vecDirLight, &vecDirLight);						//正規化
	light.Direction = vecDirLight;
	light.Diffuse.r = 1.0f;												//ライト色（ディフューズ）
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;
	pDevice->SetLight(0, &light);										//番号変えると複数の光源設置できる
	pDevice->LightEnable(0, TRUE);										//↑と同じ
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

	//アンビエント
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(144, 128, 160, 255));
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
}