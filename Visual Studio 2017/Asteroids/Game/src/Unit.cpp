#include "Unit.h"
#include "LoadResources.h"

Unit *UnitPlayer;
Unit *UnitAsteroid;
Unit *UnitAsteroid2;
Unit *UnitAsteroid3;
Unit *UnitAsteroid4;

Basis::Basis()
{
	texture = &TextureEmpty;
}
Tank::Tank(){}
Tank::Tank(sf::String Name){
	if(Name=="tank1")
	{
		width = 34;
		height = 48;
		texture = &TextureTank1;
		health = 40;
		speed = 0.04f;
		name = Name;
	}
	if(Name=="tank2")
	{
		width = 42;
		height = 48;
		texture = &TextureTank2;
		health = 80;
		speed = 0.02f;
		name = Name;
	}
	if(Name=="tank3")
	{
		width = 26;
		height = 39;
		texture = &TextureTank3;
		health = 20;
		speed = 0.06f;
		name = Name;
	}
}
BulletTank::BulletTank(){}
BulletTank::BulletTank(sf::String Name){
	if(Name=="bullet1"){
		width = 64;
		height = 64;
		texture = &TextureBullet1;
		damage = 5;
		speed = 0.5f;
		widthBullet = 12;
		heightBullet = 38;
		name = Name;
	}
	if(Name=="bullet2"){
		width = 64;
		height = 64;
		texture = &TextureBullet2;
		damage = 10;
		speed = 0.3f;
		widthBullet = 17;
		heightBullet = 28;
		name = Name;
	}
	if(Name=="bullet3"){
		width = 64;
		height = 64;
		texture = &TextureBullet3;
		damage = 15;
		speed = 0.2f;
		widthBullet = 21;
		heightBullet = 31;
		name = Name;
	}
}
WeaponTank::WeaponTank(){}
WeaponTank::WeaponTank(sf::String Name){
	if(Name=="weapon1"){
		width = 56;
		height = 56;
		texture = &TextureWeapon1;
		GunOrigin = sf::Vector2i(0,-27);
		bullet = BulletTank("bullet1");
		name = Name;
	}
	if(Name=="weapon2"){
		width = 66;
		height = 66;
		texture = &TextureWeapon2;
		GunOrigin = sf::Vector2i(0,-33);
		bullet = BulletTank("bullet2");
		name = Name;
	}
	if(Name=="weapon3"){
		width = 56;
		height = 56;
		texture = &TextureWeapon3;
		GunOrigin = sf::Vector2i(0,-27);
		bullet = BulletTank("bullet3");
		name = Name;
	}
}

void initUnit()
{
	UnitPlayer = new Unit();
	UnitPlayer->countMesh = 1;
	UnitPlayer->Name = "Player";
	UnitPlayer->Body = &MeshPlayer;

	UnitAsteroid = new Unit();
	UnitAsteroid->countMesh = 1;
	UnitAsteroid->Name = "Asteroid";
	UnitAsteroid->Body = &Asteroid;

	UnitAsteroid2 = new Unit();
	UnitAsteroid2->countMesh = 1;
	UnitAsteroid2->Name = "Asteroid";
	UnitAsteroid2->Body = &Asteroid2;

	UnitAsteroid3 = new Unit();
	UnitAsteroid3->countMesh = 1;
	UnitAsteroid3->Name = "Asteroid";
	UnitAsteroid3->Body = &Asteroid3;

	UnitAsteroid4 = new Unit();
	UnitAsteroid4->countMesh = 1;
	UnitAsteroid4->Name = "Asteroid";
	UnitAsteroid4->Body = &Asteroid4;
}

