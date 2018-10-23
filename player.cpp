#include "cube.h"
#include "texture.h"
#include "input.h"
#include "enemy.h"
#include "player.h"
#include "debug_font.h"
#include "bullet.h"
#include "joycon.h"
#include "camera.h"
#include "sprite.h"
#include "fade.h"

#define	AVOIDANCE_TIME	(20)
#define HP_MAX			(10)
#define CHARGE_MAX		(30)

static float Player_PosX;
static float Player_PosY;
static float Player_PosZ;

static float Player_Speed;
static float Player_Move;
static int Player_Mode;

static int Player_HP;

static bool bPlayer_Counter;
static bool bPlayer_Injured;

static bool bPlayer_NormalAttack;
static bool bPlayer_ChargeAttack;

static int Player_Charge;

static int avoidance_frame_cnt;
static int attack_frame_cnt;

static int time_cnt;

bool leftFlag = false;
bool rightFlag = false;
bool downFlag = false;
bool upFlag = false;
static float AccAccinX, AccAccinXOld1, AccAccinXOld2;
static float AccAccinY, AccAccinYOld1, AccAccinYOld2;
static float AccAccinZ, AccAccinZOld1, AccAccinZOld2;

bool bChargeStart;
void AccAcc(void);
void Player_Initialize(void)
{
	Player_PosX = 0.0f;
	Player_PosY = 0.0f;
	Player_PosZ = -3.0f;

	Player_Speed = 0.05f;
	Player_Move = 0.0f;
	Player_Mode = NORMAL_MODE;

	Player_HP = HP_MAX;
	AccAccinX, AccAccinXOld1, AccAccinXOld2 = 0;
	AccAccinY, AccAccinYOld1, AccAccinYOld2 = 0;
	AccAccinZ, AccAccinZOld1, AccAccinZOld2 = 0;
	bPlayer_Counter = false;
	bPlayer_Injured = false;
	bPlayer_NormalAttack = false;
	bPlayer_ChargeAttack = false;

	Player_Charge = 0;

	avoidance_frame_cnt = 0;
	attack_frame_cnt = 0;

	bChargeStart = false;
}

