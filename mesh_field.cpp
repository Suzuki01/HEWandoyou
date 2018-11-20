#include "texture.h"
#include <d3dx9.h>
#include "mydirect3d.h"
#include "debug_font.h"

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

static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL; // ���_�o�b�t�@�C���^�[�t�F�[�X
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;   // �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X
static D3DCOLOR g_Color = 0xffffffff;

void MeshField_Initialize(float lenth, int column,int row)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	// ���_�o�b�t�@�̊m�ہi���_ (column+1) * (row+1) ���j
	pDevice->CreateVertexBuffer(sizeof(Vertex3D) * (column + 1) * (row + 1), D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_DEFAULT, &g_pVertexBuffer, NULL);
	// �C���f�b�N�X�o�b�t�@�̊m�ہi�C���f�b�N�X 2*(column+1)*row + 4*(row-1) ���j
	pDevice->CreateIndexBuffer(sizeof(WORD) * 2 * (column + 1)*row + 2 * (row - 1), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIndexBuffer, NULL);

	// ���_�C���f�b�N�X�o�b�t�@�֒��_�C���f�b�N�X�̏�������
	LPWORD pIndex; // ���z�A�h���X
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0);

	for (int i = 0; i < 2 * (column + 2); i++)
	{
		for (int j = 0; j < row; j++)
		{
			if (i < 2 * (column + 1))
			{
				pIndex[j* 2 * (column + 2) + i] = ((i + 1) % 2 + j) * (column + 1) + i / 2;
			}
			else if(j < (row - 1))
			{
				pIndex[j * 2 * (column + 2) + 2 * (column + 1)] = pIndex[j * 2 * (column + 2) + 2 * (column + 1) - 1];
				pIndex[j * 2 * (column + 2) + 2 * (column + 1) + 1] = (j + 2) * (column + 1);
			}
		}
	}
	g_pIndexBuffer->Unlock();

	// ���_�o�b�t�@�ւ̒��_���̏�������

	Vertex3D* pV; // ���z�A�h���X
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);


	for (int j = 0; j <= row; j++)
	{
		for (int i = 0; i <= column; i++)
		{
			//position
			pV[(column + 1)*j + i].position = D3DXVECTOR3((i - column*0.5f)*lenth, 0.0f, -(j - row*0.5f)*lenth);
			//normal
			pV[(column + 1)*j + i].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//color
			pV[(column + 1)*j + i].color = g_Color;
			//texcoord
			pV[(column + 1)*j + i].texcoord = D3DXVECTOR2(i, j);
		}
	}

	g_pVertexBuffer->Unlock();
}

void MeshField_Finalize(void)
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

void MeshField_Draw(TextureIndex texture_index ,int column,int row)
{

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();


	//�C���[���h���W�ϊ��s��̍쐬�Ɛݒ�
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxTranslation;
	D3DXMATRIX mtxScaling;
	D3DXMATRIX mtxRotation;

	D3DXMatrixIdentity(&mtxWorld);		//�P�ʍs������

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetFVF(FVF_VERTEX3D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));        // �f�o�C�X�Ƀe�N�X�`����ݒ�

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex3D));
	pDevice->SetIndices(g_pIndexBuffer);

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�`��

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, (column + 1) * (row + 1), 0, 2 *column*row + 4 * (row - 1));

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

}