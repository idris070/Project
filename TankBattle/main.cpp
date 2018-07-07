#include "play.h"

int main()
{

	if(intersection(Vector2i(32,128),Vector2i(6*32,128),Vector2i(4*32,4*32)))
	{
		cout << "1" << endl;
	}
	/*begin { основная программа, вызов функции - тест }
	writeln(Intersection(1,1,5,5,1,2,3,1)); {test1, yes Intersection}
	writeln(Intersection(1,1,5,5,1,2,1,3)); {test2, no Intersection}
	end.*/


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