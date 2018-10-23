#include <d3dx9.h>
#include "mydirect3d.h"
#include "common.h"
#include <stdio.h>


/*------------------------------------------------------------------------------
�萔��`
------------------------------------------------------------------------------*/
#define DEBUG_PRINTF_BUFFER_MAX (256) // ������p�o�b�t�@�ʁi���Ȃ��Ƃ��͑��₷���Ɓj


/*------------------------------------------------------------------------------
�O���[�o���ϐ��錾
------------------------------------------------------------------------------*/
#if defined(_DEBUG) || defined(DEBUG)
static LPD3DXFONT g_pD3DXFont = NULL;
#endif // _DEBUG || DEBUG


/*------------------------------------------------------------------------------
�֐���`
------------------------------------------------------------------------------*/

// �f�o�b�N�t�H���g���W���[���̏�����
void DebugFont_Initialize(void)
{
#if defined(_DEBUG) || defined(DEBUG)

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXCreateFont(pDevice,
		16, 0, 0, 0,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"HGP�n�p�p�߯�ߑ�",
		&g_pD3DXFont);
#endif // _DEBUG || DEBUG
}


// �f�o�b�N�t�H���g���W���[���̏I������
void DebugFont_Finalize(void)
{
#if defined(_DEBUG) || defined(DEBUG)
	if (g_pD3DXFont) {
		g_pD3DXFont->Release();
		g_pD3DXFont = NULL;
	}
#endif // _DEBUG || DEBUG
}


// �f�o�b�N�t�H���g�̕`��
void DebugFont_Draw(int x, int y, const char* pFormat, ...)
{
#if defined(_DEBUG) || defined(DEBUG)
	RECT rect = { x, y, SCREEN_WIDTH, SCREEN_HEIGHT
	};

	va_list argp;
	va_start(argp, pFormat);
	// ������p�o�b�t�@
	char buf[DEBUG_PRINTF_BUFFER_MAX];
	vsprintf_s(buf, DEBUG_PRINTF_BUFFER_MAX, pFormat, argp);
	va_end(argp);

	g_pD3DXFont->DrawText(NULL,
		buf,
		-1, &rect, DT_LEFT,
		D3DCOLOR_RGBA(0, 255, 0, 255));
#else
	UNREFERENCED_PARAMETER(pFormat);
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
#endif // _DEBUG || DEBUG
}
