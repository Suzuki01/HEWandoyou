/*==============================================================================

Direct3Dラッパーモジュール [mydirect3d.h]
Author : Youhei Sato
Date   : 2018/06/16
--------------------------------------------------------------------------------

==============================================================================*/
#include <Windows.h>
#include <d3d9.h>
#include "common.h"


/*------------------------------------------------------------------------------
グローバル変数宣言
------------------------------------------------------------------------------*/
static LPDIRECT3D9 g_pD3D = NULL;             // Direct3Dインターフェース
static LPDIRECT3DDEVICE9 g_pD3DDevice = NULL; // Direct3Dデバイスインターフェース



											  /*------------------------------------------------------------------------------
											  関数定義
											  ------------------------------------------------------------------------------*/

											  // Direct3Dの初期化
bool MyDirect3D_Initialize(HWND hWnd)
{
	// Direct3Dインターフェースの取得
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL) {
		// Direct3Dインターフェースの取得に失敗
		MessageBox(hWnd, "Direct3Dインターフェースの作成に失敗しました", "エラー", MB_OK);
		return false;
	}

	// デバイスの生成に利用する情報構造体の作成
	D3DPRESENT_PARAMETERS d3dpp = {};

	d3dpp.BackBufferWidth = SCREEN_WIDTH;                       // バックバッファの横幅				
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;                     // バックバッファの縦幅
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;                    // バックバッファのフォーマット指定
	d3dpp.BackBufferCount = 1;                                  // バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;                   // スワップエフェクト（スワップ方法）の設定		
	d3dpp.Windowed = TRUE;                                      // ウィンドウモード or フルスクリーン
	d3dpp.EnableAutoDepthStencil = TRUE;                        // 深度バッファ・ステンシルバッファの使用				
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;                  // 深度バッファ・ステンシルバッファのフォーマット指定
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // フルスクリーン時のリフレッシュレートの指定
																// d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;       // リフレッシュレートとPresent処理の関係
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;       // リフレッシュレートとPresent処理の関係

																	  // Direct3Dデバイスの取得
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice))) {
		// デバイスの作成に失敗
		MessageBox(hWnd, "Direct3Dデバイスの作成に失敗しました", "エラー", MB_OK);
		return false;
	}

	// サンプラーの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// アルファブレンドの設定
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 頂点カラーとテクスチャのブレンド設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	return true;
}

// Direct3Dの終了処理
void MyDirect3D_Finalize(void)
{
	// Direct3Dデバイスの解放
	if (g_pD3DDevice) {
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3Dインターフェースの解放
	if (g_pD3D) {
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

// Direct3Dデバイスポインタの取得
LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void)
{
	return g_pD3DDevice;
}
