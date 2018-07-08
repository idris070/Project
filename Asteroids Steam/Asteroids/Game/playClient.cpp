#include "playClient.h"

SkinningTechnique* m_pEffect;
Camera* m_pGameCamera;
DirectionalLight m_directionalLight;
PersProjInfo m_persProjInfo;
Player* player;
sf::Clock LogicTime;
float WaveTime, BoxTime, FuelTime;
int Score;
int Level;
bool GameOver;
bool GameWin;

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
	glewInit();
	// Параметры для Pipeline.SetPerspectiveProj
	// ПРИМЕР, в шутерах при прицеливание можно уменьшать угол обзора, чтобы приблизить объект и рассмотреть его
	m_persProjInfo.FOV = 60.0f; // Поле зрения по вертикали: угол обзора камеры, через который мы видим мир. 60.0f стандарт.
	//m_persProjInfo.Width = WINDOW_WIDTH; // Соотношение сторон - коэффициент между шириной и высотой прямоугольной области, на которую и будет осуществляться проекция.
	//m_persProjInfo.Height = WINDOW_HEIGHT; // Соотношение сторон - коэффициент между шириной и высотой прямоугольной области, на которую и будет осуществляться проекция.
	m_persProjInfo.Width = WINDOW_WIDTH; // Соотношение сторон - коэффициент между шириной и высотой прямоугольной области, на которую и будет осуществляться проекция.
	m_persProjInfo.Height = WINDOW_HEIGHT; // Соотношение сторон - коэффициент между шириной и высотой прямоугольной области, на которую и будет осуществляться проекция.
	m_persProjInfo.zNear = 0.5f; // Позиция ближней Z плоскости. Позволяет нам обрезать объекты, находящиеся слишком близко к камере.
	m_persProjInfo.zFar = 1000.0f; // Позиция дальней Z плоскости. Позволяет нам обрезать объекты, находящиеся слишком далеко от камеры.
	// part 1 //
	m_directionalLight.Color = math3d::Vector3f(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.5f; // 0.55f standart // Яркость мира
	m_directionalLight.DiffuseIntensity = 0.7f; // 0.9f standart // Яркость направленного света
	m_directionalLight.Direction = math3d::Vector3f(1.0f, 0.5f, 0.0f);
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
	m_pEffect->SetMatSpecularIntensity(1.5f);
	m_pEffect->SetMatSpecularPower(12);
	// part 3 // OpenGL Setting
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.8f);
}
void InitNewGame() 
{
	SoundList.shipMove.setVolume(0);
	SoundList.shipMove.play();
	GameWin = false;
	GameOver = false;
	WaveTime = 0;
	BoxTime = random(0, 5);
	FuelTime = random(5, 10);
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

static void RenderScene(float time)
{
	m_pGameCamera->OnRender();
	m_pEffect->Enable();
	m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());

	static float lTime = 0;

	if (!GameOver && !GameWin) {
		lTime = LogicTime.getElapsedTime().asSeconds();
	}
	if (lTime > 320) lTime = 320;
	
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
		if (b->mesh->GetNumAnimation() != 0) {
			b->mesh->BoneTransform(b->trackTimer, Transforms);
			for (uint i = 0; i < Transforms.size(); i++) {
				m_pEffect->SetBoneTransform(i, Transforms[i]);
			}
		}else
		{
			for (size_t i = 0; i < 10; i++)	m_pEffect->SetBoneTransform(i, Identity);
		}
		p.Scale(b->meshScale);
		p.WorldPos(b->x, b->y, 0);
		p.Rotate(b->mesh1_r.x, b->mesh1_r.y, b->mesh1_r.z);
		m_pEffect->SetWVP(p.GetWVPTrans());
		m_pEffect->SetWorldMatrix(p.GetWorldTrans());
		b->mesh->Render(*m_pEffect);
		it++;
	}
	
	// Рендер поверхности
	Identity.InitIdentity();
	for (size_t i = 0; i < 10; i++)	m_pEffect->SetBoneTransform(i, Identity);
	float xScale = 30.0f * 0.57735f;
	float yScale = 30.0f * 0.57735f;
	float MoveY = 30*0.57735f;

