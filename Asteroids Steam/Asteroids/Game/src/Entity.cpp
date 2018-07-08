#include "Entity.h"

Entity::Entity(){

}

Entity::Entity(sf::Image &image, float X, float Y, int W, int H){
	posMouse.x = 0; posMouse.y = 0; secondTimer = 0;
	x = X; y = Y; w = W; h = H; 
	life = true;
	//sprite.setTexture(texture);
	//sprite.setOrigin(w/2, h/2);
	size = sqrt(w*w+h*h);
	sizeWidth = size;
	sizeHeight = size;
}

Entity::Entity(sf::Texture &Texture, float X, float Y, int W, int H){
	posMouse.x = 0; posMouse.y = 0; secondTimer = 0;
	x = X; y = Y; w = W; h = H; 
	life = true;
	sprite.setTexture(Texture);
	sprite.setOrigin(w/2, h/2);
	size = sqrt(w*w+h*h);
	sizeWidth = size;
	sizeHeight = size;
}

Entity::Entity(Basis &basis, float X, float Y){
	posMouse.x = 0; posMouse.y = 0; secondTimer = 0;
	x = X; y = Y; w = basis.width; h = basis.height; 
	life = true;
	sprite.setTexture(*basis.texture);
	sprite.setOrigin(w/2, h/2);
	size = sqrt(w*w+h*h);
	sizeWidth = size;
	sizeHeight = size;
}

sf::FloatRect Entity::getRect(){
	return sf::FloatRect(x-sizeWidth/2, y-sizeHeight/2, sizeWidth, sizeHeight);
}

vec2 Entity::rotate(vec2 point, float angle){
	vec2 rotated_point;
	rotated_point.x = point.x * cos(angle) - point.y * sin(angle);
	rotated_point.y = point.x * sin(angle) + point.y * cos(angle);
	return rotated_point;
}

int Entity::checkCollision(Entity* b)
{
	float mx, my;
	mx = x - b->x;
	my = y - b->y;
	float distance = sqrt(mx*mx + my*my);
	if (b!= this && distance < 1.0f+b->meshScale) {
		return 1;
	}
	return 0;
}

Entity::Entity(Unit & unit, float X, float Y)
{
	name = unit.Name;
	trackTimer = 10;
	mesh1_r.x = 0;
	mesh1_r.y = 0;
	mesh1_r.z = 0;
	mesh2_r = 0;
	life = true;
	active = false;
	x = X;
	y = Y;
	meshScale = 1;
	if (unit.Name == "Asteroid") {
		meshScale = random(0.75f, 1.25f);
	}
	int randMesh = 1;
	if(unit.countMesh==3)	randMesh = random(1, 3);
	if(unit.countMesh==2)	randMesh = random(1, 2);
	switch (randMesh)
	{
	case 1: mesh = unit.Mesh1; break;
	case 2: mesh = unit.Mesh2; break;
	case 3: mesh = unit.Mesh3; break;
	default:
		break;
	}
}

std::list<Entity*> entities;
std::list<Entity*> entenemy;
std::list<Entity*>::iterator it;
std::list<Entity*>::iterator it2;

// Класс Entity //