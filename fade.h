#ifndef FADE_H_
#define FADE_H_

#include <d3d9.h>

void Fade_Initialize();
void Fade_Update();
void Fade_Draw();
bool Fade_IsFade(void);
void Fade_Start(bool bOut,int frame,D3DCOLOR color);

#endif //FADEF_H_