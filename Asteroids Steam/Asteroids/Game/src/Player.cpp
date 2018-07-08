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


	if (Left && Right)	Left = Right = false;
	//float c = sin(mesh1_r.z * 3.14 / 180);
	//float d = cos(mesh1_r.z * 3.14 / 180);
	if(Left && Fuel>1){
		Fuel -= 2 * time/2;
		x += 15 * time;
		if ((mesh1_r.y >= 0 && mesh1_r.y <= 30) || (mesh1_r.y >= 330 && mesh1_r.y <= 360)) {
			mesh1_r.y -= 220 * time;
		}
	}
	if(Right && Fuel>1){
		Fuel -= 2 * time/2;
		x -= 15*time;
		if ((mesh1_r.y >= 0 && mesh1_r.y <= 30) || (mesh1_r.y >= 330 && mesh1_r.y <= 360)) {
			mesh1_r.y += 220 * time;
		}
	}
	if (!Left && !Right && mesh1_r.y < 2 && mesh1_r.y > -2) {
		mesh1_r.y = 0.0f;
	}
	if (!Left && !Right && mesh1_r.y != 0.0f)
	{
		if (mesh1_r.y > 180 && mesh1_r.y < 360) {
			mesh1_r.y += 120 * time;
		}
		if (mesh1_r.y < 180 && mesh1_r.y > 0) {
			mesh1_r.y -= 120 * time;
		}
	}

	if (move == Up && Fuel>1)  y += 12*time, Fuel -= 1 * time/2;
	else if (move == Down && Fuel>1)  y -= 12*time, Fuel -= 1 * time/2;
	else if (move == None)  speed = 0;
	if (x > 20) x = 20;
	if (x < -20) x = -20;
	if (y > 13) y = 13;
	if (y < -13) y = -13;

	if (Left || Right || move == Up || move == Down) {
		volShip += 240.0f * time;
		if (volShip > 100) volShip = 100;
	}
	else {
		volShip -= 190.0f * time;
		if (volShip < 15) volShip = 15;
	}
	for (it2 = entities.begin(); it2 != entities.end();)
	{
		Entity *b = *it2;//для удобства, чтобы не писать (*it)->
		if(b->name=="Asteroid")
		if(checkCollision(b)){
			SFMLDraw.push_back(new Explosion(0.0f, 0.0f, b->x, b->y, 0.0f));
			SFMLDraw.push_back(new Explosion(0.0f, 0.0f, x, y, 0.0f));
			b->life = false;
			life = false;
		}
		if(b->name=="Box")
		if(checkCollision(b)){
			countBox += 1;
			Score += 100;
			b->life = false;
			b->active = true;
		}
		if(b->name=="Fuel")
		if(checkCollision(b)){
			countFuel += 1;
			Fuel += 20;
			soundFuel = true;
			if (Fuel > 100) Fuel = 100;
			b->life = false;
			b->active = true;
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
	Fuel = 100; Score = 0;
	countBox = 0;
	countFuel = 0;
	soundFuel = true;
	move = KeyboardMove::None;
	speed = 0, health = 100;
	Left = false, Right = false; LeftClick = false;
	name = "Player";
}

Player::~Player()
{
	std::cout << "Dead" << std::endl;
}
