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
#include "judgement.h"

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
static int attack_cnt;

static bool leftFlag = false;
static bool rightFlag = false;
static bool downFlag = false;
static bool upFlag = false;

//����̎΂߂̐U��������
static bool slantingLeftUpFlag = false;
static bool slantingRightUpFlag = false;
static bool slantingLeftDownFlag = false;
static bool slantingRightDownFlag = false;

//�U�����Ă��玟�ɍU������܂ł̊Ԋu
static bool attackFlag = false;
static int attackTimeCnt;

//�U������
static bool attackLeftFlag = false;
static bool attackRightFlag = false;
static bool attackUpFlag = false;
static bool attackDownFlag = false;
static bool attackSlantingLeftUpFlag = false;
static bool attackSlantingRightUpFlag = false;
static bool attackSlantingLeftDownFlag = false;
static bool attackSlantingRightDownFlag = false;

//�_���[�W�̑���
static int damage;

//�U���̋����ɂ��ω�����a���̊g�嗦
struct Scale {
	float left;
	float right;
	float up;
	float down;
	float slantingLeftUp;
	float slantingRightUp;
	float slantingLeftDown;
	float slantingRightDown;
};
Scale scale;


static float AccAccinX, AccAccinXOld1, AccAccinXOld2;
static float AccAccinY, AccAccinYOld1, AccAccinYOld2;
static float AccAccinZ, AccAccinZOld1, AccAccinZOld2;

bool bChargeStart;

static Direction Attack_Dir;

void AccAcc(void);
void AttackFlagReset();

float AttackSwingWidth(float accelX, float accelY, float accelZ, float jyroX, float jyroY, float jyroZ);
float AttackDownSwingWidth(float accelX, float accelY, float accelZ, float jyroX, float jyroY, float jyroZ);
float AttackRightSwingWidth(float accelX, float accelY, float accelZ, float jyroX, float jyroY, float jyroZ);
float AttackLeftSwingWidth(float accelX, float accelY, float accelZ, float jyroX, float jyroY, float jyroZ);
float AttackSlantingRightUpSwingWidth(float accelX, float accelY, float accelZ, float jyroX, float jyroY, float jyroZ);
float AttackSlantingLeftUpSwingWidth(float accelX, float accelY, float accelZ, float jyroX, float jyroY, float jyroZ);
float AttackSlantingLeftDownSwingWidth(float accelX, float accelY, float accelZ, float jyroX, float jyroY, float jyroZ);
float AttackSlantingRightDownSwingWidth(float accelX, float accelY, float accelZ, float jyroX, float jyroY, float jyroZ);

void PlayerHeal();

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

	attackLeftFlag = false;
	attackRightFlag = false;
	attackUpFlag = false;
	attackDownFlag = false;
	attackSlantingLeftUpFlag = false;
	attackSlantingRightUpFlag = false;
	attackSlantingLeftDownFlag = false;
	attackSlantingRightDownFlag = false;

	//�_���[�W�̑���
	damage=0;

	scale = { 1.0f };

	bChargeStart = false;

	attackTimeCnt = 0;

	Attack_Dir = END;
}

