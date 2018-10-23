#include <stdio.h>
#include <time.h>
#include <d3dx9.h>
#include "cube.h"
#include "texture.h"
#include "player.h"
#include "debug_font.h"
#include "grid.h"
#include "enemy.h"
#include "sprite.h"
#include "fade.h"
#include "explosion.h"

#define SPEED_SLOW		(0.005f)

#define	START_LINE		(5.0f)
#define	END_LINE		(-2.0f)

#define SLOW_TIME		(90)
#define STAN_TIME		(180)
#define INJURED_TIME	(20)


#define HP_MAX			(100)

#define ATTACK_SPEED_NORMAL (D3DXToRadian(2))
#define ATTACK_SPEED_SLOW   (D3DXToRadian(0.05))
#define ATTACK_SPEED_BACK   (D3DXToRadian(10))

static float Enemy_PosX;
static float Enemy_PosY;
static float Enemy_PosZ;

static float Enemy_Speed;

static bool bEnemy_Effective;
static bool bEnemy_Slow;
static bool bEnemy_Stan;
static bool bEnemy_Advance;
static bool bEnemy_Injured;

static int Enemy_HP;

static int slow_cnt;
static int stan_cnt;
static int injured_cnt;

static int counter_mode;

static bool bDamageStart;

static float Enemy_AngleX;
static float Enemy_AngleY;
static float Enemy_AngleZ;

static bool bEnemy_Rotation;

void Enemy_Initialize(void)
{
	Enemy_PosX=0.0f;
	Enemy_PosY=0.5f;
	Enemy_PosZ=START_LINE;

	Enemy_Speed = 0.05f;

	bEnemy_Effective = false;
	bEnemy_Slow = false;
	bEnemy_Stan = false;
	bEnemy_Advance = true;

	Enemy_HP = HP_MAX;

	slow_cnt = 0;
	stan_cnt = 0;
	injured_cnt = 0;

	counter_mode = 0;
	bDamageStart = false;

	Enemy_AngleX=0.0f;
	Enemy_AngleY=0.0f;
	Enemy_AngleZ=0.0f;

	bEnemy_Rotation = false;
	bEnemy_Injured = false;
}

