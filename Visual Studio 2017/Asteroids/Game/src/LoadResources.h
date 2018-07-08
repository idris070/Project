#pragma once
#ifndef LOADRESOURCES_H
#define LOADRESOURCES_H
#include <SFML/Graphics.hpp>
#include "3D\Skinned_mesh.h"

extern sf::Texture TextureTank1,TextureTank2,TextureTank3,
	TextureBullet1,TextureBullet2,TextureBullet3,
	TextureWeapon1,TextureWeapon2,TextureWeapon3,
	TexturePlayerTank,TexturePlayerWeapon,
	TextureEmpty;
extern sf::Texture explos[24];

extern SkinnedMesh m_mesh;
extern SkinnedMesh m_mesh2;
extern SkinnedMesh GroundMesh;

extern SkinnedMesh MeshPlayer;
extern SkinnedMesh Asteroid;
extern SkinnedMesh Asteroid2;
extern SkinnedMesh Asteroid3;
extern SkinnedMesh Asteroid4;

bool initMesh();
void initTexture();
#endif  /* LOADRESOURCES_H */