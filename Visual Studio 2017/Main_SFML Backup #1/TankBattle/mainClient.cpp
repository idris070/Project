#include "imgui.h"
#include "imgui-SFML.h"

#include "playClient.h"
extern int main2();
int main()
{
	//main2();

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
	ImGui::SFML::Init(window);
	sf::Clock deltaClock;
	sf:Color bgColor;
	float color[3] = { 0.f,0.f,0.f };
    while (window.isOpen())
    {

        Event event;
        while (window.pollEvent(event))
        {
			ImGui::SFML::ProcessEvent(event);
					if (event.type == Event::Closed)
							window.close();
					if (Keyboard::isKeyPressed(Keyboard::A)) {
						play(window);
					}
		}

		ImGui::SFML::Update(window, deltaClock.restart());
		bool f = true;
		ImGui::Begin("Sample window");
		//ImGuiIO &io = ImGui::GetIO();
		//ImGui::StyleColorsDark();
		ImGui::ShowStyleEditor();

		if (ImGui::ColorEdit3("Background color", color)) {
			// код вызывается при изменении значения, поэтому всё
			// обновляется автоматически
			bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
			bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
			bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
		}
		ImGui::ColorEdit4("Color", color);

		//ImGui::InputText("Window title", windowTitle, 255);

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (Menu == "NewGame") play(window);
			if (Menu == "Exit") window.close(); 
			// на будущее
			//view.reset(FloatRect(0, 0, 1376, 768));
			//window.setView(view);
		}

		ImGui::End();
		window.clear(bgColor);

		Menu = "NoActive";
		Vector2i pixelPos = Mouse::getPosition(window); // забираем координаты курсора
		Vector2f pos = window.mapPixelToCoords(pixelPos); 
		if (NewGame.getGlobalBounds().contains(pos))	window.draw(NewGameActive), Menu = "NewGame";
		else window.draw(NewGame);
		if (Exit.getGlobalBounds().contains(pos))	window.draw(ExitActive), Menu = "Exit";
		else window.draw(Exit);

		ImGui::SFML::Render(window);
		window.display();
    }

	ImGui::SFML::Shutdown();

	return 0;
}