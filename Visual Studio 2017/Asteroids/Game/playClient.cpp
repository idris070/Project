#include "playClient.h"

SkinningTechnique* m_pEffect;
Camera* m_pGameCamera;
DirectionalLight m_directionalLight;
PersProjInfo m_persProjInfo;
Player* player;
sf::Clock LogicTime;
float WaveTime;
int Score;
int Level;
bool GameOver;

void initCamera()
{
	m_pGameCamera = NULL;

	math3d::Vector3f Pos(0.0f, 0.0f, 25.0f);
	math3d::Vector3f Target(0.0f, 0.0f, -1.0f);
	math3d::Vector3f Up(0.0f, 1.0f, 0.0f); // это вращение смотреть в точку Target, с позиции при этом вращая камеру

	m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);
}
void initOpenGL()
{
	GLenum res = glewInit();
	// Параметры для Pipeline.SetPerspectiveProj
	// ПРИМЕР, в шутерах при прицеливание можно уменьшать угол обзора, чтобы приблизить объект и рассмотреть его
	m_persProjInfo.FOV = 60.0f; // Поле зрения по вертикали: угол обзора камеры, через который мы видим мир. 60.0f стандарт.
	//m_persProjInfo.Width = WINDOW_WIDTH; // Соотношение сторон - коэффициент между шириной и высотой прямоугольной области, на которую и будет осуществляться проекция.
	//m_persProjInfo.Height = WINDOW_HEIGHT; // Соотношение сторон - коэффициент между шириной и высотой прямоугольной области, на которую и будет осуществляться проекция.
	m_persProjInfo.Width = WINDOW_WIDTH; // Соотношение сторон - коэффициент между шириной и высотой прямоугольной области, на которую и будет осуществляться проекция.
	m_persProjInfo.Height = WINDOW_HEIGHT; // Соотношение сторон - коэффициент между шириной и высотой прямоугольной области, на которую и будет осуществляться проекция.
	m_persProjInfo.zNear = 1.0f; // Позиция ближней Z плоскости. Позволяет нам обрезать объекты, находящиеся слишком близко к камере.
	m_persProjInfo.zFar = 1000.0f; // Позиция дальней Z плоскости. Позволяет нам обрезать объекты, находящиеся слишком далеко от камеры.
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
void InitNewGame() 
{
	Level = 0;
	SFMLDraw.clear();
	entities.clear();
	player = new Player(*UnitPlayer, 0, 0);
	entities.push_back(player);
}
void Init()
{
	initCamera();
	initOpenGL();
	initMesh();
	initUnit();
	InitNewGame();
}

void DrawMap() {
	
}

static void RenderScene(float time)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pGameCamera->OnRender();
	m_pEffect->Enable();
	m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());

	static float lTime = 0;

	if (!GameOver) {
		lTime = LogicTime.getElapsedTime().asSeconds();
	}
	
	Pipeline p;
	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);
	p.Scale(1.f, 1.f, 1.f);

	std::vector<Matrix4f> Transforms;
	Matrix4f Identity;
	Identity.InitIdentity();

	for (it = entities.begin(); it != entities.end();)
	{
		Entity *b = *it;
		if (b->mesh1->GetNumAnimation() != 0) {
			b->mesh1->BoneTransform(b->trackTimer, Transforms);
			for (uint i = 0; i < Transforms.size(); i++) {
				m_pEffect->SetBoneTransform(i, Transforms[i]);
			}
		}else
		{
			for (size_t i = 0; i < 10; i++)	m_pEffect->SetBoneTransform(i, Identity);
		}
		p.WorldPos(b->x, b->y, 0);
		p.Rotate(b->mesh1_r.x, b->mesh1_r.y, b->mesh1_r.z);
		m_pEffect->SetWVP(p.GetWVPTrans());
		m_pEffect->SetWorldMatrix(p.GetWorldTrans());
		b->mesh1->Render(*m_pEffect);
		it++;
	}
	
	// Рендер поверхности
	Identity.InitIdentity();
	for (size_t i = 0; i < 10; i++)	m_pEffect->SetBoneTransform(i, Identity);

	p.WorldPos(0, -lTime, -5.0f);
	p.Rotate(0, 0, 0);
	p.Scale(20, 20, 0);
	m_pEffect->SetWVP(p.GetWVPTrans());
	m_pEffect->SetWorldMatrix(p.GetWorldTrans());
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

