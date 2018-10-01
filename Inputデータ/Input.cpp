//
//	Input.cpp
//		Author:HIROMASA IKEDA	DATE:2018/09/16
//===============================================
//
//	[���O����]
//	�E�Q�[���p�b�h�̍X�V�����t���[�����ɍs���Ă͂��Ȃ��B
//	�@->CallBack�֐����֌W���Ă���H
//	  ->���������̃v���O�����~�X�H
//
//-----------------------------------------------
#include<d3dx9.h>
#include"input.h"

#if !defined(DISABLE_GAMEPAD)
#define JOYSTICK_KEY_MAX (128)	//�W���C�X�e�B�b�N�̃{�^���z��
#define JOYSTICK_MIN (-1000)	//�W���C�X�e�B�b�N�̍ŏ��l
#define JOYSTICK_MAX (1000)		//�W���C�X�e�B�b�N�̍ő�l
#define JOYSTICK_JUDGE_X (900)	//�W���C�X�e�B�b�N��X�����E����l
#define JOYSTICK_JUDGE_Y (900)	//�W���C�X�e�B�b�N��Y�����E����l
#endif //! DISABLE_GAMEPAD

//===============================================
//	�O���[�o���ϐ�
//===============================================
static LPDIRECTINPUT8		g_pInput		= NULL;				// DirectInput�I�u�W�F�N�g�ւ̃|�C���^

static LPDIRECTINPUTDEVICE8	g_pDevKeyboard = NULL;				// ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
static BYTE					g_aKeyState[NUM_KEY_MAX];			// �L�[�{�[�h�̓��͏�񃏁[�N
static BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];	// �L�[�{�[�h�̃g���K�[��񃏁[�N
static BYTE					g_aKeyStateRelease[NUM_KEY_MAX];	// �L�[�{�[�h�̃����[�X��񃏁[�N

static LPDIRECTINPUTDEVICE8	g_pDevMouse		= NULL;				// ���̓f�o�C�X(�}�E�X)
static DIMOUSESTATE2		g_MouseState2	= { NULL };			//�}�E�X�̏��
static POINT				g_MousePos		= { NULL };			//�}�E�X�̏ꏊ

#if !defined(DISABLE_GAMEPAD)
static LPDIRECTINPUTDEVICE8 g_pDevGamePad	= NULL;						//�Q�[���p�b�h
static DIDEVCAPS			g_diDevCaps		= {NULL};					//�W���C�X�e�B�b�N�̔\�͏��
static DIJOYSTATE2			g_PadStatus2	= {NULL};					//�X�e�[�^�X
static BYTE					g_aJoyKeyState2[JOYSTICK_KEY_MAX];			//�W���C�X�e�B�b�N�̓��͏��
static BYTE					g_aJoyKeyState2Trigger[JOYSTICK_KEY_MAX];	//�W���C�X�e�B�b�N�̃g���K�[���
static BYTE					g_aJoyKeyState2Release[JOYSTICK_KEY_MAX];	//�W���C�X�e�B�b�N�̃����[�X���
static DWORD				g_PadArrow = {};							//���L�[
static bool					g_bPadArrowTrigger[4] = {false};			//���L�[�̃g���K�[���

//PS4�R���g���[���[���̓X�e�[�^�X
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
	{ &GUID_Button, FIELD_OFFSET(PS4ControllerData, bButtonA),	//�l�p
	DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, },
	{ &GUID_Button, FIELD_OFFSET(PS4ControllerData, bButtonB),	//�o�c
	DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, },
	{ &GUID_Button, FIELD_OFFSET(PS4ControllerData, bButtonC),	//�܂�
	DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, },
};

//PS4�R���g���[���[�f�[�^�t�H�[�}�b�g
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
//	�֐�
//===============================================

//-------------------------------------
//	�Q�[���p�b�h
//-------------------------------------
#if !defined(DISABLE_GAMEPAD)
BOOL CALLBACK EnumJoyCallBack(const DIDEVICEINSTANCE* pDidInstance, VOID* pContext);
BOOL CALLBACK AxesCallBack(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);
#endif

