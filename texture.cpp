/*==============================================================================

�e�N�X�`���Ǘ����W���[�� [texture.cpp]
Author : Youhei Sato
Date   : 2018/06/16
--------------------------------------------------------------------------------

==============================================================================*/
#include <d3dx9.h>
#include "debug_printf.h"
#include "mydirect3d.h"
#include "texture.h"


/*------------------------------------------------------------------------------
�\���̐錾
------------------------------------------------------------------------------*/
#define TEXTURE_FILENAME_MAX (64) // �e�N�X�`���t�@�C�����ő啶�����i�p�X�ANULL�����܂ށj

// �e�N�X�`���t�@�C���Ǘ��\����
typedef struct TextureFile_tag
{
	char filename[TEXTURE_FILENAME_MAX]; // �e�N�X�`���t�@�C����
	int width;  // �e�N�X�`���𑜓x����
	int height; // �e�N�X�`���𑜓x�c��
} TextureFile;


/*------------------------------------------------------------------------------
�萔��`
------------------------------------------------------------------------------*/
// �ǂݍ��݃e�N�X�`�����
static const TextureFile g_TextureFiles[] = {
	{ "asset/texture/spice_and_wolf.png", 1024, 1024 },
	{ "asset/texture/enemy.png", 1024, 1024 },
	{ "asset/texture/enemy_up.png", 1024, 1024 },
	{ "asset/texture/enemy_down.png", 1024, 1024 },
	{ "asset/texture/enemy_left.png", 1024, 1024 },
	{ "asset/texture/enemy_right.png", 1024, 1024 },
	{ "asset/texture/enemy_stan.png", 1024, 1024 },
	{ "asset/texture/enemy_slow.png", 1024, 1024 },
	{ "asset/texture/player.png", 1024, 1024 },
	{ "asset/texture/hp_cage.png", 800, 50 },
	{ "asset/texture/hp.png", 780, 30 },
	{ "asset/texture/charge_cage.png", 30, 500 },
	{ "asset/texture/charge.png", 20, 490 },
	{ "asset/texture/explo.png", 1024, 1024 },
	{ "asset/texture/1.png", 512, 512 },
	{ "asset/texture/2.png", 512, 512 },
};
// �ǂݍ��݃e�N�X�`����
static const int TEXTURE_FILE_COUNT = sizeof(g_TextureFiles) / sizeof(g_TextureFiles[0]);
// static const int TEXTURE_FILE_COUNT = ARRAYSIZE(g_TextureFiles); // required Windows.h

// �ǂݍ��݃e�N�X�`�����ƃe�N�X�`���Ǘ��ԍ��񋓐��ɍ����������ꍇ�R���p�C���G���[�Ƃ���
static_assert(TEXTURE_INDEX_MAX == TEXTURE_FILE_COUNT, "TEXTURE_INDEX_MAX != TEXTURE_FILE_COUNT");


/*------------------------------------------------------------------------------
�O���[�o���ϐ��錾
------------------------------------------------------------------------------*/
static LPDIRECT3DTEXTURE9 g_pTextures[TEXTURE_FILE_COUNT] = {}; // �e�N�X�`���C���^�[�t�F�[�X�Ǘ��z��


																/*------------------------------------------------------------------------------
																�֐���`
																------------------------------------------------------------------------------*/

																// �e�N�X�`���̓ǂݍ���
int Texture_Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) {
		return TEXTURE_FILE_COUNT;
	}

	int failed_count = 0;

	for (int i = 0; i < TEXTURE_FILE_COUNT; i++) {

		if (FAILED(D3DXCreateTextureFromFile(pDevice, g_TextureFiles[i].filename, &g_pTextures[i]))) {
			DebugPrintf("�e�N�X�`���̓ǂݍ��݂Ɏ��s ... %s\n", g_TextureFiles[i].filename);
			failed_count++;
		}
	}

	return failed_count;
}

// �e�N�X�`���̉��
void Texture_Release(void)
{
	for (int i = 0; i < TEXTURE_FILE_COUNT; i++) {

		if (g_pTextures[i]) {
			g_pTextures[i]->Release();
			g_pTextures[i] = NULL;
		}
	}
}

// �e�N�X�`���C���^�[�t�F�[�X�̎擾
LPDIRECT3DTEXTURE9 Texture_GetTexture(TextureIndex index)
{
	if (index < 0 || index >= TEXTURE_INDEX_MAX) {
		return NULL;
	}

	return g_pTextures[index];
}

// �e�N�X�`���𑜓x���̎擾
int Texture_GetWidth(TextureIndex index)
{
	if (index < 0 || index >= TEXTURE_INDEX_MAX) {
		return NULL;
	}

	return g_TextureFiles[index].width;
}

// �e�N�X�`���𑜓x�����̎擾
int Texture_GetHeight(TextureIndex index)
{
	if (index < 0 || index >= TEXTURE_INDEX_MAX) {
		return NULL;
	}

	return g_TextureFiles[index].height;
}
