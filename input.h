//=============================================================================
//
// キーボード入力処理 [input.h]
//
//=============================================================================
#ifndef INPUT_H_
#define INPUT_H_


#include <Windows.h>
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>

bool Keyboard_Initialize(HINSTANCE hInstance, HWND hWnd);
void Keyboard_Finalize(void);
void Keyboard_Update(void);

bool Keyboard_IsPress(int nKey);
bool Keyboard_IsTrigger(int nKey);
bool Keyboard_IsRelease(int nKey);

#endif