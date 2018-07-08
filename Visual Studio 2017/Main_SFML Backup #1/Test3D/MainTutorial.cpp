#include <GL\glew.h>
#include <SFML\Window.hpp>

#include <stdio.h>
#include <string>
#include <string.h>

#include <iostream>
#include <fstream>

#pragma comment(lib, "glew32.lib")

#include "Util.h"
#include "Pipeline.h"
#include "Camera.h"
#include "Texture.h"
//#include "lighting_technique.h"
//#include "Mesh.h"
#include "Skinning_technique.h"
#include "Skinned_mesh.h"


#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define COLOR_TEXTURE_UNIT              GL_TEXTURE0
#define COLOR_TEXTURE_UNIT_INDEX        0

long long m_frameTime;
long long m_startTime;
int m_frameCount;
int m_fps;

#ifndef WIN32
Markup sMarkup = { (char*)"Arial", 64, 1, 0, 0.0, 0.0,
{ .1,1.0,1.0,.5 },{ 1,1,1,0 },
0,{ 1,0,0,1 }, 0,{ 1,0,0,1 },
0,{ 0,0,0,1 }, 0,{ 0,0,0,1 } };
#endif

void CalcFPS()
{
	m_frameCount++;

	long long time = GetCurrentTimeMillis();

	if (time - m_frameTime >= 1000) {
		m_frameTime = time;
		m_fps = m_frameCount;
		m_frameCount = 0;
	}
}

void RenderFPS()
{
	char text[32];
	ZERO_MEM(text);
	SNPRINTF(text, sizeof(text), "FPS: %d", m_fps);

#ifndef WIN32
	m_fontRenderer.RenderText(10, 10, text);
#endif
}

float GetRunningTime()
{
	float RunningTime = (float)((double)GetCurrentTimeMillis() - (double)m_startTime) / 1000.0f;
	return RunningTime;
}

//GLuint m_VBO;
//GLuint m_IBO;
//GLuint gWVPLocation;
//GLuint gSampler;
//
//LightingTechnique* m_pEffect;
//Texture* m_pTexture = NULL;
//Camera* m_pGameCamera = NULL;
//float m_scale;
//DirectionalLight m_directionalLight;
//Mesh* m_pMesh;
//
//PersProjInfo m_persProjInfo;

SkinningTechnique* m_pEffect;
Camera* m_pGameCamera;
DirectionalLight m_directionalLight;
SkinnedMesh m_mesh;
SkinnedMesh m_mesh2;
Vector3f m_position;
PersProjInfo m_persProjInfo;

const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";

static const float FieldDepth = 20.0f;
static const float FieldWidth = 10.0f;

bool Init()
{
	m_frameCount = 0;
	m_frameTime = 0;
	m_fps = 0;

	m_frameTime = m_startTime = GetCurrentTimeMillis();

	Vector3f Pos(0.0f, 0.0f, 25.0f);
	Vector3f Target(0.0f, .5f, -1.0f);
	Vector3f Up(0.0f, 1.0f, 0.0f);

	m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

	m_pEffect = new SkinningTechnique();

	if (!m_pEffect->Init()) {
		printf("Error initializing the lighting technique\n");
		return false;
	}

	m_pEffect->Enable();

	m_pEffect->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
	m_pEffect->SetDirectionalLight(m_directionalLight);
	m_pEffect->SetMatSpecularIntensity(0.0f);
	m_pEffect->SetMatSpecularPower(0);

	if (!m_mesh2.LoadMesh("gun.fbx")) {
		printf("Mesh load failed\n");
		return false;
	}
	//if (!m_mesh.LoadMesh("boblampclean.md5mesh")) {
	if (!m_mesh.LoadMesh("tank.fbx")) {
		printf("Mesh load failed\n");
		return false;
	}

#ifndef WIN32
	if (!m_fontRenderer.InitFontRenderer()) {
		return false;
	}
#endif        	
	return true;
}
float m_scale;
static void RenderScene()
{
	m_scale += 0.057f;
	/*

	m_pGameCamera->OnRender();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	Pipeline p;
	p.Scale(0.1f, 0.1f, 0.1f);
	p.Rotate(0.0f, 90, m_scale);
	p.WorldPos(0.0f, 0.0f, 10.0f);
	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);
	m_pEffect->SetWVP(p.GetWVPTrans());
	m_pEffect->SetWorldMatrix(p.GetWorldTrans());
	m_pEffect->SetDirectionalLight(m_directionalLight);
	m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());
	m_pEffect->SetMatSpecularIntensity(0.0f);
	m_pEffect->SetMatSpecularPower(0);

	glEnable(GL_TEXTURE);
	m_pMesh->Render();*/
	
	//CalcFPS();

	m_pGameCamera->OnRender();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pEffect->Enable();

	vector<Matrix4f> Transforms;

	float RunningTime = GetRunningTime();

	m_mesh.BoneTransform(RunningTime, Transforms);

	for (uint i = 0; i < Transforms.size(); i++) {
		m_pEffect->SetBoneTransform(i, Transforms[i]);
	}

	m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());

	Pipeline p;
	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);
	p.Scale(1.f, 1.f, 1.f);

	Vector3f Pos(m_position);
	p.WorldPos(Pos);
	p.Rotate(0.0f, 0.0f, 0.0f);
	m_pEffect->SetWVP(p.GetWVPTrans());
	m_pEffect->SetWorldMatrix(p.GetWorldTrans());

	m_mesh.Render(*m_pEffect);

	m_mesh2.BoneTransform(0, RunningTime, Transforms);

	for (uint i = 0; i < Transforms.size(); i++) {
		m_pEffect->SetBoneTransform(i, Transforms[i]);
	}
	p.WorldPos(Vector3f(.0f, -0.19f, 2.87+6));
	p.Rotate(0.0f, 0.0f, m_scale);
	m_pEffect->SetWVP(p.GetWVPTrans());
	m_pEffect->SetWorldMatrix(p.GetWorldTrans());
	m_mesh2.Render(*m_pEffect);

	RenderFPS();
}

