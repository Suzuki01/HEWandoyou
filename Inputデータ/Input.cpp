//
//	Input.cpp
//		Author:HIROMASA IKEDA	DATE:2018/09/16
//===============================================
//
//	[懸念事項]
//	・ゲームパッドの更新が毎フレーム毎に行われてはいない。
//	　->CallBack関数が関係している？
//	  ->そもそものプログラムミス？
//
//-----------------------------------------------
#include<d3dx9.h>
#include"input.h"

#if !defined(DISABLE_GAMEPAD)
#define JOYSTICK_KEY_MAX (128)	//ジョイスティックのボタン配列
#define JOYSTICK_MIN (-1000)	//ジョイスティックの最小値
#define JOYSTICK_MAX (1000)		//ジョイスティックの最大値
#define JOYSTICK_JUDGE_X (900)	//ジョイスティックのX軸左右判定値
#define JOYSTICK_JUDGE_Y (900)	//ジョイスティックのY軸左右判定値
#endif //! DISABLE_GAMEPAD

//===============================================
//	グローバル変数
//===============================================
static LPDIRECTINPUT8		g_pInput		= NULL;				// DirectInputオブジェクトへのポインタ

static LPDIRECTINPUTDEVICE8	g_pDevKeyboard = NULL;				// 入力デバイス(キーボード)へのポインタ
static BYTE					g_aKeyState[NUM_KEY_MAX];			// キーボードの入力情報ワーク
static BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];	// キーボードのトリガー情報ワーク
static BYTE					g_aKeyStateRelease[NUM_KEY_MAX];	// キーボードのリリース情報ワーク

static LPDIRECTINPUTDEVICE8	g_pDevMouse		= NULL;				// 入力デバイス(マウス)
static DIMOUSESTATE2		g_MouseState2	= { NULL };			//マウスの情報
static POINT				g_MousePos		= { NULL };			//マウスの場所

#if !defined(DISABLE_GAMEPAD)
static LPDIRECTINPUTDEVICE8 g_pDevGamePad	= NULL;						//ゲームパッド
static DIDEVCAPS			g_diDevCaps		= {NULL};					//ジョイスティックの能力情報
static DIJOYSTATE2			g_PadStatus2	= {NULL};					//ステータス
static BYTE					g_aJoyKeyState2[JOYSTICK_KEY_MAX];			//ジョイスティックの入力情報
static BYTE					g_aJoyKeyState2Trigger[JOYSTICK_KEY_MAX];	//ジョイスティックのトリガー情報
static BYTE					g_aJoyKeyState2Release[JOYSTICK_KEY_MAX];	//ジョイスティックのリリース情報
static DWORD				g_PadArrow = {};							//矢印キー
static bool					g_bPadArrowTrigger[4] = {false};			//矢印キーのトリガー情報

//PS4コントローラー入力ステータス
static DIOBJECTDATAFORMAT   g_dfPS4ControllerObject[] =
{
	{ &GUID_XAxis, FIELD_OFFSET(PS4ControllerData, lLeftX),
	DIDFT_AXIS | DIDFT_ANYINSTANCE, 0, },
	{ &GUID_YAxis, FIELD_OFFSET(PS4ControllerData, lLeftY),
	DIDFT_AXIS | DIDFT_ANYINSTANCE, 0, },
	{ &GUID_ZAxis, FIELD_OFFSET(PS4ControllerData, lRightX),
	DIDFT_AXIS | DIDFT_ANYINSTANCE, 0},
	{ &GUID_RzAxis, FIELD_OFFSET(PS4ControllerData, lRightY),
	DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_Button, FIELD_OFFSET(PS4ControllerData, bButtonA),	//四角
	DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, },
	{ &GUID_Button, FIELD_OFFSET(PS4ControllerData, bButtonB),	//バツ
	DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, },
	{ &GUID_Button, FIELD_OFFSET(PS4ControllerData, bButtonC),	//まる
	DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, },
};

