#ifndef MESH_SKY_H_
#define MESH_SKY_H_

#include "texture.h"

void MeshSky_Initialize(float lenth, int radius, int height);				//���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�̍쐬�A����уf�[�^�̏�������
void MeshSky_Update(void);
void MeshSky_Finalize(void);												//���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A�e�N�X�`���̂���Ȃ�
void MeshSky_Draw(TextureIndex texture_index, int n, int height);
//����DrawIndexedPrimitive
//�k�ނ𗘗p�����X�g���b�v�|���S��
//UV�A�h���b�V���O���[�hWRAP�ɂ���

#endif // MESH_SKY_H_