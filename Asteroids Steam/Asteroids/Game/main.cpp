#include "playClient.h"
#include <stdio.h>
#include <dos.h>

#include <windows.h>
#include <stdio.h>
#include <iostream>

#include <gdiplus.h>

sf::RenderWindow *windowMain;
sf::Texture PlayT, PlayHoverT, SettingT, SettingHoverT, ExitT, ExitHoverT, BackgroundT, PanelT, MusicOnT, MusicOffT, FullscreenOnT, FullscreenOffT;
CSteamAchievements*	g_SteamAchievements = NULL;

int main6()
{

	// Defining our achievements
	enum EAchievements
	{
		Act1 = 0,
		Act2 = 1,
		Act3 = 2,
	};

	// Achievement array which will hold data about the achievements and their state
	Achievement_t g_Achievements[] =
	{
		_ACH_ID(0, "1"),
		_ACH_ID(1, "2"),
		_ACH_ID(2, "3"),
		_ACH_ID(3, "3"),
		_ACH_ID(4, "3"),
		_ACH_ID(5, "3"),
		_ACH_ID(6, "3"),
		_ACH_ID(7, "3"),
		_ACH_ID(8, "3"),
		_ACH_ID(9, "3"),
		_ACH_ID(10, "3"),
		_ACH_ID(11, "3"),
	};

	// Global access to Achievements object
	//CSteamAchievements*	g_SteamAchievements = NULL;

	// Initialize Steam
	bool bRet = SteamAPI_Init();
	// Create the SteamAchievements object if Steam was successfully initialized
	if (bRet)
	{
		g_SteamAchievements = new CSteamAchievements(g_Achievements, 11);
	}

	/*SteamUserStats()->ClearAchievement("Act1");
	SteamUserStats()->ClearAchievement("Act2");
	SteamUserStats()->ClearAchievement("Act3");
	SteamUserStats()->ClearAchievement("Score5000");
	SteamUserStats()->ClearAchievement("Score10000");
	SteamUserStats()->ClearAchievement("Box1");
	SteamUserStats()->ClearAchievement("Box2");
	SteamUserStats()->ClearAchievement("Box3");
	SteamUserStats()->ClearAchievement("Fuel1");
	SteamUserStats()->ClearAchievement("Fuel2");
	SteamUserStats()->ClearAchievement("Fuel3");*/

	SteamAPI_RunCallbacks();

	using namespace std;
	using namespace sf;
	
	bool PlayMusic = true;
	bool FullScreen = true;
	int HoverMusic = 0;

	sf::View view;

	PlayT.loadFromFile("images/menu/play.png");
	PlayHoverT.loadFromFile("images/menu/playHover.png");
	SettingT.loadFromFile("images/menu/setting.png");
	SettingHoverT.loadFromFile("images/menu/settingHover.png");
	ExitT.loadFromFile("images/menu/exit.png");
	ExitHoverT.loadFromFile("images/menu/exitHover.png");
	BackgroundT.loadFromFile("images/menu/background.png");
	PanelT.loadFromFile("images/menu/panel.png");
	MusicOnT.loadFromFile("images/menu/musicOn.png");
	MusicOffT.loadFromFile("images/menu/musicOff.png");
	FullscreenOnT.loadFromFile("images/menu/fullscreenOn.png");
	FullscreenOffT.loadFromFile("images/menu/fullscreenOff.png");
	Sprite PlayButton(PlayT), PlayButtonHover(PlayHoverT), SettingButton(SettingT), SettingButtonHover(SettingHoverT), ExitButton(ExitT), ExitButtonHover(ExitHoverT), Background(BackgroundT), Panel(PanelT),
		MusicOn(MusicOnT), MusicOff(MusicOffT), FullscreenOn(FullscreenOnT), FullscreenOff(FullscreenOffT);
	
	Vector2f posPlay, posMusic, posExit, posFullscreen;
	posPlay.x = posExit.x = view.getSize().x / 2;
	posFullscreen.x = view.getSize().x / 2.3f;
	posMusic.x = view.getSize().x / 2.3f;
	posPlay.y = 350;
	posMusic.y = 400;
	posFullscreen.y = 450;
	posExit.y = 650;

	PlayButton.setPosition(posPlay);			PlayButtonHover.setPosition(posPlay);
	MusicOn.setPosition(posMusic);				MusicOff.setPosition(posMusic);
	FullscreenOn.setPosition(posFullscreen);	FullscreenOff.setPosition(posFullscreen);
	ExitButton.setPosition(posExit);			ExitButtonHover.setPosition(posExit);
	Panel.setPosition(view.getSize().x / 2,		view.getSize().y / 2);

	PlayButton.setOrigin(PlayT.getSize().x / 2, PlayT.getSize().y / 2);
	PlayButtonHover.setOrigin(PlayHoverT.getSize().x / 2, PlayHoverT.getSize().y / 2);
	ExitButton.setOrigin(ExitT.getSize().x / 2, ExitT.getSize().y / 2);
	ExitButtonHover.setOrigin(ExitHoverT.getSize().x / 2, ExitHoverT.getSize().y / 2);
	Panel.setOrigin(PanelT.getSize().x / 2, PanelT.getSize().y / 2);
	Background.setOrigin(BackgroundT.getSize().x / 2, BackgroundT.getSize().y / 2);
	Background.setPosition(view.getSize().x / 2, view.getSize().y / 2);
	Panel.setScale(0.5f, 0.5f);
	PlayButton.setScale(0.5f, 0.5f);
	PlayButtonHover.setScale(0.5f, 0.5f);
	MusicOn.setScale(0.5f, 0.5f);
	MusicOff.setScale(0.5f, 0.5f);
	FullscreenOn.setScale(0.5f, 0.5f);
	FullscreenOff.setScale(0.5f, 0.5f);
	ExitButton.setScale(0.5f, 0.5f);
	ExitButtonHover.setScale(0.5f, 0.5f);



	sf::Image icon;
	icon.loadFromFile("gameico.png");

	string Menu;
	// Create the main window
	Vector2u wSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	windowMain = new sf::RenderWindow(sf::VideoMode(wSize.x, wSize.y), "Star Advent", sf::Style::None, sf::ContextSettings(24));
	sf::RenderWindow &window = *windowMain;
	window.setView(view);
	window.setFramerateLimit(120);

	SoundList.alien.play();
	SoundList.musicMenu.play();

    while (window.isOpen())
    {
		SteamAPI_RunCallbacks();

		wSize = window.getSize();
		if (wSize.x > wSize.y) {
			view.setSize(1000.0f*wSize.x / wSize.y, 1000);
		}
		else if (wSize.x < wSize.y) {
			view.setSize(1000.0f, 1000.0f*wSize.y / wSize.x);
		}
		window.setView(view);
		Background.setScale(window.getView().getSize().x / BackgroundT.getSize().x, window.getView().getSize().y / BackgroundT.getSize().y);

        Event event;
        while (window.pollEvent(event))
        {
			if (event.type == Event::Closed)	
				window.close();
			if (event.type == sf::Event::Resized)	
				glViewport(0, 0, event.size.width, event.size.height);
			if (event.type == sf::Event::MouseMoved)	
				window.setMouseCursorVisible(true);
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (Menu == "NewGame") { SoundList.musicMenu.stop(), SoundList.alien.stop(), SoundList.select.play(), main2(), SoundList.musicGame.stop(), SoundList.musicMenu.play(), SoundList.alien.play(); }
				if (Menu == "Exit"){
					SoundList.select.play();
					sleep(sf::seconds(1));
					window.close();
				}
				if (Menu == "Music") {
					SoundList.click.play();
					if (PlayMusic) {
						SoundList.alien.setVolume(0);
						SoundList.musicMenu.setVolume(0);
						SoundList.musicGame.setVolume(0);
						PlayMusic = false;
					}
					else {
						SoundList.alien.setVolume(20);
						SoundList.musicMenu.setVolume(50);
						SoundList.musicGame.setVolume(100);
						PlayMusic = true;
					}
				}
				if (Menu == "Fullscreen") {
					SoundList.click.play();
					if (FullScreen) {
						window.create(sf::VideoMode(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2), "Star Advent", sf::Style::Default, sf::ContextSettings(24));
						window.setIcon(32, 32, icon.getPixelsPtr());
						FullScreen = false;
					}
					else {
						window.create(sf::VideoMode(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)), "Star Advent", sf::Style::None, sf::ContextSettings(24));
						FullScreen = true;
					}
				}
				// на будущее
				//view.reset(FloatRect(0, 0, 1376, 768));
				//window.setView(view);
			}
		}

		

		if (Menu != "NoActive") {
			if (HoverMusic != 1) {
				SoundList.hover.play();
				HoverMusic = 1;
			}
		}
		else {
			HoverMusic = 0;
		}

		window.clear();

		window.draw(Background);
		window.draw(Panel);
		Menu = "NoActive";
		Vector2i pixelPos = Mouse::getPosition(window); // забираем координаты курсора
		Vector2f pos = window.mapPixelToCoords(pixelPos); 
		if (PlayButton.getGlobalBounds().contains(pos))	window.draw(PlayButtonHover), Menu = "NewGame";
		else window.draw(PlayButton);
		if (ExitButton.getGlobalBounds().contains(pos))	window.draw(ExitButtonHover), Menu = "Exit";
		else window.draw(ExitButton);
		if (PlayMusic) {
			if (MusicOn.getGlobalBounds().contains(pos))	window.draw(MusicOff), Menu = "Music";
			else window.draw(MusicOn);
		}
		else {
			if (MusicOff.getGlobalBounds().contains(pos))	window.draw(MusicOn), Menu = "Music";
			else window.draw(MusicOff);
		}
		if (FullScreen) {
			if (FullscreenOn.getGlobalBounds().contains(pos))	window.draw(FullscreenOff), Menu = "Fullscreen";
			else window.draw(FullscreenOn);
		}
		else {
			if (FullscreenOff.getGlobalBounds().contains(pos))	window.draw(FullscreenOn), Menu = "Fullscreen";
			else window.draw(FullscreenOff);
		}
		window.display();
    }

	//Shutdown Steam
	SteamAPI_Shutdown();
	// Delete the SteamAchievements object
	if (g_SteamAchievements)	delete g_SteamAchievements;

	return 0;
}