p.WorldPos(0, -lTime, -5.0f);
p.Rotate(0, 0, 0);
//p.Scale((1.0f*0.66f)-0.1f, (1.0f*0.66f)-0.1f, 1);
p.Scale(xScale, yScale, 1);
m_pEffect->SetWVP(p.GetWVPTrans());
m_pEffect->SetWorldMatrix(p.GetWorldTrans());
BackgroundMap1.Render(*m_pEffect);


p.WorldPos(0, 6 * MoveY - lTime, -5.0f);
m_pEffect->SetWVP(p.GetWVPTrans());
m_pEffect->SetWorldMatrix(p.GetWorldTrans());
BackgroundMap2.Render(*m_pEffect);

p.WorldPos(0, 12 * MoveY - lTime, -5.0f);
m_pEffect->SetWVP(p.GetWVPTrans());
m_pEffect->SetWorldMatrix(p.GetWorldTrans());
BackgroundMap3.Render(*m_pEffect);

p.WorldPos(0, 18 * MoveY - lTime, -5.0f);
m_pEffect->SetWVP(p.GetWVPTrans());
m_pEffect->SetWorldMatrix(p.GetWorldTrans());
BackgroundMap4.Render(*m_pEffect);

m_pEffect->Disable();
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

	if (player->Fuel <= 1 && player->soundFuel) {
		SoundList.fuelOff.play();
		player->soundFuel = false;
	}

	SoundList.shipMove.setVolume(player->volShip);

	if (g_SteamAchievements) {
		if (Level >= 10)					g_SteamAchievements->SetAchievement("Act1");
		if (Level >= 20)					g_SteamAchievements->SetAchievement("Act2");
		if (Score + player->Score >= 5000) 	g_SteamAchievements->SetAchievement("Score5000");
		if (Score + player->Score >= 10000)	g_SteamAchievements->SetAchievement("Score10000");
		if (player->countBox >= 10)			g_SteamAchievements->SetAchievement("Box1");
		if (player->countBox >= 25)			g_SteamAchievements->SetAchievement("Box2");
		if (player->countBox >= 50)			g_SteamAchievements->SetAchievement("Box3");
		if (player->countFuel >= 10)		g_SteamAchievements->SetAchievement("Fuel1");
		if (player->countFuel >= 20)		g_SteamAchievements->SetAchievement("Fuel2");
		if (player->countFuel >= 30)		g_SteamAchievements->SetAchievement("Fuel3");
	}

	if (lTime > 320) {
		Level = 30;
		Score = 6000;
		GameWin = true;
		SoundList.win.play();
	}
	if (lTime > 300.0f){
		Level = 30;
	}
	if (lTime < 300.0f) {
		Unit *UnitCreate;
		Score = lTime * 20;
		Level = lTime / 10;

		if (lTime > BoxTime) {
			BoxTime = lTime + random(3,5);
			entities.push_back(new Enemy(*UnitBox, random(-19, 19), 20, 5.7));
		}
		if (lTime > FuelTime) {
			FuelTime = lTime + random(8,10);
			entities.push_back(new Enemy(*UnitFuel, random(-19, 19), 20, 4));
		}

		if (lTime > WaveTime) {
			float countA;
			//WaveTime = lTime + 0.6;
			SpeedAsteroids = 5 + lTime*0.06;
			if (Level >= 0) {
				WaveTime = lTime + 0.2;
				UnitCreate = UnitAsteroid;
			}
			if (Level >= 10) {
				WaveTime = lTime + 0.15;
				UnitCreate = UnitAsteroid2;
			}
			if (Level >= 20) {
				WaveTime = lTime + 0.10;
				UnitCreate = UnitAsteroid3;
			}
		
			/*switch (Level)
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
					countA = random(2, 4);
					break;
				}
				case 9:	{
					SpeedAsteroids = 11;
					UnitCreate = UnitAsteroid;
					countA = random(2, 5);
					break;
				}
				case 10:{
					SpeedAsteroids = 12;
					UnitCreate = UnitAsteroid2;
					countA = random(2, 4);
					break;
				}
				case 11:{
					SpeedAsteroids = 13;
					UnitCreate = UnitAsteroid2;
					countA = random(2, 5);
					break;
				}
				case 12:{
					SpeedAsteroids = 13;
					UnitCreate = UnitAsteroid2;
					countA = random(3, 5);
					break;
				}
				case 13:{
					SpeedAsteroids = 14;
					UnitCreate = UnitAsteroid2;
					countA = random(2, 5);
					break;
				}
				case 14:{
					SpeedAsteroids = 14;
					UnitCreate = UnitAsteroid2;
					countA = random(3, 5);
					break;
				}
				case 15:{
					SpeedAsteroids = 15;
					UnitCreate = UnitAsteroid2;
					countA = random(2, 4);
					break;
				}
				case 16:{
					SpeedAsteroids = 15;
					UnitCreate = UnitAsteroid2;
					countA = random(1, 6);
					break;
				}
				case 17:{
					SpeedAsteroids = 16;
					UnitCreate = UnitAsteroid2;
					countA = random(1, 5);
					break;
				}
				case 18:{
					SpeedAsteroids = 17;
					UnitCreate = UnitAsteroid2;
					countA = random(2, 5);
					break;
				}
				case 19:{
					SpeedAsteroids = 18;
					UnitCreate = UnitAsteroid2;
					countA = random(2, 5);
					break;
				}
				case 20:{
					SpeedAsteroids = 19;
					UnitCreate = UnitAsteroid3;
					countA = random(3, 6);
					break;
				}
				case 21:{
					SpeedAsteroids = 19;
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
					SpeedAsteroids = 20;
					UnitCreate = UnitAsteroid3;
					countA = random(2, 3);
					break;
				}
				case 24:{
					SpeedAsteroids = 21;
					UnitCreate = UnitAsteroid3;
					countA = random(2, 3);
					break;
				}
				case 25:{
					SpeedAsteroids = 21;
					UnitCreate = UnitAsteroid3;
					countA = random(2, 4);
					break;
				}
				case 26:{
					SpeedAsteroids = 21;
					UnitCreate = UnitAsteroid3;
					countA = random(2, 4);
					break;
				}
				case 27:{
					SpeedAsteroids = 21;
					UnitCreate = UnitAsteroid3;
					countA = random(3, 4);
					break;
				}
				case 28:{
					SpeedAsteroids = 22;
					UnitCreate = UnitAsteroid3;
					countA = random(3, 5);
					break;
				}
				case 29: {
					SpeedAsteroids = 22;
					UnitCreate = UnitAsteroid3;
					countA = random(2, 5);
					break;
				}
				case 30: {
					SpeedAsteroids = 22;
					UnitCreate = UnitAsteroid3;
					countA = random(4, 6);
					break;
				}
				default:
					SpeedAsteroids = 5;
					UnitCreate = UnitAsteroid;
					countA = random(1, 1);
					break;
			}
*/

			entities.push_back(new Enemy(*UnitCreate, random(-19, 19), 20+random(0, 15), SpeedAsteroids));
			/*for (int i = 0; i<countA; i++)
			{
				entities.push_back(new Enemy(*UnitCreate, random(-19, 19), 20+random(0, 15), SpeedAsteroids));
			}*/
		}
	}
}

