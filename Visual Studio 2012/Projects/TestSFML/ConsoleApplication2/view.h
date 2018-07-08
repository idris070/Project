#include <SFML\Graphics.hpp>
using namespace sf;

sf::View view;

View getPlayerCoordinateForView(float x, float y) {
	float tempX = x; float tempY = y;

	if(x<320) tempX = 320;
	if(y<240) tempY = 240;
	if(y>554) tempY = 554;

	view.setCenter(tempX, tempY);
	return view;
}

View viewmap(float time) {

	if (Keyboard::isKeyPressed(Keyboard::D)) {
		view.move(0.1*time, 0);
	}

	if (Keyboard::isKeyPressed(Keyboard::S)) {
		view.move(0, 0.1*time);
	}

	if (Keyboard::isKeyPressed(Keyboard::A)) {
		view.move(-0.1*time, 0);
	}

	if (Keyboard::isKeyPressed(Keyboard::W)) {
		view.move(0, -0.1*time);
	}

	return view;
}

View changeview() {

	if(Keyboard::isKeyPressed(Keyboard::U)){
		view.zoom(1.0100f);
	}

	if(Keyboard::isKeyPressed(Keyboard::R)){
		view.rotate(1);
	}

	if(Keyboard::isKeyPressed(Keyboard::I)){
		view.setSize(640, 480);
	}

	if(Keyboard::isKeyPressed(Keyboard::P)){
		view.setSize(540, 380);
	}

	if(Keyboard::isKeyPressed(Keyboard::Q)){
		view.setViewport(sf::FloatRect(0, 0, 0.5f, 1));
	}

	return view;
}