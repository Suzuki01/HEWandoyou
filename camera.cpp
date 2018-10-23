#include <d3dx9.h>
#include "mydirect3d.h"
#include "common.h"
#include "input.h"


#define CHARGE_ATTACK_TIME			(10)
#define CHARGE_ATTACK_ZOOM_SPEED	(0.1f)

#define AVOIDANCE_TIME				(6)
#define AVOIDANCE_ZOOM_SPEED		(0.05f)

D3DXVECTOR3 CameraAt(0.0f, 0.0f, 0.0f);					//注視点

D3DXVECTOR3 CameraPos;
float CameraLenth;									//1以上
D3DXVECTOR3 CameraFront;							//必ず長さ1（単位ベクトル）
D3DXVECTOR3 CameraRight;
D3DXVECTOR3 CameraUp;

bool bChargeAttack;
float ChargeAttack_Move;
int Charge_cnt;

bool bRight;
float Right_Move;
int Right_cnt;

bool bLeft;
float Left_Move;
int Left_cnt;

void Camera_Initialize(void)
{
	CameraPos = D3DXVECTOR3(0.5f, 1.0f, -2.0f);
	CameraLenth = 3.0f;
	CameraFront= D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVec3Normalize(&CameraFront, &CameraFront);
	CameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Cross(&CameraRight, &CameraFront, &CameraUp);
	D3DXVec3Normalize(&CameraRight, &CameraRight);
	D3DXVec3Cross(&CameraUp, &CameraRight, &CameraFront);
	D3DXVec3Normalize(&CameraUp, &CameraUp);

	bChargeAttack = false;
	ChargeAttack_Move = 0.0f;
	Charge_cnt = 0;

	bRight = false;
	Right_Move = 0.0f;
	Right_cnt = 0;

	bLeft = false;
	Left_Move = 0.0f;
	Left_cnt = 0;
}

void Camera_Update(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	D3DXVECTOR3 f(CameraFront);
	f.y = 0.0f;

	//チャージアタックする時のカメラズーム
	if (bChargeAttack)
	{
		Charge_cnt++;
		if (ChargeAttack_Move < CHARGE_ATTACK_TIME*CHARGE_ATTACK_ZOOM_SPEED)
		{
			CameraPos += CameraFront*CHARGE_ATTACK_ZOOM_SPEED;
			ChargeAttack_Move += CHARGE_ATTACK_ZOOM_SPEED;
		}
		else if (ChargeAttack_Move < 2*CHARGE_ATTACK_TIME*CHARGE_ATTACK_ZOOM_SPEED)
		{
			CameraPos -= CameraFront*CHARGE_ATTACK_ZOOM_SPEED;
			ChargeAttack_Move += CHARGE_ATTACK_ZOOM_SPEED;
		}
		if(ChargeAttack_Move >= 2 * CHARGE_ATTACK_TIME*CHARGE_ATTACK_ZOOM_SPEED)
		{
			bChargeAttack = false;
			Charge_cnt = 0;
			ChargeAttack_Move = 0;
		}
	}

	//回避する時のカメラズーム
	if (bRight)
	{
		Right_cnt++;
		if (Right_Move < AVOIDANCE_TIME*AVOIDANCE_ZOOM_SPEED)
		{
			CameraPos -= CameraRight*AVOIDANCE_ZOOM_SPEED;
			Right_Move += AVOIDANCE_ZOOM_SPEED;
		}
		else if (Right_Move < 2 * AVOIDANCE_TIME*AVOIDANCE_ZOOM_SPEED)
		{
			CameraPos += CameraRight*AVOIDANCE_ZOOM_SPEED;
			Right_Move += AVOIDANCE_ZOOM_SPEED;
		}
		if (Right_Move >= 2 * AVOIDANCE_TIME*AVOIDANCE_ZOOM_SPEED)
		{
			bRight = false;
			Right_cnt = 0;
			Right_Move = 0;
		}
	}
	if (bLeft)
	{
		Left_cnt++;
		if (Left_Move < AVOIDANCE_TIME*AVOIDANCE_ZOOM_SPEED)
		{
			CameraPos += CameraRight*AVOIDANCE_ZOOM_SPEED;
			Left_Move += AVOIDANCE_ZOOM_SPEED;
		}
		else if (Right_Move < 2 * AVOIDANCE_TIME*AVOIDANCE_ZOOM_SPEED)
		{
			CameraPos -= CameraRight*AVOIDANCE_ZOOM_SPEED;
			Left_Move += AVOIDANCE_ZOOM_SPEED;
		}
		if (Left_Move >= 2 * AVOIDANCE_TIME*AVOIDANCE_ZOOM_SPEED)
		{
			bLeft = false;
			Left_cnt = 0;
			Left_Move = 0;
		}
	}


	/*if (Keyboard_IsPress(DIK_W))
	{
		D3DXVECTOR3 f(CameraFront);
		f.y = 0.0f;
		CameraPos += f*0.05f;
	}
	if (Keyboard_IsPress(DIK_S))
	{
		D3DXVECTOR3 f(CameraFront);
		f.y = 0.0f;
		CameraPos -= f*0.05f;
	}
	if (Keyboard_IsPress(DIK_A))
	{
		D3DXMATRIX mtxR;
		D3DXMatrixRotationY(&mtxR, D3DXToRadian(-1));
		D3DXVec3TransformNormal(&CameraFront, &CameraFront, &mtxR);
	}
	if (Keyboard_IsPress(DIK_D))
	{
		D3DXMATRIX mtxR;
		D3DXMatrixRotationY(&mtxR, D3DXToRadian(1));
		D3DXVec3TransformNormal(&CameraFront, &CameraFront, &mtxR);
	}*/

	CameraAt = CameraFront*CameraLenth + CameraPos;

	//⑤ビュー変換行列の作成と設定
	D3DXMATRIX mtxView;
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);					//カメラの上ベクトル
	D3DXMatrixLookAtLH(&mtxView, &CameraPos, &CameraAt, &CameraUp);

	pDevice->SetTransform(D3DTS_VIEW, &mtxView);


	//⑥プロジェクション(※パースペクティブ)変換行列の作成と設定
	D3DXMATRIX mtxProjection;
	D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DXToRadian(60)/*画角(視野角)*/, (float)SCREEN_WIDTH / SCREEN_HEIGHT/*aspect*/, 0.1f/*near*/, 100.0f/*far*/);

	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}


void Camera_Set_ChargeAttack(bool ChargeAttack)
{
	bChargeAttack = ChargeAttack;
}

void Camera_Set_Right(bool Right)
{
	bRight = Right;
}

void Camera_Set_Left(bool Left)
{
	bLeft = Left;
}