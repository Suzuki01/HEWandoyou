#include <stdio.h>
#include <time.h>
#include <d3dx9.h>
#include "texture.h"
#include "cube.h"
#include "enemy.h"

#define MAX_EXPLOSION		(8)
typedef struct {
	float X;
	float Y;
	float Z;
	bool bIsEnable;
	int FrameCount;
}EXPLOSION;

EXPLOSION Explosion[MAX_EXPLOSION];


void Explosion_Initialize(void)
{
	for (int i=0; i < MAX_EXPLOSION; i++)
	{
		Explosion[i].X = 0.0f;
		Explosion[i].Y = 0.0f;
		Explosion[i].Z = 0.0f;
		Explosion[i].bIsEnable = false;
		Explosion[i].FrameCount = 0;
	}
}
void Explosion_Update(void)
{
	for (int i=0; i < MAX_EXPLOSION; i++)
	{
		if (Explosion[i].bIsEnable)
		{
			Explosion[i].FrameCount++;
			Explosion[0].X -= 0.005f;
			Explosion[0].Y += 0.005f;
			Explosion[1].X += 0.005f;
			Explosion[1].Y += 0.005f;
			Explosion[2].X -= 0.005f;
			Explosion[2].Y -= 0.005f;
			Explosion[3].X += 0.005f;
			Explosion[3].Y -= 0.005f;
			Explosion[4].X += 0.005f;
			Explosion[5].X -= 0.005f;
			Explosion[6].Y += 0.005f;
			Explosion[7].Y -= 0.005f;
			/*switch (Enemy_Get_Mode())
			{
			case COUNTER_UP:
				Explosion[i].X += (rand() % 10 - 20)*0.005f;
				Explosion[i].Y += (rand() % 10)*0.01f;
				Explosion[i].Z += (rand() % 10 - 20)*0.005f;
				break;
			case COUNTER_DOWN:
				Explosion[i].X += (rand() % 10 - 20)*0.005f;
				Explosion[i].Y -= (rand() % 10)*0.01f;
				Explosion[i].Z += (rand() % 10 - 20)*0.005f;
				break;
			case COUNTER_LEFT:
				Explosion[i].X -= (rand() % 10)*0.01f;
				Explosion[i].Y += (rand() % 10 - 20)*0.005f;
				Explosion[i].Z += (rand() % 10 - 20)*0.005f;
				break;
			case COUNTER_RIGHT:
				Explosion[i].X += (rand() % 10)*0.01f;
				Explosion[i].Y += (rand() % 10 - 20)*0.005f;
				Explosion[i].Z += (rand() % 10 - 20)*0.005f;
				break;
			}*/
		}
		if (Explosion[i].FrameCount > 20)
		{
			Explosion[i].bIsEnable = false;
		}
	}
}
void Explosion_Draw(void)
{
	for (int i=0; i < MAX_EXPLOSION; i++)
	{
		if (Explosion[i].bIsEnable)
		{
			Cube_Draw(TEXTURE_INDEX_EXPLO,Explosion[i].X, Explosion[i].Y, Explosion[i].Z, 0.1f, 0.1f, 0.1f, 0, 0, 0);
		}
	}
}
void Explosion_Create(float x, float y,float z)
{
	for (int i=0; i < MAX_EXPLOSION; i++)
	{
		if (!Explosion[i].bIsEnable)
		{
			Explosion[i].X = x;
			Explosion[i].Y = y;
			Explosion[i].Z = z;
			Explosion[i].bIsEnable = true;
			Explosion[i].FrameCount = 0;
		}
	}
}