void Player_Update(void)
{
	if (Player_HP > 0) 
	{
		//回避
		if (/*Enemy_Get_bEffective()*/1)
		{
			//左回避
			if (JoyconVector(1) < 250 && GetAccel(1).x > 3.0 && GetJyro(1).x < -1.0  && Player_Mode == NORMAL_MODE)
			{
				Player_Mode = MOVE_MODE_L;
				Camera_Set_Left(true);
			}
			if (Player_Mode == AVOIDANCE_MODE_L)
			{
				avoidance_frame_cnt++;
			}
			if (avoidance_frame_cnt>= AVOIDANCE_TIME && Player_Mode == AVOIDANCE_MODE_L)
			{
				Player_Mode = RETURN_MODE_L;
			}
			if (Player_Mode == NORMAL_MODE)
			{
				avoidance_frame_cnt = 0;
			}

			//右回避
			if (JoyconVector(1) < 250&&GetAccel(1).x < -3.0 && GetJyro(1).x > 1.0 && Player_Mode == NORMAL_MODE)
			{
				Player_Mode = MOVE_MODE_R;
				Camera_Set_Right(true);
			}
			if (Player_Mode == AVOIDANCE_MODE_R)
			{
				avoidance_frame_cnt++;
			}
			if (avoidance_frame_cnt >= AVOIDANCE_TIME && Player_Mode == AVOIDANCE_MODE_R)
			{
				Player_Mode = RETURN_MODE_R;
			}
			if (Player_Mode == NORMAL_MODE)
			{
				avoidance_frame_cnt = 0;
			}
		}
		//左回避
		if (Player_Mode == MOVE_MODE_L)
		{
			Player_PosX -= Player_Speed;
			Player_Move += Player_Speed;

			if (Player_Move > 1.0f)
			{
				Player_Mode = AVOIDANCE_MODE_L;
				Player_Move = 0.0f;
			}
		}
		if (Player_Mode == RETURN_MODE_L)
		{
			Player_PosX += Player_Speed;
			Player_Move += Player_Speed;
			if (Player_Move > 1.0f)
			{
				Player_Mode = NORMAL_MODE;
				Player_Move = 0.0f;
			}
		}
		//右回避
		if (Player_Mode == MOVE_MODE_R)
		{
			Player_PosX += Player_Speed;
			Player_Move += Player_Speed;

			if (Player_Move > 1.0f)
			{
				Player_Mode = AVOIDANCE_MODE_R;
				Player_Move = 0.0f;
			}
		}
		if (Player_Mode == RETURN_MODE_R)
		{
			Player_PosX -= Player_Speed;
			Player_Move += Player_Speed;
			if (Player_Move > 1.0f)
			{
				Player_Mode = NORMAL_MODE;
				Player_Move = 0.0f;
			}
		}

		//普通攻撃
		if (Enemy_Get_bSlow())
		{
			if ((GetAccel(0).y < -40.0f && GetJyro(0).z <-10.0f)||
				(GetAccel(0).y > 10.0f && GetJyro(0).z > 3.0f)||
				(GetAccel(0).z <-30.0f && GetJyro(0).x < -10.0f)||
				(GetAccel(0).z > 40.0f && GetJyro(0).x >-10.0f))
			{
				if (!bPlayer_NormalAttack)
				{
					bPlayer_NormalAttack = true;
				}
			}
		}
		if (bPlayer_NormalAttack)
		{
			Bullet_Create(Player_PosX, Player_PosY, Player_PosZ);
			bPlayer_NormalAttack = false;
			if (Player_Mode == NORMAL_MODE)
			{
				Enemy_Set_Damage();
			}
		}

		//ふる方向判断
		AccAcc();

		//スタン
		if (Enemy_Get_bEffective()&& !Enemy_Get_bStan()&& Enemy_Get_bAdvance()&& !Enemy_Get_bSlow())
		{
			if (Player_Mode == NORMAL_MODE)
			{
				if((Enemy_Get_Mode() == COUNTER_UP && AccAccinY > 10.0f&& GetJyro(0).z < -10.0f) ||
					(Enemy_Get_Mode() == COUNTER_DOWN && AccAccinZ >-10.0f&& GetJyro(0).x > -15.0f&&AccAccinY< -10.0f&& GetJyro(0).z > 10.0f)||
					(Enemy_Get_Mode() == COUNTER_LEFT && AccAccinZ <-10.0f&& GetJyro(0).x < -15.0f)||
					(Enemy_Get_Mode() == COUNTER_RIGHT && AccAccinZ > 10.0f&& GetJyro(0).x >15.0f))
				{
					bPlayer_Counter = true;
					Enemy_Set_bRotation(false);
				}
			}
		}

		///////////////////////////// 
		//＝＝＝＝＝左＝＝＝＝＝
		if (
			AccAccinZ <-10.0f 
			&& GetJyro(0).x < -15.0f
			) {
			leftFlag = true;
		}

		//＝＝＝＝＝右＝＝＝＝＝
		if (
			AccAccinZ > 10.0f
			&& GetJyro(0).x >15.0f
			) {
			rightFlag = true;
		}

		//＝＝＝＝＝下＝＝＝＝＝
		if (
			AccAccinZ >-10.0f
			&& GetJyro(0).x > -15.0f
			&&AccAccinY< -10.0f
			&& GetJyro(0).z > 10.0f
			) {
			downFlag = true;
		}

		//＝＝＝＝＝上＝＝＝＝＝
		if (
			AccAccinY > 10.0f
			&& GetJyro(0).z < -10.0f
			) 
		{
			upFlag = true;
		}

		////////////////////////////////////////



		//チャージ
		if (bPlayer_Counter)
		{
			if (GetAccel(0).x<-3.0f&&
				//GetAccel(0).x>-1.0f&&
				//GetAccel(0).y<10.0f&&
				//GetAccel(0).y>6.0f&&
				GetAccel(0).z>-3.0f&&
				GetAccel(0).z<3.0f
				&&GetJyro(0).x<1.0f&&
				GetJyro(0).x>-1.0f&&
				GetJyro(0).y<1.0f&&
				GetJyro(0).y>-1.0f&&
				GetJyro(0).z<1.0f&&
				GetJyro(0).z>-1.0f)
			{
				if (!bChargeStart) {
					Fade_Start(true, 50, D3DCOLOR_RGBA(0, 0, 0, 0));
					bChargeStart = true;
				}
				Player_Charge++;
				if (Player_Charge > 30)
				{
					Player_Charge = 30;
					Rumble(0);
				}
			}
			if ((GetJyro(0).x > 1.0f || GetJyro(0).x<-1.0f || GetJyro(0).y>1.0f || GetJyro(0).y<-1.0f || GetJyro(0).z>1.0f || GetJyro(0).z < -1.0f) && Player_Charge < 30)
			{
				Fade_Start(false, 1, D3DCOLOR_RGBA(0, 0, 0, 0));
				bChargeStart = false;
			}
		}
		if (bChargeStart && !bPlayer_Counter)
		{
			Fade_Start(false, 1, D3DCOLOR_RGBA(0, 0, 0, 0));
			bChargeStart = false;
		}


		//必殺技
		if (GetAccel(0).y < -50.0f && Player_Charge == 30)
		{
			Enemy_Set_BigDamage();
			Player_Charge = 0;
			bChargeStart = false;
			//Fade_Start(true, 60, D3DCOLOR_RGBA(255, 255, 10, 0));
			Camera_Set_ChargeAttack(true);
			bPlayer_ChargeAttack = true;
		}
	}


}

