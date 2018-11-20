#include <d3dx9.h>
#include <math.h>
#include "mydirect3d.h"
#include "texture.h"
#include "input.h"
#include "cube.h"
#include "common.h"
#include "light.h"
#include "camera.h"
#include "debug_font.h"
#include <time.h>

#define	FRICTION	0.96f//���C��
#define PI  (3.1415926f)
//�@���_�\����
typedef struct Vertex3D_tag
{
	D3DXVECTOR3 position; // 3 xyz rhw <----���ꂪ����Ɓ@���W�ϊ��ϒ��_
	D3DXVECTOR3 normal;
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;

}Vertex3D;

static float x = 0;// front_bottom_left.x;
static float y = 0;// front_bottom_left.y;
static float z = 0;// front_bottom_left.z;

//�AFVF�̒�`
#define FVF_VERTEX3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1) // 3D�|���S�����_�t�H�[�}�b�g

// �B���_�f�[�^(cube)  draw
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static D3DCOLOR g_Color = 0xffffffff;
static const int kNumFaces_ = 6;			// The number of faces in a cube, this is always 6.
static LPDIRECT3DINDEXBUFFER9  pIB[kNumFaces_];
float length_;


static D3DXVECTOR3 cube1;
static D3DXVECTOR3 cube2;
static float cube2_x, cube2_y, cube2_z;
static float cube1_X, cube1_Y, cube1_Z; //cube1�̈ʒu
static float cube1_VX, cube1_VY, cube1_VZ;  //cube1 �̑��x
static float prev_cube1_X, prev_cube1_Y, prev_cube1_Z; //�O��cube1�̈ʒu
static float cube2_speed_x;
static float cube2_speed_y;
static float cube2_speed_z;





