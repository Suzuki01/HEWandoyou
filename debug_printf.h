/*==============================================================================

Debug_Printf������Ďg���� [debug_printf.h]
Author : Youhei Sato
Date   : 2018/4/22
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef DEBUG_PRINTF_H_
#define DEBUG_PRINTF_H_

#include <windows.h>
#include <stdio.h>


/*------------------------------------------------------------------------------
�萔��`
------------------------------------------------------------------------------*/
#define DEBUG_PRINTF_BUFFER_MAX (256) // ������p�o�b�t�@�ʁi���Ȃ��Ƃ��͑��₷���Ɓj


/*------------------------------------------------------------------------------
�֐���`(inline)
------------------------------------------------------------------------------*/

// �f�o�b�N�pprintf...VisualStudio�̏o�̓E�B���h�E�ɏo�͂����
inline void DebugPrintf(const char* pFormat, ...)
{
	// ����Ȃӂ��ɏ�����Release�r���h���̓r���h����Ȃ��͈͂�ݒ�ł���
#if defined(_DEBUG) || defined(DEBUG)
	va_list argp;
	va_start(argp, pFormat);
	// ������p�o�b�t�@
	char buf[DEBUG_PRINTF_BUFFER_MAX];
	vsprintf_s(buf, DEBUG_PRINTF_BUFFER_MAX, pFormat, argp);
	va_end(argp);
	// VisualStudio�̏o�̓E�B���h�E�֕������\��
	OutputDebugStringA(buf);
#else
	UNREFERENCED_PARAMETER(pFormat);
#endif // _DEBUG || DEBUG
}

#endif // DEBUG_PRINTF_H_
