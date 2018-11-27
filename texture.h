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
	TEXTURE_INDEX_3,
	TEXTURE_INDEX_SWORD,
	TEXTURE_INDEX_DANGERCAGE,
	TEXTURE_INDEX_DANGER,
	TEXTURE_INDEX_FIELD,
	TEXTURE_INDEX_SKY,
	TEXTURE_INDEX_SWORD_EFFECT,   //��؂��ǉ������e�N�X�`���͂������炵��
	TEXTURE_INDEX_SPECIAL_ATTACK,
	TEXTURE_INDEX_COUNTER,
	TEXTURE_INDEX_AVOIDANCE_ATTACK,
	TEXTURE_INDEX_ZANSHIN,
	TEXTURE_INDEX_ATTACK_NORMAL,
	TEXTURE_INDEX_ATTACK_UP,
	TEXTURE_INDEX_ATTACK_DOWN,
	TEXTURE_INDEX_ATTACK_LEFT,
	TEXTURE_INDEX_ATTACK_RIGHT,
	TEXTURE_INDEX_ATTACK_LEFT_UP,
	TEXTURE_INDEX_ATTACK_RIGHT_UP,
	TEXTURE_INDEX_ATTACK_LEFT_DOWN,
	TEXTURE_INDEX_ATTACK_RIGHT_DOWN,
	TEXTURE_INDEX_COUNTER_ATTACK_LEFT,
	TEXTURE_INDEX_COUNTER_ATTACK_RIGHT,
	TEXTURE_INDEX_AVOIDANCE_NORMAL,
	TEXTURE_INDEX_AVOIDANCE_ACTIVE,
	TEXTURE_INDEX_ENEMY_ATTACK_GAGE,
	TEXTURE_INDEX_ENEMY_ATTACK_GAGE_FRAME,
	TEXTURE_INDEX_TITLE_LOGO,
	TEXTURE_INDEX_WOOD_BILL,
	TEXTURE_INDEX_TITLE_START,
	TEXTURE_INDEX_TITLE_RANKING,
	TEXTURE_INDEX_TITLE_OPTION,
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
