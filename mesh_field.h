#ifndef MESH_FIELD_H_
#define MESH_FIELD_H_

#include "texture.h"

void MeshField_Initialize(float lenth, int column, int row);				//���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�̍쐬�A����уf�[�^�̏�������
void MeshField_Finalize(void);												//���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A�e�N�X�`���̂���Ȃ�
void MeshField_Draw(TextureIndex texture_index, int column, int row);
//����DrawIndexedPrimitive
//�k�ނ𗘗p�����X�g���b�v�|���S��
//UV�A�h���b�V���O���[�hWRAP�ɂ���

#endif // MESH_FIELD_H_