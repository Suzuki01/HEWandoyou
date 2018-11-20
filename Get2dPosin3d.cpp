#include"Get2dPosin3d.h"
#include"camera.h"
#include "debug_font.h"
#include"common.h"
static D3DXVECTOR2 output;
//s—ñæŽZŠÖ”i{x,y,z,w}‚ÆD3DXMATRIXj
D3DXVECTOR4 kakeru(D3DXVECTOR4 four, D3DXMATRIX sixteen);


D3DXVECTOR2 GetScreenPosition(D3DXMATRIX input)
{
	D3DXVECTOR4 temp4 = { 0,0,0,1 };	
	temp4 = kakeru(temp4, input);
	temp4 = kakeru(temp4, Camera_GetView());
	temp4 = kakeru(temp4, Camera_GetProjection());
	output.x = temp4.x/temp4.w/3.0f;
	output.y = temp4.y/temp4.w;
	//output.x += 3.0f;
	output.x += 1.0f;
	output.x = output.x*SCREEN_WIDTH / 2.0f;
	//output.x += 3.0f;
	output.y = 2.0f - (output.y + 1.0f);
	output.y = output.y*SCREEN_HEIGHT / 2.0f;
	if (output.x > SCREEN_WIDTH || output.y > SCREEN_HEIGHT || output.x < 0 || output.y < 0)
	{
		output.x = -1.0f;
		output.y = -1.0f;
	}
	return output;
}

D3DXVECTOR2 GetScreenPosition(float x, float y, float z)
{
	D3DXVECTOR4 temp4 = { x,y,z,1 };
	temp4 = kakeru(temp4, Camera_GetView());
	temp4 = kakeru(temp4, Camera_GetProjection());
	output.x = temp4.x / temp4.w / 3.0f;
	output.y = temp4.y / temp4.w;

	output.x += 1.0f;
	output.x = output.x*SCREEN_WIDTH / 2.0f;
	//output.x += 3.0f;
	output.y = 2.0f - (output.y+1.0f);
	output.y = output.y*SCREEN_HEIGHT / 2.0f;
	if (output.x > SCREEN_WIDTH || output.y > SCREEN_HEIGHT || output.x < 0 || output.y < 0)
	{
		output.x = -1.0f;
		output.y = -1.0f;
	}
	return output;
}

D3DXVECTOR4 kakeru(D3DXVECTOR4 four, D3DXMATRIX sixteen)
{
	D3DXVECTOR4 result;
	result.x = four.x*sixteen._11 + four.y*sixteen._21 + four.z*sixteen._31 + four.w*sixteen._41;
	result.y = four.x*sixteen._12 + four.y*sixteen._22 + four.z*sixteen._32 + four.w*sixteen._42;
	result.z = four.x*sixteen._13 + four.y*sixteen._23 + four.z*sixteen._33 + four.w*sixteen._43;
	result.w = four.x*sixteen._14 + four.y*sixteen._24 + four.z*sixteen._34 + four.w*sixteen._44;
	return result;
}