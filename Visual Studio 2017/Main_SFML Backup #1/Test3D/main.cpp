////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
//#include "stdafx.h"
#ifdef _DEBUG
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#else
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-audio.lib")
#pragma comment(lib,"sfml-system.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-network.lib")
#endif
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

#include "Mesh.h"

#include <map>
#include <vector>
#include <gl\glew.h>

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\OpenGL.hpp>

#include <Importer.hpp>      // C++ importer interface
#include <scene.h>           // Output data structure
#include <postprocess.h>     // Post processing flags
#include <gl/GLU.h>
#include <gl/GL.h>


//#include <glaux.h>
// #include "glad.h" // узнать что это
//#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <list>

using namespace std;

//void DoTheSceneProcessing(const aiScene* x);

//struct Vertex {
//	sf::Vector3f Pos;
//	sf::Vector2f TexCord;
//	sf::Vector3f Normal;
//	Vertex(sf::Vector3f pPos, sf::Vector2f pTexCord, sf::Vector3f pNormal) {
//		Pos = pPos;
//		TexCord = pTexCord;
//		Normal = pNormal;
//	}
//};

struct Objects
{
	sf::Vector3f cord1;
	sf::Vector3f cord2;
	sf::Vector3f cord3;
	Objects(sf::Vector3f vect1, sf::Vector3f vect2, sf::Vector3f vect3)
	{
		cord1 = vect1;
		cord2 = vect2;
		cord3 = vect3;
	}
};

list<Objects*> testModel;
list<Objects*>::iterator it;

bool DoTheImportThing(const std::string& pFile)
{
	// Create an instance of the Importer class
	Assimp::Importer importer;
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(pFile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);
	// If the import failed, report it
	if (!scene)
	{
		//DoTheErrorLogging(importer.GetErrorString());
		return false;
	}
	cout << scene->mNumMeshes << endl;
	aiMesh **mesh = scene->mMeshes;
	cout << mesh[0]->mNumVertices << endl;
	cout << mesh[0]->mNumFaces << endl;
	for (size_t y = 0; y < scene->mNumMeshes; y++)
	{
		aiFace* face = mesh[y]->mFaces;
		aiVector3D *vect = mesh[y]->mVertices;
		cout << face->mNumIndices << endl;
		cout << "End Mesh " << endl;
		if(face->mNumIndices==3)
		for (size_t i = 0; i < mesh[y]->mNumFaces; i++)
		{
			//cout << face[i].mIndices[0] << endl;
			//cout << face[i].mIndices[1] << endl;
			//cout << face[i].mIndices[2] << endl;
			float x1 = vect[face[i].mIndices[0]].x;
			float y1 = vect[face[i].mIndices[0]].y;
			float z1 = vect[face[i].mIndices[0]].z;
			float x2 = vect[face[i].mIndices[1]].x;
			float y2 = vect[face[i].mIndices[1]].y;	
			float z2 = vect[face[i].mIndices[1]].z;
			float x3 = vect[face[i].mIndices[2]].x;
			float y3 = vect[face[i].mIndices[2]].y;
			float z3 = vect[face[i].mIndices[2]].z;
			testModel.push_back(new Objects(sf::Vector3f(x1, y1, z1), sf::Vector3f(x2, y2, z2), sf::Vector3f(x3, y3, z3)));
		}
	}
	return true;
}

////////////////////////////////////////////////////////////
/// Entry point of application
////////////////////////////////////////////////////////////

#define INVALID_OGL_VALUE2 0xFFFFFFFF

	sf::Texture t1;
int main2(int argc, char** argv)
{
	
	//DoTheImportThing("test3.dae");
	Mesh* m_pMesh;

	m_pMesh = new Mesh();


	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 0;
	settings.majorVersion = 3;
	settings.minorVersion = 0;


	// Create the main window
	sf::RenderWindow App(sf::VideoMode(800, 600, 32), "SFML OpenGL", sf::Style::Default, settings);
	Magick::InitializeMagick(*argv);
	glewInit();

	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	t1.loadFromFile("Spinnen_Bein_tex_COLO.jpg");
	sf::Texture::bind(&t1);
	// Create a clock for measuring time elapsed
	sf::Clock Clock;

	//prepare OpenGL surface for HSR
	glShadeModel(GL_SMOOTH);      // Разрешение сглаженного закрашивания
	glClearColor(0.3f, 0.3f, 0.3f, 0.f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//// Setup a perspective projection & Camera position
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.f, 1.0f, 1.0f, 100.0f);//fov, aspect, zNear, zFar



	bool rotate = true;
	float angle;
	float xRot = 0, Yrot = 0;

	m_pMesh->LoadMesh("spider.dae");
	// Start game loop
	while (App.isOpen())
	{
		// Process events
		sf::Event Event;
		while (App.pollEvent(Event))
		{
			// Close window : exit
			if (Event.type == sf::Event::Closed)
				App.close();

			// Escape key : exit
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
				App.close();

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::A)) {
				rotate = !rotate;
			}

		}

		//Prepare for drawing
		// Clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Apply some transformations for the cube
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.f, 0.f, -5.f);
		//gluLookAt(0.0, 0.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

		if (rotate) {
			angle = Clock.getElapsedTime().asSeconds();
		}
		if((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Left))
		{
			xRot -= 0.1;
		}
		if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Right))
		{
			xRot += 0.1;
		}
		if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Down))
		{
			Yrot -= 0.1;
		}
		if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Up))
		{
			Yrot += 0.1;
		}
		glRotatef(angle * 70, 1.f, 0.f, 0.f);
		glRotatef(angle * 30, 0.f, 1.f, 0.f);
		glRotatef(angle * 90, 0.f, 0.f, 1.f);

		m_pMesh->Render();

		//Draw a cube
		glBegin(GL_TRIANGLES);//draw some squares
		for (it = testModel.begin(); it != testModel.end();)
		{
			Objects *b = *it;
			glTexCoord2f(0.0f, 0.0f); glVertex3f(b->cord1.x, b->cord1.y, b->cord1.z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(b->cord2.x, b->cord2.y, b->cord2.z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(b->cord3.x, b->cord3.y, b->cord3.z);
			it++;
		}
		//glColor3f(0, 1, 0);
		//glTexCoord2f(0.0f, 0.0f); glVertex3f(1.f, 1.f, -1.f);
		//glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.f, 1.f, -1.f);
		//glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.f, 1.f, 1.f);
		//glTexCoord2f(0.0f, 1.0f); glVertex3f(1.f, 1.f, 1.f);
		//
		glEnd();

		// Finally, display rendered frame on screen
		App.display();
	}

	return EXIT_SUCCESS;
}