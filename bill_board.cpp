#include "bill_board.h"
#include "mydirect3d.h"
#include "texture.h"
#include "camera.h"
#include <d3dx9.h>

//�AFVF�̒�`
#define FVF_BILL_BOARD_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL)
#define CUBE_VERTEX_COUNT (4)
#define CUBE_DRAW_MAX (4)
#define THRESHOLD_VALUE	(60)

typedef struct {
	D3DXVECTOR3 position;//VECTOR4�͍��W�ϊ��ςݒ��_rhw = 1.0��2d�̎��ɓ����Ă���
	D3DXVECTOR3 normal; //�@���x�N�g��
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
	//�C���[���h���W�ϊ��s��̍쐬�Ɛݒ�
	D3DXMATRIX mtxWorld; //4*4
	D3DXMATRIX mtxScale;
	D3DXMATRIX mtxTranslation;

	BillBoardVertex* pBillBoardVertex = NULL;
	g_pVertexBuffer->Lock(0, 0, (void**)&pBillBoardVertex, 0);

	//���_�f�[�^
	static const BillBoardVertex v[] = {
		{ D3DXVECTOR3(-0.5f, 0.5f,0.0f), D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0,0) },
		{ D3DXVECTOR3(0.5f, 0.5f,0.0f), D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1,0) },
		{ D3DXVECTOR3(-0.5f,-0.5f,0.0f), D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0,1) },
		{ D3DXVECTOR3(0.5f,-0.5f,0.0f), D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1,1) },
	};

	memcpy(&pBillBoardVertex[0], &v[0], sizeof(v[0]) * 4);
	g_pVertexBuffer->Unlock();

	D3DXMatrixTranslation(&mtxTranslation, posX, posY, posZ); //�s��̕��s�ړ�
	D3DXMatrixScaling(&mtxScale, scaleX, scaleY, scaleZ); //�s��̕��s�ړ�
	//D3DXMatrixInverse(&mtxInvV, NULL, &GetViewMatrix()); //�t�s��̍쐬
	D3DXMatrixTranspose(&mtxInvV,&Camera_GetView());	//�]�u�s��̍쐬
	mtxInvV._14 = 0.0f; //����f�̕����̒l��0��
	mtxInvV._24 = 0.0f;
	mtxInvV._34 = 0.0f;

	mtxWorld = mtxScale * mtxInvV * mtxTranslation;
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld); //��1�����F�ǂ������ϊ��^�C�v�H��2�����F���̍s��̃A�h���X�́H

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE); //���C�g���g�����g��Ȃ���
												   //�A���r�G���g�̐ݒ�
	
	//z�e�X�g
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, Texture_GetTexture(index));

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(BillBoardVertex));
	pDevice->SetFVF(FVF_BILL_BOARD_VERTEX);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

}