using namespace std;

#pragma comment(lib, "GdiPlus.lib") /* наш многострадальный lib-файл */
using namespace Gdiplus; /* как хочешь, но мне не в кайф постоянно писать Gdiplus:: */

static const GUID png =
{ 0x557cf406, 0x1a04, 0x11d3,{ 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };
static const GUID png2 =
{ 0x557cf406, 0x1a04, 0x11d3,{ 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };


int main()
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	HDC scrdc, memdc;
	HBITMAP membit;
	// Получаем HDC рабочего стола
	// Параметр HWND для рабочего стола всегда равен нулю.
	scrdc = GetDC(0);
	// Определяем разрешение экрана
	int Height, Width;
	Height = GetSystemMetrics(SM_CYSCREEN);
	Width = GetSystemMetrics(SM_CXSCREEN);
	// Создаем новый DC, идентичный десктоповскому и битмап размером с экран.
	memdc = CreateCompatibleDC(scrdc);
	membit = CreateCompatibleBitmap(scrdc, Width, Height);
	SelectObject(memdc, membit);
	// Улыбаемся... Снято!
	BitBlt(memdc, 0, 0, Width, Height, scrdc, 0, 0, SRCCOPY);
	HBITMAP hBitmap;
	hBitmap = (HBITMAP)SelectObject(memdc, membit);
	Gdiplus::Bitmap bitmap(hBitmap, NULL);
	bitmap.Save(L"screen1.bmp", &png);

	Sleep(10000);

	BitBlt(memdc, 0, 0, Width, Height, scrdc, 0, 0, SRCCOPY);
	HBITMAP hBitmap2;
	hBitmap2 = (HBITMAP)SelectObject(memdc, membit);
	Gdiplus::Bitmap bitmap2(hBitmap2, NULL);
	bitmap2.Save(L"screen2.bmp", &png2);

	DeleteObject(hBitmap);

	sf::Image image1, image2;
	image1.loadFromFile("screen1.bmp");
	image2.loadFromFile("screen2.bmp");

	int img_Width = image1.getSize().x;
	int img_Height = image1.getSize().y;

	for (uint32 i = 0; i != img_Width; ++i) {
		for (uint32 j = 0; j != img_Height; ++j) {
			sf::Color color = image1.getPixel(i, j);
			sf::Color color2 = image2.getPixel(i, j);
			if (color != color2) {
				color.r = 0;
				color.b = 0;
				color.g = 255;
			}
			
			/*std::cout << "Color of Pixel [i] x [j] \n";
			std::cout << "Red = " << (int)color.r;
			std::cout << " Green = " << (int)color.g;
			std::cout << " Blue = " << (int)color.b << std::endl;*/
			image1.setPixel(i, j, color);
		}
	}
	image1.saveToFile("screen3.bmp");
	//
	//for (uint32 i = 0; i != img_Width; ++i) {
	//	for (uint32 j = 0; j != img_Height; ++j) {
	//		const sf::Color color = image1.getPixel(i, j);
	//		std::cout << "Color of Pixel [i] x [j] \n";
	//		std::cout << "Red = " << color.r;
	//		std::cout << " Green = " << color.g;
	//		std::cout << " Blue = " << color.b << std::endl;

	//	}
	//}

	//GdiplusShutdown(gdiplusToken);
	return 0;
}