#include <d3dx9.h>
#include "mydirect3d.h"
#include "common.h"
#include "debug_font.h"
#include "input.h"
#include "texture.h"

//�@���_�\����
typedef struct Vertex3D_tag
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;
}Vertex3D;


//�AFVF�̒�`
#define FVF_VERTEX3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)		// 3D�|���S�����_�t�H�[�}�b�g
#define CUBE_LENTH		(0.5f)

//�B���_�f�[�^�iCube�j
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



static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL; // ���_�o�b�t�@�C���^�[�t�F�[�X
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;   // �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X
static D3DCOLOR g_Color = 0xffffffff;


void Cube_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	// ���_�o�b�t�@�̊m�ہi���_24���j
	pDevice->CreateVertexBuffer(sizeof(Vertex3D) * 24, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_DEFAULT, &g_pVertexBuffer, NULL);
	// �C���f�b�N�X�o�b�t�@�̊m�ہi�C���f�b�N�X36���j
	pDevice->CreateIndexBuffer(sizeof(WORD) * 36, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIndexBuffer, NULL);

	// ���_�C���f�b�N�X�o�b�t�@�֒��_�C���f�b�N�X�̏�������
	LPWORD pIndex; // ���z�A�h���X
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

	// ���_�o�b�t�@�ւ̒��_���̏�������

	Vertex3D* pV; // ���z�A�h���X
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
	// �C���f�b�N�X�o�b�t�@�̉��
	if (g_pIndexBuffer) {
		g_pIndexBuffer->Release();
		g_pIndexBuffer = NULL;
	}

	// ���_�o�b�t�@�̉��
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

	//�C���[���h���W�ϊ��s��̍쐬�Ɛݒ�
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxTranslation;
	D3DXMATRIX mtxScaling;
	D3DXMATRIX mtxRotation;
	D3DXMatrixIdentity(&mtxWorld);		//�P�ʍs������

	/////////���[���h�ϊ��s�񉉏K�i���s�E��]�E�g��j/////////
	/*******���s*******/
	//D3DXMatrixTranslation(&mtxTranslation, 1, 0, 0);			//�e����(x,y,z)�Ɉړ����鋗��
	/*******�g��*******////(���܂�g��Ȃ��悤��)
	//D3DXMatrixScaling(&mtxScaling, 1, 1, 1);					//�e����(x,y,z)�Ɋg�傷��{��			/�����������@���x�N�g�����g��k�����������ŁA�����x�N�g���̒�����1����Ȃ��Ȃ�\�����適��������/
	/*******��]*******/
	//D3DXMatrixRotationX(&mtxRotation, angleX);
	//D3DXMatrixRotationY(&mtxRotation, angleY);
	//D3DXMatrixRotationZ(&mtxRotation, angleZ);									//���̎O�̊֐��͈�����i��ԉ��ɏ�������j�������s���Ȃ�
	//D3DXMatrixRotationYawPitchRoll(&mtxRotation, angleY, angleX, angleZ);			//Y X Z�̏��ԂɑS������]����
	//D3DXMatrixRotationAxis(&mtxRotation,���x�N�g���̃A�h���X,angle)				//����̎��𒆐S�ɉ��i�΂߂ɉ�]����̉\�j

	/*******�s��̍���*******/
	//���@�@�FD3DXMatrixMultiply(�s��̃A�h���X,����A�s��̃A�h���X,����A�s��̃A�h���X)	//A��B�̍������ʂ�ۑ�����A�����̍s����������鎞���̊֐�������g��Ȃ���΂����Ȃ��AWii �i�S�~������j�͎�ɂ���g��
	//���@�A�FmtxWorld=mtxRotation*mtxTranslation;		//�����̍s�񍇐��\�A�u*=�v���g����



	pDevice->SetFVF(FVF_VERTEX3D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));         // �f�o�C�X�Ƀe�N�X�`����ݒ�

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex3D));
	pDevice->SetIndices(g_pIndexBuffer);


	//�`��
	D3DXMatrixTranslation(&mtxTranslation, dx + CUBE_LENTH, dy + CUBE_LENTH, dz + CUBE_LENTH);
	D3DXMatrixScaling(&mtxScaling, sx, sy, sz);
	//D3DXMatrixRotationX(&mtxRotation, angleX);
	//D3DXMatrixRotationY(&mtxRotation, angleY);
	mtxWorld = mtxScaling*mtxTranslation;
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);
}
