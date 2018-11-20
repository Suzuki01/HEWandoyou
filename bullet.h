#ifndef BULLET_H_
#define BULLET_H_

#define	MAX_BULLET	(8) 

void Bullet_Initialize(void);
void Bullet_Update(void);
void Bullet_Draw(void);

void Bullet_Create(float x, float y, float z);
void Bullet_Destroy(int i);

bool Bullet_IsEnable(int i);

#endif // BULLET_H_