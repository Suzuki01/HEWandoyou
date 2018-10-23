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


//�B���_�f�[�^�iCube�j
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


	//�`��
	D3DXMatrixTranslation(&mtxTranslation, dx+ CUBE_LENTH, dy+ CUBE_LENTH, dz+CUBE_LENTH);
	D3DXMatrixScaling(&mtxScaling, sx, sy, sz);
	//D3DXMatrixRotationX(&mtxRotation, angleX);
	//D3DXMatrixRotationY(&mtxRotation, angleY);
	mtxWorld = mtxScaling*mtxTranslation;
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, v, sizeof(Vertex3D));
}
