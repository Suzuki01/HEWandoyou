#include"judgement.h"
#include<math.h>
#include"common.h"
#define UPDOWN_WIDTH (30)
#define LEFTRIGHT_WIDTH (30)
#define NANAME_WIDTH (30)

//âEè„Ç∆ç∂â∫ÇÃê¸
static float lineRU_1a;
static float lineRU_1b;
static float lineRU_1c;
static float lineRU_2c;

//ç∂è„Ç∆âEâ∫ÇÃê¸
static float lineLU_1a;
static float lineLU_1b;
static float lineLU_1c;
static float lineLU_2c;


void Jugment_Init(void)
{
	lineLU_1a = SCREEN_HEIGHT;
	lineLU_1b = -SCREEN_WIDTH;
	lineLU_1c = 0- NANAME_WIDTH/2;
	lineLU_2c=0+ NANAME_WIDTH / 2;

	lineRU_1a = -SCREEN_HEIGHT;
	lineRU_1b = -SCREEN_WIDTH;
	lineRU_1c = 0 - NANAME_WIDTH / 2;
	lineRU_2c = 0 + NANAME_WIDTH / 2;
}
bool Judgement(Direction input, indicator indicator)
{
	if (indicator.alive==0||indicator.state==TOOFAR||indicator.posX<0||indicator.posY<0)
	{
		return false;
	}
	float temp, temp2, temp3;
	switch (input)
	{
	case UP:
		if (indicator.posX+ indicator.r>(SCREEN_WIDTH/2- UPDOWN_WIDTH/2)&&indicator.posX- indicator.r< (SCREEN_WIDTH / 2 + UPDOWN_WIDTH / 2))
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case RIGHTUP:
		
		temp = (indicator.posX*lineRU_1a + indicator.posY*lineRU_1b + lineRU_1c)*(indicator.posX*lineRU_1a + indicator.posY*lineRU_1b + lineRU_1c);
		temp2= indicator.r*indicator.r*(lineRU_1a * lineRU_1a + lineRU_1b * lineRU_1b);
		temp3= (indicator.posX*lineRU_1a + indicator.posY*lineRU_1b + lineRU_1c)*(indicator.posX*lineRU_1a + indicator.posY*lineRU_1b + lineRU_2c);
		if (temp2<temp||temp2<temp3)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case RIGHT:
		if (indicator.posY-indicator.r>SCREEN_HEIGHT/2-LEFTRIGHT_WIDTH/2&& indicator.posY + indicator.r > SCREEN_HEIGHT / 2 - LEFTRIGHT_WIDTH / 2)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case RIGHTDOWN:
		//float temp, temp2, temp3;
		temp = (indicator.posX*lineLU_1a + indicator.posY*lineLU_1b + lineLU_1c)*(indicator.posX*lineLU_1a + indicator.posY*lineLU_1b + lineLU_1c);
		temp2 = indicator.r*indicator.r*(lineLU_1a * lineLU_1a + lineLU_1b * lineLU_1b);
		temp3 = (indicator.posX*lineLU_1a + indicator.posY*lineLU_1b + lineLU_1c)*(indicator.posX*lineLU_1a + indicator.posY*lineLU_1b + lineLU_2c);
		if (temp2 < temp || temp2 < temp3)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case DOWN:
		if (indicator.posX + indicator.r > (SCREEN_WIDTH / 2 - UPDOWN_WIDTH / 2) && indicator.posX - indicator.r < (SCREEN_WIDTH / 2 + UPDOWN_WIDTH / 2))
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case LEFTDOWN:
		//float temp, temp2, temp3;
		temp = (indicator.posX*lineRU_1a + indicator.posY*lineRU_1b + lineRU_1c)*(indicator.posX*lineRU_1a + indicator.posY*lineRU_1b + lineRU_1c);
		temp2 = indicator.r*indicator.r*(lineRU_1a * lineRU_1a + lineRU_1b * lineRU_1b);
		temp3 = (indicator.posX*lineRU_1a + indicator.posY*lineRU_1b + lineRU_1c)*(indicator.posX*lineRU_1a + indicator.posY*lineRU_1b + lineRU_2c);
		if (temp2 > temp || temp2 > temp3)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case LEFT:
		if (indicator.posY - indicator.r > SCREEN_HEIGHT / 2 - LEFTRIGHT_WIDTH / 2 && indicator.posY + indicator.r > SCREEN_HEIGHT / 2 - LEFTRIGHT_WIDTH / 2)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case LEFTUP:
		//float temp, temp2, temp3;
		temp = (indicator.posX*lineLU_1a + indicator.posY*lineLU_1b + lineLU_1c)*(indicator.posX*lineLU_1a + indicator.posY*lineLU_1b + lineLU_1c);
		temp2 = indicator.r*indicator.r*(lineLU_1a * lineLU_1a + lineLU_1b * lineLU_1b);
		temp3 = (indicator.posX*lineLU_1a + indicator.posY*lineLU_1b + lineLU_1c)*(indicator.posX*lineLU_1a + indicator.posY*lineLU_1b + lineLU_2c);
		if (temp2 < temp || temp2 < temp3)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case END:
		return false;
		break;
	default:
		break;
	}
}