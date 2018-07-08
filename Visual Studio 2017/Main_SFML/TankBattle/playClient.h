#include <gl\glew.h>
#include <SFML/Graphics.hpp>
#include <SFML\OpenGL.hpp>
#include <SFML\Window.hpp>
#include <SFML/Network.hpp>

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
#include "src/level.h"
#include "src/HandyMathLib\hm.h"
#include "src/Objects.h"
#include "src/LoadResources.h"

int play(sf::RenderWindow & window);