void Enemy_Update(void)
{
	if (Enemy_HP > 0)
	{
		//攻撃受ける時色変わる
		if (bEnemy_Injured)
		{
			injured_cnt++;
			if (injured_cnt > INJURED_TIME)
			{
				bEnemy_Injured = false;
				injured_cnt = 0;
			}
		}

		//前進後退
		if (bEnemy_Advance)
		{
			Enemy_PosZ -= Enemy_Speed;
		}
		else
		{
			Enemy_PosZ += Enemy_Speed;
		}

		if (Enemy_PosZ < END_LINE&&bEnemy_Advance)
		{
			bEnemy_Advance = false;
			Enemy_Speed = (rand() % 5 + 5)*0.01;
		}
		if (Enemy_PosZ > START_LINE&&!bEnemy_Advance)
		{
			bEnemy_Advance = true;
			Enemy_Speed = (rand() % 5 + 5)*0.01;
		}
		

		if (Enemy_PosZ <= ATTACK_LINE&&Enemy_PosZ>=PLAYER_LINE&&!bEnemy_Effective)
		{
			bEnemy_Effective = true;
			counter_mode = rand() % 4 + 1;
		}
		if (bEnemy_Effective/*&&bEnemy_Advance*/)
		{
			switch (counter_mode)
			{
			case COUNTER_UP:
				if (Enemy_AngleX == 0)
				{
					bEnemy_Rotation = true;
					Player_Set_bInjured(false);
					bDamageStart = false;
				}
				if (bEnemy_Rotation&&bEnemy_Advance&&!bEnemy_Stan)
				{
					if (!bEnemy_Slow)
					{
						Enemy_AngleX += ATTACK_SPEED_NORMAL;
					}
					else
					{
						Enemy_AngleX += ATTACK_SPEED_SLOW;
					}
					if (Enemy_AngleX >= D3DXToRadian(90) && !Player_Get_bInjured())
					{
						bEnemy_Rotation = false;
						Player_Set_Damage();
						bEnemy_Advance = false;
						if (!bDamageStart) {
							Fade_Start(false, 60, D3DCOLOR_RGBA(255, 0, 0, 0));
							bDamageStart = true;
						}
						Player_Set_bInjured(true);
					}
				}
				if (!bEnemy_Rotation||!bEnemy_Advance)
				{
					Enemy_AngleX -= ATTACK_SPEED_BACK;
					if (Enemy_AngleX <= 0)
					{
						Enemy_AngleX = 0;
					}
				}
				break;
			case COUNTER_DOWN:
				if (Enemy_AngleX == 0)
				{
					bEnemy_Rotation = true;
					Player_Set_bInjured(false);
					bDamageStart = false;
				}
				if (bEnemy_Rotation&&bEnemy_Advance && !bEnemy_Stan)
				{
					if (!bEnemy_Slow)
					{
						Enemy_AngleX += ATTACK_SPEED_NORMAL;
					}
					else
					{
						Enemy_AngleX += ATTACK_SPEED_SLOW;
					}
					if (Enemy_AngleX >= D3DXToRadian(90) && !Player_Get_bInjured())
					{
						bEnemy_Rotation = false;
						Player_Set_Damage();
						bEnemy_Advance = false;
						if (!bDamageStart) {
							Fade_Start(false, 60, D3DCOLOR_RGBA(255, 0, 0, 0));
							bDamageStart = true;
						}
						Player_Set_bInjured(true);
					}
				}
				if (!bEnemy_Rotation || !bEnemy_Advance)
				{
					Enemy_AngleX -= ATTACK_SPEED_BACK;
					if (Enemy_AngleX <= 0)
					{
						Enemy_AngleX = 0;
					}
				}
				break;
			case COUNTER_LEFT:
				if (Enemy_AngleY == 0)
				{
					bEnemy_Rotation = true;
					Player_Set_bInjured(false);
					bDamageStart = false;
				}
				if (bEnemy_Rotation&&bEnemy_Advance && !bEnemy_Stan)
				{
					if (!bEnemy_Slow)
					{
						Enemy_AngleY += ATTACK_SPEED_NORMAL;
					}
					else
					{
						Enemy_AngleY += ATTACK_SPEED_SLOW;
					}
					if (Enemy_AngleY >= D3DXToRadian(90) && !Player_Get_bInjured())
					{
						bEnemy_Rotation = false;
						Player_Set_Damage();
						bEnemy_Advance = false;
						if (!bDamageStart) {
							Fade_Start(false, 60, D3DCOLOR_RGBA(255, 0, 0, 0));
							bDamageStart = true;
						}
						Player_Set_bInjured(true);
					}
				}
				if (!bEnemy_Rotation || !bEnemy_Advance)
				{
					Enemy_AngleY -= ATTACK_SPEED_BACK;
					if (Enemy_AngleY <= 0)
					{
						Enemy_AngleY = 0;
					}
				}
				break;
			case COUNTER_RIGHT:
				if (Enemy_AngleY == 0)
				{
					bEnemy_Rotation = true;
					Player_Set_bInjured(false);
					bDamageStart = false;
				}
				if (bEnemy_Rotation&&bEnemy_Advance && !bEnemy_Stan)
				{
					if (!bEnemy_Slow)
					{
						Enemy_AngleY += ATTACK_SPEED_NORMAL;
					}
					else
					{
						Enemy_AngleY += ATTACK_SPEED_SLOW;
					}
					if (Enemy_AngleY >= D3DXToRadian(90) && !Player_Get_bInjured())
					{
						bEnemy_Rotation = false;
						Player_Set_Damage();
						bEnemy_Advance = false;
						if (!bDamageStart) {
							Fade_Start(false, 60, D3DCOLOR_RGBA(255, 0, 0, 0));
							bDamageStart = true;
						}
						Player_Set_bInjured(true);
					}
				}
				if (!bEnemy_Rotation || !bEnemy_Advance)
				{
					Enemy_AngleY -= ATTACK_SPEED_BACK;
					if (Enemy_AngleY <= 0)
					{
						Enemy_AngleY = 0;
					}
				}
				break;
			default:
				break;
			}
		}
		if(Enemy_PosZ>ATTACK_LINE|| Enemy_PosZ<PLAYER_LINE)
		{
			bEnemy_Effective = false;
			Enemy_AngleX = 0;
			Enemy_AngleY = 0;
			Enemy_AngleZ = 0;
			counter_mode = 0;
		}

		//敵の攻撃
		if (Enemy_PosZ <= PLAYER_LINE&&!Player_Get_bInjured())
		{
			Player_Set_Damage();
			if (!bDamageStart) {
				Fade_Start(false, 60, D3DCOLOR_RGBA(255, 0, 0, 0));
				bDamageStart = true;
			}
			Player_Set_bInjured(true);
		}
		if (Enemy_PosZ > PLAYER_LINE&&Player_Get_bInjured())
		{
			Player_Set_bInjured(false);
		}

		//スロー
		if (bEnemy_Effective&&(Player_Get_Mode() == AVOIDANCE_MODE_L|| Player_Get_Mode() == AVOIDANCE_MODE_R)&&bEnemy_Advance&&!bEnemy_Slow&& !bEnemy_Stan)
		{
			bEnemy_Slow = true;
			Fade_Start(true, 1, D3DCOLOR_RGBA(200, 200, 200, 0));
		}
		if (bEnemy_Slow)
		{
			Enemy_Speed = SPEED_SLOW;
			slow_cnt++;
			if (slow_cnt >= SLOW_TIME)
			{
				bEnemy_Advance = false;
				Enemy_Speed = (rand() % 5 + 5)*0.01;
				slow_cnt = 0;
				bEnemy_Slow = false;
				Fade_Start(false, 1, D3DCOLOR_RGBA(200, 200, 200, 255));
			}
		}
		//スタン
		if (Player_Get_bCounter())
		{
			bEnemy_Stan = true;
		}
		if (bEnemy_Stan)
		{
			Enemy_Speed = 0;
			stan_cnt++;
			if (stan_cnt==1)
			{
				switch (counter_mode)
				{
				case COUNTER_UP:
					Explosion_Create(Enemy_PosX, Enemy_PosY + 1.0f - (1.0f - (1.5f*cos(Enemy_AngleX))), Enemy_PosZ - (1.0f*sin(Enemy_AngleX)));
					break;
				case COUNTER_DOWN:
					Explosion_Create(Enemy_PosX, Enemy_PosY - 1.0f + (1.0f - (1.5f*cos(Enemy_AngleX))), Enemy_PosZ - (1.0f*sin(Enemy_AngleX)));
					break;
				case COUNTER_LEFT:
					Explosion_Create(Enemy_PosX - 1.0f + (1.0f - (1.0f*cos(Enemy_AngleY))), Enemy_PosY, Enemy_PosZ - (1.0f*sin(Enemy_AngleY)));
					break;
				case COUNTER_RIGHT:
					Explosion_Create(Enemy_PosX + 1.0f - (1.0f - (1.0f*cos(Enemy_AngleY))),Enemy_PosY, Enemy_PosZ - (1.0f*sin(Enemy_AngleY)));
					break;
				default:
					break;
				}
			}
			if (Player_Get_bChargeAttack())
			{
				Enemy_Speed = -1;
				
				if (Enemy_PosZ > START_LINE)
				{
					bEnemy_Advance = true;
					Enemy_Speed = (rand() % 5 + 5)*0.01;
					Player_Set_bChargeAttack(false);
				}
			}
			else if (stan_cnt >= STAN_TIME)
			{
				bEnemy_Advance = false;
				Enemy_Speed = (rand() % 5 + 5)*0.01;
				stan_cnt = 0;
				bEnemy_Stan = false;
				Player_Set_bCounter(false);
				Player_Set_Charge(0);
			}
		}
	}
}

