#include "playClient.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define COLOR_TEXTURE_UNIT              GL_TEXTURE0
#define COLOR_TEXTURE_UNIT_INDEX        0

SkinningTechnique* m_pEffect;
Camera* m_pGameCamera;
DirectionalLight m_directionalLight;
PersProjInfo m_persProjInfo;
Player* player;

void initCamera()
{
	m_pGameCamera = NULL;

	math3d::Vector3f Pos(0.0f, 0.0f, 25.0f);
	math3d::Vector3f Target(0.0f, .25f, -1.0f);
	math3d::Vector3f Up(0.0f, 1.0f, 0.0f); // это вращение смотреть в точку Target, с позиции при этом вращая камеру

	m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);
}
void initOpenGL()
{
	GLenum res = glewInit();
	// Параметры для Pipeline.SetPerspectiveProj
	m_persProjInfo.FOV = 60.0f; // Это еще не определил что такое!
	m_persProjInfo.Height = WINDOW_HEIGHT;
	m_persProjInfo.Width = WINDOW_WIDTH;
	m_persProjInfo.zNear = 1.0f; // Не показывает полигоны близкие к камере!
	m_persProjInfo.zFar = 1000.0f; // дальность видимости (или туман так скажем!)
	// part 1 //
	m_directionalLight.Color = math3d::Vector3f(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.5f; // 0.55f standart // Яркость мира
	m_directionalLight.DiffuseIntensity = 0.3f; // 0.9f standart // Яркость направленного света
	m_directionalLight.Direction = math3d::Vector3f(0.0f, 0.0, -1.0);
	// part 2 //
	m_pEffect = NULL;
	m_pEffect = new SkinningTechnique();
	if (!m_pEffect->Init()) {
		printf("Error initializing the lighting technique\n");
		//return false;
	}
	m_pEffect->Enable();
	m_pEffect->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
	m_pEffect->SetDirectionalLight(m_directionalLight);
	m_pEffect->SetMatSpecularIntensity(0.0f);
	m_pEffect->SetMatSpecularPower(0);
	// part 3 // OpenGL Setting
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.8f);
}
void Init()
{
	initCamera();
	initOpenGL();
	initMesh();
	initUnit();
	
	player = new Player(*Tank1, 0, 0);
	entities.push_back(player);
	entities.push_back(new Player(*Tank1, 5, 5));
}

void DrawMap() {
	glBegin(GL_QUADS);//draw some squares
	glColor3i(0, 1, 1);
	glVertex3f(-50.f, -50.f, -50.f);
	glVertex3f(-50.f, 50.f, -50.f);
	glVertex3f(50.f, 50.f, -50.f);
	glVertex3f(50.f, -50.f, -50.f);

	glColor3f(0, 0, 1);
	glVertex3f(-50.f, -50.f, 50.f);
	glVertex3f(-50.f, 50.f, 50.f);
	glVertex3f(50.f, 50.f, 50.f);
	glVertex3f(50.f, -50.f, 50.f);

	glColor3f(1, 0, 1);
	glVertex3f(-50.f, -50.f, -50.f);
	glVertex3f(-50.f, 50.f, -50.f);
	glVertex3f(-50.f, 50.f, 50.f);
	glVertex3f(-50.f, -50.f, 50.f);

	glColor3f(0, 1, 0);
	glVertex3f(50.f, -50.f, -50.f);
	glVertex3f(50.f, 50.f, -50.f);
	glVertex3f(50.f, 50.f, 50.f);
	glVertex3f(50.f, -50.f, 50.f);

	glColor3f(1, 1, 0);
	glVertex3f(-50.f, -50.f, 50.f);
	glVertex3f(-50.f, -50.f, -50.f);
	glVertex3f(50.f, -50.f, -50.f);
	glVertex3f(50.f, -50.f, 50.f);

	glColor3f(1, 0, 0);
	glVertex3f(-50.f, 50.f, 50.f);
	glVertex3f(-50.f, 50.f, -50.f);
	glVertex3f(50.f, 50.f, -50.f);
	glVertex3f(50.f, 50.f, 50.f);

	glEnd();
}

