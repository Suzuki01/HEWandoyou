#include <d3dx9.h>
#include "mydirect3d.h"
#include "common.h"
#include "grid.h"

//�@���_�\����
typedef struct Vertex3D_tag
{
	D3DXVECTOR3 position;
	D3DCOLOR color;
}Vertex3D;


//�AFVF�̒�`
#define FVF_VERTEX3D (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)		// 3D�|���S�����_�t�H�[�}�b�g


//�B���_�f�[�^�iGrid�j
static Vertex3D v[44];
static Vertex3D attack[2];
static Vertex3D player[2];

void Grid_Initialize(void)
{
	int i;
	for (i = 0; i < 11; i++)
	{
		v[i * 2].position = D3DXVECTOR3(-5.0f, 0.0f, (5-i)*1.0f);
		v[i * 2 + 1].position = D3DXVECTOR3(5.0f, 0.0f, (5-i)*1.0f);
		v[i * 2].color = D3DCOLOR_RGBA(0, 255, 0, 255);
		v[i * 2 + 1].color = D3DCOLOR_RGBA(0, 255, 0, 255);
	}
	for (i = 11; i < 22; i++)
	{
		v[i * 2].position = D3DXVECTOR3(((i - 11) - 5)*1.0f, 0.0f, 5.0f);
		v[i * 2 + 1].position = D3DXVECTOR3(((i - 11) - 5)*1.0f, 0.0f, -5.0f);
		v[i * 2].color = D3DCOLOR_RGBA(0, 255, 0, 255);
		v[i * 2 + 1].color = D3DCOLOR_RGBA(0, 255, 0, 255);
	}

	attack[0].position= D3DXVECTOR3(-8.0f, 0.0f, ATTACK_LINE);
	attack[0].color = D3DCOLOR_RGBA(255, 255, 0, 255);
	attack[1].position = D3DXVECTOR3(8.0f, 0.0f, ATTACK_LINE);
	attack[1].color = D3DCOLOR_RGBA(255, 255, 0, 255);

	player[0].position = D3DXVECTOR3(-8.0f, 0.0f, PLAYER_LINE);
	player[0].color = D3DCOLOR_RGBA(255, 0, 0, 255);
	player[1].position = D3DXVECTOR3(8.0f, 0.0f, PLAYER_LINE);
	player[1].color = D3DCOLOR_RGBA(255, 0, 0, 255);
}


void Grid_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//�C���[���h���W�ϊ��s��̍쐬�Ɛݒ�
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxTranslation;
	D3DXMATRIX mtxScaling;
	D3DXMATRIX mtxRotation;
	D3DXMatrixIdentity(&mtxWorld);		//�P�ʍs������

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetFVF(FVF_VERTEX3D);

	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 22, v, sizeof(Vertex3D));
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, attack, sizeof(Vertex3D));
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, player, sizeof(Vertex3D));
}