static void SpecialKeyboardCB(int Key)
{
	if (Key == sf::Keyboard::A) {
		m_directionalLight.AmbientIntensity += 0.05f;
		return;
	}
	if (Key == sf::Keyboard::S) {
		m_directionalLight.AmbientIntensity -= 0.05f;
		return;
	}
	if (Key == sf::Keyboard::Z) {
		m_directionalLight.DiffuseIntensity += 0.05f;
		return;
	}
	if (Key == sf::Keyboard::X) {
		m_directionalLight.DiffuseIntensity -= 0.05f;
		return;
	}
	//if (Key == sf::Keyboard::Right) {
	//	//m_startTime = GetCurrentTimeMillis();
	//	//m_mesh.SetAnimation(aiString("die"));
	//	if (m_mesh.iAnim < 14) {
	//		m_mesh.iAnim++;
	//	}
	//}
	//if (Key == sf::Keyboard::Left) {
	//	//m_startTime = GetCurrentTimeMillis();
	//	//m_mesh.SetAnimation(aiString("Attack"));
	//	if (m_mesh.iAnim != 0) {
	//		m_mesh.iAnim--;
	//	}
	//}
m_pGameCamera->OnKeyboard(Key);
}
static void PassiveMouseCB(int x, int y)
{
	m_pGameCamera->OnMouse(x, y);
}
int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 32;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 0;


	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "SFML OpenGL", sf::Style::Default, settings);
	window.setFramerateLimit(60);

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	m_pGameCamera = NULL;
	m_pEffect = NULL;
	m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.55f;
	m_directionalLight.DiffuseIntensity = 0.9f;
	m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);

	m_persProjInfo.FOV = 60.0f;
	m_persProjInfo.Height = WINDOW_HEIGHT;
	m_persProjInfo.Width = WINDOW_WIDTH;
	m_persProjInfo.zNear = 1.0f;
	m_persProjInfo.zFar = 100000.0f; // дальность видимости (или туман так скажем!)

	m_position = Vector3f(0.0f, 0.0f, 6.0f);

	Init();

	printf("GL version: %s\n", glGetString(GL_VERSION));

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glFrontFace(GL_CW);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);      // –азрешение сглаженного закрашивани€
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.8f);

	while (window.isOpen())
	{
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			if (Event.type == sf::Event::KeyPressed)
			{
				//m_pGameCamera->OnKeyboard(Event.key.code);
				SpecialKeyboardCB(Event.key.code);
			}
			if (Event.type == sf::Event::MouseWheelScrolled)
			{
				if ((Event.mouseWheelScroll.delta) > 0){
					m_pGameCamera->SetPos(1);
				}else {
					m_pGameCamera->SetPos(-1);
				}
			}
			if (Event.type == sf::Event::MouseMoved)
			{
				//PassiveMouseCB(Event.mouseMove.x, Event.mouseMove.y);
			}
			if (Event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (Event.type == sf::Event::Resized)
			{
				// adjust the viewport when the window is resized
				glViewport(0, 0, Event.size.width, Event.size.height);
			}
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
				window.close();
		}
		
		RenderScene();

		window.display();
	}
	return 0;
}