static void RenderScene(float time)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pGameCamera->OnRender();
	m_pEffect->Enable();
	m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());
	
	Pipeline p;
	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);
	p.Scale(1.f, 1.f, 1.f);

	std::vector<Matrix4f> Transforms;

	for (it = entities.begin(); it != entities.end();)
	{
		Entity *b = *it;
		if (b->name == "Enemy" || b->name == "Player")
			b->mesh1->BoneTransform(1, b->trackTimer, Transforms);
			for (uint i = 0; i < Transforms.size(); i++) {
				m_pEffect->SetBoneTransform(i, Transforms[i]);
			}
			p.WorldPos(b->x, b->y, 0);
			p.Rotate(0, 0, b->mesh1_r);
			m_pEffect->SetWVP(p.GetWVPTrans());
			m_pEffect->SetWorldMatrix(p.GetWorldTrans());
			b->mesh1->Render(*m_pEffect);

			b->mesh2->BoneTransform(time, Transforms);
			for (uint i = 0; i < Transforms.size(); i++) {
				m_pEffect->SetBoneTransform(i, Transforms[i]);
			}
			p.WorldPos(b->m2loc.x + b->x, b->m2loc.y + b->y, b->m2loc.z);
			p.Rotate(0, 0, b->mesh2_r);
			m_pEffect->SetWVP(p.GetWVPTrans());
			m_pEffect->SetWorldMatrix(p.GetWorldTrans()); 
			b->mesh2->Render(*m_pEffect);
			//b->secondTimer = second.getElapsedTime().asSeconds();
		//b->update(time);
		if (b->life == false) { entenemy.remove(*it); it = entities.erase(it); delete b; }
		else it++;
	}
	
	// Рендер поверхности
	Matrix4f Identity;
	Identity.InitIdentity();
	for (size_t i = 0; i < 10; i++)
	{
		m_pEffect->SetBoneTransform(i, Identity);
	}
	p.WorldPos(0, 0, 0);
	p.Rotate(0, 0, 0);
	p.Scale(20, 20, 0);
	m_pEffect->SetWVP(p.GetWVPTrans());
	GroundMesh.Render(*m_pEffect);
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
	sf::Font font;
	font.loadFromFile("arial.ttf");
	sf::Text text;
	text.setFont(font); // font is a sf::Font
	text.setCharacterSize(24); // in pixels, not points!
	text.setFillColor(sf::Color::Red);
	
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "OpenGL", sf::Style::Default, sf::ContextSettings(32)); 
	window.setFramerateLimit(60);

	Init();

	sf::Clock clock;
	sf::Clock second;

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		float timeSec = second.getElapsedTime().asSeconds();
		clock.restart();
		time /= 800;

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
				if ((Event.mouseWheelScroll.delta) > 0) {
					m_pGameCamera->SetPos(1);
				}
				else {
					m_pGameCamera->SetPos(-1);
				}
			}
			if (Event.type == sf::Event::MouseMoved)
			{
				//PassiveMouseCB(Event.mouseMove.x, Event.mouseMove.y);
			}
			if (Event.type == sf::Event::Closed || (Event.key.code == sf::Keyboard::Escape))
			{
				window.close();
			}
			if (Event.type == sf::Event::Resized)
			{
				glViewport(0, 0, Event.size.width, Event.size.height);
			}
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) player->move = Up;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) player->move = Down;
			else player->move = None;
			player->Left = (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) ? true : false;
			player->Right = (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) ? true : false;
			//player->LeftClick = (sf::Mouse::isButtonPressed(sf::Mouse::Left)) ? true : false;
		}
		for (it = entities.begin(); it != entities.end();)
		{
			Entity *b = *it;
			b->update(time);
			if (b->life == false) { entenemy.remove(*it); it = entities.erase(it); delete b; }
			else it++;
		}

		RenderScene(timeSec);

		window.pushGLStates();
		math3d::Vector3f pS= m_pGameCamera->GetPos();
		text.setString(
			"X: " + std::to_string((int)pS.x) + 
			"\nY: " + std::to_string((int)pS.y) + 
			"\nZ:" + std::to_string((int)pS.z));
		window.draw(text);
		window.popGLStates();

		window.display();
	}
	return 0;
}