void GameLogic(float time) {
	float lTime = LogicTime.getElapsedTime().asSeconds();
	float SpeedAsteroids = 1;

	Unit *UnitCreate;
	Score = lTime * 10;
	Level = lTime / 10;
	if (Level > 30) Level = 30;
	if (lTime > WaveTime) {
		float countA;
		WaveTime += 0.5;
		
		switch (Level)
		{
			case 1: {
				SpeedAsteroids = 6;
				UnitCreate = UnitAsteroid;
				countA = random(1, 1);
				break;
			}
			case 2:	{
				SpeedAsteroids = 7;
				UnitCreate = UnitAsteroid;
				countA = random(1, 2);
				break;
			}
			case 3:	{
				SpeedAsteroids = 8;
				UnitCreate = UnitAsteroid;
				countA = random(1, 2);
				break;
			}
			case 4:	{
				SpeedAsteroids = 9;
				UnitCreate = UnitAsteroid;
				countA = random(2, 2);
				break;
			}
			case 5:	{
				SpeedAsteroids = 9;
				UnitCreate = UnitAsteroid;
				countA = random(2, 3);
				break;
			}
			case 6:	{
				SpeedAsteroids = 10;
				UnitCreate = UnitAsteroid;
				countA = random(2, 4);
				break;
			}
			case 7:	{
				SpeedAsteroids = 10;
				UnitCreate = UnitAsteroid;
				countA = random(2, 5);
				break;
			}
			case 8:	{
				SpeedAsteroids = 10;
				UnitCreate = UnitAsteroid;
				countA = random(3, 5);
				break;
			}
			case 9:	{
				SpeedAsteroids = 13;
				UnitCreate = UnitAsteroid2;
				countA = random(2, 4);
				break;
			}
			case 10:{
				SpeedAsteroids = 14;
				UnitCreate = UnitAsteroid2;
				countA = random(2, 4);
				break;
			}
			case 11:{
				SpeedAsteroids = 15;
				UnitCreate = UnitAsteroid2;
				countA = random(2, 5);
				break;
			}
			case 12:{
				SpeedAsteroids = 15;
				UnitCreate = UnitAsteroid2;
				countA = random(3, 5);
				break;
			}
			case 13:{
				SpeedAsteroids = 16;
				UnitCreate = UnitAsteroid2;
				countA = random(3, 5);
				break;
			}
			case 14:{
				SpeedAsteroids = 15;
				UnitCreate = UnitAsteroid2;
				countA = random(3, 5);
				break;
			}
			case 15:{
				SpeedAsteroids = 17;
				UnitCreate = UnitAsteroid3;
				countA = random(1, 3);
				break;
			}
			case 16:{
				SpeedAsteroids = 18;
				UnitCreate = UnitAsteroid3;
				countA = random(2, 3);
				break;
			}
			case 17:{
				SpeedAsteroids = 19;
				UnitCreate = UnitAsteroid3;
				countA = random(3, 3);
				break;
			}
			case 18:{
				SpeedAsteroids = 19;
				UnitCreate = UnitAsteroid3;
				countA = random(3, 4);
				break;
			}
			case 19:{
				SpeedAsteroids = 20;
				UnitCreate = UnitAsteroid3;
				countA = random(3, 5);
				break;
			}
			case 20:{
				SpeedAsteroids = 20;
				UnitCreate = UnitAsteroid3;
				countA = random(3, 6);
				break;
			}
			case 21:{
				SpeedAsteroids = 20;
				UnitCreate = UnitAsteroid3;
				countA = random(3, 6);
				break;
			}
			case 22:{
				SpeedAsteroids = 20;
				UnitCreate = UnitAsteroid3;
				countA = random(3, 6);
				break;
			}
			case 23:{
				SpeedAsteroids = 22;
				UnitCreate = UnitAsteroid4;
				countA = random(2, 3);
				break;
			}
			case 24:{
				SpeedAsteroids = 23;
				UnitCreate = UnitAsteroid4;
				countA = random(2, 3);
				break;
			}
			case 25:{
				SpeedAsteroids = 23.5;
				UnitCreate = UnitAsteroid4;
				countA = random(2, 4);
				break;
			}
			case 26:{
				SpeedAsteroids = 23.5;
				UnitCreate = UnitAsteroid4;
				countA = random(2, 4);
				break;
			}
			case 27:{
				SpeedAsteroids = 24;
				UnitCreate = UnitAsteroid4;
				countA = random(3, 4);
				break;
			}
			case 28:{
				SpeedAsteroids = 24;
				UnitCreate = UnitAsteroid4;
				countA = random(3, 5);
				break;
			}
			case 29: {
				SpeedAsteroids = 24.5;
				UnitCreate = UnitAsteroid4;
				countA = random(3, 6);
				break;
			}
			case 30: {
				SpeedAsteroids = 25;
				UnitCreate = UnitAsteroid4;
				countA = random(4, 6);
				break;
			}
			default:
				SpeedAsteroids = 5;
				UnitCreate = UnitAsteroid;
				countA = random(1, 1);
				break;
		}

		float xPos, yPos;
		for (int i = 0; i<countA; i++)
		{
			xPos = random(-20, 20);
			yPos = 0 + rand() % 5;
			entities.push_back(new Enemy(*UnitCreate, xPos, 20 + yPos, SpeedAsteroids));
		}
	}
}

