/*==============================================================================

�e�N�X�`���Ǘ����W���[�� [texture.h]
Author : Youhei Sato
Date   : 2018/06/16
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef TEXTURE_H_
#define TEXTURE_H_


#include <d3d9.h>


/*------------------------------------------------------------------------------
�e�N�X�`���񋓌^
------------------------------------------------------------------------------*/
// �e�N�X�`���Ǘ��ԍ�
typedef enum
{
	TEXTURE_INDEX_SPICE_AND_WOLF,
	TEXTURE_INDEX_ENEMY,
	TEXTURE_INDEX_ENEMY_UP,
	TEXTURE_INDEX_ENEMY_DOWN,
	TEXTURE_INDEX_ENEMY_LEFT,
	TEXTURE_INDEX_ENEMY_RIGHT,
	TEXTURE_INDEX_ENEMY_STAN,
	TEXTURE_INDEX_ENEMY_SLOW,
	TEXTURE_INDEX_PLAYER,
	TEXTURE_INDEX_HPCAGE,
	TEXTURE_INDEX_HP,
	TEXTURE_INDEX_CHARGECAGE,
	TEXTURE_INDEX_CHARGE,
	TEXTURE_INDEX_EXPLO,
	TEXTURE_INDEX_1,
	TEXTURE_INDEX_2,
	TEXTURE_INDEX_TITLE,
	TEXTURE_INDEX_OPENING,
	TEXTURE_INDEX_TUTORIAL,
	TEXTURE_INDEX_BOSS_SELECT,
	TEXTURE_INDEX_RANKING,
	TEXTURE_INDEX_ENDING,
	TEXTURE_INDEX_CONTINUE,
	TEXTURE_INDEX_MAX


}TextureIndex;


/*------------------------------------------------------------------------------
�֐��̃v���g�^�C�v�錾
------------------------------------------------------------------------------*/
// �e�N�X�`���̓ǂݍ���
//
// �߂�l:�ǂݍ��߂Ȃ�������
//
int Texture_Load(void);

// �e�N�X�`���̉��
void Texture_Release(void);

// �e�N�X�`���C���^�[�t�F�[�X�̎擾
//
// �߂�l:�e�N�X�`���C���^�[�t�F�[�X
//        ���������݂��Ȃ�index���w�肵���ꍇ�ANULL
LPDIRECT3DTEXTURE9 Texture_GetTexture(TextureIndex index);

// �e�N�X�`���𑜓x���̎擾
//
// ���� ... index �e�N�X�`���Ǘ��ԍ�
//
int Texture_GetWidth(TextureIndex index);

// �e�N�X�`���𑜓x�����̎擾
//
// ���� ... index �e�N�X�`���Ǘ��ԍ�
//
int Texture_GetHeight(TextureIndex index);


#endif // TEXTURE_H_