int play(sf::RenderWindow & window)
{
	using namespace sf;
	using namespace std;

	initUnit();
	initTexture();
	setArray(&Ground);

	entities.clear();
	entenemy.clear();

	view.reset(sf::FloatRect(0, 0, 1600, 900));

	Level lvl;
	lvl.LoadFromFile("map1.tmx");
	vector<Object> obj;
	obj = lvl.GetAllObjects();

	sf::Image bricks, TrackImage;
	bricks.loadFromFile("images/bricks.png");
	TrackImage.loadFromFile("images/tank1_track.png");

	sf::Vector2f SpawnPlayer;
	struct SpawnEnemy
	{
		int left, top, width, height;
	};
	SpawnEnemy spawn01;

	for (int i = 0; i<obj.size(); i++)
	{
		if (obj[i].name == "solid")
		{
			int left = obj[i].rect.left / 32;
			int width = obj[i].rect.width / 32 + left;
			int top = obj[i].rect.top / 32;
			int height = obj[i].rect.height / 32 + top;

			for (int x = left; x < width; x++)
				for (int y = top; y < height; y++)
					Ground[x][y] = 1;

			entenemy.push_back(new Wall(bricks,
				obj[i].rect.left + obj[i].rect.width / 2,
				obj[i].rect.top + obj[i].rect.height / 2,
				obj[i].rect.width,
				obj[i].rect.height));
		}
		if (obj[i].name == "SpawnPlayer")
		{
			SpawnPlayer.x = obj[i].rect.left + obj[i].rect.width / 2;
			SpawnPlayer.y = obj[i].rect.top + obj[i].rect.height / 2;
		}
		if (obj[i].name == "SpawnEnemy")
		{
			spawn01.left = obj[i].rect.left;
			spawn01.top = obj[i].rect.top;
			spawn01.width = obj[i].rect.width;
			spawn01.height = obj[i].rect.height;
		}
	}

	//entities.push_back(new Player (image, image2, SpawnPlayer.x, SpawnPlayer.y, 34, 48));
	entities.push_back(new Player(Tank("tank1"), WeaponTank("weapon2"), SpawnPlayer.x, SpawnPlayer.y));
	it = entities.begin();
	entenemy.push_back(entities.back());
	Entity *p = *it;

	sf::Clock clock;
	sf::Clock checker;
	sf::Clock second;

	float spawnTank = 5;
	bool wave1 = true, wave2 = true, wave3 = true;
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		float trackTimer = checker.getElapsedTime().asMilliseconds();
		PassedTime = second.getElapsedTime().asSeconds();
		p->secondTimer = second.getElapsedTime().asSeconds();
		clock.restart();
		time = time / 800;
		if (time>60)
			time = 60;

		playerPos.x = p->x;
		playerPos.y = p->y;

		view.setCenter(p->x, p->y);

		if (PassedTime>1 && wave1) {
			spawnTank += 500;
			wave1 = false;
			int xPos, yPos;
			for (int i = 0; i<10; i++)
			{
				xPos = spawn01.left + rand() % spawn01.width;
				yPos = spawn01.top + rand() % spawn01.height;
				/*	entities.push_back(new Enemy(tank1, weapon1, xPos, yPos));
				entenemy.push_back(entities.back());*/
			}
		}
		if (PassedTime>20 && wave2) {
			spawnTank += 500;
			wave2 = false;
			int xPos, yPos;
			for (int i = 0; i<10; i++)
			{
				xPos = spawn01.left + rand() % spawn01.width;
				yPos = spawn01.top + rand() % spawn01.height;
				/*	entities.push_back(new Enemy(tank2, weapon2, xPos, yPos));
				entenemy.push_back(entities.back());*/
			}
		}
		if (PassedTime>1 && wave3) {
			spawnTank += 500;
			wave3 = false;
			int xPos, yPos;
			for (int i = 0; i<10; i++)
			{
				xPos = spawn01.left + rand() % spawn01.width;
				yPos = spawn01.top + rand() % spawn01.height;
				//entities.push_back(new Enemy(tank3, weapon3, xPos, yPos));
				entities.push_back(new Enemy(Tank("tank1"), WeaponTank("weapon3"), xPos, yPos));
				entenemy.push_back(entities.back());
			}
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				//m_pGameCamera->OnKeyboard(Event.key.code);
				SpecialKeyboardCB(event.key.code);
			}
			if (event.type == sf::Event::MouseWheelScrolled)
			{
				if ((event.mouseWheelScroll.delta) > 0) {
					m_pGameCamera->SetPos(1);
				}
				else {
					m_pGameCamera->SetPos(-1);
				}
			}
			if (event.type == sf::Event::MouseMoved)
			{
				//PassiveMouseCB(Event.mouseMove.x, Event.mouseMove.y);
			}
			else if (event.type == sf::Event::Resized)
			{
				// adjust the viewport when the window is resized
				glViewport(0, 0, event.size.width, event.size.height);
			}

			sf::Vector2i pixelPos = sf::Mouse::getPosition(window); // забираем координаты курсора
			sf::Vector2f pos = window.mapPixelToCoords(pixelPos);
			p->posMouse.x = pos.x;
			p->posMouse.y = pos.y;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) p->move = Up;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) p->move = Down;
			else p->move = None;
			p->Left = (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) ? true : false;
			p->Right = (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) ? true : false;
			p->LeftClick = (sf::Mouse::isButtonPressed(sf::Mouse::Left)) ? true : false;

			if (event.type == sf::Event::Closed)
				window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				view.reset(sf::FloatRect(0, 0, 1600, 900));
				window.setView(view); return 0;
			}
		}


		for (it = entities.begin(); it != entities.end();)
		{
			Entity *b = *it;
			if (b->name == "Enemy" || b->name == "Player")

				//window.draw(line, 2, sf::Lines);
				/*if(b->trackTimer>150){
				entities.push_front(new Track(TrackImage, b->x, b->y, 34, 28, b->sprite.getRotation()));
				}*/
				b->secondTimer = second.getElapsedTime().asSeconds();
			b->update(time);
			if (b->life == false) { entenemy.remove(*it); it = entities.erase(it); delete b; }
			else it++;
		}
		//window.setView(view);

		//window.clear(Color::Red);
		
		//RenderScene();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		window.pushGLStates();
		lvl.Draw(window);
		for (it = entities.begin(); it != entities.end(); it++) {
			window.draw((*it)->sprite);
			window.draw((*it)->sprite2);
			/*Entity *b = *it;
			sf::Vertex line[] =
			{
			sf::Vertex(sf::Vector2f(b->test1.x, b->test1.y)),
			sf::Vertex(sf::Vector2f(b->test3.x, b->test3.y)),
			};
			window.draw(line, 2, sf::Lines);
			sf::Vertex line2[] =
			{
			sf::Vertex(sf::Vector2f(b->test2.x, b->test2.y)),
			sf::Vertex(sf::Vector2f(b->test4.x, b->test4.y)),
			};
			window.draw(line2, 2, sf::Lines);*/
		}
		window.popGLStates();

		window.display();
	}
	return 0;
}