void Player_Update(void)
{
	D3DXVECTOR3 accel = GetAccel(0);
	D3DXVECTOR3 jyro = GetJyro(0);

	if (Player_HP > 0)
	{
		//���
		if (/*Enemy_Get_bEffective()*/1)
		{
			//�����
			if (GetJoyconDevice(1).btns.zr == 1 && Player_Mode == NORMAL_MODE)
			{
				Player_Mode = MOVE_MODE_L;
				Camera_Set_Left(true);
			}
			if (Player_Mode == AVOIDANCE_MODE_L)
			{
				avoidance_frame_cnt++;
			}
			if (avoidance_frame_cnt >= AVOIDANCE_TIME && Player_Mode == AVOIDANCE_MODE_L)
			{
				Player_Mode = RETURN_MODE_L;
			}
			if (Player_Mode == NORMAL_MODE)
			{
				avoidance_frame_cnt = 0;
			}
			//�E���
			if (JoyconVector(1) < 250 && GetAccel(1).x < -3.0 && GetJyro(0).x > 1.0 && Player_Mode == NORMAL_MODE)
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
		//�����
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
		//�E���
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

		//���ʍU��
		//Attack_Dir = END;
		if (/*Enemy_Get_bSlow() && */!attackFlag)
		{
			////���΂ߏ�U��
			//if (AccAccinY > 6.0f && GetJyro(0).z < -8.0f
			//	&& AccAccinZ < -7.0f && GetJyro(0).x < -8.0f) {
			//	if (!bPlayer_NormalAttack)
			//	{
			//		bPlayer_NormalAttack = true;
			//		attackSlantingLeftUpFlag = true;
			//	}
			//}
			////�E�΂ߏ�U��
			//if (AccAccinY > 6.0f && GetJyro(0).z < -8.0f
			//	&& AccAccinZ > 7.0f && GetJyro(0).x > 4.0f) {
			//	if (!bPlayer_NormalAttack)
			//	{
			//		bPlayer_NormalAttack = true;
			//		attackSlantingRightUpFlag = true;
			//	}
			//}
			////���΂߉��U��
			//if (AccAccinZ <-10.0f
			//	&& GetJyro(0).x < -15.0f &&AccAccinY < -10.0f
			//	&& GetJyro(0).z > 7.0f
			//	) {
			//	if (!bPlayer_NormalAttack)
			//	{
			//		bPlayer_NormalAttack = true;
			//		attackSlantingLeftDownFlag = true;
			//	}
			//}
			////�E�΂߉��U��
			//if (AccAccinZ > -6.0f && GetJyro(0).x > -10.0f && AccAccinY < -7.0f && GetJyro(0).z > 7.0f
			//	&& AccAccinZ > 7.0f && GetJyro(0).x > 15.0f) {
			//	if (!bPlayer_NormalAttack)
			//	{
			//		bPlayer_NormalAttack = true;
			//		attackSlantingRightDownFlag = true;
			//	}
			//}
			//���U��
			if (AccAccinZ <-10.0f && jyro.x < -15.0f
				|| accel.z > 9.0 && AccAccinZ > -10.0f && GetJyro(0).x > -15.0f &&AccAccinY < -10.0f && GetJyro(0).z > 10.0f) {
				if (1)
				{
					bPlayer_NormalAttack = true;
					attackLeftFlag = true;
					damage = AttackLeftSwingWidth(accel.x, accel.y, accel.z, jyro.x, jyro.y, jyro.z);
					if (damage < 0) {
						damage = 1;
					}
				}
			}
			//�E�U��
			if (AccAccinZ > -10.0f && GetJyro(0).x > -15.0f
				&&AccAccinY< -10.0f && GetJyro(0).z > 10.0f && accel.z < -9.0 || AccAccinZ > 10.0f && GetJyro(0).x > 15.0f) {
				if (1)
				{
					//bPlayer_NormalAttack = true;
					attackRightFlag = true;
					damage = AttackRightSwingWidth(accel.x, accel.y, accel.z, jyro.x, jyro.y, jyro.z);
					if (damage < 0) {
						damage = 1;
					}
				}
			}
			//��U��
			if (AccAccinY > 10.0f && GetJyro(0).z < -10.0f) {
				if (1)
				{
					//bPlayer_NormalAttack = true;
					attackUpFlag = true;
					damage = AttackSwingWidth(accel.x, accel.y, accel.z, jyro.x, jyro.y, jyro.z);
					if (damage < 0) {
						damage = 1;
					}
				}
			}
			//���U��
			if (AccAccinZ >-10.0f && GetJyro(0).x > -15.0f
				&&AccAccinY< -10.0f && GetJyro(0).z > 10.0f) {
				if (1)
				{
					//bPlayer_NormalAttack = true;
					attackDownFlag = true;
					damage = AttackDownSwingWidth(accel.x, accel.y, accel.z, jyro.x, jyro.y, jyro.z);
				}
			}
			//DEBUG�L�[�{�[�h
			if (Keyboard_IsTrigger(DIK_A))
			{
				if (!bPlayer_NormalAttack)
				{
					bPlayer_NormalAttack = true;
				}
			}
			if (Judgement(Attack_Dir, Enemy_Getindicator()))
			{
				if (!bPlayer_NormalAttack)
				{
					bPlayer_NormalAttack = true;
				}
			}
			
		}
		if (bPlayer_NormalAttack)
		{
			//Bullet_Create(Player_PosX, Player_PosY, Player_PosZ);
			bPlayer_NormalAttack = false;
			attackFlag = true;

			if (Enemy_Get_bEffective() == true)
			{
				Enemy_Set_Damage(damage);
			}
		}
		if (attackFlag) {
			attackTimeCnt++;
		}

		if (attackTimeCnt > 20) {
			attackFlag = false;
			attackTimeCnt = 0;
		}

		//�ӂ�������f
		AccAcc();

		//�X�^��
		if (Enemy_Get_bSpecialAttack() && Enemy_Get_bEffective() && !Enemy_Get_bStan() && Enemy_Get_bAdvance() && !Enemy_Get_bSlow())
		{
			if (Player_Mode == NORMAL_MODE)
			{
				if ((Enemy_Get_Mode() == COUNTER_UP && AccAccinY > 10.0f&&     jyro.z < -10.0f) ||
					(Enemy_Get_Mode() == COUNTER_DOWN && AccAccinZ >-10.0f&&  jyro.x > -15.0f&&AccAccinY< -10.0f&& GetJyro(0).z > 10.0f) ||
					(Enemy_Get_Mode() == COUNTER_LEFT && AccAccinZ <-10.0f&&  jyro.x < -15.0f) ||
					(Enemy_Get_Mode() == COUNTER_RIGHT && AccAccinZ > 10.0f&& jyro.x >15.0f))
				{
					bPlayer_Counter = true;
					Enemy_Set_bRotation(false);
				}
			}
		}

		///////////////////////////// 
		//�����������E�΂ߏぁ��������
		if (AccAccinY > 6.0f && GetJyro(0).z < -8.0f
			&& AccAccinZ > 7.0f && GetJyro(0).x > 4.0f) {
			slantingRightUpFlag = true;
			upFlag = false;
			rightFlag = false;
		}
		//�������������΂ߏぁ��������
		if (AccAccinY > 6.0f && GetJyro(0).z < -8.0f
			&& AccAccinZ < -7.0f && GetJyro(0).x < -8.0f) {
			slantingLeftUpFlag = true;
			upFlag = false;
			leftFlag = false;
		}
		//�������������΂߉�����������
		if (
			AccAccinZ <-10.0f
			&& GetJyro(0).x < -15.0f &&AccAccinY < -10.0f
			&& GetJyro(0).z > 10.0f
			) {
			slantingLeftDownFlag = true;
			downFlag = false;
			leftFlag = false;
		}
		//�����������E�΂߉�����������
		if (AccAccinZ > -6.0f && GetJyro(0).x > -10.0f && AccAccinY < -7.0f && GetJyro(0).z > 7.0f
			&& AccAccinZ > 7.0f && GetJyro(0).x > 15.0f) {
			slantingRightDownFlag = true;
			downFlag = false;
			leftFlag = false;
		}

		//����������������������
		if (
			AccAccinZ <-10.0f
			&& GetJyro(0).x < -15.0f && !slantingLeftUpFlag
			) {
			leftFlag = true;
		}

		//�����������E����������
		if (
			AccAccinZ > 10.0f
			&& GetJyro(0).x >15.0f && !slantingRightUpFlag
			) {
			rightFlag = true;
		}

		//����������������������
		if (
			AccAccinZ >-10.0f
			&& GetJyro(0).x > -15.0f
			&&AccAccinY< -10.0f
			&& GetJyro(0).z > 10.0f
			) {
			downFlag = true;
		}

		//�����������ぁ��������
		if (
			AccAccinY > 10.0f
			&& GetJyro(0).z < -10.0f && !slantingRightUpFlag && !slantingLeftUpFlag
			)
		{
			upFlag = true;
		}
		////////////////////////////////////////



		//�`���[�W
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


		//�K�E�Z
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
		//�v���C���[
		Cube_Draw(TEXTURE_INDEX_PLAYER, Player_PosX, Player_PosY, Player_PosZ, 1.0f, 1.0f, 1.0f);
	}

	//HP�P�[�W
	Sprite_Draw(TEXTURE_INDEX_HPCAGE, 10, 650);
	float HP_W;
	HP_W = ((float)Player_HP / HP_MAX) * 780;
	Sprite_Draw(TEXTURE_INDEX_HP, 20, 660, 0, 0, HP_W, 30);

	//�`���[�W�P�[�W
	Sprite_Draw(TEXTURE_INDEX_CHARGECAGE, 780, 120);
	float CHARGE_H;
	CHARGE_H = ((float)Player_Charge / CHARGE_MAX) * 490;
	Sprite_Draw(TEXTURE_INDEX_CHARGE, 785, 125 + (490 - CHARGE_H), 0, 0, 20, CHARGE_H);

	//Sprite_Draw(TEXTURE_INDEX_CHARGE, 500, 400);

	if (Enemy_Get_bSlow())
	{
		DebugFont_Draw(500, 100, "�G�̑��x���������I");
	}

	if (bPlayer_Counter)
	{
		DebugFont_Draw(500, 100, "�G���X�^���I");
		if (Player_Charge == 30)
		{
			DebugFont_Draw(500, 150, "�`���[�W�����I");
		}
	}

	if (leftFlag&&time_cnt<30) {
		time_cnt++;
		DebugFont_Draw(100, 200, "��");
		if (time_cnt == 30)
		{
			leftFlag = false;
			time_cnt = 0;
		}
	}
	if (rightFlag&&time_cnt<30) {
		time_cnt++;
		DebugFont_Draw(100, 250, "�E");
		if (time_cnt == 30)
		{
			rightFlag = false;
			time_cnt = 0;
		}
	}
	if (upFlag&&time_cnt<30) {
		time_cnt++;
		DebugFont_Draw(100, 300, "��");
		if (time_cnt == 30)
		{
			upFlag = false;
			time_cnt = 0;
		}
	}
	if (downFlag&&time_cnt<30) {
		time_cnt++;
		DebugFont_Draw(100, 350, "��");
		if (time_cnt == 30)
		{
			downFlag = false;
			time_cnt = 0;
		}
	}
	if (slantingRightUpFlag&&time_cnt<30) {
		time_cnt++;
		DebugFont_Draw(100, 400, "�E�΂ߏ�");
		if (time_cnt == 30)
		{
			slantingRightUpFlag = false;
			time_cnt = 0;
		}
	}
	if (slantingLeftUpFlag&&time_cnt<30) {
		time_cnt++;
		DebugFont_Draw(100, 450, "���΂ߏ�");
		if (time_cnt == 30)
		{
			slantingLeftUpFlag = false;
			time_cnt = 0;
		}
	}
	if (slantingLeftDownFlag&&time_cnt<30) {
		time_cnt++;
		DebugFont_Draw(150, 400, "���΂߉�");
		if (time_cnt == 30)
		{
			slantingLeftDownFlag = false;
			time_cnt = 0;
		}
	}
	if (slantingRightDownFlag&&time_cnt<20) {
		time_cnt++;
		DebugFont_Draw(150, 350, "�E�΂߉�");
		if (time_cnt == 20)
		{
			slantingRightDownFlag = false;
			time_cnt = 0;
		}
	}
	//--�X���[��ԂōU��������Ƃ��Ɍ��M��`�悷��--//

	if (attackLeftFlag) {
		Sprite_Draw(TEXTURE_INDEX_SWORD, 40, 250, 0, 0, 225, 225, 112, 112, 1.0f, 1.0f, 0.8f);
		attack_cnt++;
		if (attack_cnt == 20)
		{
			attackLeftFlag = false;
			attack_cnt = 0;
		}
	}
	if (attackRightFlag) {
		Sprite_Draw(TEXTURE_INDEX_SWORD, 470, 250, 0, 0, 225, 225, 112, 112, 1.0f, 1.0f, 0.8f);
		attack_cnt++;
		if (attack_cnt == 20)
		{
			attackRightFlag = false;
			attack_cnt = 0;
		}
	}
	if (attackUpFlag) {
		Sprite_Draw(TEXTURE_INDEX_SWORD, 300, 50, 0, 0, 225, 225, 112, 112, 1.0f, 1.0f, -0.8f);
		attack_cnt++;
		if (attack_cnt == 20)
		{
			attackUpFlag = false;
			attack_cnt = 0;
		}
	}
	if (attackDownFlag) {
		Sprite_Draw(TEXTURE_INDEX_SWORD, 300, 450, 0, 0, 225, 225, 112, 112, 1.0f, 1.0f, -0.8f);
		attack_cnt++;
		if (attack_cnt == 20)
		{
			attackDownFlag = false;
			attack_cnt = 0;
		}
	}
	if (attackSlantingLeftUpFlag) {
		Sprite_Draw(TEXTURE_INDEX_SWORD, 100, 70, 0, 0, 225, 225, 112, 112, 1.0f, 1.0f, -1.6f);
		attack_cnt++;
		if (attack_cnt == 20)
		{
			attackSlantingLeftUpFlag = false;
			attack_cnt = 0;
		}
	}
	if (attackSlantingRightUpFlag) {
		Sprite_Draw(TEXTURE_INDEX_SWORD, 500, 100, 0, 0, 225, 225, 112, 112, 1.0f, 1.0f, 0.0f);
		attack_cnt++;
		if (attack_cnt == 20)
		{
			attackSlantingRightUpFlag = false;
			attack_cnt = 0;
		}
	}
	if (attackSlantingRightDownFlag) {
		Sprite_Draw(TEXTURE_INDEX_SWORD, 470, 400, 0, 0, 225, 225, 112, 112, 1.0f, 1.0f, 1.6f);
		attack_cnt++;
		if (attack_cnt == 20)
		{
			attackSlantingRightDownFlag = false;
			attack_cnt = 0;
		}
	}
	if (attackSlantingLeftDownFlag) {
		Sprite_Draw(TEXTURE_INDEX_SWORD, 70, 400, 0, 0, 225, 225, 112, 112, 1.0f, 1.0f, 0.0);
		attack_cnt++;
		if (attack_cnt == 20)
		{
			attackSlantingLeftDownFlag = false;
			attack_cnt = 0;
		}
	}

	//--�����܂Ō��M�`��--//

	/*DebugFont_Draw(500, 150, "ax: %f", AccAccinX);
	DebugFont_Draw(500, 200, "ay: %f", AccAccinY);
	DebugFont_Draw(500, 250, "az: %f", AccAccinZ);
	DebugFont_Draw(500, 300, "g1: %f", GetJyro(0).x);
	DebugFont_Draw(500, 350, "g2: %f", GetJyro(0).y);
	DebugFont_Draw(500, 400, "g3: %f", GetJyro(0).z);*/

	//DebugFont_Draw(800, 350, "Charge: %d", Player_Charge);
	/*
	//�E�΂ߏ�
	Sprite_Draw(TEXTURE_INDEX_CHARGE, 500, 50, 0, 0, 10, 300, 5, 250, 1.0f, 1.0f, 0.7f);
	//��
	Sprite_Draw(TEXTURE_INDEX_CHARGE, 470, 400,0, 0, 300, 10, 150, 5, 1.0f, 1.0f, 0.0f);
	//�E�΂߉�
	Sprite_Draw(TEXTURE_INDEX_CHARGE, 470, 550,0, 0, 300, 10, 150, 5, 1.0f, 1.0f, 0.6f);
	//��
	Sprite_Draw(TEXTURE_INDEX_CHARGE, 400, 450,0, 0, 10, 300, 5, 150, 1.0f, 1.0f, 0.0f);
	//���΂߉�
	Sprite_Draw(TEXTURE_INDEX_CHARGE, 60, 550, 0, 0, 300, 10, 150, 5, 1.0f, 1.0f, -0.7f);
	//��
	Sprite_Draw(TEXTURE_INDEX_CHARGE, 40, 400, 0, 0, 300, 10, 150, 5, 1.0f, 1.0f, 0.0f);
	//���΂ߏ�
	Sprite_Draw(TEXTURE_INDEX_CHARGE, 300,70, 0, 0, 10, 300, 5, 250, 1.0f, 1.0f, -0.7f);
	//��
	Sprite_Draw(TEXTURE_INDEX_CHARGE, 400,50, 0, 0, 10, 300, 5, 150, 1.0f, 1.0f, 0.0f);*/

	DebugFont_Draw(350, 200, "damage: %d", damage);
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
	bPlayer_ChargeAttack = bChargeAttack;
}

void Player_Set_bCounter(bool bCounter)
{
	bPlayer_Counter = bCounter;
}

bool Player_Get_bInjured(void)
{
	return bPlayer_Injured;
}

void Player_Set_bInjured(bool bInjured)
{
	bPlayer_Injured = bInjured;
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

void AttackFlagReset() {

	attackLeftFlag = false;
	attackRightFlag = false;
	attackUpFlag = false;
	attackDownFlag = false;
	attackSlantingLeftUpFlag = false;
	attackSlantingRightUpFlag = false;
	attackSlantingLeftDownFlag = false;
	attackSlantingRightDownFlag = false;
}

//������c�S�V�X�e��
float AttackSwingWidth(float accelX, float accelY, float accelZ, float jyroX, float jyroY, float jyroZ) {
	float judgeDamage = 0.0f;
	if (accelX >= -75) {
		scale.up = 0.5f;
		judgeDamage = 1.0f;
	}
	if (accelX < -75 && jyroZ > -20) {
		scale.down = 1.0f;
		scale.up = 1.0f;
		judgeDamage = 5.0f;
	}
	if (accelX < -75 && jyroZ <= -20 && jyroY < 0) {
		scale.down = 2.0f;
		scale.up = 2.0f;
		PlayerHeal();
		judgeDamage = 10.0f;
	}

	return judgeDamage;
}

//�������c�S�V�X�e��
float AttackDownSwingWidth(float accelX, float accelY, float accelZ, float jyroX, float jyroY, float jyroZ) {
	float judgeDamage = 0.0f;
	if (accelX >= -75) {
		scale.down = 0.5f;
		judgeDamage = 1.0f;
	}
	if (accelX < -75 && jyroZ > -20) {
		scale.down = 1.0f;
		judgeDamage = 5.0f;
	}
	if (accelX < -75 && jyroZ <= -20 && jyroY < 0) {
		scale.down = 2.0f;
		PlayerHeal();
		judgeDamage = 10.0f;
	}

	return judgeDamage;
}

//�E�����̎c�S�V�X�e��
float AttackRightSwingWidth(float accelX, float accelY, float accelZ, float jyroX, float jyroY, float jyroZ) {
	float judgeDamage = 0.0f;
	if (jyroX > 18 && accelX > -75 && jyroZ <= 17) {
		scale.right = 0.5f;
		scale.left = 0.5f;
		judgeDamage = 1.0f;
	}
	if (accelX < -75 && jyroZ > 17) {
		scale.right = 1.0f;
		scale.left = 1.0f;
		judgeDamage = 5.0f;
	}
	if (accelX < -75 && jyroZ >= 17) {
		scale.right = 2.0f;
		scale.left = 2.0f;
		PlayerHeal();
		judgeDamage = 10.0f;
	}

	return judgeDamage;
}
//�������̎c�S�V�X�e�� 
float AttackLeftSwingWidth(float accelX, float accelY, float accelZ, float jyroX, float jyroY, float jyroZ) {
	float judgeDamage = 0.0f;
	if (jyroZ < 15 && accelX < -75) {
		scale.left = 0.5f;
		judgeDamage = 1.0f;
	}
	if (accelX < -75 && jyroZ >= 21 && accelX < -75 && jyroZ < 15) {
		scale.left = 1.0f;
		judgeDamage = 5.0f;
	}
	if (accelX < -75 && jyroZ >= 15 && jyroZ < 21) {
		scale.left = 2.0f;
		PlayerHeal();
		judgeDamage = 10.0f;
	}

	return judgeDamage;
}

//�E�΂ߏ�����̎c�S�V�X�e�� 
float AttackSlantingRightUpSwingWidth(float accelX, float accelY, float accelZ, float jyroX, float jyroY, float jyroZ) {
	float judgeDamage = 0.0f;
	if (accelX > -75) {
		scale.slantingRightUp = 0.5f;
		judgeDamage = 1.0f;
	}
	if (accelX < -75 && jyroZ >= 21 && accelX < -75 && jyroZ < 15) {
		scale.slantingRightUp = 1.0f;
		judgeDamage = 5.0f;
	}
	if (accelX < -75 && accelZ >= 30 && jyroZ < -10) {
		scale.slantingRightUp = 2.0f;
		PlayerHeal();
		judgeDamage = 10.0f;
	}

	return judgeDamage;
}

//���΂ߏ�����̎c�S�V�X�e�� 
float AttackSlantingLeftUpSwingWidth(float accelX, float accelY, float accelZ, float jyroX, float jyroY, float jyroZ) {
	float judgeDamage = 0.0f;
	if (accelX > -75) {
		scale.slantingLeftUp = 0.5f;
		judgeDamage = 1.0f;
	}
	if (accelX < -75 && jyroZ >= 21 && accelX < -75 && jyroZ < 15) {
		scale.slantingLeftUp = 1.0f;
		judgeDamage = 5.0f;
	}
	if (accelX < -75 && accelZ >= 30 && jyroZ < -10) {
		scale.slantingLeftUp = 2.0f;
		PlayerHeal();
		judgeDamage = 10.0f;
	}

	return judgeDamage;
}

//�E�΂߉������̎c�S�V�X�e�� 
float AttackSlantingRightDownSwingWidth(float accelX, float accelY, float accelZ, float jyroX, float jyroY, float jyroZ) {
	float judgeDamage = 0.0f;
	if (accelX > -75) {
		scale.slantingRightDown = 0.5f;
		judgeDamage = 1.0f;
	}
	if (accelX < -75 && jyroZ >= 21 && accelX < -75 && jyroZ < 15) {
		scale.slantingRightDown = 1.0f;
		judgeDamage = 5.0f;
	}
	if (accelX < -75 && accelZ >= 30 && jyroZ < -10) {
		scale.slantingRightDown = 2.0f;
		PlayerHeal();
		judgeDamage = 10.0f;
	}

	return judgeDamage;
}

//���΂߉������̎c�S�V�X�e�� 
float AttackSlantingLeftDownSwingWidth(float accelX, float accelY, float accelZ, float jyroX, float jyroY, float jyroZ) {
	float judgeDamage = 0.0f;
	if (accelX > -75) {
		scale.slantingLeftDown = 0.5f;
		judgeDamage = 1.0f;
	}
	if (accelX < -75 && jyroZ >= 21 && accelX < -75 && jyroZ < 15) {
		scale.slantingLeftDown = 1.0f;
		judgeDamage = 5.0f;
	}
	if (accelX < -75 && accelZ >= 30 && jyroZ < -10) {
		scale.slantingLeftDown = 2.0f;
		PlayerHeal();
		judgeDamage = 10.0f;
	}

	return judgeDamage;
}
//�񕜏���
void PlayerHeal() {
	Player_HP ++;
}
