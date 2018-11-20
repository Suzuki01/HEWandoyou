#include "mesh_field.h"
#include "mesh_sky.h"
#include "camera.h"
#include <d3dx9.h>
#include "debug_font.h"
#include "cube.h"
#include "input.h"
#include "player_action.h"
#include "judgement.h"
#include "joycon.h"

#define PHASE_DOOR_A	(1)
#define PHASE_BRIDGE	(2)
#define PHASE_DOOR_B	(3)
#define PHASE_SPEAR		(4)
#define PHASE_ROCK		(5)
#define PHASE_WIRE_A	(6)
#define PHASE_WIRE_B	(7)
#define PHASE_DOOR_C	(8)
#define PHASE_STAIRS	(9)

#define DOOR_A_POS_Z	(0.0f)
#define BRIDGE_POS_Z	(6.0f)
#define DOOR_B_POS_Z	(18.0f)
#define SPEAR_POS_Z		(22.0f)
#define ROCK_POS_Z		(25.0f)

#define ROCK_POS_X		(13.0f)
#define DOOR_C_POS_X	(18.0f)
#define WIRE_POS_Y		(1.0f)
#define WIRE_A_POS_Z	(23.0f)
#define WIRE_B_POS_Z	(27.0f)
#define STAIRS_POS_X	(36.0f)

#define CAMERA_SPEED	(0.05f)
#define SPEAR_SPEED		(0.1f)
#define WIRE_SPEED		(0.1f)
#define AVOIDANCE_SPEED	(0.1f)
#define ROTATION_SPEED	(1.0f)

#define ROCK_HP			(10)



static int turorial_phase;

static float Camera_PosX;
static float Camera_PosY;
static float Camera_PosZ;

static bool bTutorial_Door_A;
static bool bTutorial_Bridge;
static bool bTutorial_Door_B;
static bool bTutorial_Spear;
static bool bTutorial_Rock;
static bool bTutorial_Wire_A;
static bool bTutorial_Wire_B;
static bool bTutorial_Door_C;

static bool bAvoidance;

static float Spear_PosZ;
static float Wire_A_PosZ;
static float Wire_B_PosZ;
static float Camaera_Deg;

static bool bIsWireA_Attack;
static bool bIsWireB_Attack;

static int Rock_HP;

//�U�����Ă��玟�ɍU������܂ł̊Ԋu
static bool bIsAttack;
static int Attack_timeCnt;


