#ifndef MESH_FIELD_H_
#define MESH_FIELD_H_

#include "texture.h"

void MeshField_Initialize(float lenth, int column, int row);				//頂点バッファ、インデックスバッファの作成、およびデータの書き込み
void MeshField_Finalize(void);												//頂点バッファ、インデックスバッファ、テクスチャのさよなら
void MeshField_Draw(TextureIndex texture_index, int column, int row);
//一回のDrawIndexedPrimitive
//縮退を利用したストリップポリゴン
//UVアドレッシングモードWRAPにして

#endif // MESH_FIELD_H_