#include "Explosion.h"

Explosion::Explosion(float X, float Y, float lastX, float lastY, float Rotation){//всё так же, только взяли в конце состояние игрока (int dir)
	x = X; y = Y; num = 0;
	last_x = lastX;
	last_y = lastY;

	life = true;
	sprite.setTexture(explos[0]);
	sprite.setOrigin(64/2, 64/2);

	rotation = Rotation;
	//sprite.setRotation(rotation);
	sprite.setPosition(x, y);//задается позицию
}

void Explosion::update(float time)
{
	name = "Explosion";
	timeS = Time.getElapsedTime().asMilliseconds();
	num = timeS/30;
	sprite.setTexture(explos[num]);
	sprite.setPosition(x, y);//задается позицию

	if (timeS >= 30 * 23)
		life = false;
}

std::list<Explosion*> SFMLDraw;
std::list<Explosion*>::iterator SFMLit;