#ifndef MESH_SKY_H_
#define MESH_SKY_H_

#include "texture.h"

void MeshSky_Initialize(float lenth, int radius, int height);				//頂点バッファ、インデックスバッファの作成、およびデータの書き込み
void MeshSky_Update(void);
void MeshSky_Finalize(void);												//頂点バッファ、インデックスバッファ、テクスチャのさよなら
void MeshSky_Draw(TextureIndex texture_index, int n, int height);
//一回のDrawIndexedPrimitive
//縮退を利用したストリップポリゴン
//UVアドレッシングモードWRAPにして

#endif // MESH_SKY_H_