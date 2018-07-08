#include "playClient.h"
extern int main2();
int main()
{
	main2();

	Texture NewGameT, NewGameActiveT, ExitT, ExitActiveT;
	NewGameT.loadFromFile("images/button/NewGame.png");
	NewGameActiveT.loadFromFile("images/button/NewGameActive.png");
	ExitT.loadFromFile("images/button/Exit.png");
	ExitActiveT.loadFromFile("images/button/ExitActive.png");
	Sprite NewGame(NewGameT), NewGameActive(NewGameActiveT), Exit(ExitT), ExitActive(ExitActiveT);
	NewGame.setPosition(200,200);
	NewGameActive.setPosition(200,200);
	Exit.setPosition(200, 264);
	ExitActive.setPosition(200, 264);

	string Menu;

    RenderWindow window(VideoMode(1600, 900), "Tanks");
    while (window.isOpen())
    {

        Event event;
        while (window.pollEvent(event))
        {
					if (event.type == Event::Closed)
							window.close();
					if (Keyboard::isKeyPressed(Keyboard::A)) {
						play(window);
					}
		}

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (Menu == "NewGame") play(window);
			if (Menu == "Exit") window.close(); 
			// на будущее
			//view.reset(FloatRect(0, 0, 1376, 768));
			//window.setView(view);
		}

		window.clear();

		Menu = "NoActive";
		Vector2i pixelPos = Mouse::getPosition(window); // забираем координаты курсора
		Vector2f pos = window.mapPixelToCoords(pixelPos); 
		if (NewGame.getGlobalBounds().contains(pos))	window.draw(NewGameActive), Menu = "NewGame";
		else window.draw(NewGame);
		if (Exit.getGlobalBounds().contains(pos))	window.draw(ExitActive), Menu = "Exit";
		else window.draw(Exit);

		window.display();
    }

	return 0;
}