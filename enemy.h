#ifndef ENEMY_H_
#define ENEMY_H_

#define COUNTER_UP		(1)
#define COUNTER_DOWN	(2)
#define COUNTER_LEFT	(3)
#define COUNTER_RIGHT	(4)

void Enemy_Initialize(void);
void Enemy_Update(void);
void Enemy_Draw(void);

bool Enemy_Get_bEffective(void);
bool Enemy_Get_bSlow(void);
bool Enemy_Get_bStan(void);
bool Enemy_Get_bAdvance(void);
void Enemy_Set_Damage(void);
void Enemy_Set_BigDamage(void);
int Enemy_Get_Mode(void);
int Enemy_Get_HP(void);
void Enemy_Set_bRotation(bool bRotation);

#endif // ENEMY_H_