int main2()
{
	float MouseMove = 0;
	int eventWidth, eventHeight, middle;

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
	
	Init();

	SoundList.musicGame.play();

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

	glEnable(GL_TEXTURE_2D);

	sf::Clock clock;
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
	glViewport(0, 0, eventWidth, eventHeight);

	while (window.isOpen())
	{
		//g_SteamAchievements->RequestStats();
		SteamAPI_RunCallbacks();
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();

		if (MouseMove < LogicTime.getElapsedTime().asSeconds()) {
			window.setMouseCursorVisible(false);
		}

		sf::Event Event;
		while (window.pollEvent(Event))
		{
			//if (Event.type == sf::Event::KeyPressed)
			//{
			//	//m_pGameCamera->OnKeyboard(Event.key.code);
			//	SpecialKeyboardCB(Event.key.code);
			//}
			if (Event.type == sf::Event::MouseWheelScrolled)
			{
				if ((Event.mouseWheelScroll.delta) > 0) {
					//m_pGameCamera->SetPos(1);
				}
				else {
				//	m_pGameCamera->SetPos(-1);
				}
			}
			if (Event.type == sf::Event::MouseMoved)
			{
				window.setMouseCursorVisible(true);
				MouseMove = LogicTime.getElapsedTime().asSeconds() + 2;
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
				glViewport(0, 0, Event.size.width, Event.size.height);
			}
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) player->move = Up;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) player->move = Down;
			else player->move = None;
			player->Left = (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ? true : false;
			player->Right = (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) ? true : false;
			//player->LeftClick = (sf::Mouse::isButtonPressed(sf::Mouse::Left)) ? true : false;
		}
		
		if (!GameOver && !GameWin) {
			GameLogic(time);
		}
		if (g_SteamAchievements) {
			if (Level >= 30)	g_SteamAchievements->SetAchievement("Act3");
		}

		for (it = entities.begin(); it != entities.end();)
		{
			Entity *b = *it;
			b->update(time);
			if (b->life == false) { 
				if (b->name == "Player") GameOver = true, SoundList.explosion.play(), SoundList.lose.play(), SoundList.shipMove.stop();
				if (b->name == "Box" && b->active) SoundList.pickBox.play();
				if (b->name == "Fuel" && b->active) SoundList.pickFuel.play();
				
				it = entities.erase(it); 
				delete b; 
			}
			else it++;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
				//ScreenX *= (float)eventHeight * WINDOW_WIDTH / (float)window.getSize().x;
			}
			else {
				//ScreenY *= (float)eventWidth / WINDOW_WIDTH / (float)window.getSize().y;
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
		text.setString("Score: " + std::to_string(Score+player->Score) +
			"\nLevel: " + std::to_string(Level) + " / 30"
			+ "\nFuel: " + std::to_string((int)player->Fuel) +" / 100"
		);
		window.draw(text);
		
		if (GameOver || GameWin) {

			sf::View view = window.getView();
			sf::Vector2f pos = view.getSize();
			math3d::Vector3f pS= m_pGameCamera->GetPos();
			
			panel.setPosition(pos.x / 2, pos.y / 2);
			restart.setPosition(pos.x / 2 - 120, pos.y / 2 + 40);
			restartHover.setPosition(pos.x / 2 - 120, pos.y / 2 + 40);
			menu.setPosition(pos.x / 2 + 120, pos.y / 2 + 40);
			menuHover.setPosition(pos.x / 2 + 120, pos.y / 2 + 40);

			sf::Vector2i pixelPos = sf::Mouse::getPosition(window); // забираем координаты курсора
			sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
			window.draw(panel);
			if (restart.getGlobalBounds().contains(mousePos)) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					SoundList.click.play();
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
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) { SoundList.click.play(); return 0; }
			}
			else window.draw(menu);
			text.setPosition(pos.x / 2 - 70, pos.y / 2 - 240);

			if (GameWin) {
				text.setString("You Win !!!");
				text.setFillColor(sf::Color::Green);
			}else {
				text.setString("You Dead...");
				text.setFillColor(sf::Color::Red);
			}
			window.draw(text);
			text.setFillColor(sf::Color::White);
			text.setPosition(pos.x / 2 - 15, pos.y / 2 - 92);
			text.setString(std::to_string(int(Score+player->Score)));
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