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
#include "judgement.h"
#include "Get2dPosin3d.h"

#define SPEED_ADVANCE	(0.015f)
#define SPEED_BACK		(0.1f)
#define SPEED_SLOW		(0.005f)

#define	START_LINE		(5.0f)
#define	END_LINE		(-1.0f)

#define SLOW_TIME		(90)
#define STAN_TIME		(180)
#define INJURED_TIME	(20)					//4の倍数にしてください


#define HP_MAX			(100)
#define DANGER_MAX		(20)

#define SPECIAL_ATTACK_SPEED_NORMAL (D3DXToRadian(1.5))
#define SPECIAL_ATTACK_SPEED_SLOW   (D3DXToRadian(0.05))
#define SPECIAL_ATTACK_SPEED_BACK   (D3DXToRadian(6))

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

static float Enemy_Angle;
static float Enemy_AngleUp;
static float Enemy_AngleDown;
static float Enemy_AngleLeft;
static float Enemy_AngleRight;

static bool bEnemy_SpecialAttack;
static bool bEnemy_SpecialAttackStart;

static bool bEnemy_Clockwise;
static float Enemy_Rotation;
static float Enemy_RotationAdd;

static bool bEnemy_Aim;

static int Enemy_Danger;

static indicator Enemy_Indicator;


void Enemy_InjuredEffect(int injured_cnt);


void Enemy_Initialize(void)
{
	Enemy_PosX = 0.0f;
	Enemy_PosY = 0.5f;
	Enemy_PosZ = START_LINE;

	Enemy_Indicator.posX = 0;
	Enemy_Indicator.posY = 0;
	Enemy_Indicator.r = 50;
	Enemy_Indicator.alive = true;

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

	Enemy_Angle = 0.0f;
	Enemy_AngleUp = 0.0f;
	Enemy_AngleDown = 0.0f;
	Enemy_AngleLeft = 0.0f;
	Enemy_AngleRight = 0.0f;

	bEnemy_SpecialAttack = false;
	bEnemy_SpecialAttackStart = false;
	bEnemy_Injured = false;

	bEnemy_Clockwise = false;
	Enemy_Rotation = 1.0f;
	Enemy_RotationAdd = 0.01f;

	bEnemy_Aim = false;

	Enemy_Danger = DANGER_MAX;
}