//===============================================
//	������
//===============================================
void Input_Initalize(HWND hWnd,HINSTANCE hInstance )
{

	if (g_pInput == NULL) {

		// DirectInput�I�u�W�F�N�g�̍쐬
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL))) {
			MessageBox(hWnd, "Input�f�o�C�X�̎擾�Ɏ��s���܂����B", "�G���[", MB_OK);
			DestroyWindow(hWnd);
			return;
		}
	}

	// ���͏����̏�����
	if (g_pInput == NULL) {
		MessageBox(hWnd, "DirectInput�I�u�W�F�N�g�������ł�", "�G���[", MB_ICONWARNING);
		DestroyWindow(hWnd);
		return;
	}

	//------------------------------------
	//	�L�[�{�[�h
	//------------------------------------
	// �f�o�C�X�̍쐬
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		MessageBox(hWnd, "�L�[�{�[�h������", "�G���[", MB_ICONWARNING);
		DestroyWindow(hWnd);
		return;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�G���[", MB_ICONWARNING);
		DestroyWindow(hWnd);
		return;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�G���[", MB_ICONWARNING);
		DestroyWindow(hWnd);
		return;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDevKeyboard->Acquire();

	//------------------------------------
	//	�}�E�X
	//------------------------------------

	//�}�E�X�f�o�C�X�擾
	if (FAILED(g_pInput->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{
		MessageBox(hWnd, "�}�E�X�������ł�", "�G���[", MB_ICONWARNING);
		DestroyWindow(hWnd);
		return;
	}

	//�}�E�X�̃f�[�^�t�H�[�}�b�g
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		MessageBox(hWnd, "�}�E�X�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�G���[", MB_ICONWARNING);
		DestroyWindow(hWnd);
		return;
	}

	//�}�E�X�̋������[�h
	if (FAILED(g_pDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "�}�E�X�̋������[�h��ݒ�ł��܂���ł����B", "�G���[", MB_ICONWARNING);
		DestroyWindow(hWnd);
		return;
	}

	g_pDevMouse->Acquire();

	//---------------------------------
	//	�Q�[���p�b�h
	//---------------------------------
#if !defined(DISABLE_GAMEPAD)

	if(FAILED(g_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallBack, NULL, DIEDFL_ATTACHEDONLY)) || g_pDevGamePad == NULL)
	{
		MessageBox(hWnd, "�Q�[���p�b�h���擾�ł��܂���ł����B�B", "�G���[", MB_ICONWARNING);
	}
	if (g_pDevGamePad != NULL)
	{
		if (FAILED(g_pDevGamePad->SetDataFormat(&g_dfPS4Controller)))
		{
			MessageBox(hWnd, "�Q�[���p�b�h�̐ݒ�Ɏ��s���܂����B", "�G���[", MB_ICONWARNING);
			DestroyWindow(hWnd);
			return;
		}

		if (FAILED(g_pDevGamePad->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		{
			MessageBox(hWnd, "�Q�[���p�b�h�̋������[�h��ݒ�ł��܂���ł����B", "�G���[", MB_ICONWARNING);
			DestroyWindow(hWnd);
			return;
		}

		if (FAILED(g_pDevGamePad->EnumObjects(AxesCallBack, NULL, DIDFT_AXIS)))
		{
			MessageBox(hWnd, "�Q�[���p�b�h�̎��̗񋓂Ɏ��s���܂����B", "�G���[", MB_ICONWARNING);
			DestroyWindow(hWnd);
			return;
		}

		g_pDevGamePad->Acquire();
	}
#endif // !DISABLE_GAMEPAD
}

//===============================================
//	���͍X�V
//===============================================
void Input_Update(HWND hWnd)
{
	BYTE aKeyState[NUM_KEY_MAX];	//�L�[�X�e�[�^�X

	// �f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		// aKeyState[DIK_SPACE] & 0x80

		for (int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			// �L�[�g���K�[�E�����[�X���𐶐�
			g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];
			g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & g_aKeyState[nCnKey];

			// �L�[�v���X����ۑ�
			g_aKeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		g_pDevKeyboard->Acquire();
	}

	//------------------------------------
	//	�}�E�X���
	//------------------------------------
	GetCursorPos(&g_MousePos);					//�}�E�X��XY���W
	ScreenToClient(hWnd, &g_MousePos);		//�E�B���h�E�����W�ɕϊ�


	//�}�E�X�̃X�e�[�^�X
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &g_MouseState2)))
	{
		//���ڃf�[�^�̎擾�ɐ���
		int i = 0;
	}
	else
	{
		g_pDevMouse->Acquire();
	}

	//---------------------------------
	//	�Q�[���p�b�h���
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
				//�L�[�g���K�[�E�����[�X���
				g_aJoyKeyState2Trigger[nCnKey] = (g_aJoyKeyState2[nCnKey] ^ PadStatus.rgbButtons[nCnKey]) & PadStatus.rgbButtons[nCnKey];
				g_aJoyKeyState2Release[nCnKey] = (g_aJoyKeyState2[nCnKey] ^ PadStatus.rgbButtons[nCnKey]) & g_aJoyKeyState2[nCnKey];

				//�L�[�v���X���
				g_aJoyKeyState2[nCnKey] = PadStatus.rgbButtons[nCnKey];
			}

			//�\���L�[
			for (int i = 0; i < 4; i++)
			{
				g_bPadArrowTrigger[i] = false;
			}

			//�O�̏��ƈ���Ă�����
			if (g_PadArrow != PadStatus.rgdwPOV[0])
			{
				switch (PadStatus.rgdwPOV[0])
				{
					//��
				case 0:
					g_bPadArrowTrigger[0] = true;
					break;
					//�E
				case 9000:
					g_bPadArrowTrigger[1] = true;
					break;
					//��
				case 18000:
					g_bPadArrowTrigger[2] = true;
					break;
					//��
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
//	�I������
//===============================================
void Input_Finalize()
{
#if !defined(DISABLE_GAMEPAD)
	//�Q�[���p�b�h
	if(g_pDevGamePad != NULL)
	{
		g_pDevGamePad->Unacquire();
		g_pDevGamePad->Release();
		g_pDevGamePad = NULL;
	}
#endif  // !DISABLE_GAMEPAD
	//�}�E�X�f�o�C�X�̊J��
	if (g_pDevMouse != NULL)
	{
		g_pDevMouse->Unacquire();
		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}

	if (g_pDevKeyboard != NULL)
	{
		// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
		// �L�[�{�[�h�ւ̃A�N�Z�X�����J��(���͐���I��)
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// ���͏����̏I������
	if (g_pInput != NULL) {

		// DirectInput�I�u�W�F�N�g�̊J��
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//===============================================
//	�Q�[���p�b�h�̎擾�@CALLBACK
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

	//�W���C�X�e�B�b�N�̔\�͂��`�F�b�N
	pDidInstance = pDidInstance;
	return DIENUM_STOP;

}
#endif  // !DISABLE_GAMEPAD

//===============================================
//	�\���L�[�̐ݒ�@CALLBACK
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
	diprg.lMin = JOYSTICK_MIN;	//�W���C�X�e�B�b�N�̍ŏ��l(�さ��)
	diprg.lMax = JOYSTICK_MAX;	//�W���C�X�e�B�b�N�̍ő�l(�����E)

	if(FAILED(g_pDevGamePad->SetProperty( DIPROP_RANGE, &diprg.diph)))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}
#endif  // !DISABLE_GAMEPAD

//===============================================
//	�L�[�{�[�h���
//===============================================

//-------------------------------------
//	�L�[�{�[�h�������Ă�
//-------------------------------------
bool Keyboard_IsPress(int nDIK)
{
	return (g_aKeyState[nDIK] & 0x80) ? true : false;
}

//-------------------------------------
//	�L�[�{�[�h��������
//-------------------------------------
bool Keyboard_IsTrigger(int nDIK)
{
	return (g_aKeyStateTrigger[nDIK] & 0x80) ? true : false;
}

//-------------------------------------
//	�L�[�{�[�h�������
//-------------------------------------
bool Keyboard_IsRelease(int nDIK)
{
	return (g_aKeyStateRelease[nDIK] & 0x80) ? true : false;
}

//===============================================
//	�}�E�X���
//===============================================

//-------------------------------------
//	���N���b�N
//-------------------------------------
bool Mouse_IsLeftDown()
{
	return (g_MouseState2.rgbButtons[0] & 0x80) ? true : false;
}

//-------------------------------------
//	�E�N���b�N
//-------------------------------------
bool Mouse_IsRightDown()
{
	return (g_MouseState2.rgbButtons[1] & 0x80) ? true : false;
}

//-------------------------------------
//	�z�C�[���N���b�N
//-------------------------------------
bool Mouse_IsCenterDown()
{
	return (g_MouseState2.rgbButtons[2] & 0x80) ? true : false;
}

//-------------------------------------
//	�}�E�X�����x�@X
//-------------------------------------
LONG Mouse_IsAccelerationX()
{
	return g_MouseState2.lX;
}

//-------------------------------------
//	�}�E�X�����x�@Y
//-------------------------------------
LONG Mouse_IsAccelerationY()
{
	return g_MouseState2.lY;
}

//-------------------------------------
//	�}�E�X�����x�@Z�@(�X�N���[��)
//-------------------------------------
LONG Mouse_IsAccelerationZ()
{
	return g_MouseState2.lZ;
}

//-------------------------------------
//	�}�E�XX���W
//-------------------------------------
LONG Mouse_IsMouseX()
{
	return g_MousePos.x;
}

//-------------------------------------
//	�}�E�XY���W
//-------------------------------------
LONG Mouse_IsMouseY()
{
	return g_MousePos.y;
}

//===============================================
//	�Q�[���p�b�h
//===============================================
#if !defined(DISABLE_GAMEPAD)
//-------------------------------------
//	�Q�[���p�b�h�@�W���C�X�e�B�b�N��X�l
//-------------------------------------
LONG JoyStick_IsLeftStick_X()
{
	return g_PadStatus2.lX;
}

//-------------------------------------
//	�Q�[���p�b�h�@�W���C�X�e�B�b�N��Y�l
//-------------------------------------
LONG JoyStick_IsLeftStick_Y()
{
	return g_PadStatus2.lY;
}

//-------------------------------------
//	�Q�[���p�b�h�@�W���C�X�e�B�b�N�EX�l
//-------------------------------------
LONG JoyStick_IsRightStick_X()
{
	return g_PadStatus2.lZ;
}

//-------------------------------------
//	�Q�[���p�b�h�@�W���C�X�e�B�b�N�EY�l
//-------------------------------------
LONG JoyStick_IsRightStick_Y()
{
	return g_PadStatus2.lRz;
}

//-------------------------------------
//	�Q�[���p�b�h�@R2�̉������ݒl
//-------------------------------------
LONG JoyStick_IsR2PressPower()
{
	return g_PadStatus2.lRx;
}

//-------------------------------------
//	�Q�[���p�b�h�@L2�̉������ݒl
//-------------------------------------
LONG JoyStick_IsL2PressPower()
{
	return g_PadStatus2.lRy;
}

//-------------------------------------
//	�\���������Ă�
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
//	�\����������
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
//	�L�[�������Ă�
//-------------------------------------
bool JoyStick_IsPress(int nJSK)
{
	return (g_aJoyKeyState2[nJSK] & 0x80) ? true : false;
}

//-------------------------------------
//	�L�[��������
//-------------------------------------
bool JoyStick_IsTrigger(int nJSK)
{
	return (g_aJoyKeyState2Trigger[nJSK] & 0x80) ? true : false;
}

//-------------------------------------
//	�L�[�������
//-------------------------------------
bool JoyStick_IsRelease(int nJSK)
{
	return (g_aJoyKeyState2Release[nJSK] & 0x80) ? true : false;
}

#endif  // !DISABLE_GAMEPAD


