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
#include "sword_effect.h"
#include "common.h"
#include "sprite.h"
#include "bill_board.h"

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

#define ROCK_HP			(7)

//title画面用define
#define TITLE_LOGO_WIDTH		(4093)
#define TITLE_LOGO_HEIGHT		(2894)
#define TITLE_LOGO_SCALE		(0.1)
#define WOOD_BILL_WIDTH			(1500)
#define WOOD_BILL_HEIGHT		(1500)
#define WOOD_BILL_SCALE			(0.2)
#define TITLE_START_WDITH		(176)
#define TITLE_START_HEIGHT		(468)
#define TITLE_START_SCALE		(0.4)
#define TITLE_RANKING_WDITH		(300)
#define TITLE_RANKING_HEIGHT	(500)
#define TITLE_RANKING_SCALE		(0.4)
#define TITLE_OPTION_WDITH		(300)
#define TITLE_OPTION_HEIGHT		(500)
#define TITLE_OPTION_SCALE		(0.4)

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

//攻撃してから次に攻撃するまでの間隔
static bool bIsAttack;
static int Attack_timeCnt;


void Tutorial_Initialize(void)
{

	//カメラの初期位置設定
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
			//任意方向斬撃
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
			//一定方向斬撃
			if (PlayerAction_GetSlash(UP) && !bIsAttack)
			{
				bTutorial_Bridge = false;
				bIsAttack = true;
				PlayerAction_SetSlash(UP, false);
				SwordEffectCreate(SCREEN_WIDTH * 0.5 - 100, SCREEN_HEIGHT * 0.5, D3DXToRadian(90), TEXTURE_INDEX_ZANSHIN, 15, 5);
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
			//一定方向斬撃
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
			//回避
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
			//連打
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
			//右方向カウンター
			if (/*PlayerAction_GetSlash(RIGHT) && !bIsAttack*/Keyboard_IsTrigger(DIK_T))
			{
				bIsWireA_Attack = true;
				bIsAttack = true;
				SwordEffectCreate(SCREEN_WIDTH * 0.5 - 100, SCREEN_HEIGHT * 0.5 - 100, 0, TEXTURE_INDEX_COUNTER,15,5);
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
			//左方向カウンター
			if (/*PlayerAction_GetSlash(LEFT) && !bIsAttack*/Keyboard_IsTrigger(DIK_T))
			{
				bIsWireB_Attack = true;
				bIsAttack = true;
				PlayerAction_SetSlash(LEFT, false);
				SwordEffectCreate(SCREEN_WIDTH * 0.5 - 100, SCREEN_HEIGHT * 0.5 - 100, 0, TEXTURE_INDEX_COUNTER, 15, 5);
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
			//チャージ攻撃
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
			//ステージを選ぶ
		}
		break;
	default:
		break;
	}

	//攻撃時間間隔
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
	
	//タイトル画面での処理
	if (Keyboard_IsTrigger(DIK_1)) {
		SwordEffectCreate(SCREEN_WIDTH * 0.5 - 500, SCREEN_HEIGHT * 0.5 + 50, D3DXToRadian(45), TEXTURE_INDEX_SWORD_EFFECT, 5, 5);
	}
	if (Keyboard_IsTrigger(DIK_2)) {
		SwordEffectCreate(SCREEN_WIDTH * 0.5 - 100, SCREEN_HEIGHT * 0.5, D3DXToRadian(-45), TEXTURE_INDEX_SWORD_EFFECT, 5, 5);
	}
	if (Keyboard_IsTrigger(DIK_3)) {
		SwordEffectCreate(SCREEN_WIDTH * 0.5 + 300, SCREEN_HEIGHT * 0.5 + 50, D3DXToRadian(-135), TEXTURE_INDEX_SWORD_EFFECT, 5, 5);
	}
}
void Tutorial_Draw(void)
{
	D3DXVECTOR3 pos = Camera_Get_Pos();

	MeshField_Draw(TEXTURE_INDEX_FIELD, 200, 200);
	MeshSky_Draw(TEXTURE_INDEX_SKY, 100, 30);
	if (turorial_phase == PHASE_SPEAR) {
		Sprite_Draw(TEXTURE_INDEX_AVOIDANCE_ACTIVE, INDICATOR_POSX, INDICATOR_POSY, 0, 0, 1950, 1950, 975, 975, 0.1, 0.1, 0);
	}
	else {
		Sprite_Draw(TEXTURE_INDEX_AVOIDANCE_NORMAL, INDICATOR_POSX, INDICATOR_POSY, 0, 0, 1950, 1950, 975, 975, 0.1, 0.1, 0);
	}

	//ステージ描画
	//扉A
	if (turorial_phase <= PHASE_DOOR_A)
	{
		Cube_Draw(TEXTURE_INDEX_1, 0.0f, 0.0f, DOOR_A_POS_Z, 1.0f, 1.0f, 1.0f);
	}
	//橋
	if (turorial_phase <= PHASE_BRIDGE)
	{
		Cube_Draw(TEXTURE_INDEX_1, 0.0f, 0.0f, BRIDGE_POS_Z, 1.0f, 1.0f, 1.0f);
	}
	//扉B
	if (turorial_phase <= PHASE_DOOR_B)
	{
		Cube_Draw(TEXTURE_INDEX_1, 0.0f, 0.0f, DOOR_B_POS_Z, 1.0f, 1.0f, 1.0f);
	}
	//槍
	if (turorial_phase <= PHASE_SPEAR)
	{
		Cube_Draw(TEXTURE_INDEX_2, 0.0f, 0.5f, Spear_PosZ, 1.0f, 1.0f, 1.0f);
	}
	//岩
	if (turorial_phase <= PHASE_ROCK)
	{
		Cube_Draw(TEXTURE_INDEX_1, ROCK_POS_X, 0.0f, ROCK_POS_Z, 1.0f, 1.0f, 1.0f);
		if (turorial_phase == PHASE_ROCK) {
			Sprite_Draw(TEXTURE_INDEX_ENEMY_ATTACK_GAGE,	   -300, -600, 0, 0, 1500 * ((float)Rock_HP / ROCK_HP), 1500, 750, 750, 0.4, 0.4, 0);
			Sprite_Draw(TEXTURE_INDEX_ENEMY_ATTACK_GAGE_FRAME, -300, -600, 0, 0, 1500, 1500, 750, 750, 0.4, 0.4, 0);
		}
	}
	//ワイヤーA
	if (turorial_phase <= PHASE_WIRE_A)
	{
		Cube_Draw(TEXTURE_INDEX_2, DOOR_C_POS_X, WIRE_POS_Y, Wire_A_PosZ, 1.0f, 1.0f, 1.0f);
		if (turorial_phase == PHASE_WIRE_A) {
			Sprite_Draw(TEXTURE_INDEX_COUNTER_ATTACK_RIGHT, INDICATOR_POSX, INDICATOR_POSY, 0, 0, 1950, 1950, 975, 975, 0.1, 0.1, 0);
		}
	}
	//ワイヤーB
	if (turorial_phase <= PHASE_WIRE_B)
	{
		Cube_Draw(TEXTURE_INDEX_2, DOOR_C_POS_X, WIRE_POS_Y, Wire_B_PosZ, 1.0f, 1.0f, 1.0f);
		if (turorial_phase == PHASE_WIRE_B) {
			Sprite_Draw(TEXTURE_INDEX_COUNTER_ATTACK_LEFT, INDICATOR_POSX, INDICATOR_POSY, 0, 0, 1950, 1950, 975, 975, 0.1, 0.1, 0);
		}
	}
	//扉C
	if (turorial_phase <= PHASE_DOOR_C)
	{
		Cube_Draw(TEXTURE_INDEX_1, DOOR_C_POS_X, 0.0f, ROCK_POS_Z, 1.0f, 1.0f, 1.0f);
	}
	//階段
	Cube_Draw(TEXTURE_INDEX_1, STAIRS_POS_X, 0.0f, ROCK_POS_Z, 1.0f, 1.0f, 1.0f);

	//チュートリアル
	if (bTutorial_Door_A)
	{
		DebugFont_Draw(500, 50, "任意方向斬撃して扉を破壊する");
	}
	if (bTutorial_Bridge)
	{
		DebugFont_Draw(500, 50, "一定方向斬撃してロープを切る");
	}
	if (bTutorial_Door_B)
	{
		DebugFont_Draw(500, 50, "一定方向斬撃して扉を破壊する");
	}
	if (bTutorial_Spear)
	{
		DebugFont_Draw(500, 50, "回避");
	}
	if (bTutorial_Rock)
	{
		DebugFont_Draw(500, 50, "連続攻撃で岩を破壊する");
	}
	if (bTutorial_Wire_A)
	{
		DebugFont_Draw(500, 50, "右カウンター");
	}
	if (bTutorial_Wire_B)
	{
		DebugFont_Draw(500, 50, "左カウンター");
	}
	if (bTutorial_Door_C)
	{
		DebugFont_Draw(500, 50, "チャージアタックして扉を破壊する");
	}


	DebugFont_Draw(100, 50, "Wire_A_PosZ : %.2f", Wire_A_PosZ);
	DebugFont_Draw(100, 100, "Wire_B_PosZ : %.2f", Wire_B_PosZ);

	//タイトル画面で表示する画像
	Sprite_Draw(TEXTURE_INDEX_TITLE_LOGO,    SCREEN_WIDTH * 0.5  - TITLE_LOGO_WIDTH * TITLE_LOGO_SCALE * 0.5 ,SCREEN_HEIGHT * 0.25 - TITLE_LOGO_HEIGHT * TITLE_LOGO_SCALE * 0.5,0, 0, Texture_GetWidth(TEXTURE_INDEX_TITLE_LOGO), Texture_GetHeight(TEXTURE_INDEX_TITLE_LOGO),0, 0, 0.1 ,0.1,0);
	Sprite_Draw(TEXTURE_INDEX_TITLE_OPTION,  SCREEN_WIDTH * 0.75 - WOOD_BILL_WIDTH  * WOOD_BILL_SCALE  * 0.5 ,SCREEN_HEIGHT * 0.5											   ,0, 0, Texture_GetWidth(TEXTURE_INDEX_WOOD_BILL),  Texture_GetHeight(TEXTURE_INDEX_WOOD_BILL), 0, 0, 0.2, 0.2,0);
	Sprite_Draw(TEXTURE_INDEX_TITLE_START,   SCREEN_WIDTH * 0.5  - WOOD_BILL_WIDTH  * WOOD_BILL_SCALE  * 0.5 ,SCREEN_HEIGHT * 0.5											   ,0, 0, Texture_GetWidth(TEXTURE_INDEX_WOOD_BILL),  Texture_GetHeight(TEXTURE_INDEX_WOOD_BILL), 0, 0, 0.2, 0.2,0);
	Sprite_Draw(TEXTURE_INDEX_TITLE_RANKING, SCREEN_WIDTH * 0.25 - WOOD_BILL_WIDTH  * WOOD_BILL_SCALE  * 0.5 ,SCREEN_HEIGHT * 0.5											   ,0, 0, Texture_GetWidth(TEXTURE_INDEX_WOOD_BILL),  Texture_GetHeight(TEXTURE_INDEX_WOOD_BILL), 0, 0, 0.2, 0.2,0);
	PlayerAction_Draw();
}