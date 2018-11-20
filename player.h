#ifndef PLAYER_H_
#define PLAYER_H_

#define NORMAL_MODE			(0)
#define MOVE_MODE_L			(1)
#define AVOIDANCE_MODE_L		(2)
#define RETURN_MODE_L			(3)
#define MOVE_MODE_R			(4)
#define AVOIDANCE_MODE_R		(5)
#define RETURN_MODE_R			(6)

void Player_Initialize(void);
void Player_Update(void);
void Player_Draw(void);

int Player_Get_Mode(void);
void Player_Set_Damage(void);
void Player_Set_Charge(int charge);
bool Player_Get_bCounter(void);
void Player_Set_bCounter(bool bCounter);
bool Player_Get_bInjured(void);
void Player_Set_bInjured(bool bInjured);
bool Player_Get_bChargeAttack(void);
void Player_Set_bChargeAttack(bool bChargeAttack);
int Player_Get_HP(void);

#endif // PLAYER_H_