void Player_Draw(void)
{
	if (Player_HP > 0)
	{
		//プレイヤー
		Cube_Draw(TEXTURE_INDEX_PLAYER, Player_PosX, Player_PosY, Player_PosZ, 1.0f, 1.0f, 1.0f, 0,0,0);
	}

	//HPケージ
	Sprite_Draw(TEXTURE_INDEX_HPCAGE, 10,650);
	float HP_W;
	HP_W = ((float)Player_HP / HP_MAX) * 780;
	Sprite_Draw(TEXTURE_INDEX_HP, 20, 660, 0, 0, HP_W, 30);

	//チャージケージ
	Sprite_Draw(TEXTURE_INDEX_CHARGECAGE, 780, 120);
	float CHARGE_H;
	CHARGE_H = ((float)Player_Charge / CHARGE_MAX) * 490;
	Sprite_Draw(TEXTURE_INDEX_CHARGE, 785, 125+(490- CHARGE_H), 0, 0, 20, CHARGE_H);


	if (Enemy_Get_bSlow())
	{
		DebugFont_Draw(500, 100, "敵の速度下がった！");
	}

	if (bPlayer_Counter)
	{
		DebugFont_Draw(500, 100, "敵がスタン！");
		if (Player_Charge == 30)
		{
			DebugFont_Draw(500, 150, "チャージ完了！");
		}
	}

	if (leftFlag&&time_cnt<30) {
		time_cnt++;
		DebugFont_Draw(100, 200, "左");
		if (time_cnt == 30)
		{
			leftFlag = false;
			time_cnt = 0;
		}
	}
	if (rightFlag&&time_cnt<30) {
		time_cnt++;
		DebugFont_Draw(100, 250, "右");
		if (time_cnt == 30)
		{
			rightFlag = false;
			time_cnt = 0;
		}
	}
	if (upFlag&&time_cnt<30) {
		time_cnt++;
		DebugFont_Draw(100, 300, "上");
		if (time_cnt == 30)
		{
			upFlag = false;
			time_cnt = 0;
		}
	}
	if (downFlag&&time_cnt<30) {
		time_cnt++;
		DebugFont_Draw(100, 350, "下");
		if (time_cnt == 30)
		{
			downFlag = false;
			time_cnt = 0;
		}
	}

	/*DebugFont_Draw(500, 150, "ax: %f", AccAccinX);
	DebugFont_Draw(500, 200, "ay: %f", AccAccinY);
	DebugFont_Draw(500, 250, "az: %f", AccAccinZ);
	DebugFont_Draw(500, 300, "g1: %f", GetJyro(0).x);
	DebugFont_Draw(500, 350, "g2: %f", GetJyro(0).y);
	DebugFont_Draw(500, 400, "g3: %f", GetJyro(0).z);*/
	//DebugFont_Draw(800, 350, "Charge: %d", Player_Charge);
}

int Player_Get_Mode(void)
{
	return Player_Mode;
}

void Player_Set_Damage(void)
{
	Player_HP--;
	if (Player_HP < 0)
	{
		Player_HP = 0;
	}
}

void Player_Set_Charge(int charge)
{
	Player_Charge = charge;
}

bool Player_Get_bCounter(void)
{
	return bPlayer_Counter;
}

bool Player_Get_bChargeAttack(void)
{
	return bPlayer_ChargeAttack;
}

void Player_Set_bChargeAttack(bool bChargeAttack)
{
	bPlayer_ChargeAttack= bChargeAttack;
}

void Player_Set_bCounter(bool bCounter)
{
	bPlayer_Counter= bCounter;
}

bool Player_Get_bInjured(void)
{
	return bPlayer_Injured;
}

void Player_Set_bInjured(bool bInjured)
{
	bPlayer_Injured= bInjured;
}

int Player_Get_HP(void)
{
	return Player_HP;
}

void AccAcc(void)
{
	AccAccinXOld1 = AccAccinXOld2;
	AccAccinXOld2 = GetAccel(0).x;
	AccAccinX = AccAccinXOld2 - AccAccinXOld1;

	AccAccinYOld1 = AccAccinYOld2;
	AccAccinYOld2 = GetAccel(0).y;
	AccAccinY = AccAccinYOld2 - AccAccinYOld1;

	AccAccinZOld1 = AccAccinZOld2;
	AccAccinZOld2 = GetAccel(0).z;
	AccAccinZ = AccAccinZOld2 - AccAccinZOld1;
}