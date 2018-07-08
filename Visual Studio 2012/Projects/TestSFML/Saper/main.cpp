#include <SFML/Graphics.hpp>
#include <iostream>;
#include <ctime>;
#include "func.h";
#include "func.cpp";
#include <sstream>;
#include <cmath>

using namespace sf;
using namespace std;


int main () 
{
	createObCl();
	createMine();

	Font font;
	font.loadFromFile("font.ttf");

	std::ostringstream textString;
	textString << mineCount;
	text.setFont(font);
	text.setString(textString.str());
	text.setCharacterSize(24);
	text.setPosition(64, 0);
	text.setFillColor(Color::Red);

	RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SaperEsli4to");

	Texture mineX,mine0,mine1,mine2,mine3,mine4,mine5,mine6,mine7,mine8,mineM,mineMenu,mineF,MineReset;
	mineX.loadFromFile("images/MINESWEEPER_X.png");
	mine0.loadFromFile("images/MINESWEEPER_0.png");
	mine1.loadFromFile("images/MINESWEEPER_1.png");
	mine2.loadFromFile("images/MINESWEEPER_2.png");
	mine3.loadFromFile("images/MINESWEEPER_3.png");
	mine4.loadFromFile("images/MINESWEEPER_4.png");
	mine5.loadFromFile("images/MINESWEEPER_5.png");
	mine6.loadFromFile("images/MINESWEEPER_6.png");
	mine7.loadFromFile("images/MINESWEEPER_7.png");
	mine8.loadFromFile("images/MINESWEEPER_8.png");
	mineM.loadFromFile("images/MINESWEEPER_M.png");
	mineF.loadFromFile("images/MINESWEEPER_F.png");
	mineMenu.loadFromFile("images/MINESWEEPER_tray.png");
	MineReset.loadFromFile("images/MINESWEEPER_C.png");

	mineMenu.setSmooth(true);
	MineReset.setSmooth(true);

	Sprite s_mineX,s_mine0,s_mine1,s_mine2,s_mine3,s_mine4,s_mine5,s_mine6,s_mine7,s_mine8,s_mineM,s_mineF,s_mineMenu,s_mineReset;
	s_mineX.setTexture(mineX);
	s_mine0.setTexture(mine0);
	s_mine1.setTexture(mine1);
	s_mine2.setTexture(mine2);
	s_mine3.setTexture(mine3);
	s_mine4.setTexture(mine4);
	s_mine5.setTexture(mine5);
	s_mine6.setTexture(mine6);
	s_mine7.setTexture(mine7);
	s_mine8.setTexture(mine8);
	s_mineM.setTexture(mineM);
	s_mineF.setTexture(mineF);
	s_mineMenu.setTexture(mineMenu);
	s_mineReset.setTexture(MineReset);
	
	s_mineReset.setPosition(windowWidth/2-16,3);

	sf::Vector2f scale;
	sf::Vector2f scaleReset;
	float sizeX = mineMenu.getSize().x;
	float sizeY = mineMenu.getSize().y;
	scaleReset.x = 32.0f / 216.0f;
	scaleReset.y = 28.0f / 216.0f;
	cout << sizeX << endl;
	cout << sizeY << endl;

	scale.x = windowWidth / sizeX;
	scale.y = windowHeight / sizeY;
	s_mineMenu.setScale(scale.x, scale.y);
	s_mineReset.setScale(scaleReset.x, scaleReset.y);

	while(window.isOpen()){

		Event event;
		while(window.pollEvent(event)){

			if(event.type == Event::Closed)
			{
				window.close();
			}

			if(event.type == Event::MouseButtonPressed){ // если нажата клавиша мыши // а именно левая
				Vector2i pixelPos = Mouse::getPosition(window);
				Vector2f pos = window.mapPixelToCoords(pixelPos);
					if(s_mineReset.getGlobalBounds().contains(pos.x, pos.y)){
						reset_game();
							view.reset(FloatRect(0, 0, windowWidth, windowHeight));
							window.setView(view);
							Vector2u size;
							size.x = windowWidth;
							size.y = windowHeight;
							window.setSize(size);
							s_mineReset.setPosition(windowWidth/2-16,3);
								scale.x = windowWidth / sizeX;
								scale.y = windowHeight / sizeY;
								s_mineMenu.setScale(scale.x, scale.y);
								s_mineMenu.setScale(scale.x, scale.y);
					}
					
					for(int i=0; i<widthMap; i++)
					for(int j=0; j<heightMap; j++)
					{
						if(event.key.code == Mouse::Left)
							if(coords[i][j][0] < pos.x && coords[i][j][0]+16 > pos.x && coords[i][j][1] < pos.y && coords[i][j][1]+16 > pos.y)
							{
								clear0(i, j);
							}
						if(event.key.code == Mouse::Right){
							if(coords[i][j][0] < pos.x && coords[i][j][0]+16 > pos.x && coords[i][j][1] < pos.y && coords[i][j][1]+16 > pos.y)
							{
								pos.x = (pos.x) / 16;
								pos.y = (pos.y - 32) / 16;
								if(click[i][j] == 'f'){
									click[i][j] = 0;
									std::ostringstream textString;
									mineCount += 1;
									textString << mineCount;
									text.setString(textString.str());
								}else if(click[i][j] == 0){
								click[i][j] = 'f';
								std::ostringstream textString;
								mineCount -= 1;
								textString << mineCount;
								text.setString(textString.str());
								}
							}
						}}
					}
		}

	window.clear(Color(128,128,128));
	window.draw(s_mineMenu);
	window.draw(s_mineReset);
	for(int j=0; j<heightMap; j++)
	for(int i=0; i<widthMap; i++)
	{
			if(click[i][j] == false){ 
				s_mineX.setPosition(32+i*16,32+j*16);
				window.draw(s_mineX);
			}else if(click[i][j] == 'f'){
				s_mineF.setPosition(32+i*16,32+j*16);
				window.draw(s_mineF);
			}else{
				switch (object[i][j])
				{
				case 'm':s_mineM.setPosition(32+i*16,32+j*16);window.draw(s_mineM);break;
				case 1:s_mine1.setPosition(32+i*16,32+j*16);window.draw(s_mine1);break;
				case 2:s_mine2.setPosition(32+i*16,32+j*16);window.draw(s_mine2);break;
				case 3:s_mine3.setPosition(32+i*16,32+j*16);window.draw(s_mine3);break;
				case 4:s_mine4.setPosition(32+i*16,32+j*16);window.draw(s_mine4);break;
				case 5:s_mine5.setPosition(32+i*16,32+j*16);window.draw(s_mine5);break;
				case 6:s_mine6.setPosition(32+i*16,32+j*16);window.draw(s_mine6);break;
				case 7:s_mine7.setPosition(32+i*16,32+j*16);window.draw(s_mine7);break;
				case 8:s_mine8.setPosition(32+i*16,32+j*16);window.draw(s_mine8);break;
				default:s_mine0.setPosition(32+i*16,32+j*16);window.draw(s_mine0);break;
				}
			}
	}
	window.draw(text);

	window.display();
	}
}