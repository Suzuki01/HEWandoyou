/*==============================================================================

キーボード入力とプレイヤーの作成 [main.cpp]
Author : Youhei Sato
Date   : 2018/09/05
--------------------------------------------------------------------------------

==============================================================================*/
#include <Windows.h>
#include "common.h"
#include "system_timer.h"
#include "mydirect3d.h"
#include "debug_font.h"
#include "input.h"
#include "texture.h"
#include "sprite.h"
#include "game.h"
#include "joycon.h"

/*------------------------------------------------------------------------------
定数定義
------------------------------------------------------------------------------*/
#define CLASS_NAME     "GameWindow"       // ウインドウクラスの名前
#define WINDOW_CAPTION "ゲームウィンドウ" // ウィンドウの名前

#if defined(_DEBUG) || defined(DEBUG)
#define FPS_MEASUREMENT_TIME (1.0f)       // FPS計測時間
#endif // _DEBUG || DEBUG


/*------------------------------------------------------------------------------
プロトタイプ宣言
------------------------------------------------------------------------------*/
// ウィンドウプロシージャ(コールバック関数)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ゲームの初期化関数
// 戻り値:初期化に失敗したときfalse
static bool Initialize(HINSTANCE hInstance, HWND hWnd);
// ゲームの更新関数
static void Update(void);
// ゲームの描画関数
static void Draw(void);
// ゲームの終了処理
static void Finalize(void);


/*------------------------------------------------------------------------------
グローバル変数宣言
------------------------------------------------------------------------------*/
#if defined(_DEBUG) || defined(DEBUG)
static int g_FrameCount = 0;            // フレームカウンター
static int g_FPSBaseFrameCount = 0;     // FPS計測の基となるフレームカウント
static double g_FPSBaseTime = 0.0;      // FPS計測の基となる時間
static float g_FPS = 0.0f;              // FPS
#endif // _DEBUG || DEBUG

static double g_StaticFrameTime = 0.0f; // フレーム固定用計測時間


										/*------------------------------------------------------------------------------
										関数定義
										------------------------------------------------------------------------------*/

										// メイン関数
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 使用しない一時変数を明示
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// ウィンドウクラス構造体の設定
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;                          // ウィンドウプロシージャの指定
	wc.lpszClassName = CLASS_NAME;                     // クラス名の設定
	wc.hInstance = hInstance;                          // インスタンスハンドルの指定
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);          // マウスカーソルを指定
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); // ウインドウのクライアント領域の背景色を設定

													   // クラス登録
	RegisterClass(&wc);


	// ウィンドウスタイル
	DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);

	// 基本矩形座標
	RECT window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// 指定したクライアント領域を確保するために新たな矩形座標を計算
	AdjustWindowRect(&window_rect, window_style, FALSE);

	// 新たなWindowの矩形座標から幅と高さを算出
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	// プライマリモニターの画面解像度取得
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	// デスクトップの真ん中にウィンドウが生成されるように座標を計算
	// ※ただし万が一、デスクトップよりウィンドウが大きい場合は左上に表示
	int window_x = max((desktop_width - window_width) / 2, 0);
	int window_y = max((desktop_height - window_height) / 2, 0);

	// ウィンドウの生成
	HWND hWnd = CreateWindow(
		CLASS_NAME,     // ウィンドウクラス
		WINDOW_CAPTION, // ウィンドウテキスト
		window_style,   // ウィンドウスタイル
		window_x,       // ウィンドウ座標x
		window_y,       // ウィンドウ座標y
		window_width,   // ウィンドウの幅
		window_height,  // ウィンドウの高さ
		NULL,           // 親ウィンドウハンドル
		NULL,           // メニューハンドル
		hInstance,      // インスタンスハンドル
		NULL            // 追加のアプリケーションデータ
	);

	if (hWnd == NULL) {
		// ウィンドウハンドルが何らかの理由で生成出来なかった
		return -1;
	}

	// 指定のウィンドウハンドルのウィンドウを指定の方法で表示
	ShowWindow(hWnd, nCmdShow);


	// ゲームの初期化(Direct3Dの初期化)
	if (!Initialize(hInstance, hWnd)) {
		// ゲームの初期化に失敗した
		return -1;
	}

	// Windowsゲーム用メインループ
	MSG msg = {}; // msg.message == WM_NULL
	while (WM_QUIT != msg.message) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// メッセージがある場合はメッセージ処理を優先
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// 現在のシステム時間を取得
			double time = SystemTimer_GetTime();

			if (time - g_StaticFrameTime < 1.0 / 60.0) {
				// 1 / 60 秒経っていなかったら空回り
				Sleep(0);
			}
			else {
				// フレーム固定用の計測時間を更新する
				g_StaticFrameTime = time;

				// ゲームの更新
				Update();
				// ゲームの描画
				Draw();
			}
		}
	}

	// ゲームの終了処理(Direct3Dの終了処理)
	Finalize();

	return (int)msg.wParam;
}