//PS4コントローラーデータフォーマット
static DIDATAFORMAT			g_dfPS4Controller =
{
	sizeof(DIDATAFORMAT),
	sizeof(DIOBJECTDATAFORMAT),
	DIDF_ABSAXIS,
	sizeof(PS4ControllerData),
	sizeof(g_dfPS4ControllerObject) / sizeof(g_dfPS4ControllerObject[0]),
	g_dfPS4ControllerObject,
};

#endif

//===============================================
//	関数
//===============================================

//-------------------------------------
//	ゲームパッド
//-------------------------------------
#if !defined(DISABLE_GAMEPAD)
BOOL CALLBACK EnumJoyCallBack(const DIDEVICEINSTANCE* pDidInstance, VOID* pContext);
BOOL CALLBACK AxesCallBack(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);
#endif

//===============================================
//	初期化
//===============================================
void Input_Initalize(HWND hWnd,HINSTANCE hInstance )
{

	if (g_pInput == NULL) {

		// DirectInputオブジェクトの作成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL))) {
			MessageBox(hWnd, "Inputデバイスの取得に失敗しました。", "エラー", MB_OK);
			DestroyWindow(hWnd);
			return;
		}
	}

	// 入力処理の初期化
	if (g_pInput == NULL) {
		MessageBox(hWnd, "DirectInputオブジェクトが無いです", "エラー", MB_ICONWARNING);
		DestroyWindow(hWnd);
		return;
	}

	//------------------------------------
	//	キーボード
	//------------------------------------
	// デバイスの作成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		MessageBox(hWnd, "キーボードが無い", "エラー", MB_ICONWARNING);
		DestroyWindow(hWnd);
		return;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "エラー", MB_ICONWARNING);
		DestroyWindow(hWnd);
		return;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "エラー", MB_ICONWARNING);
		DestroyWindow(hWnd);
		return;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDevKeyboard->Acquire();

	//------------------------------------
	//	マウス
	//------------------------------------

	//マウスデバイス取得
	if (FAILED(g_pInput->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{
		MessageBox(hWnd, "マウスが無いです", "エラー", MB_ICONWARNING);
		DestroyWindow(hWnd);
		return;
	}

	//マウスのデータフォーマット
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		MessageBox(hWnd, "マウスのデータフォーマットを設定できませんでした。", "エラー", MB_ICONWARNING);
		DestroyWindow(hWnd);
		return;
	}

	//マウスの協調モード
	if (FAILED(g_pDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "マウスの協調モードを設定できませんでした。", "エラー", MB_ICONWARNING);
		DestroyWindow(hWnd);
		return;
	}

	g_pDevMouse->Acquire();

	//---------------------------------
	//	ゲームパッド
	//---------------------------------
#if !defined(DISABLE_GAMEPAD)

	if(FAILED(g_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallBack, NULL, DIEDFL_ATTACHEDONLY)) || g_pDevGamePad == NULL)
	{
		MessageBox(hWnd, "ゲームパッドを取得できませんでした。。", "エラー", MB_ICONWARNING);
	}
	if (g_pDevGamePad != NULL)
	{
		if (FAILED(g_pDevGamePad->SetDataFormat(&g_dfPS4Controller)))
		{
			MessageBox(hWnd, "ゲームパッドの設定に失敗しました。", "エラー", MB_ICONWARNING);
			DestroyWindow(hWnd);
			return;
		}

		if (FAILED(g_pDevGamePad->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		{
			MessageBox(hWnd, "ゲームパッドの協調モードを設定できませんでした。", "エラー", MB_ICONWARNING);
			DestroyWindow(hWnd);
			return;
		}

		if (FAILED(g_pDevGamePad->EnumObjects(AxesCallBack, NULL, DIDFT_AXIS)))
		{
			MessageBox(hWnd, "ゲームパッドの軸の列挙に失敗しました。", "エラー", MB_ICONWARNING);
			DestroyWindow(hWnd);
			return;
		}

		g_pDevGamePad->Acquire();
	}
#endif // !DISABLE_GAMEPAD
}

//===============================================
//	入力更新
//===============================================
void Input_Update(HWND hWnd)
{
	BYTE aKeyState[NUM_KEY_MAX];	//キーステータス

	// デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		// aKeyState[DIK_SPACE] & 0x80

		for (int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			// キートリガー・リリース情報を生成
			g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];
			g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & g_aKeyState[nCnKey];

			// キープレス情報を保存
			g_aKeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		g_pDevKeyboard->Acquire();
	}

	//------------------------------------
	//	マウス情報
	//------------------------------------
	GetCursorPos(&g_MousePos);					//マウスのXY座標
	ScreenToClient(hWnd, &g_MousePos);		//ウィンドウ内座標に変換


	//マウスのステータス
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &g_MouseState2)))
	{
		//直接データの取得に成功
		int i = 0;
	}
	else
	{
		g_pDevMouse->Acquire();
	}

	//---------------------------------
	//	ゲームパッド情報
	//---------------------------------