void Enemy_Update(void)
{
	if (Enemy_HP > 0)
	{
		//攻撃受ける時色変わる
		if (bEnemy_Injured)
		{
			if (injured_cnt > INJURED_TIME)
			{
				bEnemy_Injured = false;
				injured_cnt = 0;
			}
			injured_cnt++;
		}

		//前進後退
		if (bEnemy_Advance)
		{
			Enemy_PosZ -= SPEED_ADVANCE;
		}
		else
		{
			Enemy_PosZ += SPEED_BACK;
		}

		if (Enemy_PosZ < END_LINE&&bEnemy_Advance)
		{
			bEnemy_Advance = false;
		}
		if (Enemy_PosZ > START_LINE&&!bEnemy_Advance)
		{
			bEnemy_Advance = true;
			if (Enemy_Danger > 0)
			{
				Enemy_Danger = DANGER_MAX;
			}
			else if (Enemy_Danger == 0)
			{
				bEnemy_SpecialAttack = true;
				counter_mode = rand() % 4 + 1;
			}
		}
		
		//攻撃ラインを超えると弱点出現（位置ランダム）
		if (Enemy_PosZ <= ATTACK_LINE&&Enemy_PosZ>=PLAYER_LINE&&!bEnemy_Effective)
		{
			bEnemy_Effective = true;
			if (Enemy_Danger == DANGER_MAX)
			{
				counter_mode = rand() % 4 + 1;
			}

		}

		//弱点出現
		if (bEnemy_Effective&&bEnemy_Advance)
		{
			bEnemy_Aim = true;				//まだ使ってない

			if (Enemy_Danger == 0&& !bEnemy_SpecialAttack)
			{
				bEnemy_Advance = false;
			}
		}





		//必殺攻撃
		if (bEnemy_Effective&&bEnemy_SpecialAttack/*&&bEnemy_Advance*/)
		{
			if (Enemy_Angle == 0)
			{
				bEnemy_SpecialAttackStart = true;
				Player_Set_bInjured(false);
				bDamageStart = false;
			}
			if (bEnemy_SpecialAttackStart&&bEnemy_Advance && !bEnemy_Stan)
			{
				if (!bEnemy_Slow)
				{
					Enemy_Angle += SPECIAL_ATTACK_SPEED_NORMAL;
				}
				else
				{
					Enemy_Angle += SPECIAL_ATTACK_SPEED_SLOW;
				}
				if (Enemy_Angle >= D3DXToRadian(90) && !Player_Get_bInjured())
				{
					bEnemy_SpecialAttackStart = false;
					Player_Set_Damage();
					bEnemy_Advance = false;
					bEnemy_SpecialAttack = false;
					Enemy_Danger = DANGER_MAX;
					if (!bDamageStart) {
						Fade_Start(false, 60, D3DCOLOR_RGBA(255, 0, 0, 0));
						bDamageStart = true;
					}
					Player_Set_bInjured(true);
				}
			}
			if (!bEnemy_SpecialAttackStart || !bEnemy_Advance)
			{
				Enemy_Angle -= SPECIAL_ATTACK_SPEED_BACK;
				if (Enemy_Angle <= 0)
				{
					Enemy_Angle = 0;
				}
			}

			switch (counter_mode)
			{
			case COUNTER_UP:
				Enemy_AngleUp = Enemy_Angle;
				break;
			case COUNTER_DOWN:
				Enemy_AngleDown = Enemy_Angle;
				break;
			case COUNTER_LEFT:
				Enemy_AngleLeft = Enemy_Angle;
				break;
			case COUNTER_RIGHT:
				Enemy_AngleRight = Enemy_Angle;
				break;
			default:
				break;
			}
		}

		if(Enemy_PosZ>ATTACK_LINE|| Enemy_PosZ<PLAYER_LINE)
		{
			bEnemy_Effective = false;
			Enemy_Angle = 0;
			Enemy_AngleUp = 0;
			Enemy_AngleDown = 0;
			Enemy_AngleLeft = 0;
			Enemy_AngleRight = 0;
			//counter_mode = 0;
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
		if (bEnemy_SpecialAttack&&bEnemy_Effective&&(Player_Get_Mode() == AVOIDANCE_MODE_L|| Player_Get_Mode() == AVOIDANCE_MODE_R)&&bEnemy_Advance&&!bEnemy_Slow&& !bEnemy_Stan)
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
					Explosion_Create(Enemy_PosX, Enemy_PosY + 1.0f - (1.0f - (1.5f*cos(Enemy_AngleUp))), Enemy_PosZ - (1.0f*sin(Enemy_AngleUp)));
					break;
				case COUNTER_DOWN:
					Explosion_Create(Enemy_PosX, Enemy_PosY - 1.0f + (1.0f - (1.5f*cos(Enemy_AngleDown))), Enemy_PosZ - (1.0f*sin(Enemy_AngleDown)));
					break;
				case COUNTER_LEFT:
					Explosion_Create(Enemy_PosX - 1.0f + (1.0f - (1.0f*cos(Enemy_AngleLeft))), Enemy_PosY, Enemy_PosZ - (1.0f*sin(Enemy_AngleLeft)));
					break;
				case COUNTER_RIGHT:
					Explosion_Create(Enemy_PosX + 1.0f - (1.0f - (1.0f*cos(Enemy_AngleRight))),Enemy_PosY, Enemy_PosZ - (1.0f*sin(Enemy_AngleRight)));
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
		//ダメージ受けた時のテクスチャ変更
		TextureIndex texture;
		if (!bEnemy_Injured)
		{
			texture = TEXTURE_INDEX_1;
		}
		else
		{
			texture = TEXTURE_INDEX_2;
		}

		//時計回りか反時計回り
		if (bEnemy_Clockwise)
		{
			Enemy_Rotation += Enemy_RotationAdd;
		}
		else
		{
			Enemy_Rotation -= Enemy_RotationAdd;
		}

		//回転状態描画
		if (!bEnemy_SpecialAttack&&bEnemy_Advance)
		{
			Cube_Draw(texture, Enemy_PosX, Enemy_PosY, Enemy_PosZ, 1.0f, 1.0f, 1.0f);

			//右
			Cube_Draw(texture, Enemy_PosX + 0.5f*cosf(Enemy_Rotation), Enemy_PosY + 0.5f*sinf(Enemy_Rotation), Enemy_PosZ, 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX + 1.0f*cosf(Enemy_Rotation), Enemy_PosY + 1.0f*sinf(Enemy_Rotation), Enemy_PosZ, 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX + 1.5f*cosf(Enemy_Rotation), Enemy_PosY + 1.5f*sinf(Enemy_Rotation), Enemy_PosZ, 0.5f, 0.5f, 0.5f);

			//上
			Cube_Draw(texture, Enemy_PosX + 0.5f*cosf(Enemy_Rotation + D3DXToRadian(90)), Enemy_PosY + 0.5f*sinf(Enemy_Rotation + D3DXToRadian(90)), Enemy_PosZ, 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX + 1.0f*cosf(Enemy_Rotation + D3DXToRadian(90)), Enemy_PosY + 1.0f*sinf(Enemy_Rotation + D3DXToRadian(90)), Enemy_PosZ, 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX + 1.5f*cosf(Enemy_Rotation + D3DXToRadian(90)), Enemy_PosY + 1.5f*sinf(Enemy_Rotation + D3DXToRadian(90)), Enemy_PosZ, 0.5f, 0.5f, 0.5f);

			//左
			Cube_Draw(texture, Enemy_PosX + 0.5f*cosf(Enemy_Rotation + D3DXToRadian(180)), Enemy_PosY + 0.5f*sinf(Enemy_Rotation + D3DXToRadian(180)), Enemy_PosZ, 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX + 1.0f*cosf(Enemy_Rotation + D3DXToRadian(180)), Enemy_PosY + 1.0f*sinf(Enemy_Rotation + D3DXToRadian(180)), Enemy_PosZ, 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX + 1.5f*cosf(Enemy_Rotation + D3DXToRadian(180)), Enemy_PosY + 1.5f*sinf(Enemy_Rotation + D3DXToRadian(180)), Enemy_PosZ, 0.5f, 0.5f, 0.5f);

			//下
			Cube_Draw(texture, Enemy_PosX + 0.5f*cosf(Enemy_Rotation + D3DXToRadian(270)), Enemy_PosY + 0.5f*sinf(Enemy_Rotation + D3DXToRadian(270)), Enemy_PosZ, 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX + 1.0f*cosf(Enemy_Rotation + D3DXToRadian(270)), Enemy_PosY + 1.0f*sinf(Enemy_Rotation + D3DXToRadian(270)), Enemy_PosZ, 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX + 1.5f*cosf(Enemy_Rotation + D3DXToRadian(270)), Enemy_PosY + 1.5f*sinf(Enemy_Rotation + D3DXToRadian(270)), Enemy_PosZ, 0.5f, 0.5f, 0.5f);
		}
		//普通状態描画
		if ((bEnemy_SpecialAttack && !bEnemy_Effective) || !bEnemy_Advance|| bEnemy_Stan)
		{
			Cube_Draw(texture, Enemy_PosX, Enemy_PosY, Enemy_PosZ, 1.0f, 1.0f, 1.0f);
			Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 0.5f, Enemy_PosZ, 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 1.0f, Enemy_PosZ, 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 1.5f, Enemy_PosZ, 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX + 0.5f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX + 1.0f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX + 1.5f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX - 0.5f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX - 1.0f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX - 1.5f, Enemy_PosY, Enemy_PosZ, 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 0.5f, Enemy_PosZ, 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 1.0f, Enemy_PosZ, 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 1.5f, Enemy_PosZ, 0.5f, 0.5f, 0.5f);
		}
		//弱点描画
		if (bEnemy_Effective&&bEnemy_Advance && !bEnemy_SpecialAttack)
		{
			float tempx, tempy, tempz;
			switch (counter_mode)
			{
			case COUNTER_UP:
				Cube_Draw(TEXTURE_INDEX_3, Enemy_PosX + 1.5f*cosf(Enemy_Rotation + D3DXToRadian(90)), Enemy_PosY + 1.5f*sinf(Enemy_Rotation + D3DXToRadian(90)), Enemy_PosZ - 0.25f, 0.5f, 0.5f, 0.02f);
				tempx = Enemy_PosX + 1.5f*cosf(Enemy_Rotation + D3DXToRadian(90));
				tempy = Enemy_PosY + 1.5f*sinf(Enemy_Rotation + D3DXToRadian(90));
				tempz = Enemy_PosZ - 0.25f;
				Enemy_Indicator.posX = GetScreenPosition(tempx, tempy, tempz).x;
				Enemy_Indicator.posY = GetScreenPosition(tempx, tempy, tempz).y;
				break;
			case COUNTER_DOWN:
				Cube_Draw(TEXTURE_INDEX_3, Enemy_PosX + 1.5f*cosf(Enemy_Rotation + D3DXToRadian(270)), Enemy_PosY + 1.5f*sinf(Enemy_Rotation + D3DXToRadian(270)), Enemy_PosZ - 0.25f, 0.5f, 0.5f, 0.02f);
				tempx = Enemy_PosX + 1.5f*cosf(Enemy_Rotation + D3DXToRadian(270));
				tempy = Enemy_PosY + 1.5f*sinf(Enemy_Rotation + D3DXToRadian(270));
				tempz = Enemy_PosZ - 0.25f;
				Enemy_Indicator.posX = GetScreenPosition(tempx, tempy, tempz).x;
				Enemy_Indicator.posY = GetScreenPosition(tempx, tempy, tempz).y;
				break;
			case COUNTER_LEFT:
				Cube_Draw(TEXTURE_INDEX_3, Enemy_PosX + 1.5f*cosf(Enemy_Rotation + D3DXToRadian(180)), Enemy_PosY + 1.5f*sinf(Enemy_Rotation + D3DXToRadian(180)), Enemy_PosZ - 0.25f, 0.5f, 0.5f, 0.02f);
				tempx = Enemy_PosX + 1.5f*cosf(Enemy_Rotation + D3DXToRadian(180));
				tempy = Enemy_PosY + 1.5f*sinf(Enemy_Rotation + D3DXToRadian(180));
				tempz = Enemy_PosZ - 0.25f;
				Enemy_Indicator.posX = GetScreenPosition(tempx, tempy, tempz).x;
				Enemy_Indicator.posY = GetScreenPosition(tempx, tempy, tempz).y;

				break;
			case COUNTER_RIGHT:
				Cube_Draw(TEXTURE_INDEX_3, Enemy_PosX + 1.5f*cosf(Enemy_Rotation), Enemy_PosY + 1.5f*sinf(Enemy_Rotation), Enemy_PosZ - 0.25f, 0.5f, 0.5f, 0.02f);
				tempx = Enemy_PosX + 1.5f*cosf(Enemy_Rotation);
				tempy = Enemy_PosY + 1.5f*sinf(Enemy_Rotation);
				tempz = Enemy_PosZ - 0.25f;
				Enemy_Indicator.posX = GetScreenPosition(tempx, tempy, tempz).x;
				Enemy_Indicator.posY = GetScreenPosition(tempx, tempy, tempz).y;
				break;
			}
		}
		//必殺攻撃描画
		if (bEnemy_Effective&&bEnemy_Advance&&bEnemy_SpecialAttack && !bEnemy_Slow && !bEnemy_Stan)
		{
			Cube_Draw(texture, Enemy_PosX, Enemy_PosY, Enemy_PosZ, 1.0f, 1.0f, 1.0f);

			Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 0.5f - (0.5f - (0.5f*cos(Enemy_AngleUp))), Enemy_PosZ - (0.5f*sin(Enemy_AngleUp)), 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 1.0f - (1.0f - (1.0f*cos(Enemy_AngleUp))), Enemy_PosZ - (1.0f*sin(Enemy_AngleUp)), 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX, Enemy_PosY + 1.5f - (1.5f - (1.5f*cos(Enemy_AngleUp))), Enemy_PosZ - (1.5f*sin(Enemy_AngleUp)), 0.5f, 0.5f, 0.5f);

			Cube_Draw(texture, Enemy_PosX + 0.5f - (0.5f - (0.5f*cos(Enemy_AngleRight))), Enemy_PosY, Enemy_PosZ - (0.5f*sin(Enemy_AngleRight)), 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX + 1.0f - (1.0f - (1.0f*cos(Enemy_AngleRight))), Enemy_PosY, Enemy_PosZ - (1.0f*sin(Enemy_AngleRight)), 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX + 1.5f - (1.5f - (1.5f*cos(Enemy_AngleRight))), Enemy_PosY, Enemy_PosZ - (1.5f*sin(Enemy_AngleRight)), 0.5f, 0.5f, 0.5f);

			Cube_Draw(texture, Enemy_PosX - 0.5f + (0.5f - (0.5f*cos(Enemy_AngleLeft))), Enemy_PosY, Enemy_PosZ - (0.5f*sin(Enemy_AngleLeft)), 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX - 1.0f + (1.0f - (1.0f*cos(Enemy_AngleLeft))), Enemy_PosY, Enemy_PosZ - (1.0f*sin(Enemy_AngleLeft)), 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX - 1.5f + (1.5f - (1.5f*cos(Enemy_AngleLeft))), Enemy_PosY, Enemy_PosZ - (1.5f*sin(Enemy_AngleLeft)), 0.5f, 0.5f, 0.5f);

			Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 0.5f + (0.5f - (0.5f*cos(Enemy_AngleDown))), Enemy_PosZ - (0.5f*sin(Enemy_AngleDown)), 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 1.0f + (1.0f - (1.0f*cos(Enemy_AngleDown))), Enemy_PosZ - (1.0f*sin(Enemy_AngleDown)), 0.5f, 0.5f, 0.5f);
			Cube_Draw(texture, Enemy_PosX, Enemy_PosY - 1.5f + (1.5f - (1.5f*cos(Enemy_AngleDown))), Enemy_PosZ - (1.5f*sin(Enemy_AngleDown)), 0.5f, 0.5f, 0.5f);
		}


			/*	if (bEnemy_Slow)
				{
					Cube_Draw(TEXTURE_INDEX_ENEMY_SLOW, Enemy_PosX, Enemy_PosY, Enemy_PosZ, 1.0f, 1.0f, 1.0f, Enemy_AngleX, Enemy_AngleY, Enemy_AngleZ);
				}
				if (bEnemy_Stan)
				{
					Cube_Draw(TEXTURE_INDEX_ENEMY_STAN, Enemy_PosX, Enemy_PosY, Enemy_PosZ, 1.0f, 1.0f, 1.0f, Enemy_AngleX, Enemy_AngleY, Enemy_AngleZ);
				}*/

		//HPケージ
		Sprite_Draw(TEXTURE_INDEX_HPCAGE, 10, 10);
		float HP_W;
		HP_W = ((float)Enemy_HP / HP_MAX) * 780;
		Sprite_Draw(TEXTURE_INDEX_HP, 20, 20, 0, 0, HP_W, 30);

		//Dangerケージ
		if (1/*bEnemy_Effective&&bEnemy_Advance*/)
		{
			Sprite_Draw(TEXTURE_INDEX_DANGERCAGE, 10, 70);
			float Danger_W;
			Danger_W = ((float)Enemy_Danger / DANGER_MAX) * 490;
			Sprite_Draw(TEXTURE_INDEX_DANGER, 15, 75, 0, 0, Danger_W, 30);
		}



		DebugFont_Draw(50, 100, "Enemy_Indicator.posX: %.2f", Enemy_Indicator.posX);
		DebugFont_Draw(50, 150, "Enemy_Indicator.posY: %.2f", Enemy_Indicator.posY);
	}
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
bool Enemy_Get_bSpecialAttack(void)
{
	return bEnemy_SpecialAttack;
}
void Enemy_Set_Damage(int damage)
{
	Enemy_HP--;
	bEnemy_Injured = true;

	if (!bEnemy_SpecialAttack)
	{
		Enemy_Danger-= damage;
		//弱点位置変わる
		counter_mode = rand() % 4 + 1;
		//時計回りか反時計回り
		bEnemy_Clockwise = rand() % 2;
		//回転速度変わる
		Enemy_RotationAdd = (rand() % 5 + 1)*0.010f;
	}



	if (Enemy_HP < 0)
	{
		Enemy_HP = 0;
	}
	if (Enemy_Danger < 0)
	{
		Enemy_Danger = 0;
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
	bEnemy_SpecialAttackStart = bRotation;
}

void Enemy_InjuredEffect(int injured_cnt)
{
	if (injured_cnt == 0)
	{

	}
	else if(injured_cnt < INJURED_TIME)
	{
		if (injured_cnt % 4 == 2)
		{
			Enemy_PosX += 0.1f;
			Enemy_PosY += 0.1f;
			Enemy_PosZ -= 0.1f;
		}
		if (injured_cnt % 4 == 0)
		{
			Enemy_PosX -= 0.1f;
			Enemy_PosY -= 0.1f;
			Enemy_PosZ += 0.1f;
		}
	}
	else if(injured_cnt == INJURED_TIME)
	{
		Enemy_PosX -= 0.1f;
		Enemy_PosY -= 0.1f;
		Enemy_PosZ += 0.1f;
	}
	else if(injured_cnt > INJURED_TIME)
	{
		return;
	}
}

indicator Enemy_Getindicator(void)
{
	return Enemy_Indicator;
}
