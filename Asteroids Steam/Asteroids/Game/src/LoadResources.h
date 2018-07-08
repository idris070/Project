#pragma once
#ifndef LOADRESOURCES_H
#define LOADRESOURCES_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "3D/Skinned_mesh.h"

class EntitySound {
public:
	sf::Music alien, fuelOff, lose, pickBox, pickFuel, select, shipMove, unlock, win, musicGame, musicMenu, hover, click, explosion;
	EntitySound();
private:
};

extern sf::Texture TextureTank1,TextureTank2,TextureTank3,
	TextureBullet1,TextureBullet2,TextureBullet3,
	TextureWeapon1,TextureWeapon2,TextureWeapon3,
	TexturePlayerTank,TexturePlayerWeapon,
	TextureEmpty;
extern sf::Texture explos[24];

extern SkinnedMesh MeshPlayer;

extern SkinnedMesh Asteroid1_1;
extern SkinnedMesh Asteroid1_2;
extern SkinnedMesh Asteroid1_3;
extern SkinnedMesh Asteroid2_1;
extern SkinnedMesh Asteroid2_2;
extern SkinnedMesh Asteroid2_3;
extern SkinnedMesh Asteroid3_1;
extern SkinnedMesh Asteroid3_2;
extern SkinnedMesh Asteroid3_3;
extern SkinnedMesh Fuel;
extern SkinnedMesh Box1;
extern SkinnedMesh Box2;

extern SkinnedMesh BackgroundMap1;
extern SkinnedMesh BackgroundMap2;
extern SkinnedMesh BackgroundMap3;
extern SkinnedMesh BackgroundMap4;

extern EntitySound SoundList;

bool initMesh();
void initTexture();
#endif  /* LOADRESOURCES_H */