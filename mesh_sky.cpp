#include "texture.h"
#include <d3dx9.h>
#include "mydirect3d.h"
#include "debug_font.h"
#include "math.h"
#include "camera.h"
#include "input.h"

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

static int g_n;
static int g_height;

static Vertex3D* pV; // ���z�A�h���X

void MeshSky_Initialize(float lenth, int n, int height)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	g_n = n;
	g_height = height;

	float radius;
	radius = (0.5f*lenth) / sinf(D3DXToRadian(180.0f / n));

	// ���_�o�b�t�@�̊m�ہi���_ (n+1) * (height+1) ���j
	pDevice->CreateVertexBuffer(sizeof(Vertex3D) * (n + 1) * (height + 1), D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_DEFAULT, &g_pVertexBuffer, NULL);
	// �C���f�b�N�X�o�b�t�@�̊m�ہi�C���f�b�N�X 2*(n+1)*height + 4*(height-1) ���j
	pDevice->CreateIndexBuffer(sizeof(WORD) * 2 * (n + 1)*height + 2 * (height - 1), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIndexBuffer, NULL);

	// ���_�C���f�b�N�X�o�b�t�@�֒��_�C���f�b�N�X�̏�������
	LPWORD pIndex; // ���z�A�h���X
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0);

	for (int i = 0; i < 2 * (n + 2); i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (i < 2 * (n + 1))
			{
				pIndex[j * 2 * (n + 2) + i] = ((i + 1) % 2 + j) * (n + 1) + i / 2;
			}
			else if (j < (height - 1))
			{
				pIndex[j * 2 * (n + 2) + 2 * (n + 1)] = pIndex[j * 2 * (n + 2) + 2 * (n + 1) - 1];
				pIndex[j * 2 * (n + 2) + 2 * (n + 1) + 1] = (j + 2) * (n + 1);
			}
		}
	}
	g_pIndexBuffer->Unlock();

	// ���_�o�b�t�@�ւ̒��_���̏�������


	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);


	for (int j = 0; j <= height; j++)						//row
	{
		for (int i = 0; i <= n; i++)					//column
		{
			//position
			pV[(n + 1)*j + i].position = D3DXVECTOR3(cosf(D3DXToRadian(360.0f / n*i))*radius, (j)*lenth, sinf(D3DXToRadian(360.0f / n*i))*radius);
			//normal
			pV[(n + 1)*j + i].normal = D3DXVECTOR3(-cosf(D3DXToRadian(360.0f / n*i)), 0.0f, -sinf(D3DXToRadian(360.0f / n*i)));
			//color
			pV[(n + 1)*j + i].color = g_Color;
			//texcoord
			//pV[(n + 1)*j + i].texcoord = D3DXVECTOR2(i, j);
			pV[(n + 1)*j + i].texcoord = D3DXVECTOR2(i*(1.0f / n), j*(1.0f / height));
		}
	}

	g_pVertexBuffer->Unlock();
}

void MeshSky_Finalize(void)
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

void MeshSky_Update(void)
{
	D3DXVECTOR3 CameraFront = Camera_Get_Front();
	D3DXVECTOR3 CameraRight = Camera_Get_Right();
	D3DXVECTOR3 f(CameraFront);
	f.y = 0.0f;
	D3DXVECTOR3 r(CameraRight);
	r.y = 0.0f;

	for (int j = 0; j <= g_height; j++)						//row
	{
		for (int i = 0; i <= g_n; i++)					//column
		{
			if (!Keyboard_IsPress(DIK_LALT) && Keyboard_IsPress(DIK_LCONTROL) && Keyboard_IsPress(DIK_W))
			{
				pV[(g_n + 1)*j + i].position += 0.03f*f;
			}
			if (!Keyboard_IsPress(DIK_LALT) && Keyboard_IsPress(DIK_LCONTROL) && Keyboard_IsPress(DIK_S))
			{
				pV[(g_n + 1)*j + i].position -= 0.03f*f;
			}
			if (!Keyboard_IsPress(DIK_LALT) && Keyboard_IsPress(DIK_LCONTROL) && Keyboard_IsPress(DIK_A))
			{
				pV[(g_n + 1)*j + i].position -= 0.03f*r;
			}
			if (!Keyboard_IsPress(DIK_LALT) && Keyboard_IsPress(DIK_LCONTROL) && Keyboard_IsPress(DIK_D))
			{
				pV[(g_n + 1)*j + i].position += 0.03f*r;
			}
		}
	}
}

void MeshSky_Draw(TextureIndex texture_index, int n, int height)
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
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, (n + 1) * (height + 1), 0, 2 * n*height + 4 * (height - 1));

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
}