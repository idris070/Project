#ifndef PLAYCLIENT_H
#define PLAYCLIENT_H

#define WINDOW_WIDTH 1.5f
#define WINDOW_HEIGHT 1.0f
#define COLOR_TEXTURE_UNIT              GL_TEXTURE0
#define COLOR_TEXTURE_UNIT_INDEX        0

#include <gl\glew.h>
#include <SFML\Graphics.hpp>
#include <SFML\OpenGL.hpp>
#include <SFML\Window.hpp>
#include <SFML\Network.hpp>
#include <SFML\Audio.hpp>

#include "src\3D\Util.h"
#include "src\3D\Pipeline.h"
#include "src\3D\Camera.h"
#include "src\3D\Skinning_technique.h"
#include "src\3D\Skinned_mesh.h"

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <math.h>
#include "src\Objects.h"
#include "src\LoadResources.h"

int play(sf::RenderWindow & window);
int main2();

extern sf::RenderWindow *windowMain;

extern sf::Texture PlayT, PlayHoverT, SettingT, SettingHoverT, ExitT, ExitHoverT, BackgroundT, PanelT;

#endif /* PLAYCLIENT_H */