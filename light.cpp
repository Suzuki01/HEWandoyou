#include <d3dx9.h>
#include "mydirect3d.h"

D3DLIGHT9 light;

void Light_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);				//�����I�����x�N�g���̒�����1�ɂ���i�g��k���ɂ��s���h���j
}


void Light_SetLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//���C�e�B���O�ݒ�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						//���C�e�B���O�L��
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;									//���s����
	D3DXVECTOR3 vecDirLight(0.0f, 0.0f, 1.0f);							//���C�g�x�N�g��
	D3DXVec3Normalize(&vecDirLight, &vecDirLight);						//���K��
	light.Direction = vecDirLight;
	light.Diffuse.r = 1.0f;												//���C�g�F�i�f�B�t���[�Y�j
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;
	pDevice->SetLight(0, &light);										//�ԍ��ς���ƕ����̌����ݒu�ł���
	pDevice->LightEnable(0, TRUE);										//���Ɠ���
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

	//�A���r�G���g
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(144, 128, 160, 255));
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
}