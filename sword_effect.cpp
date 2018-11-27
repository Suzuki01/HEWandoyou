#include "sprite.h"
#include "texture.h"
#include "sword_effect.h"

#define SWORD_EFFECT_PATTERNCOUNT	(5)
#define SWORD_EFFECT_WIDTH			(192)
#define SWORD_EFFECT_MAX			(20)

//-------------------------------------------------------------------------------
//	グローバル変数宣言	必ずstaticを入れること！
//-------------------------------------------------------------------------------
static float x;
static float y;
static bool swordEffectFlag = false;
static int g_SwordEffectFrameCount = 0;

typedef struct SwordEffectData {
	float dx;			//描画位置のx座標
	float dy;			//描画位置のy座標
	int tw;				//切り取った画像の開始位置のx座標
	int th;				//切り取った画像の開始位置のy座標
	bool flag;
	int CreateFrame;
	int PatternCount;	//今のパターン数
	int MaxPattern;		//マックスパターン
	int MaxWidthPattern;
	float rotation;		//画像の角度
	TextureIndex textureIndex;
}SwordEffectData;

SwordEffectData SwordEffect[SWORD_EFFECT_MAX];

void SwordEffectInit() {

	for (int i = 0; i < SWORD_EFFECT_MAX; i++) {
		SwordEffect[i].flag = false;
		SwordEffect[i].CreateFrame = 0;
		SwordEffect[i].PatternCount = 0;
		SwordEffect[i].dx = 0;
		SwordEffect[i].dy = 0;
		SwordEffect[i].tw = 0;
		SwordEffect[i].th = 0;
		SwordEffect[i].rotation = 0;
		SwordEffect[i].MaxPattern = 0;
		SwordEffect[i].textureIndex = TEXTURE_INDEX_MAX;
	}
}
void SwordEffectUpdate() {

/*	for (int i = 0; i < SWORD_EFFECT_MAX; i++) {
		int age = g_SwordEffectFrameCount - SwordEffect[i].CreateFrame;
		if (SwordEffect[i].flag == true) {
			int patternWidth = Texture_GetWidth(SwordEffect[i].textureIndex) / SwordEffect[i].MaxWidthPattern;
			int patternHeight = Texture_GetHeight(SwordEffect[i].textureIndex) % SwordEffect[i].MaxPattern / SwordEffect[i].MaxWidthPattern;
			SwordEffect[i].PatternCount = age / SwordEffect[i].MaxPattern;
			SwordEffect[i].tw = patternWidth *  (SwordEffect[i].PatternCount % SwordEffect[i].MaxPattern);
			SwordEffect[i].th = patternHeight * (SwordEffect[i].PatternCount / SwordEffect[i].MaxPattern);
			if (SwordEffect[i].PatternCount > SwordEffect[i].MaxPattern) {
				SwordEffect[i].flag = false;
			}
		}
	}*/
	TextureAnimation(3);
	g_SwordEffectFrameCount++;
}

void SwordEffectDraw() {

	for (int i = 0; i < SWORD_EFFECT_MAX; i++) {
		if (SwordEffect[i].flag == true) {
			Sprite_Draw(SwordEffect[i].textureIndex, SwordEffect[i].dx, SwordEffect[i].dy, SwordEffect[i].tw, SwordEffect[i].th, 192, 192, 96, 96, 3.0f, 3.0f, SwordEffect[i].rotation);
		}
	}
}

void SwordEffectCreate(float cx, float cy,float rotation,TextureIndex textureIndex,int MaxPattern,int MaxWidthPattern) {
	for (int i = 0; i < SWORD_EFFECT_MAX; i++) {
		if (SwordEffect[i].flag == false) {
			SwordEffect[i].dx = cx;
			SwordEffect[i].dy = cy;
			SwordEffect[i].rotation = rotation;
			SwordEffect[i].flag = true;
			SwordEffect[i].CreateFrame = 0;
			SwordEffect[i].PatternCount = 0;
			SwordEffect[i].textureIndex = textureIndex;
			SwordEffect[i].MaxPattern = MaxPattern;
			SwordEffect[i].MaxWidthPattern = MaxWidthPattern;
			break;
		}
	}
}



void TextureAnimation(int waitFrame) {

	for (int i = 0; i < SWORD_EFFECT_MAX; i++) {
		if (SwordEffect[i].flag) {
			/*			int patternWidth = Texture_GetWidth(SwordEffect[i].textureIndex) / SwordEffect[i].MaxWidthPattern;
						int patternHeight = Texture_GetHeight(SwordEffect[i].textureIndex) / (SwordEffect[i].MaxPattern / SwordEffect[i].MaxWidthPattern);
						SwordEffect[i].PatternCount = (g_SwordEffectFrameCount / waitFrame) % SwordEffect[i].MaxPattern;

						SwordEffect[i].tw = patternWidth * (SwordEffect[i].PatternCount % SwordEffect[i].MaxWidthPattern);
						SwordEffect[i].th = patternHeight * (SwordEffect[i].PatternCount / SwordEffect[i].MaxWidthPattern);

						if (waitFrame * SwordEffect[i].MaxPattern < g_SwordEffectFrameCount - SwordEffect[i].CreateFrame) {
							SwordEffect[i].flag = false;
							SwordEffect[i].PatternCount = 0;
						}
					}
				}*/
			int patternWidth = Texture_GetWidth(SwordEffect[i].textureIndex) / SwordEffect[i].MaxWidthPattern;
			int patternHeight = Texture_GetHeight(SwordEffect[i].textureIndex) / (SwordEffect[i].MaxPattern / SwordEffect[i].MaxWidthPattern);

			//表示パタン目＝（（フレームカウンター/待ちフレーム数）,最大パタン数ー１）
			//int PatternCount = min((FrameCount / FrameSpeed), MaxPattern - 1);

			//表示パタン目＝（フレームカウンター/待ちフレーム数）%　最大パタン数
/*			SwordEffect[i].PatternCount = (g_SwordEffectFrameCount / waitFrame) % SwordEffect[i].MaxPattern;

			SwordEffect[i].tw = patternWidth  * (SwordEffect[i].PatternCount % SwordEffect[i].MaxWidthPattern);
			SwordEffect[i].th = patternHeight * (SwordEffect[i].PatternCount / SwordEffect[i].MaxWidthPattern);
			if (waitFrame * SwordEffect[i].MaxPattern < g_SwordEffectFrameCount - SwordEffect[i].CreateFrame) {
				SwordEffect[i].flag = false;
				SwordEffect[i].PatternCount = 0;
			}*/
			SwordEffect[i].PatternCount = (SwordEffect[i].CreateFrame / waitFrame) % SwordEffect[i].MaxPattern;

			SwordEffect[i].tw = patternWidth  * (SwordEffect[i].PatternCount % SwordEffect[i].MaxWidthPattern);
			SwordEffect[i].th = patternHeight * (SwordEffect[i].PatternCount / SwordEffect[i].MaxWidthPattern);
			if (waitFrame * SwordEffect[i].MaxPattern < SwordEffect[i].CreateFrame) {
				SwordEffect[i].flag = false;
				SwordEffect[i].PatternCount = 0;
				SwordEffect[i].CreateFrame = 0;
			}
			SwordEffect[i].CreateFrame++;
		}
	}
}