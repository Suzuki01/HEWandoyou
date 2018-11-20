/*========================�R�c��ɓ_�̃X�N���[�����W�����======================
					��ҁFYE LINYIN
				���ӁF�_���J�����ɓ����ĂȂ���ԂŁA�_�̍��W�������Ă�
				���͓_����J�����̋�����near��菬��������far���傫��
				�̏ꍇ�A�_�̍��W�������Ă�B
================================================================================
*/
#ifndef GET2DPOSIN3D_H
#define GET2DPOSIN3D_H
#include"mydirect3d.h"
#include<d3dx9.h>

//�_�̃X�N���[�����W�����i���[���h�s������āj
D3DXVECTOR2 GetScreenPosition(D3DXMATRIX input);


//�_�̃X�N���[�����W�����(���ڍ��W���w�肷��)
D3DXVECTOR2 GetScreenPosition(float x,float y,float z);


/*camera.cpp��camera.h�Ɉȉ��̊֐���ǉ����邱��
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