void Cube_Follow_Init()
{

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	cube1 = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	cube2 = D3DXVECTOR3(-0.5f, 1.5f, -0.5f);

	cube1.x = prev_cube1_X = -0.5f;
	cube1.y = prev_cube1_Y = -0.5f;
	cube1.z = prev_cube1_Z = -0.5f;

	cube2_speed_x = 0.01f;
	cube2_speed_y = 0.01f;
	cube2_speed_z = 0.01f;

	//random seed
	srand((unsigned int)time(NULL));  //�V�X�e�����ԏ�����

	length_ = 1.0f;

	// ���_�o�b�t�@�̊m�ہi���_2�S���j
	pDevice->CreateVertexBuffer(
		sizeof(Vertex3D) * 24,	//���_�� * �T�C�Y
		0,
		D3DFVF_XYZ,
		D3DPOOL_MANAGED,  //D3DPOOL_DEFAULT
		&g_pVertexBuffer,
		0);


	// ���_�C���f�b�N�X�o�b�t�@�֒��_�C���f�b�N�X�̏�������
	// Indices for triangle strips
	WORD indicesFront[] = { 0,  1,  3,  2 };
	WORD indicesBack[] = { 4,  5,  7,  6 };
	WORD indicesLeft[] = { 8,  9, 11, 10 };
	WORD indicesRight[] = { 12, 13, 15, 14 };
	WORD indicesTop[] = { 16, 17, 19, 18 };
	WORD indicesBottom[] = { 20, 21, 23, 22 };

	WORD* indices[kNumFaces_] = { indicesFront, indicesBack, indicesLeft, indicesRight, indicesTop, indicesBottom };

	for (int i = 0; i < kNumFaces_; ++i)
	{
		// Only create index buffer once, prevent high memory usage when user press 'R' frequently, see comments in InitVertexBuffer.
		if (pIB[i] == NULL)
		{
			if (FAILED(pDevice->CreateIndexBuffer(sizeof(indicesFront) * sizeof(WORD),//���_�� * �T�C�Y
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,//D3DPOOL_DEFAULT
				&pIB[i],
				0)))
			{
				MessageBox(NULL, "Create index buffer failed", "Error", 0);
			}
		}

		// Copy index data
		VOID *pIndices;
		if (FAILED(pIB[i]->Lock(0, sizeof(indicesFront), (void **)&pIndices, 0)))
			MessageBox(NULL, "Copy index buffer data failed", "Error", 0);
		memcpy(pIndices, indices[i], sizeof(indicesFront));
		pIB[i]->Unlock();
	}


	/* Example of front face
1               2
 ---------------
 |             |
 |             |
 |             |
 |             |
 |             |
 ---------------
0               3
 */
 // Vertex buffer data
	Vertex3D vertices[] =
	{
		// Front face
		{D3DXVECTOR3(x,           y,           z),  D3DXVECTOR3(0.0f,  0.0f, -1.0f),D3DCOLOR_RGBA(200,50,0,255),D3DXVECTOR2(0.0f, 0.0f)}, // 0
		{D3DXVECTOR3(x, y + length_,           z),  D3DXVECTOR3(0.0f,  0.0f, -1.0f),D3DCOLOR_RGBA(200,50,0,255),D3DXVECTOR2(1.0f / 4.0f, 0.0f)}, // 1
		{D3DXVECTOR3(x + length_, y + length_,           z),  D3DXVECTOR3(0.0f,  0.0f, -1.0f),D3DCOLOR_RGBA(200,50,0,255),D3DXVECTOR2(1.0f / 4.0f, 1.0f / 4.0f)}, // 2
		{D3DXVECTOR3(x + length_,           y,           z),  D3DXVECTOR3(0.0f,  0.0f, -1.0f),D3DCOLOR_RGBA(200,50,0,255),D3DXVECTOR2(0.0f, 1.0f / 4.0f)}, // 3

		// Back face
		{D3DXVECTOR3(x + length_,           y, z + length_),   D3DXVECTOR3(0.0f,  0.0f,  1.0f),D3DCOLOR_RGBA(255,255,0,255),D3DXVECTOR2(0.0f, 0.0f)}, // 4
		{D3DXVECTOR3(x + length_, y + length_, z + length_),   D3DXVECTOR3(0.0f,  0.0f,  1.0f),D3DCOLOR_RGBA(255,255,0,255),D3DXVECTOR2(1.0f, 0.0f)}, // 5
		{D3DXVECTOR3(x, y + length_, z + length_),   D3DXVECTOR3(0.0f,  0.0f,  1.0f),D3DCOLOR_RGBA(255,255,0,255),D3DXVECTOR2(1.0f, 1.0f)}, // 6
		{D3DXVECTOR3(x,           y, z + length_),   D3DXVECTOR3(0.0f,  0.0f,  1.0f),D3DCOLOR_RGBA(255,255,0,255),D3DXVECTOR2(0.0f, 1.0f)}, // 7

		// Left face
		{D3DXVECTOR3(x,           y, z + length_), D3DXVECTOR3(-1.0f,  0.0f,  0.0f),D3DCOLOR_RGBA(0,0,255,255), D3DXVECTOR2(0.0f, 0.0f)}, // 8
		{D3DXVECTOR3(x, y + length_, z + length_), D3DXVECTOR3(-1.0f,  0.0f,  0.0f),D3DCOLOR_RGBA(0,0,255,255), D3DXVECTOR2(1.0f, 0.0f)}, // 9
		{D3DXVECTOR3(x, y + length_,           z), D3DXVECTOR3(-1.0f,  0.0f,  0.0f),D3DCOLOR_RGBA(0,0,255,255),  D3DXVECTOR2(1.0f, 1.0f)}, // 10
		{D3DXVECTOR3(x,           y,           z), D3DXVECTOR3(-1.0f,  0.0f,  0.0f),D3DCOLOR_RGBA(0,0,255,255),  D3DXVECTOR2(0.0f, 1.0f)}, // 11

		// Right face 
		{D3DXVECTOR3(x + length_,           y,           z), D3DXVECTOR3(1.0f,  0.0f,  0.0f),D3DCOLOR_RGBA(255,0,255,255),D3DXVECTOR2(0.0f, 0.0f)}, // 12
		{D3DXVECTOR3(x + length_, y + length_,           z), D3DXVECTOR3(1.0f,  0.0f,  0.0f),D3DCOLOR_RGBA(255,0,255,255),D3DXVECTOR2(1.0f, 0.0f)}, // 13
		{D3DXVECTOR3(x + length_, y + length_, z + length_), D3DXVECTOR3(1.0f,  0.0f,  0.0f),D3DCOLOR_RGBA(255,0,255,255),D3DXVECTOR2(1.0f, 1.0f)}, // 14
		{D3DXVECTOR3(x + length_,           y, z + length_), D3DXVECTOR3(1.0f,  0.0f,  0.0f),D3DCOLOR_RGBA(255,0,255,255),D3DXVECTOR2(0.0f, 1.0f)}, // 15

		// Top face
		{D3DXVECTOR3(x, y + length_,           z), D3DXVECTOR3(0.0f,  1.0f,  0.0f),D3DCOLOR_RGBA(0,255,0,255),D3DXVECTOR2(0.0f, 0.0f)}, // 16
		{D3DXVECTOR3(x, y + length_, z + length_), D3DXVECTOR3(0.0f,  1.0f,  0.0f),D3DCOLOR_RGBA(0,255,0,255),D3DXVECTOR2(1.0f, 0.0f)}, // 17
		{D3DXVECTOR3(x + length_, y + length_, z + length_), D3DXVECTOR3(0.0f,  1.0f,  0.0f),D3DCOLOR_RGBA(0,255,0,255),D3DXVECTOR2(1.0f, 1.0f)}, // 18
		{D3DXVECTOR3(x + length_, y + length_,           z), D3DXVECTOR3(0.0f,  1.0f,  0.0f),D3DCOLOR_RGBA(0,255,0,255),D3DXVECTOR2(0.0f, 1.0f)}, // 19

		// Bottom face
		{D3DXVECTOR3(x + length_,           y,           z), D3DXVECTOR3(0.0f, -1.0f,  0.0f),D3DCOLOR_RGBA(50,0,0,255),D3DXVECTOR2(0.0f, 0.0f)}, // 20
		{D3DXVECTOR3(x + length_,           y, z + length_), D3DXVECTOR3(0.0f, -1.0f,  0.0f),D3DCOLOR_RGBA(50,0,0,255),D3DXVECTOR2(1.0f, 0.0f)}, // 21
		{D3DXVECTOR3(x,           y, z + length_), D3DXVECTOR3(0.0f, -1.0f,  0.0f),D3DCOLOR_RGBA(50,0,0,255),D3DXVECTOR2(1.0f, 1.0f)}, // 22
		{D3DXVECTOR3(x,           y,           z), D3DXVECTOR3(0.0f, -1.0f,  0.0f),D3DCOLOR_RGBA(50,0,0,255),D3DXVECTOR2(0.0f, 1.0f)}, // 23
	};


	if (g_pVertexBuffer == NULL)
	{
		// Create vertex buffer
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(vertices) * sizeof(Vertex3D),
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX3D,
			D3DPOOL_MANAGED,
			&g_pVertexBuffer,
			NULL)))
		{
			MessageBox(NULL, "Create vertex buffer failed", "Error", 0);
		}
	}

	// Copy vertex data
	VOID* pVertices;
	if (FAILED(g_pVertexBuffer->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		MessageBox(NULL, "Copy vertex buffer failed", "Error", 0);
	memcpy(pVertices, vertices, sizeof(vertices));
	g_pVertexBuffer->Unlock();
}
void Cube_Follow_Fin()
{	// �C���f�b�N�X�o�b�t�@�̉��
	for (int i = 0; i < kNumFaces_; ++i)
	{
		if (pIB[i] != NULL)
		{
			pIB[i]->Release();
			pIB[i] != NULL;
		}
	}

	//���_�o�b�t�@�̉��

	if (g_pVertexBuffer) {
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}

}
void Cube_Follow_Update()
{
	//cube1.x = get_enemy.x()...  
	//cube1.y = get_enemy.y()...  
	//cube1.z = get_enemy.z()...  

	cube1_VX = cube1.x - prev_cube1_X;
	cube1_VY = cube1.y - prev_cube1_Y;
	cube1_VZ = cube1.z - prev_cube1_Z;

	prev_cube1_X = cube1.x;
	prev_cube1_Y = cube1.y;
	prev_cube1_Z = cube1.z;

	float stirDist = 100.0f;		//�͈͓���cube2��cube1�ֈړ�
	float toDist = 100.0f;         //�@�͈͓�cube1�̑��x��cube2���e������

	float cube2_X = cube2.x;
	float cube2_Y = cube2.y;
	float cube2_Z = cube2.z;

	float cube2_vX = cube2_speed_x;
	float cube2_vY = cube2_speed_y;
	float cube2_vZ = cube2_speed_z;

	float distance_x = cube2_X - cube1.x;//length
	float distance_y = cube2_Y - cube1.y;
	float distance_z = cube2_Z - cube1.z;
	float distance = (float)sqrt(distance_x * distance_x + distance_y * distance_y + distance_z * distance_z);

	distance_x = distance ? distance_x / distance : 0;//�@if dX = d �͐^�@�@dx/d=1
	distance_y = distance ? distance_y / distance : 0;//�@if dY = d �͐^�@�@dy/d=1
	distance_z = distance ? distance_z / distance : 0;//�@if dZ = d �͐^�@�@dz/d=1

	if (distance < toDist)
	{
		float toAcc = (1 - (distance / toDist))  * 0.15f;//�͈͓���cube2��cube1�ֈړ�
		cube2_vX -= distance_x * toAcc;
		cube2_vY -= distance_y * toAcc;
		cube2_vZ -= distance_z * toAcc;
		if (distance <= 0.1f)
		{
			cube2_vX = 0;
			cube2_vY = 0;
			cube2_vZ = 0;
		}
	}

	if (distance < stirDist)
	{
		float mAcc = (1 - (distance / stirDist)) * 0.35f; //�͈͓�cube1�̑��x��cube2���e������
		cube2_vX += cube1_VX * mAcc;
		cube2_vY += cube1_VY * mAcc;
		cube2_vZ += cube1_VZ * mAcc;
	}

	cube2_speed_x *= FRICTION;
	cube2_speed_y *= FRICTION;//���C��
	cube2_speed_z *= FRICTION;


	float next_cube2_x = cube2.x + cube2_vX;//���̈ʒu
	float next_cube2_y = cube2.y + cube2_vY;
	float next_cube2_z = cube2.z + cube2_vZ;

	cube2.x = next_cube2_x;
	cube2.y = next_cube2_y;
	cube2.z = next_cube2_z;
}

void Cube_Follow_Draw()
{


	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	// �C���[���h���W�ϊ��ϊ��s��쐬�Ɛݒ�  

	D3DXMATRIX mtxTranslation;

	D3DXMATRIX mtxWorld;

	//�s��̍���
	D3DXMatrixIdentity(&mtxWorld);//�P�ʍs������
	//�g��k��

	for (int i = 0; i < kNumFaces_; ++i)
	{
		pDevice->SetFVF(FVF_VERTEX3D);
	//	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_TEX));
		//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex3D));
		pDevice->SetIndices(pIB[i]);
		x += 1;
		//cube1
		//D3DXMatrixRotationAxis(&mtxRotation, &rotationAixs, angle);
		D3DXMatrixTranslation(&mtxTranslation, cube1.x, cube1.y, cube1.z);
		DebugFont_Draw(100, 100, "cube1.x=%f\ncube1.y=%f\ncube1.x=%f\n", cube1.x, cube1.y, cube1.z);
		mtxWorld = mtxTranslation;
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 24, 0, 2);

		//cube2
		//D3DXMatrixTranslation(&mtxTranslation, cube2.x, cube2.y, cube2.z);
		//DebugFont_Draw(800, 100, "cube2.x=%f\ncube2.y=%f\ncube2.x=%f\n", cube2.x, cube2.y, cube2.z);
		//mtxWorld = mtxTranslation;
		//pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
		//pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 24, 0, 2);
	}
}

D3DXVECTOR3 Get_Cube2()
{
	return cube2;
}



