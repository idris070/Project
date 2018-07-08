#include "playClient.h"
#include <stdio.h>
#include <dos.h>

sf::RenderWindow *windowMain;
sf::Texture PlayT, PlayHoverT, SettingT, SettingHoverT, ExitT, ExitHoverT, BackgroundT, PanelT;

int main()
{
	using namespace std;
	using namespace sf;

	int HoverMusic = 0;

	sf::View view;
	sf::Music music,hover,click;
	hover.openFromFile("Sound/buttonHover.flac");
	click.openFromFile("Sound/buttonClick.wav");
	if (!music.openFromFile("Sound/ObservingTheStar.ogg"))
		return EXIT_FAILURE;

	PlayT.loadFromFile("images/menu/play.png");
	PlayHoverT.loadFromFile("images/menu/playHover.png");
	SettingT.loadFromFile("images/menu/setting.png");
	SettingHoverT.loadFromFile("images/menu/settingHover.png");
	ExitT.loadFromFile("images/menu/exit.png");
	ExitHoverT.loadFromFile("images/menu/exitHover.png");
	BackgroundT.loadFromFile("images/menu/background.jpg");
	PanelT.loadFromFile("images/menu/panel.png");
	Sprite PlayButton(PlayT), PlayButtonHover(PlayHoverT), SettingButton(SettingT), SettingButtonHover(SettingHoverT), ExitButton(ExitT), ExitButtonHover(ExitHoverT), Background(BackgroundT), Panel(PanelT);
	
	Vector2f posPlay, posSetting, posExit;
	posPlay.x = posSetting.x = posExit.x = view.getSize().x / 2;
	posPlay.y = 350;
	posSetting.y = 400;
	posExit.y = 650;

	PlayButton.setPosition(posPlay);		PlayButtonHover.setPosition(posPlay);
	SettingButton.setPosition(posSetting);	SettingButtonHover.setPosition(posSetting);
	ExitButton.setPosition(posExit);		ExitButtonHover.setPosition(posExit);
	Panel.setPosition(view.getSize().x / 2, view.getSize().y / 2);

	PlayButton.setOrigin(PlayT.getSize().x / 2, PlayT.getSize().y / 2);
	PlayButtonHover.setOrigin(PlayHoverT.getSize().x / 2, PlayHoverT.getSize().y / 2);
	SettingButton.setOrigin(SettingT.getSize().x / 2, SettingT.getSize().y / 2);
	SettingButtonHover.setOrigin(SettingHoverT.getSize().x / 2, SettingHoverT.getSize().y / 2);
	ExitButton.setOrigin(ExitT.getSize().x / 2, ExitT.getSize().y / 2);
	ExitButtonHover.setOrigin(ExitHoverT.getSize().x / 2, ExitHoverT.getSize().y / 2);
	Panel.setOrigin(PanelT.getSize().x / 2, PanelT.getSize().y / 2);

	Background.setScale(view.getSize().x / BackgroundT.getSize().x, view.getSize().y / BackgroundT.getSize().y);
	Panel.setScale(0.5f, 0.5f);
	PlayButton.setScale(0.5f, 0.5f);
	PlayButtonHover.setScale(0.5f, 0.5f);
	SettingButton.setScale(0.5f, 0.5f);
	SettingButtonHover.setScale(0.5f, 0.5f);
	ExitButton.setScale(0.5f, 0.5f);
	ExitButtonHover.setScale(0.5f, 0.5f);


	string Menu;
	// Create the main window
	Vector2u wSize(640, 480);
	cout << "Start Init " << endl;
	windowMain = new sf::RenderWindow(sf::VideoMode(640, 480), "OpenGL", sf::Style::Default, sf::ContextSettings(24));
	sf::RenderWindow &window = *windowMain;
	cout << "Init OpenGL" << endl;
	window.setView(view);
	window.setFramerateLimit(60);

	music.setLoop(true);
	music.play();

    while (window.isOpen())
    {
		wSize = window.getSize();
		if (wSize.x > wSize.y) {
			view.setSize(1000.0f*wSize.x / wSize.y, 1000);
		}
		else if (wSize.x < wSize.y) {
			view.setSize(1000.0f, 1000.0f*wSize.y / wSize.x);
		}
		window.setView(view);

        Event event;
        while (window.pollEvent(event))
        {
					if (event.type == Event::Closed)
							window.close();
					if (Keyboard::isKeyPressed(Keyboard::A)) {
						music.stop(), main2(), music.play();
					}
					if (event.type == sf::Event::Resized)
					{
						glViewport(0, 0, event.size.width, event.size.height);
					}
		}

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (Menu == "NewGame") music.stop(), click.play(), main2(), music.play();
			if (Menu == "Exit") click.play(), sleep(sf::seconds(1)), window.close();
			// на будущее
			//view.reset(FloatRect(0, 0, 1376, 768));
			//window.setView(view);
		}

		if (Menu != "NoActive") {
			if (HoverMusic != 1) {
				hover.play();
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
		if (SettingButton.getGlobalBounds().contains(pos))	window.draw(SettingButtonHover), Menu = "Setting";
		else window.draw(SettingButton);

		window.display();
    }

	return 0;
}