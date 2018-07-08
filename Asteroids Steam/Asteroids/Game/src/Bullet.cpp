#include "Bullet.h"

sf::FloatRect Bullet::getRect(){
	return sf::FloatRect(x-sizeWidth/2, y-sizeHeight/2, sizeWidth, sizeHeight);
}

void Bullet::update(float time){
	timeS = Time.getElapsedTime().asSeconds();
	x += c * time*speed;
	y -= d * time*speed;
	if(timeS >= 5)	life = false;
	for (it2 = entenemy.begin(); it2 != entenemy.end();)
	{
		Entity *enemy2 = *it2;
		if(checkCollision(enemy2))
		{
			enemy2->health -= damage;
			if(enemy2->health <= 0)
			{
				enemy2->life = false;
			}
			life = false;
			//entities.push_back(new Explosion(x, y, rotation));
			break;
		}
		it2++;
	}
	sprite.setPosition(x, y);//задается позицию пуле
}