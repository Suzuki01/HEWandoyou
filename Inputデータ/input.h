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
//	�}�N����`
//===============================================
#define	DISABLE_GAMEPAD		//�Q�[���p�b�h�𖳌���
#define DISABLE_KEYBOARD	//�L�[�{�[�h�𖳌���	(���Ή�)
#define DISABLE_MOUSE		//�}�E�X�𖳌���		(���Ή�)


#define	NUM_KEY_MAX			(256)	// �L�[�̍ő吔

//-------------------------------------
//	�W���C�X�e�B�b�N�̃L�[ (DS4)
//-------------------------------------
#if !defined DISABLE_GAMEPAD
#define JSK_SQUARE		(0x00)		//�l�p�{�^��
#define JSK_CROSS		(0x01)		//�o�c�{�^��
#define JSK_CIRCLE		(0x02)		//�ۃ{�^��
#define JSK_TRIANGLE	(0x03)		//�O�p�{�^��
#define JSK_L1			(0x04)		//L1�{�^��
#define JSK_R1			(0x05)		//R1�{�^��
#define JSK_L2			(0x06)		//L2�{�^��
#define JSK_R2			(0x07)		//R2�{�^��
#define JSK_SHARE		(0x08)		//�V�F�A�{�^��
#define JSK_OPTION		(0x09)		//�I�v�V����
#define JSK_R3			(0x0A)		//R3�{�^��
#define JSK_L3			(0x0B)		//L3�{�^��
#define JSK_TOUCHPAD	(0x0D)		//�^�b�`�p�b�h

typedef enum JoyStickArrow_Tag
{
	ARROW_UP,
	ARROW_DOWN,
	ARROW_LEFT,
	ARROW_RIGHT
}ARROW_TYPE;

typedef struct PS4ControllerData
{
	LONG	lLeftX;		//��X
	LONG	lLeftY;		//��Y
	LONG	lRightX;	//�EX
	LONG	lRightY;	//�EY
	BYTE	bButtonA;	//�l�p
	BYTE	bButtonB;	//�o�c
	BYTE	bButtonC;	//�܂�
	BYTE	bPadding[3];
	
}PS4ControllerData;

#endif  // !DISABLE_GAMEPAD

//===============================================
//	�֐�
//===============================================	
void Input_Initalize(HWND hWnd, HINSTANCE hInstance);
void Input_Update(HWND hWnd);
void Input_Finalize();

//-------------------------------------
//	�f�o�C�X���
//-------------------------------------
#if !defined SYSTEM_H
LPDIRECT3DDEVICE9 System_GetDevice(void);	//�f�o�C�X�̎擾
#endif // !SYSTEM_H

//-------------------------------------
//	�L�[�{�[�h���	DIK
//-------------------------------------

bool Keyboard_IsPress(int nDIK);	//�L�[�{�[�h�������Ă�
bool Keyboard_IsTrigger(int nDIK);	//�L�[�{�[�h��������
bool Keyboard_IsRelease(int nDIK);	//�L�[�{�[�h�������

//-------------------------------------
//	�}�E�X���
//-------------------------------------

bool Mouse_IsLeftDown();			//�}�E�X����������
bool Mouse_IsRightDown();			//�}�E�X�E��������
bool Mouse_IsCenterDown();			//�}�E�X�̃Z���^�[
LONG Mouse_IsAccelerationX();		//�}�E�X�����x�@X
LONG Mouse_IsAccelerationY();		//�}�E�X�����x�@Y
LONG Mouse_IsAccelerationZ();		//�}�E�X�����x	Z(�z�C�[��)
LONG Mouse_IsMouseX();				//�}�E�X��X���W
LONG Mouse_IsMouseY();				//�}�E�X��Y���W

//�����x�Ȃǂ̎擾���\

//-------------------------------------
//	�Q�[���p�b�h��� (DS4)
//-------------------------------------
#if !defined DISABLE_GAMEPAD
LONG JoyStick_IsLeftStick_X();		//�W���C�X�e�B�b�N�� X��
LONG JoyStick_IsLeftStick_Y();		//�W���C�X�e�B�b�N�� Y��
LONG JoyStick_IsRightStick_X();		//�W���C�X�e�B�b�N�E X��
LONG JoyStick_IsRightStick_Y();		//�W���C�X�e�B�b�N�E Y��
LONG JoyStick_IsR2PressPower();		//R2�̉������ݒl
LONG JoyStick_IsL2PressPower();		//L2�̉������ݒl
bool JoyStick_IsArrowPress(ARROW_TYPE Type);	//�\���L�[�������Ă�
bool JoyStick_IsArrowTrigger(ARROW_TYPE Type);	//�\���L�[��������
bool JoyStick_IsPress(int nJSK);	//�L�[�������Ă�
bool JoyStick_IsTrigger(int nJSK);	//�L�[��������
bool JoyStick_IsRelease(int nJSK);	//�L�[�������
#endif

#endif