// ウィンドウプロシージャ(コールバック関数)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			SendMessage(hWnd, WM_CLOSE, 0, 0); // WM_CLOSEメッセージの送信
		}
		break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(hWnd); // 指定のウィンドウにWM_DESTROYメッセージを送る
		}
		return 0; // DefWindowProc関数にメッセージを流さず終了することによって何もなかったことにする

	case WM_DESTROY: // ウィンドウの破棄メッセージ
		PostQuitMessage(0); // WM_QUITメッセージの送信
		return 0;
	};

	// 通常メッセージ処理はこの関数に任せる
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


// ゲームの初期化関数
bool Initialize(HINSTANCE hInstance, HWND hWnd)
{
	// Direct3Dラッパーモジュールの初期化
	if (!MyDirect3D_Initialize(hWnd)) {
		return false;
	}

	// DirectInput版キーボードモジュールの初期化
	if (!Keyboard_Initialize(hInstance, hWnd)) {
		return false;
	}

	// テクスチャの読み込み
	if (Texture_Load() > 0) {
		MessageBox(hWnd, "いくつか読み込めなかったテクスチャファイルがあります", "エラー", MB_OK);
	}

	// システムタイマーの初期化
	SystemTimer_Initialize();

	// システムタイマーの起動
	SystemTimer_Start();

#if defined(_DEBUG) || defined(DEBUG)
	// デバックフォントモジュールの初期化
	DebugFont_Initialize();

	// FPS計測用変数群の初期化
	g_FrameCount = g_FPSBaseFrameCount = 0;
	g_FPSBaseTime = SystemTimer_GetTime();
	g_FPS = 0.0f;
#endif // _DEBUG || DEBUG

	// フレーム固定用計測時間
	g_StaticFrameTime = SystemTimer_GetTime();;

	// スプライト描画モジュールの初期化
	Sprite_Initialize();

	// ゲームの初期化
	Game_Initialize();

	Joycon_Initialize();

	return true;
}

// ゲームの更新関数
void Update(void)
{
	// キーボードモジュールの更新
	Keyboard_Update();

	// ゲームの更新
	Game_Update();

#if defined(_DEBUG) || defined(DEBUG)
	// フレームカウントの更新
	g_FrameCount++;

	// 現在のシステム時間を取得
	double time = SystemTimer_GetTime();

	// 前回のFPS計測時間から規定時間経っていたらFPS計測
	if (time - g_FPSBaseTime >= FPS_MEASUREMENT_TIME) {

		// FPS計算(前回からの経過フレーム数÷経過時間)
		g_FPS = (float)((g_FrameCount - g_FPSBaseFrameCount) / (time - g_FPSBaseTime));
		// FPS計測の基となる変数の更新
		g_FPSBaseTime = time;
		g_FPSBaseFrameCount = g_FrameCount;
	}
#endif // _DEBUG || DEBUG
}

// ゲームの描画関数
void Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	// 画面のクリア
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(100, 80, 200, 255), 1.0f, 0);

	// 描画バッチ命令の開始
	pDevice->BeginScene();

	// ゲームの描画
	Game_Draw();

#if defined(_DEBUG) || defined(DEBUG)
	// FPSのデバック表示
	DebugFont_Draw(8, 8, "FPS : %.2f", g_FPS);
#endif // _DEBUG || DEBUG

	// 描画バッチ命令の終了
	pDevice->EndScene();

	// バックバッファをフリップ（タイミングはD3DPRESENT_PARAMETERSの設定による）
	pDevice->Present(NULL, NULL, NULL, NULL);
}

// ゲームの終了処理
void Finalize(void)
{
	//Joycon_Finalize();
	// スプライト描画モジュールの終了処理
	Sprite_Finalize();

	// テクスチャの解放
	Texture_Release();

	// DirectInput版キーボードモジュールの終了処理
	Keyboard_Finalize();

#if defined(_DEBUG) || defined(DEBUG)
	// デバックフォントモジュールの終了処理
	DebugFont_Finalize();
#endif // _DEBUG || DEBUG

	// Direct3Dラッパーモジュールの終了処理
	MyDirect3D_Finalize();
}
