/*==============================================================================

�X�v���C�g�`�惂�W���[�� [sprite.cpp]
Author : Youhei Sato
Date   : 2018/06/17
--------------------------------------------------------------------------------

==============================================================================*/
#include <d3dx9.h>
#include <math.h>
#include "mydirect3d.h"
#include "texture.h"


/*------------------------------------------------------------------------------
�\���̐錾
------------------------------------------------------------------------------*/
// �QD�|���S�����_�\����
typedef struct Vertex2D_tag
{
	D3DXVECTOR4 position; // ���_���W�i���W�ϊ��ςݒ��_�j
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;
} Vertex2D;
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) // �QD�|���S�����_�t�H�[�}�b�g
#define THRESHOLD_VALUE	(10)


/*------------------------------------------------------------------------------
�O���[�o���ϐ��錾
------------------------------------------------------------------------------*/
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL; // ���_�o�b�t�@�C���^�[�t�F�[�X
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;   // �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X
static D3DCOLOR g_Color = 0xffffffff;


/*------------------------------------------------------------------------------
�֐���`
------------------------------------------------------------------------------*/

// �X�v���C�g�`�惂�W���[���̏�����
void Sprite_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	// ���_�o�b�t�@�̊m�ہi���_�S���j
	pDevice->CreateVertexBuffer(sizeof(Vertex2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_DEFAULT, &g_pVertexBuffer, NULL);
	// �C���f�b�N�X�o�b�t�@�̊m�ہi�C���f�b�N�X�U���j
	pDevice->CreateIndexBuffer(sizeof(WORD) * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIndexBuffer, NULL);

	// ���_�C���f�b�N�X�o�b�t�@�֒��_�C���f�b�N�X�̏�������
	LPWORD pIndex; // ���z�A�h���X
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0);
	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;
	pIndex[3] = 1;
	pIndex[4] = 3;
	pIndex[5] = 2;
	g_pIndexBuffer->Unlock();
}

// �X�v���C�g�`�惂�W���[���̏I������
void Sprite_Finalize(void)
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

// �X�v���C�g�|���S���̒��_�J���[�ݒ�
void Sprite_SetColor(D3DCOLOR color)
{
	g_Color = color;
}

// �X�v���C�g�`��
// ���e�N�X�`���𑜓x�Ɠ����傫���̃X�v���C�g���w����W�ɕ`�悷��
void Sprite_Draw(TextureIndex texture_index, float dx, float dy)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	// �e�N�X�`���̕��A�����擾
	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	// ���_�o�b�t�@�ւ̒��_���̏�������

	Vertex2D* pV; // ���z�A�h���X
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);

	pV[0].position = D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f);
	pV[1].position = D3DXVECTOR4(dx + w - 0.5f, dy - 0.5f, 0.0f, 1.0f);
	pV[2].position = D3DXVECTOR4(dx - 0.5f, dy + h - 0.5f, 0.0f, 1.0f);
	pV[3].position = D3DXVECTOR4(dx + w - 0.5f, dy + h - 0.5f, 0.0f, 1.0f);

	for (int i = 0; i < 4; i++) {
		pV[i].color = g_Color;
	}

	pV[0].texcoord = D3DXVECTOR2(0.0f, 0.0f);
	pV[1].texcoord = D3DXVECTOR2(1.0f, 0.0f);
	pV[2].texcoord = D3DXVECTOR2(0.0f, 1.0f);
	pV[3].texcoord = D3DXVECTOR2(1.0f, 1.0f);

	g_pVertexBuffer->Unlock();

	pDevice->SetFVF(FVF_VERTEX2D);                                     // �f�o�C�X�ɒ��_�̍\���v�f��ݒ�
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));         // �f�o�C�X�Ƀe�N�X�`����ݒ�
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex2D)); // ���_�o�b�t�@�̐ݒ�
	pDevice->SetIndices(g_pIndexBuffer);                               // �C���f�b�N�X�o�b�t�@�̐ݒ�

																	   // �v���~�e�B�u�̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

