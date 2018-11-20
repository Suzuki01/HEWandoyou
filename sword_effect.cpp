#include "sprite.h"
#include "texture.h"

#define SWORD_EFFECT_PATTERNCOUNT	(5)
#define SWORD_EFFECT_WIDTH			(192)
#define SWORD_EFFECT_MAX			(20)

//-------------------------------------------------------------------------------
//	�O���[�o���ϐ��錾	�K��static�����邱�ƁI
//-------------------------------------------------------------------------------
static float x;
static float y;
static bool swordEffectFlag = false;
static int g_SwordEffectFrameCount = 0;
int patternWidth =  Texture_GetWidth(TEXTURE_INDEX_SWORD_EFFECT) / SWORD_EFFECT_PATTERNCOUNT;
int patternHeight = Texture_GetHeight(TEXTURE_INDEX_SWORD_EFFECT);


typedef struct SwordEffectData {
	float dx;			//�`��ʒu��x���W
	float dy;			//�`��ʒu��y���W
	int tw;				//�؂������摜�̊J�n�ʒu��x���W
	int th;				//�؂������摜�̊J�n�ʒu��y���W
	bool flag;
	int CreateFrame;
	int PatternCount;
	float rotation;		//�摜�̊p�x
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
	}
}
void SwordEffectUpdate() {
	for (int i = 0; i < SWORD_EFFECT_MAX; i++) {
		int age = g_SwordEffectFrameCount - SwordEffect[i].CreateFrame;
		if (SwordEffect[i].flag == true) {
			SwordEffect[i].PatternCount = age / SWORD_EFFECT_PATTERNCOUNT;
			SwordEffect[i].tw = patternWidth *  (SwordEffect[i].PatternCount % SWORD_EFFECT_PATTERNCOUNT);
			SwordEffect[i].th = patternHeight * (SwordEffect[i].PatternCount / SWORD_EFFECT_PATTERNCOUNT);
			if (SwordEffect[i].PatternCount > SWORD_EFFECT_PATTERNCOUNT) {
				SwordEffect[i].flag = false;
			}
		}
	}
	g_SwordEffectFrameCount++;
}

void SwordEffectDraw() {

	for (int i = 0; i < SWORD_EFFECT_MAX; i++) {
		if (SwordEffect[i].flag == true) {
			Sprite_Draw(TEXTURE_INDEX_SWORD_EFFECT, SwordEffect[i].dx, SwordEffect[i].dy, SwordEffect[i].tw, SwordEffect[i].th, 192, 192, 96, 96, 3.0f, 3.0f, SwordEffect[i].rotation);
		}
	}
}

void SwordEffectCreate(float cx, float cy,float rotation) {
	for (int i = 0; i < SWORD_EFFECT_MAX; i++) {
		if (SwordEffect[i].flag == false) {
			SwordEffect[i].dx = cx;
			SwordEffect[i].dy = cy;
			SwordEffect[i].rotation = rotation;
			SwordEffect[i].flag = true;
			SwordEffect[i].CreateFrame = g_SwordEffectFrameCount;
			SwordEffect[i].PatternCount = 0;
			break;
		}
	}
}
