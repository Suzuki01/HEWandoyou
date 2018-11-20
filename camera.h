#ifndef CAMERA_H_
#define CAMERA_H_

#include <d3dx9.h>

void Camera_Initialize(void);
void Camera_Update(void);
void Camera_Set_ChargeAttack(bool ChargeAttack);
void Camera_Set_Right(bool Right);
void Camera_Set_Left(bool Left);

D3DXVECTOR3 Camera_Get_Up(void);
D3DXVECTOR3 Camera_Get_Right(void);
D3DXVECTOR3 Camera_Get_Front(void);

void Camera_Set_Pos(D3DXVECTOR3 pos);
D3DXVECTOR3 Camera_Get_Pos(void);

void Camera_turn_right(float speed);

D3DXMATRIX Camera_GetView(void);
D3DXMATRIX Camera_GetProjection(void);

void Camera_Follow(void);

#endif // CAMERA_H_