// �X�v���C�g�`��
// ���e�N�X�`���؂��蕝�A�����Ɠ����傫���̃X�v���C�g���w����W�ɕ`�悷��
void Sprite_Draw(TextureIndex texture_index, float dx, float dy, int tx, int ty, int tw, int th)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	// UV���W�v�Z
	float u[2], v[2];
	u[0] = (float)tx / w;
	v[0] = (float)ty / h;
	u[1] = (float)(tx + tw) / w;
	v[1] = (float)(ty + th) / h;

	Vertex2D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);

	pV[0].position = D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f);
	pV[1].position = D3DXVECTOR4(dx + tw - 0.5f, dy - 0.5f, 0.0f, 1.0f);
	pV[2].position = D3DXVECTOR4(dx - 0.5f, dy + th - 0.5f, 0.0f, 1.0f);
	pV[3].position = D3DXVECTOR4(dx + tw - 0.5f, dy + th - 0.5f, 0.0f, 1.0f);

	for (int i = 0; i < 4; i++) {
		pV[i].color = g_Color;
	}

	pV[0].texcoord = D3DXVECTOR2(u[0], v[0]);
	pV[1].texcoord = D3DXVECTOR2(u[1], v[0]);
	pV[2].texcoord = D3DXVECTOR2(u[0], v[1]);
	pV[3].texcoord = D3DXVECTOR2(u[1], v[1]);

	g_pVertexBuffer->Unlock();

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex2D));
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

// �X�v���C�g�`��
// ���e�N�X�`���؂��蕝�A�����Ɠ����傫���̃X�v���C�g���w����W�ɔz�u���ăX�v���C�g�̎w����W�𒆐S�Ɋg��A��]�`�悷��
void Sprite_Draw(TextureIndex texture_index, float dx, float dy, int tx, int ty, int tw, int th, float cx, float cy, float sx, float sy, float rotation)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	float u[2], v[2];
	u[0] = (float)tx / w;
	v[0] = (float)ty / h;
	u[1] = (float)(tx + tw) / w;
	v[1] = (float)(ty + th) / h;

	// ���W�ϊ�
	float px[4], py[4];
	px[0] = -cx        * sx * cosf(rotation) - -cy        * sy * sinf(rotation);
	py[0] = -cx        * sx * sinf(rotation) + -cy        * sy * cosf(rotation);
	px[1] = (-cx + tw) * sx * cosf(rotation) - -cy        * sy * sinf(rotation);
	py[1] = (-cx + tw) * sx * sinf(rotation) + -cy        * sy * cosf(rotation);
	px[2] = -cx        * sx * cosf(rotation) - (-cy + th) * sy * sinf(rotation);
	py[2] = -cx        * sx * sinf(rotation) + (-cy + th) * sy * cosf(rotation);
	px[3] = (-cx + tw) * sx * cosf(rotation) - (-cy + th) * sy * sinf(rotation);
	py[3] = (-cx + tw) * sx * sinf(rotation) + (-cy + th) * sy * cosf(rotation);

	Vertex2D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);

	pV[0].position = D3DXVECTOR4(px[0] + dx + cx - 0.5f, py[0] + dy + cy - 0.5f, 0.0f, 1.0f);
	pV[1].position = D3DXVECTOR4(px[1] + dx + cx - 0.5f, py[1] + dy + cy - 0.5f, 0.0f, 1.0f);
	pV[2].position = D3DXVECTOR4(px[2] + dx + cx - 0.5f, py[2] + dy + cy - 0.5f, 0.0f, 1.0f);
	pV[3].position = D3DXVECTOR4(px[3] + dx + cx - 0.5f, py[3] + dy + cy - 0.5f, 0.0f, 1.0f);

	for (int i = 0; i < 4; i++) {
		pV[i].color = g_Color;
	}

	pV[0].texcoord = D3DXVECTOR2(u[0], v[0]);
	pV[1].texcoord = D3DXVECTOR2(u[1], v[0]);
	pV[2].texcoord = D3DXVECTOR2(u[0], v[1]);
	pV[3].texcoord = D3DXVECTOR2(u[1], v[1]);

	g_pVertexBuffer->Unlock();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE); //���e�X�g�L���@�f�o�C�X�쐬����FALSE
	pDevice->SetRenderState(D3DRS_ALPHAREF, THRESHOLD_VALUE); // ��2�����ɂ������l�̒l������
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex2D));
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

