#ifndef BILL_BOARD_H_
#define BILL_BOARD_H_

#include "texture.h"

void BillBoard_Initialize();
void BillBoard_Draw(TextureIndex index, float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ);
void BillBoard_Finalize();


#endif // !BILL_BOARD_H_