#if !defined(DISABLE_GAMEPAD)
	DIJOYSTATE2 PadStatus;
	PS4ControllerData Ps4CData;

	if(g_pDevGamePad != NULL)
	{
		if(FAILED(g_pDevGamePad->Poll()))
		{
			HRESULT hr = g_pDevGamePad->Acquire();
			while (hr == DIERR_INPUTLOST) hr = g_pDevGamePad->Acquire();
		}

		if(SUCCEEDED(g_pDevGamePad->GetDeviceState(sizeof(PS4ControllerData),&Ps4CData)))
		{
			Ps4CData = Ps4CData;
			DIDEVICEOBJECTDATA rgdod[10];
			DWORD Item = 10;
			int i = 0;
			switch (g_pDevGamePad->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), rgdod, &Item, 0))
			{
			case DIERR_INPUTLOST:
				i = i;
				break;
			case DIERR_INVALIDPARAM:
				i = i;
				break;
			case DIERR_NOTACQUIRED:
				i = i;
				break;
			case DIERR_NOTBUFFERED:
				i = i;
				break;
			case DIERR_NOTINITIALIZED:
				i = i;
				break;
			default:
				break;
			}
		}
	}

	/*
	if (g_pDevGamePad != NULL)
	{
		if (FAILED(g_pDevGamePad->Poll()))
		{
			HRESULT hr = g_pDevGamePad->Acquire();
			while (hr == DIERR_INPUTLOST) hr = g_pDevGamePad->Acquire();
		}

		if (SUCCEEDED(g_pDevGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &PadStatus)))
		{
			for (int nCnKey = 0; nCnKey < JOYSTICK_KEY_MAX; nCnKey++)
			{
				//キートリガー・リリース情報
				g_aJoyKeyState2Trigger[nCnKey] = (g_aJoyKeyState2[nCnKey] ^ PadStatus.rgbButtons[nCnKey]) & PadStatus.rgbButtons[nCnKey];
				g_aJoyKeyState2Release[nCnKey] = (g_aJoyKeyState2[nCnKey] ^ PadStatus.rgbButtons[nCnKey]) & g_aJoyKeyState2[nCnKey];

				//キープレス情報
				g_aJoyKeyState2[nCnKey] = PadStatus.rgbButtons[nCnKey];
			}

			//十字キー
			for (int i = 0; i < 4; i++)
			{
				g_bPadArrowTrigger[i] = false;
			}

			//前の情報と違っていたら
			if (g_PadArrow != PadStatus.rgdwPOV[0])
			{
				switch (PadStatus.rgdwPOV[0])
				{
					//上
				case 0:
					g_bPadArrowTrigger[0] = true;
					break;
					//右
				case 9000:
					g_bPadArrowTrigger[1] = true;
					break;
					//下
				case 18000:
					g_bPadArrowTrigger[2] = true;
					break;
					//左
				case 27000:
					g_bPadArrowTrigger[3] = true;
					break;
				default:
					break;
				}
			}
			else
			{

			}

			g_PadArrow = g_PadStatus2.rgdwPOV[0];
			PadStatus = PadStatus;
			g_PadStatus2 = PadStatus;

		}
		else
		{
			g_pDevGamePad->Acquire();
		}
	}
	*/
