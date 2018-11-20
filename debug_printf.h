/*==============================================================================

Debug_Printfを作って使おう [debug_printf.h]
Author : Youhei Sato
Date   : 2018/4/22
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef DEBUG_PRINTF_H_
#define DEBUG_PRINTF_H_

#include <windows.h>
#include <stdio.h>


/*------------------------------------------------------------------------------
定数定義
------------------------------------------------------------------------------*/
#define DEBUG_PRINTF_BUFFER_MAX (256) // 文字列用バッファ量（少ないときは増やすこと）


/*------------------------------------------------------------------------------
関数定義(inline)
------------------------------------------------------------------------------*/

// デバック用printf...VisualStudioの出力ウィンドウに出力される
inline void DebugPrintf(const char* pFormat, ...)
{
	// こんなふうに書くとReleaseビルド時はビルドされない範囲を設定できる
#if defined(_DEBUG) || defined(DEBUG)
	va_list argp;
	va_start(argp, pFormat);
	// 文字列用バッファ
	char buf[DEBUG_PRINTF_BUFFER_MAX];
	vsprintf_s(buf, DEBUG_PRINTF_BUFFER_MAX, pFormat, argp);
	va_end(argp);
	// VisualStudioの出力ウィンドウへ文字列を表示
	OutputDebugStringA(buf);
#else
	UNREFERENCED_PARAMETER(pFormat);
#endif // _DEBUG || DEBUG
}

#endif // DEBUG_PRINTF_H_