void Enemy_Draw(void)
{
	if (Enemy_HP > 0)
	{
		TextureIndex texture;
		if (!bEnemy_Injured)
		{
			texture = TEXTURE_INDEX_1;
		}
		else
		{
			texture = TEXTURE_INDEX_2;
		}

		if (!bEnemy_Effective)
		{
			Cube_Draw(texture, Enemy_PosX, Enemy_PosY, Enemy_PosZ, 1.0f, 1.0f, 1.0f, 0, 0, 0);
			Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 0.5f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
			Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 1.0f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
			Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 1.5f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
			Cube_Draw(texture, Enemy_PosX + 0.5f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
			Cube_Draw(texture, Enemy_PosX + 1.0f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
			Cube_Draw(texture, Enemy_PosX + 1.5f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
			Cube_Draw(texture, Enemy_PosX - 0.5f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
			Cube_Draw(texture, Enemy_PosX - 1.0f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
			Cube_Draw(texture, Enemy_PosX - 1.5f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
			Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 0.5f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
			Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 1.0f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
			Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 1.5f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
		}
		else /*if(!bEnemy_Slow&&!bEnemy_Stan)*/
		{
			switch (counter_mode)
			{
			case COUNTER_UP:
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY, Enemy_PosZ, 1.0f, 1.0f, 1.0f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 0.5f - (0.5f - (0.5f*cos(Enemy_AngleX))), Enemy_PosZ - (0.5f*sin(Enemy_AngleX)), 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 1.0f - (1.0f - (1.0f*cos(Enemy_AngleX))), Enemy_PosZ - (1.0f*sin(Enemy_AngleX)), 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 1.5f - (1.5f - (1.5f*cos(Enemy_AngleX))), Enemy_PosZ - (1.5f*sin(Enemy_AngleX)), 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX + 0.5f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX + 1.0f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX + 1.5f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX - 0.5f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX - 1.0f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX - 1.5f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 0.5f, Enemy_PosZ , 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 1.0f, Enemy_PosZ , 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 1.5f, Enemy_PosZ , 0.5f, 0.5f, 0.5f, 0, 0, 0);
				break;
			case COUNTER_DOWN:
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY, Enemy_PosZ, 1.0f, 1.0f, 1.0f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 0.5f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 1.0f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 1.5f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX + 0.5f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX + 1.0f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX + 1.5f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX - 0.5f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX - 1.0f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX - 1.5f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 0.5f + (0.5f - (0.5f*cos(Enemy_AngleX))), Enemy_PosZ - (0.5f*sin(Enemy_AngleX)), 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 1.0f + (1.0f - (1.0f*cos(Enemy_AngleX))), Enemy_PosZ - (1.0f*sin(Enemy_AngleX)), 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 1.5f + (1.5f - (1.5f*cos(Enemy_AngleX))), Enemy_PosZ - (1.5f*sin(Enemy_AngleX)), 0.5f, 0.5f, 0.5f, 0, 0, 0);
				break;
			case COUNTER_LEFT:
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY, Enemy_PosZ, 1.0f, 1.0f, 1.0f,  0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 0.5f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 1.0f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 1.5f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX + 0.5f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX + 1.0f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX + 1.5f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX - 0.5f + (0.5f - (0.5f*cos(Enemy_AngleY))), Enemy_PosY, Enemy_PosZ - (0.5f*sin(Enemy_AngleY)), 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX - 1.0f + (1.0f - (1.0f*cos(Enemy_AngleY))), Enemy_PosY, Enemy_PosZ - (1.0f*sin(Enemy_AngleY)), 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX - 1.5f + (1.5f - (1.5f*cos(Enemy_AngleY))), Enemy_PosY, Enemy_PosZ - (1.5f*sin(Enemy_AngleY)), 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 0.5f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 1.0f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 1.5f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				break;
			case COUNTER_RIGHT:
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY, Enemy_PosZ, 1.0f, 1.0f, 1.0f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 0.5f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 1.0f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 1.5f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX + 0.5f - (0.5f - (0.5f*cos(Enemy_AngleY))), Enemy_PosY, Enemy_PosZ - (0.5f*sin(Enemy_AngleY)), 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX + 1.0f - (1.0f - (1.0f*cos(Enemy_AngleY))), Enemy_PosY, Enemy_PosZ - (1.0f*sin(Enemy_AngleY)), 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX + 1.5f - (1.5f - (1.5f*cos(Enemy_AngleY))), Enemy_PosY, Enemy_PosZ - (1.5f*sin(Enemy_AngleY)), 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX - 0.5f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX - 1.0f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX - 1.5f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 0.5f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 1.0f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 1.5f, Enemy_PosZ, 0.5f, 0.5f, 0.5f, 0, 0, 0);
				break;
			default:
				break;
			}	
		}

	/*	if (bEnemy_Slow)
		{
			Cube_Draw(TEXTURE_INDEX_ENEMY_SLOW, Enemy_PosX, Enemy_PosY, Enemy_PosZ, 1.0f, 1.0f, 1.0f, Enemy_AngleX, Enemy_AngleY, Enemy_AngleZ);
		}
		if (bEnemy_Stan)
		{
			Cube_Draw(TEXTURE_INDEX_ENEMY_STAN, Enemy_PosX, Enemy_PosY, Enemy_PosZ, 1.0f, 1.0f, 1.0f, Enemy_AngleX, Enemy_AngleY, Enemy_AngleZ);
		}*/
	}

	//HPケージ
	Sprite_Draw(TEXTURE_INDEX_HPCAGE, 10, 10);
	float HP_W;
	HP_W = ((float)Enemy_HP / HP_MAX) * 780;
	Sprite_Draw(TEXTURE_INDEX_HP, 20, 20, 0, 0, HP_W, 30);
	//DebugFont_Draw(50, 100, "敵の速度: %f", Enemy_Speed);
	//DebugFont_Draw(50, 150, "敵のモード: %d", counter_mode);
	//DebugFont_Draw(50, 150, "Enemy_AngleX: %f", Enemy_AngleX);
}

bool Enemy_Get_bEffective(void)
{
	return bEnemy_Effective;
}
bool Enemy_Get_bSlow(void)
{
	return bEnemy_Slow;
}
bool Enemy_Get_bStan(void)
{
	return bEnemy_Stan;
}
bool Enemy_Get_bAdvance(void)
{
	return bEnemy_Advance;
}
void Enemy_Set_Damage(void)
{
	Enemy_HP--;
	bEnemy_Injured = true;
	if (Enemy_HP < 0)
	{
		Enemy_HP = 0;
	}
}
void Enemy_Set_BigDamage(void)
{
	Enemy_HP-=20;
	bEnemy_Injured = true;
	if (Enemy_HP < 0)
	{
		Enemy_HP = 0;
	}
}
int Enemy_Get_Mode(void)
{
	return counter_mode;
}
int Enemy_Get_HP(void)
{
	return Enemy_HP;
}

void Enemy_Set_bRotation(bool bRotation)
{
	bEnemy_Rotation = bRotation;
}
