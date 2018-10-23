#include "bullet.h"
#include "cube.h"
#include "texture.h"
#include "grid.h"

typedef struct {
	float X;
	float Y;
	float Z;
	float Speed;

	bool bIsEnable;

}BULLET;

BULLET Bullet[MAX_BULLET];

void Bullet_Initialize(void)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		Bullet[i].X = 0.0f;
		Bullet[i].Y = 0.0f;
		Bullet[i].Z = 0.0f;
		Bullet[i].Speed = 0.1f;
		Bullet[i].bIsEnable = false;
	}
}
void Bullet_Update(void)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (Bullet[i].bIsEnable)
		{
			Bullet[i].Z += Bullet[i].Speed;

			if (Bullet[i].Z > ATTACK_LINE)
			{
				Bullet[i].bIsEnable = false;
			}
		}
	}
}
void Bullet_Draw(void)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (Bullet[i].bIsEnable)
		{
			Cube_Draw(TEXTURE_INDEX_PLAYER, Bullet[i].X, Bullet[i].Y, Bullet[i].Z, 0.25f, 0.25f, 0.25f, 0, 0, 0);
		}
	}
}

void Bullet_Create(float x, float y,float z)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (!Bullet[i].bIsEnable)
		{
			Bullet[i].X = x;
			Bullet[i].Y = y;
			Bullet[i].Z = z;
			Bullet[i].bIsEnable = true;
			break;
		}
	}
}
void Bullet_Destroy(int i)
{
	Bullet[i].bIsEnable = false;
}
bool Bullet_IsEnable(int i)
{
	return Bullet[i].bIsEnable;
}