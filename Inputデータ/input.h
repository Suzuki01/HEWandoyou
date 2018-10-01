//
//	input.h
//		Author:HIROMASA IKEDA	DATE:2018/09/13
//===============================================
#ifndef INPUT_H
#define INPUT_H

#include<Windows.h>
#include<d3dx9.h>
#define INIGUID
#define DIRECTINPUT_VERSION (0x0800)
#include<dinput.h>

//===============================================
//	マクロ定義
//===============================================
#define	DISABLE_GAMEPAD		//ゲームパッドを無効化
#define DISABLE_KEYBOARD	//キーボードを無効化	(未対応)
#define DISABLE_MOUSE		//マウスを無効化		(未対応)


#define	NUM_KEY_MAX			(256)	// キーの最大数

//-------------------------------------
//	ジョイスティックのキー (DS4)
//-------------------------------------
#if !defined DISABLE_GAMEPAD
#define JSK_SQUARE		(0x00)		//四角ボタン
#define JSK_CROSS		(0x01)		//バツボタン
#define JSK_CIRCLE		(0x02)		//丸ボタン
#define JSK_TRIANGLE	(0x03)		//三角ボタン
#define JSK_L1			(0x04)		//L1ボタン
#define JSK_R1			(0x05)		//R1ボタン
#define JSK_L2			(0x06)		//L2ボタン
#define JSK_R2			(0x07)		//R2ボタン
#define JSK_SHARE		(0x08)		//シェアボタン
#define JSK_OPTION		(0x09)		//オプション
#define JSK_R3			(0x0A)		//R3ボタン
#define JSK_L3			(0x0B)		//L3ボタン
#define JSK_TOUCHPAD	(0x0D)		//タッチパッド

typedef enum JoyStickArrow_Tag
{
	ARROW_UP,
	ARROW_DOWN,
	ARROW_LEFT,
	ARROW_RIGHT
}ARROW_TYPE;

typedef struct PS4ControllerData
{
	LONG	lLeftX;		//左X
	LONG	lLeftY;		//左Y
	LONG	lRightX;	//右X
	LONG	lRightY;	//右Y
	BYTE	bButtonA;	//四角
	BYTE	bButtonB;	//バツ
	BYTE	bButtonC;	//まる
	BYTE	bPadding[3];
	
}PS4ControllerData;

#endif  // !DISABLE_GAMEPAD

//===============================================
//	関数
//===============================================	
void Input_Initalize(HWND hWnd, HINSTANCE hInstance);
void Input_Update(HWND hWnd);
void Input_Finalize();

//-------------------------------------
//	デバイス情報
//-------------------------------------
#if !defined SYSTEM_H
LPDIRECT3DDEVICE9 System_GetDevice(void);	//デバイスの取得
#endif // !SYSTEM_H

//-------------------------------------
//	キーボード情報	DIK
//-------------------------------------

bool Keyboard_IsPress(int nDIK);	//キーボードを押してる
bool Keyboard_IsTrigger(int nDIK);	//キーボードを押した
bool Keyboard_IsRelease(int nDIK);	//キーボードを放した

//-------------------------------------
//	マウス情報
//-------------------------------------

bool Mouse_IsLeftDown();			//マウス左を押した
bool Mouse_IsRightDown();			//マウス右を押した
bool Mouse_IsCenterDown();			//マウスのセンター
LONG Mouse_IsAccelerationX();		//マウス加速度　X
LONG Mouse_IsAccelerationY();		//マウス加速度　Y
LONG Mouse_IsAccelerationZ();		//マウス加速度	Z(ホイール)
LONG Mouse_IsMouseX();				//マウスのX座標
LONG Mouse_IsMouseY();				//マウスのY座標

//加速度などの取得も可能

//-------------------------------------
//	ゲームパッド情報 (DS4)
//-------------------------------------
#if !defined DISABLE_GAMEPAD
LONG JoyStick_IsLeftStick_X();		//ジョイスティック左 X軸
LONG JoyStick_IsLeftStick_Y();		//ジョイスティック左 Y軸
LONG JoyStick_IsRightStick_X();		//ジョイスティック右 X軸
LONG JoyStick_IsRightStick_Y();		//ジョイスティック右 Y軸
LONG JoyStick_IsR2PressPower();		//R2の押し込み値
LONG JoyStick_IsL2PressPower();		//L2の押し込み値
bool JoyStick_IsArrowPress(ARROW_TYPE Type);	//十字キーを押してる
bool JoyStick_IsArrowTrigger(ARROW_TYPE Type);	//十字キーを押した
bool JoyStick_IsPress(int nJSK);	//キーを押してる
bool JoyStick_IsTrigger(int nJSK);	//キーを押した
bool JoyStick_IsRelease(int nJSK);	//キーを放した
#endif

#endif