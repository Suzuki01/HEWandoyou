#ifndef SWORD_EFFECT
#define SWORD_EFFECT

#include "texture.h"

void SwordEffectInit();
void SwordEffectUpdate();
void SwordEffectDraw();
void SwordEffectCreate(float cx, float cy,float rotation, TextureIndex textureIndex,int MaxPattern, int MaxWidthPattern);
void TextureAnimation(int waitFrame);

#endif // !SWORD_EFFECT
