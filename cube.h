#ifndef CUBE_H_
#define CUBE_H_

#include "texture.h"

void Cube_Initialize(void);
void Cube_Finalize(void);
void Cube_Update(void);
void Cube_Draw(TextureIndex texture_index, float dx, float dy, float dz, float sx, float sy, float sz, float angleX, float angleY, float angleZ);

#endif // CUBE_H_