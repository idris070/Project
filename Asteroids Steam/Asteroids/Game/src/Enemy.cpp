#include "Enemy.h"
#include "Bullet.h"

Enemy::Enemy(Unit & unit, float X, float Y, float Speed) : Entity(unit, X, Y)
{
	SpeedX = random(-1, 1);
	SpeedY = random(Speed, Speed*1.5f);
}

void Enemy::update(float time)
{
	if (y < -20)	life = false;

	while (mesh1_r.x > 360)	mesh1_r.x -= 360;
	while (mesh1_r.x < 0)	mesh1_r.x += 360;
	
	while (mesh1_r.y > 360)	mesh1_r.y -= 360;
	while (mesh1_r.y < 0)	mesh1_r.y += 360;
	
	while (mesh1_r.z > 360)	mesh1_r.z -= 360;
	while (mesh1_r.z < 0)	mesh1_r.z += 360;

	mesh1_r.x += 8*SpeedY*time;
	mesh1_r.y += 8*SpeedX*time;
	mesh1_r.z += SpeedX*SpeedY*time;

	x += SpeedX * time;
	y -= SpeedY * time;
}
