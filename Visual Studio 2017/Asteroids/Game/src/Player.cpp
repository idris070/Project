#include "Player.h"
#include "Bullet.h"

void Player::rotation(float time, sf::Vector2f pos){
	float dX = pos.x - x;
	float dY = pos.y - y;
	float rotP = sprite2.getRotation();
	float rotation = (atan2(dY,dX)) * 180 / 3.14159265 + 90; // получаем угол в радианах и переводим его в градусы
	if(rotation<0)rotation +=360;
	if(rotP+180>360){
		if(rotation>rotP || rotation < rotP-180) sprite2.rotate(0.5*time*0.1);
		else sprite2.rotate(-0.5*time*0.1);
	}else{
		if(rotP<rotation && rotation<rotP+180) sprite2.rotate(0.5*time*0.1);
		else sprite2.rotate(-0.5*time*0.1);
	}
}

void Player::update(float time)
{
	while (mesh1_r.x > 360)	mesh1_r.x -= 360;
	while (mesh1_r.x < 0)	mesh1_r.x += 360;

	while (mesh1_r.y > 360)	mesh1_r.y -= 360;
	while (mesh1_r.y < 0)	mesh1_r.y += 360;

	while (mesh1_r.z > 360)	mesh1_r.z -= 360;
	while (mesh1_r.z < 0)	mesh1_r.z += 360;


	//float c = sin(mesh1_r.z * 3.14 / 180);
	//float d = cos(mesh1_r.z * 3.14 / 180);
	if(Left){
		x += 10*time;
		if ((mesh1_r.y >= 0 && mesh1_r.y <= 30) || (mesh1_r.y >= 330 && mesh1_r.y <= 360)) {
			mesh1_r.y -= 220 * time;
		}
	}
	if(Right){
		x -= 10*time;
		if ((mesh1_r.y >= 0 && mesh1_r.y <= 30) || (mesh1_r.y >= 330 && mesh1_r.y <= 360)) {
			mesh1_r.y += 220 * time;
		}
	}
	if(!Left)
	if (mesh1_r.y > 180 && mesh1_r.y < 360) {
		mesh1_r.y += 120 * time;
	}
	if(!Right)
	if (mesh1_r.y < 180 && mesh1_r.y > 0) {
		mesh1_r.y -= 120 * time;
	}
	if (move == Up)  y += 10*time;
	else if (move == Down)  y -= 10*time;
	else if (move == None)  speed = 0;
	if (x > 20) x = 20;
	if (x < -20) x = -20;
	if (y > 13) y = 13;
	if (y < -13) y = -13;
	for (it2 = entities.begin(); it2 != entities.end();)
	{
		Entity *b = *it2;//для удобства, чтобы не писать (*it)->
		if(checkCollision(b)){
			SFMLDraw.push_back(new Explosion(0.0f, 0.0f, b->x, b->y, 0.0f));
			SFMLDraw.push_back(new Explosion(0.0f, 0.0f, x, y, 0.0f));
			b->life = false;
			life = false;
		}
		it2++;
	}
	/*if(LeftClick)
	if(reload<secondTimer){
		reload = secondTimer + 0.5;
		float angl = sprite2.getRotation() *3.14/180;
		vec2 vecX = rotate(vec2(0,1),angl);
		vec2 vecY = rotate(vec2(1,0),angl);
		sf::Vector2f c1((weaponGun.y*vecX.x + weaponGun.x*vecY.x) + x, (weaponGun.y*vecX.y + weaponGun.x*vecY.y) + y);
		entities.push_back(new Bullet(bullet, c1.x, c1.y, sprite2.getRotation(), "Player"));
	}*/

	/*if(PassedTime>moveTimer)
	{
		moveTimer = PassedTime + 0.1;
	}*/
//sprite.setPosition(x, y);
//sprite2.setPosition(x, y);
}

Player::Player(Unit & unit, float X, float Y): Entity(unit, X, Y)
{
	move = KeyboardMove::None;
	speed = 0, health = 100;
	Left = false, Right = false; LeftClick = false;
	name = "Player";
}

Player::~Player()
{
	std::cout << "Dead" << std::endl;
}