int main2()
{

	int eventWidth, eventHeight, middle;
	sf::Music music, explosion;
	if (!music.openFromFile("Sound/through space.ogg"))
		return EXIT_FAILURE;
	explosion.openFromFile("Sound/explosion.ogg");
	music.setLoop(true);
	music.setVolume(50);

	sf::Texture tPanelDefault, tMenu, tMenuHover, tRestart, tRestartHover;
	tPanelDefault.loadFromFile("images/menu/panelDefault.png");
	tMenu.loadFromFile("images/menu/menu.png");
	tMenuHover.loadFromFile("images/menu/menuHover.png");
	tRestart.loadFromFile("images/menu/restart.png");
	tRestartHover.loadFromFile("images/menu/restartHover.png");
	sf::Sprite restart(tRestart), restartHover(tRestartHover), menu(tMenu), menuHover(tMenuHover), panel(tPanelDefault);
	restart.setOrigin(tRestart.getSize().x / 2, tRestart.getSize().y / 2);
	restartHover.setOrigin(tRestartHover.getSize().x / 2, tRestartHover.getSize().y / 2);
	menu.setOrigin(tMenu.getSize().x / 2, tMenu.getSize().y / 2);
	menuHover.setOrigin(tMenuHover.getSize().x / 2, tMenuHover.getSize().y / 2);
	panel.setOrigin(tPanelDefault.getSize().x / 2, tPanelDefault.getSize().y / 2 + 75);
	
	//restart.setScale(0.5f, 0.5f);
	//menu.setScale(0.5f, 0.5f);
	//panel.setScale(0.8f, 0.2f);

	music.play();

	sf::Font font;
	font.loadFromFile("arial.ttf");
	sf::Text text;
	text.setFont(font); // font is a sf::Font
	text.setCharacterSize(32); // in pixels, not points!
	text.setFillColor(sf::Color::White);
	
	//sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "OpenGL", sf::Style::Default, sf::ContextSettings(32)); 
	sf::RenderWindow &window = *windowMain;
	window.setView(sf::View());
	sf::View view = window.getView();

	Init();

	sf::Clock clock;
	GameOver = false;
	WaveTime = 0;
	clock.restart();
	LogicTime.restart();

	eventWidth = window.getSize().x;
	eventHeight = window.getSize().y;
	if (eventWidth*WINDOW_HEIGHT > eventHeight*WINDOW_WIDTH) {
		middle = (eventWidth - eventHeight*WINDOW_WIDTH) / 2;
		glViewport(middle, 0, eventHeight*WINDOW_WIDTH, eventHeight*WINDOW_HEIGHT);
	}
	else {
		middle = (eventHeight - eventWidth / WINDOW_WIDTH) / 2;
		glViewport(0, middle, eventWidth, eventWidth / WINDOW_WIDTH);
	}

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();

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
				window.pushGLStates();
				return 0;
				//window.close();
			}
			if (Event.type == sf::Event::Resized)
			{
				eventWidth = Event.size.width;
				eventHeight = Event.size.height;
				if (eventWidth*WINDOW_HEIGHT > eventHeight*WINDOW_WIDTH) {
					middle = (eventWidth - eventHeight*WINDOW_WIDTH) / 2;
					glViewport(middle, 0, eventHeight*WINDOW_WIDTH, eventHeight*WINDOW_HEIGHT);
				}
				else {
					middle = (eventHeight - eventWidth/WINDOW_WIDTH) / 2;
					glViewport(0, middle, eventWidth, eventWidth/WINDOW_WIDTH);
				}
			}
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) player->move = Up;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) player->move = Down;
			else player->move = None;
			player->Left = (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) ? true : false;
			player->Right = (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) ? true : false;
			//player->LeftClick = (sf::Mouse::isButtonPressed(sf::Mouse::Left)) ? true : false;
		}
		
		if (!GameOver) {
			GameLogic(time);
		}

		for (it = entities.begin(); it != entities.end();)
		{
			Entity *b = *it;
			b->update(time);
			if (b->life == false) { if (b->name == "Player") GameOver = true, explosion.play(); /*entenemy.remove(*it);*/ it = entities.erase(it); delete b; }
			else it++;
		}

		RenderScene(time);

		window.pushGLStates();
		for (SFMLit = SFMLDraw.begin(); SFMLit != SFMLDraw.end();) {
			Explosion *b = *SFMLit;
			Pipeline p;
			p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
			p.SetPerspectiveProj(m_persProjInfo);
			p.WorldPos(b->last_x, b->last_y, 0.0f);
			Matrix4f mat = p.GetWVPTrans();
			float  ScreenX, ScreenY;
			ScreenX = mat.m[0][3] / mat.m[3][3];
			ScreenY = mat.m[1][3] / mat.m[3][3];
			if (eventWidth*WINDOW_HEIGHT > eventHeight*WINDOW_WIDTH) {
				ScreenX *= (float)eventHeight * WINDOW_WIDTH / (float)window.getSize().x;
			}
			else {
				ScreenY *= (float)eventWidth / WINDOW_WIDTH / (float)window.getSize().y;
			}
			ScreenX = view.getSize().x / 2 + ScreenX * view.getSize().x / 2;
			ScreenY = view.getSize().y / 2 - ScreenY * view.getSize().y / 2;
			
			b->x = ScreenX;
			b->y = ScreenY;
			b->update(time);
			window.draw(b->sprite);
			if (b->life == false) 
			{
				SFMLit = SFMLDraw.erase(SFMLit);
				delete b; 
			}else SFMLit++;
		}
		text.setPosition(0, 0);
		text.setString("Score: " + std::to_string(Score) +
			"/30\nLevel: " + std::to_string(Level));
		window.draw(text);
		
		if (GameOver) {
			sf::View view = window.getView();
			sf::Vector2f pos = view.getSize();
			math3d::Vector3f pS= m_pGameCamera->GetPos();
			
			panel.setPosition(pos.x / 2, pos.y / 2);
			restart.setPosition(pos.x / 2 - 120, pos.y / 2 + 40);
			restartHover.setPosition(pos.x / 2 - 120, pos.y / 2 + 40);
			menu.setPosition(pos.x / 2 + 120, pos.y / 2 + 40);;
			menuHover.setPosition(pos.x / 2 + 120, pos.y / 2 + 40);;

			sf::Vector2i pixelPos = sf::Mouse::getPosition(window); // забираем координаты курсора
			sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
			window.draw(panel);
			if (restart.getGlobalBounds().contains(mousePos)) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					InitNewGame();
					GameOver = false;
					WaveTime = 0;
					clock.restart();
					LogicTime.restart();
				}
				window.draw(restartHover);
			}
			else window.draw(restart);
			if (menu.getGlobalBounds().contains(mousePos)) { 
				window.draw(menuHover); 
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) return 0; 
			}
			else window.draw(menu);

			text.setPosition(pos.x / 2 - 15, pos.y / 2 - 92);
			text.setString(std::to_string(int(Score)));
			window.draw(text);
			text.setPosition(pos.x / 2 - 15, pos.y / 2 - 172);
			text.setString(std::to_string(Level) + "/30");
			window.draw(text);
		}
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

	sf::Image bricks, TrackImage;
	bricks.loadFromFile("images/bricks.png");
	TrackImage.loadFromFile("images/tank1_track.png");

	sf::Vector2f SpawnPlayer;
	struct SpawnEnemy
	{
		int left, top, width, height;
	};

	//entities.push_back(new Player (image, image2, SpawnPlayer.x, SpawnPlayer.y, 34, 48));
	//entities.push_back(new Player(Tank("tank1"), WeaponTank("weapon2"), SpawnPlayer.x, SpawnPlayer.y));
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

		window.clear(Color::Red);
		
		//RenderScene();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		window.pushGLStates();
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