void Tutorial_Initialize(void)
{

	//�J�����̏����ʒu�ݒ�
	Camera_PosX = 0.5f;
	Camera_PosY = 1.0f;
	Camera_PosZ = -3.0f;
	Camera_Set_Pos(D3DXVECTOR3(Camera_PosX, Camera_PosY, Camera_PosZ));

	MeshField_Initialize(2.0f, 200, 200);
	MeshSky_Initialize(5.0f, 100, 30);
	PlayerAction_Initialize();

	turorial_phase = PHASE_DOOR_A;

	bTutorial_Door_A = false;
	bTutorial_Bridge = false;
	bTutorial_Door_B = false;
	bTutorial_Spear = false;
	bTutorial_Rock = false;
	bTutorial_Wire_A = false;
	bTutorial_Wire_B = false;
	bTutorial_Door_C = false;

	bAvoidance = false;

	Spear_PosZ = SPEAR_POS_Z;
	Wire_A_PosZ = WIRE_A_POS_Z;
	Wire_B_PosZ = WIRE_B_POS_Z;
	Camaera_Deg = 0.0f;

	bIsWireA_Attack = false;
	bIsWireB_Attack = false;

	Rock_HP = ROCK_HP;

	bIsAttack = false;
	Attack_timeCnt = 0;
}
void Tutorial_Finalize(void)
{
	MeshField_Finalize();
	MeshSky_Finalize();
}
void Tutorial_Update(void)
{
	MeshSky_Update();
	PlayerAction_Update();
	pollLoop();

	switch (turorial_phase)
	{
	case PHASE_DOOR_A:
		Camera_PosZ += CAMERA_SPEED;
		if (Camera_PosZ > DOOR_A_POS_Z - 2.0f)
		{
			Camera_PosZ = DOOR_A_POS_Z - 2.0f;
		}
		if (Camera_PosZ == DOOR_A_POS_Z - 2.0f)
		{
			bTutorial_Door_A = true;
		}
		if (bTutorial_Door_A)
		{
			//�C�ӕ����a��
			if ((PlayerAction_GetSlash(UP) ||
				PlayerAction_GetSlash(RIGHTUP) ||
				PlayerAction_GetSlash(RIGHT) ||
				PlayerAction_GetSlash(RIGHTDOWN) ||
				PlayerAction_GetSlash(DOWN) ||
				PlayerAction_GetSlash(LEFTDOWN) ||
				PlayerAction_GetSlash(LEFT) ||
				PlayerAction_GetSlash(LEFTUP))
				&&!bIsAttack)
			{
				bTutorial_Door_A = false;
				bIsAttack = true;
				PlayerAction_SetSlash(UP, false);
				PlayerAction_SetSlash(RIGHTUP, false);
				PlayerAction_SetSlash(RIGHT, false);
				PlayerAction_SetSlash(RIGHTDOWN, false);
				PlayerAction_SetSlash(DOWN, false);
				PlayerAction_SetSlash(LEFTDOWN, false);
				PlayerAction_SetSlash(LEFT, false);
				PlayerAction_SetSlash(LEFTUP, false);
				turorial_phase = PHASE_BRIDGE;
			}
		}
		break;
	case PHASE_BRIDGE:
		Camera_PosZ += CAMERA_SPEED;
		if (Camera_PosZ > BRIDGE_POS_Z - 2.0f)
		{
			Camera_PosZ = BRIDGE_POS_Z - 2.0f;
		}
		if (Camera_PosZ == BRIDGE_POS_Z - 2.0f)
		{
			bTutorial_Bridge = true;
		}
		if (bTutorial_Bridge)
		{
			//�������a��
			if (PlayerAction_GetSlash(UP) && !bIsAttack)
			{
				bTutorial_Bridge = false;
				bIsAttack = true;
				PlayerAction_SetSlash(UP, false);
				turorial_phase = PHASE_DOOR_B;
			}
		}
		break;
	case PHASE_DOOR_B:
		Camera_PosZ += CAMERA_SPEED;
		if (Camera_PosZ > DOOR_B_POS_Z - 2.0f)
		{
			Camera_PosZ = DOOR_B_POS_Z - 2.0f;
		}
		if (Camera_PosZ == DOOR_B_POS_Z - 2.0f)
		{
			bTutorial_Door_B = true;
		}
		if (bTutorial_Door_B)
		{
			//�������a��
			if (PlayerAction_GetSlash(UP) && !bIsAttack)
			{
				bTutorial_Door_B = false;
				bIsAttack = true;
				PlayerAction_SetSlash(UP, false);
				turorial_phase = PHASE_SPEAR;
			}
		}
		break;
	case PHASE_SPEAR:
		Spear_PosZ -= SPEAR_SPEED;
		if (Spear_PosZ < SPEAR_POS_Z - 5.0f)
		{
			Spear_PosZ = SPEAR_POS_Z - 5.0f;
		}
		if (Spear_PosZ >= SPEAR_POS_Z - 5.0f&&Camera_PosX==0.5f)
		{
			bTutorial_Spear = true;
		}
		if (bTutorial_Spear)
		{
			//���
			if (PlayerAction_GetbIsAvoidance_Right())
			{
				bAvoidance = true;
				bTutorial_Spear = false;
			}
		}
		if(bAvoidance)
		{
			Camera_PosX += AVOIDANCE_SPEED;
			if (Camera_PosX > 2.0f)
			{
				Camera_PosX = 2.0f;
				bAvoidance = false;
				turorial_phase = PHASE_ROCK;
				PlayerAction_SetbIsAvoidance_Right(false);
			}
		}
		break;
	case PHASE_ROCK:
		Camera_PosZ += CAMERA_SPEED;
		if (Camera_PosZ > ROCK_POS_Z + 0.5f)
		{
			Camera_PosZ = ROCK_POS_Z + 0.5f;
		}
		if (Camera_PosZ == ROCK_POS_Z + 0.5f&&Camaera_Deg<90)
		{
			Camera_turn_right(ROTATION_SPEED);
			Camaera_Deg++;
		}
		if (Camaera_Deg == 90)
		{
			Camera_PosX += CAMERA_SPEED;
		}
		if (Camera_PosX > ROCK_POS_X - 2.0f)
		{
			Camera_PosX = ROCK_POS_X - 2.0f;
		}
		if (Camera_PosX == ROCK_POS_X - 2.0f)
		{
			bTutorial_Rock=true;
		}
		if (bTutorial_Rock)
		{
			//�A��
			if (Rock_HP > 0 && !bIsAttack)
			{
				if (PlayerAction_GetSlash(UP) ||
					PlayerAction_GetSlash(RIGHTUP) ||
					PlayerAction_GetSlash(RIGHT) ||
					PlayerAction_GetSlash(RIGHTDOWN) ||
					PlayerAction_GetSlash(DOWN) ||
					PlayerAction_GetSlash(LEFTDOWN) ||
					PlayerAction_GetSlash(LEFT) ||
					PlayerAction_GetSlash(LEFTUP))
				{
					Rock_HP--;
					bIsAttack = true;
					PlayerAction_SetSlash(UP, false);
					PlayerAction_SetSlash(RIGHTUP, false);
					PlayerAction_SetSlash(RIGHT, false);
					PlayerAction_SetSlash(RIGHTDOWN, false);
					PlayerAction_SetSlash(DOWN, false);
					PlayerAction_SetSlash(LEFTDOWN, false);
					PlayerAction_SetSlash(LEFT, false);
					PlayerAction_SetSlash(LEFTUP, false);
					
				}
			}
			if (Rock_HP <= 0)
			{
				bTutorial_Rock = false;
				turorial_phase = PHASE_WIRE_A;
			}
		}
		break;
	case PHASE_WIRE_A:
		Camera_PosX += CAMERA_SPEED;
		if (Camera_PosX > DOOR_C_POS_X - 2.0f)
		{
			Camera_PosX = DOOR_C_POS_X - 2.0f;
		}
		if (Camera_PosX == DOOR_C_POS_X - 2.0f&&!bIsWireA_Attack)
		{
			Wire_A_PosZ += WIRE_SPEED;
			if (Wire_A_PosZ > WIRE_A_POS_Z + 2.0f)
			{
				Wire_A_PosZ = WIRE_A_POS_Z + 2.0f;
			}
			if (Wire_A_PosZ <= WIRE_A_POS_Z + 2.0f)
			{
				bTutorial_Wire_A = true;
			}
		}
		if (bTutorial_Wire_A)
		{
			//�E�����J�E���^�[
			if (/*PlayerAction_GetSlash(RIGHT) && !bIsAttack*/Keyboard_IsTrigger(DIK_T))
			{
				bIsWireA_Attack = true;
				bIsAttack = true;
				PlayerAction_SetSlash(RIGHT, false);
			}
			if(bIsWireA_Attack)
			{
				Wire_A_PosZ -= WIRE_SPEED;
				if (Wire_A_PosZ < WIRE_A_POS_Z)
				{
					bTutorial_Wire_A = false;
					turorial_phase = PHASE_WIRE_B;
				}
			}
		}
		break;
	case PHASE_WIRE_B:
		if (!bIsWireB_Attack)
		{
			Wire_B_PosZ -= WIRE_SPEED;
			if (Wire_B_PosZ < WIRE_B_POS_Z - 2.0f)
			{
				Wire_B_PosZ = WIRE_B_POS_Z - 2.0f;
			}
			if (Wire_B_PosZ >= WIRE_B_POS_Z - 2.0f)
			{
				bTutorial_Wire_B = true;
			}
		}
		if (bTutorial_Wire_B)
		{
			//�������J�E���^�[
			if (/*PlayerAction_GetSlash(LEFT) && !bIsAttack*/Keyboard_IsTrigger(DIK_T))
			{
				bIsWireB_Attack = true;
				bIsAttack = true;
				PlayerAction_SetSlash(LEFT, false);
			}
			if(bIsWireB_Attack)
			{
				Wire_B_PosZ += WIRE_SPEED;
				if (Wire_B_PosZ > WIRE_B_POS_Z)
				{
					bTutorial_Wire_B = false;
					turorial_phase = PHASE_DOOR_C;
				}
			}
		}
		break;
	case PHASE_DOOR_C:
		Camera_PosX += CAMERA_SPEED;
		if (Camera_PosX > DOOR_C_POS_X - 2.0f)
		{
			Camera_PosX = DOOR_C_POS_X - 2.0f;
		}
		if (Camera_PosX == DOOR_C_POS_X - 2.0f)
		{
			bTutorial_Door_C = true;
		}
		if (bTutorial_Door_C)
		{
			//�`���[�W�U��
			if (PlayerAction_GetbIsChargeAttack())
			{
				bTutorial_Door_C = false;
				turorial_phase = PHASE_STAIRS;
			}
		}	
		break;
	case PHASE_STAIRS:
		Camera_PosX += CAMERA_SPEED;
		if (Camera_PosX > STAIRS_POS_X - 2.0f)
		{
			Camera_PosX = STAIRS_POS_X - 2.0f;
		}
		if (Camera_PosX == STAIRS_POS_X - 2.0f)
		{
			//�X�e�[�W��I��
		}
		break;
	default:
		break;
	}

	//�U�����ԊԊu
	if (bIsAttack)
	{
		Attack_timeCnt++;
	}
	if (Attack_timeCnt > ATTACK_INTERVAL)
	{
		Attack_timeCnt = 0;
		bIsAttack = false;
	}

	Camera_Set_Pos(D3DXVECTOR3(Camera_PosX, Camera_PosY, Camera_PosZ));
}
void Tutorial_Draw(void)
{
	D3DXVECTOR3 pos = Camera_Get_Pos();

	MeshField_Draw(TEXTURE_INDEX_FIELD, 200, 200);
	MeshSky_Draw(TEXTURE_INDEX_SKY, 100, 30);


	//�X�e�[�W�`��
	//��A
	if (turorial_phase <= PHASE_DOOR_A)
	{
		Cube_Draw(TEXTURE_INDEX_1, 0.0f, 0.0f, DOOR_A_POS_Z, 1.0f, 1.0f, 1.0f);
	}
	//��
	if (turorial_phase <= PHASE_BRIDGE)
	{
		Cube_Draw(TEXTURE_INDEX_1, 0.0f, 0.0f, BRIDGE_POS_Z, 1.0f, 1.0f, 1.0f);
	}
	//��B
	if (turorial_phase <= PHASE_DOOR_B)
	{
		Cube_Draw(TEXTURE_INDEX_1, 0.0f, 0.0f, DOOR_B_POS_Z, 1.0f, 1.0f, 1.0f);
	}
	//��
	if (turorial_phase <= PHASE_SPEAR)
	{
		Cube_Draw(TEXTURE_INDEX_2, 0.0f, 0.5f, Spear_PosZ, 1.0f, 1.0f, 1.0f);
	}
	//��
	if (turorial_phase <= PHASE_ROCK)
	{
		Cube_Draw(TEXTURE_INDEX_1, ROCK_POS_X, 0.0f, ROCK_POS_Z, 1.0f, 1.0f, 1.0f);
	}
	//���C���[A
	if (turorial_phase <= PHASE_WIRE_A)
	{
		Cube_Draw(TEXTURE_INDEX_2, DOOR_C_POS_X, WIRE_POS_Y, Wire_A_PosZ, 1.0f, 1.0f, 1.0f);
	}
	//���C���[B
	if (turorial_phase <= PHASE_WIRE_B)
	{
		Cube_Draw(TEXTURE_INDEX_2, DOOR_C_POS_X, WIRE_POS_Y, Wire_B_PosZ, 1.0f, 1.0f, 1.0f);
	}
	//��C
	if (turorial_phase <= PHASE_DOOR_C)
	{
		Cube_Draw(TEXTURE_INDEX_1, DOOR_C_POS_X, 0.0f, ROCK_POS_Z, 1.0f, 1.0f, 1.0f);
	}
	//�K�i
	Cube_Draw(TEXTURE_INDEX_1, STAIRS_POS_X, 0.0f, ROCK_POS_Z, 1.0f, 1.0f, 1.0f);


	//�`���[�g���A��
	if (bTutorial_Door_A)
	{
		DebugFont_Draw(500, 50, "�C�ӕ����a�����Ĕ���j�󂷂�");
	}
	if (bTutorial_Bridge)
	{
		DebugFont_Draw(500, 50, "�������a�����ă��[�v��؂�");
	}
	if (bTutorial_Door_B)
	{
		DebugFont_Draw(500, 50, "�������a�����Ĕ���j�󂷂�");
	}
	if (bTutorial_Spear)
	{
		DebugFont_Draw(500, 50, "���");
	}
	if (bTutorial_Rock)
	{
		DebugFont_Draw(500, 50, "�A���U���Ŋ��j�󂷂�");
	}
	if (bTutorial_Wire_A)
	{
		DebugFont_Draw(500, 50, "�E�J�E���^�[");
	}
	if (bTutorial_Wire_B)
	{
		DebugFont_Draw(500, 50, "���J�E���^�[");
	}
	if (bTutorial_Door_C)
	{
		DebugFont_Draw(500, 50, "�`���[�W�A�^�b�N���Ĕ���j�󂷂�");
	}


	DebugFont_Draw(100, 50, "Wire_A_PosZ : %.2f", Wire_A_PosZ);
	DebugFont_Draw(100, 100, "Wire_B_PosZ : %.2f", Wire_B_PosZ);

	PlayerAction_Draw();
}