#endif  // !DISABLE_GAMEPAD
	
	int i = 0;

}

//===============================================
//	終了処理
//===============================================
void Input_Finalize()
{
#if !defined(DISABLE_GAMEPAD)
	//ゲームパッド
	if(g_pDevGamePad != NULL)
	{
		g_pDevGamePad->Unacquire();
		g_pDevGamePad->Release();
		g_pDevGamePad = NULL;
	}
#endif  // !DISABLE_GAMEPAD
	//マウスデバイスの開放
	if (g_pDevMouse != NULL)
	{
		g_pDevMouse->Unacquire();
		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}

	if (g_pDevKeyboard != NULL)
	{
		// 入力デバイス(キーボード)の開放
		// キーボードへのアクセス権を開放(入力制御終了)
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// 入力処理の終了処理
	if (g_pInput != NULL) {

		// DirectInputオブジェクトの開放
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//===============================================
//	ゲームパッドの取得　CALLBACK
//===============================================
#if !defined(DISABLE_GAMEPAD)
BOOL CALLBACK EnumJoyCallBack(const DIDEVICEINSTANCE* pDidInstance, VOID* pContext)
{
	if (FAILED(g_pInput->CreateDevice(pDidInstance->guidInstance, &g_pDevGamePad, NULL)))
	{
		return DIENUM_CONTINUE;
	}

	g_diDevCaps.dwSize = sizeof(DIDEVCAPS);
	if(FAILED(g_pDevGamePad->GetCapabilities(&g_diDevCaps)))
	{
		g_pDevGamePad->Release();
		g_pDevGamePad = NULL;
		return DIENUM_CONTINUE;
	}

	//ジョイスティックの能力をチェック
	pDidInstance = pDidInstance;
	return DIENUM_STOP;

}
#endif  // !DISABLE_GAMEPAD

//===============================================
//	十字キーの設定　CALLBACK
//===============================================
#if !defined(DISABLE_GAMEPAD)
BOOL CALLBACK AxesCallBack(const DIDEVICEOBJECTINSTANCE * pdidoi, VOID* pContext)
{
	DIPROPRANGE diprg;
	ZeroMemory(&diprg,sizeof(diprg));
	diprg.diph.dwSize		= sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.diph.dwHow = DIPH_BYID;
	diprg.lMin = JOYSTICK_MIN;	//ジョイスティックの最小値(上＆左)
	diprg.lMax = JOYSTICK_MAX;	//ジョイスティックの最大値(下＆右)

	if(FAILED(g_pDevGamePad->SetProperty( DIPROP_RANGE, &diprg.diph)))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}
#endif  // !DISABLE_GAMEPAD

//===============================================
//	キーボード情報
//===============================================

//-------------------------------------
//	キーボードを押してる
//-------------------------------------
bool Keyboard_IsPress(int nDIK)
{
	return (g_aKeyState[nDIK] & 0x80) ? true : false;
}

//-------------------------------------
//	キーボードを押した
//-------------------------------------
bool Keyboard_IsTrigger(int nDIK)
{
	return (g_aKeyStateTrigger[nDIK] & 0x80) ? true : false;
}

//-------------------------------------
//	キーボードを放した
//-------------------------------------
bool Keyboard_IsRelease(int nDIK)
{
	return (g_aKeyStateRelease[nDIK] & 0x80) ? true : false;
}

//===============================================
//	マウス情報
//===============================================

//-------------------------------------
//	左クリック
//-------------------------------------
bool Mouse_IsLeftDown()
{
	return (g_MouseState2.rgbButtons[0] & 0x80) ? true : false;
}

//-------------------------------------
//	右クリック
//-------------------------------------
bool Mouse_IsRightDown()
{
	return (g_MouseState2.rgbButtons[1] & 0x80) ? true : false;
}

//-------------------------------------
//	ホイールクリック
//-------------------------------------
bool Mouse_IsCenterDown()
{
	return (g_MouseState2.rgbButtons[2] & 0x80) ? true : false;
}

//-------------------------------------
//	マウス加速度　X
//-------------------------------------
LONG Mouse_IsAccelerationX()
{
	return g_MouseState2.lX;
}

//-------------------------------------
//	マウス加速度　Y
//-------------------------------------
LONG Mouse_IsAccelerationY()
{
	return g_MouseState2.lY;
}

//-------------------------------------
//	マウス加速度　Z　(スクロール)
//-------------------------------------
LONG Mouse_IsAccelerationZ()
{
	return g_MouseState2.lZ;
}

//-------------------------------------
//	マウスX座標
//-------------------------------------
LONG Mouse_IsMouseX()
{
	return g_MousePos.x;
}

//-------------------------------------
//	マウスY座標
//-------------------------------------
LONG Mouse_IsMouseY()
{
	return g_MousePos.y;
}

//===============================================
//	ゲームパッド
//===============================================
#if !defined(DISABLE_GAMEPAD)
//-------------------------------------
//	ゲームパッド　ジョイスティック左X値
//-------------------------------------
LONG JoyStick_IsLeftStick_X()
{
	return g_PadStatus2.lX;
}

//-------------------------------------
//	ゲームパッド　ジョイスティック左Y値
//-------------------------------------
LONG JoyStick_IsLeftStick_Y()
{
	return g_PadStatus2.lY;
}

//-------------------------------------
//	ゲームパッド　ジョイスティック右X値
//-------------------------------------
LONG JoyStick_IsRightStick_X()
{
	return g_PadStatus2.lZ;
}

//-------------------------------------
//	ゲームパッド　ジョイスティック右Y値
//-------------------------------------
LONG JoyStick_IsRightStick_Y()
{
	return g_PadStatus2.lRz;
}

//-------------------------------------
//	ゲームパッド　R2の押し込み値
//-------------------------------------
LONG JoyStick_IsR2PressPower()
{
	return g_PadStatus2.lRx;
}

//-------------------------------------
//	ゲームパッド　L2の押し込み値
//-------------------------------------
LONG JoyStick_IsL2PressPower()
{
	return g_PadStatus2.lRy;
}

//-------------------------------------
//	十字を押してる
//-------------------------------------
bool JoyStick_IsArrowPress(ARROW_TYPE Type) 
{
	switch (Type)
	{
	case ARROW_UP:
		return g_PadStatus2.rgdwPOV[0] == 0;
		break;
	case ARROW_LEFT:
		return g_PadStatus2.rgdwPOV[0] == 9000;
		break;
	case ARROW_DOWN:
		return g_PadStatus2.rgdwPOV[0] == 18000;
		break;
	case ARROW_RIGHT:
		return g_PadStatus2.rgdwPOV[0] == 27000;
		break;
	default:
		return false;
		break;
	}

	return false;
}

//-------------------------------------
//	十字を押した
//-------------------------------------
bool JoyStick_IsArrowTrigger(ARROW_TYPE Type)
{
	switch (Type)
	{
	case ARROW_UP:
		return g_bPadArrowTrigger[0];
		break;
	case ARROW_DOWN:
		return g_bPadArrowTrigger[2];
		break;
	case ARROW_LEFT:
		return g_bPadArrowTrigger[1];
		break;
	case ARROW_RIGHT:
		return g_bPadArrowTrigger[3];
		break;
	default:
		return false;
		break;
	}

	return false;
}

//-------------------------------------
//	キーを押してる
//-------------------------------------
bool JoyStick_IsPress(int nJSK)
{
	return (g_aJoyKeyState2[nJSK] & 0x80) ? true : false;
}

//-------------------------------------
//	キーを押した
//-------------------------------------
bool JoyStick_IsTrigger(int nJSK)
{
	return (g_aJoyKeyState2Trigger[nJSK] & 0x80) ? true : false;
}

//-------------------------------------
//	キーを放した
//-------------------------------------
bool JoyStick_IsRelease(int nJSK)
{
	return (g_aJoyKeyState2Release[nJSK] & 0x80) ? true : false;
}

#endif  // !DISABLE_GAMEPAD


