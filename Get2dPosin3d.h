/*========================３Ｄ上に点のスクリーン座標を取る======================
					作者：YE LINYIN
				注意：点がカメラに入ってない状態で、点の座標が狂ってる
				又は点からカメラの距離がnearより小さい又はfarより大きい
				の場合、点の座標が狂ってる。
================================================================================
*/
#ifndef GET2DPOSIN3D_H
#define GET2DPOSIN3D_H
#include"mydirect3d.h"
#include<d3dx9.h>

//点のスクリーン座標を取る（ワールド行列を入れて）
D3DXVECTOR2 GetScreenPosition(D3DXMATRIX input);


//点のスクリーン座標を取る(直接座標を指定する)
D3DXVECTOR2 GetScreenPosition(float x,float y,float z);


/*camera.cppとcamera.hに以下の関数を追加すること
======================camera.h======================
D3DXMATRIX Camera_GetView(void);
D3DXMATRIX Camera_GetProjection(void);




======================camera.cpp======================
D3DXMATRIX Camera_GetView(void)
{
	D3DXMatrixLookAtLH(&mtxView, &CameraPos, &CameraAt, &CameraUp);
	return mtxView;
}

D3DXMATRIX Camera_GetProjection(void)
{
	D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DXToRadian(60), (float)SCREEN_HEIGHT / SCREEN_WIDTH, 0.1f, 100.0f);
	return mtxProjection;
}


*/

#endif // !GET2DPOSIN3D
