#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <list>
#include <string>
#include <math.h>
#include "src/Unit.h"
#include "src/ReceiveEntity.h"
#include "src/level.h"
#include "src/LoadResources.h"
#include "src/Packet.h"

using namespace std;
using namespace sf;

IpAddress ip = IpAddress::getLocalAddress();	//��������� ip ������
TcpSocket socket;//����������� ��������� ��� ����������� ������ ������� ����� ����������
Packet packet;	//��� ������������� �������� �������� ������
Packet packet2;	//��� ������������� �������� �������� ������
char buffer[2000];
size_t received;	//??
string text = "connect to: ";
TcpListener listener;

int main2() {

	Level lvl;
	lvl.LoadFromFile("map1.tmx");
	initUnit();
	initTexture();

	setlocale(LC_ALL, "");							// ��������� ��������� � ������� Windows
	//***********�����������***************//
	cout << ip << endl;
	cout << "new ip:";
	cin >> ip;

	socket.connect(ip, 2000); //ip � ����
	text += "client";
	socket.send(text.c_str(), text.length() + 1);
	socket.receive(buffer, sizeof(buffer), received);

	//**********��������� �����***********************//
	RenderWindow window(sf::VideoMode(1600, 900), "Network");//������� ������ �������� ����
	//*******��������********************//
	while(window.isOpen()){
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed){
				window.close();
			}
		}

		if(1){
			int keyRight = 0, keyLeft = 0, keyDown = 0, keyUp = 0;
			int KeyLeftClick;
			Vector2f posMouse;
			if (Keyboard::isKeyPressed(Keyboard::Right)) { //������ ���������� � ������������ =>��� �����
				keyRight = 1;
			}
			if (Keyboard::isKeyPressed(Keyboard::Left)) { //������ ���������� � ������������ =>��� �����
				keyLeft = 1;
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) { //������ ���������� � ������������ =>��� �����
				keyDown = 1;
			}
			if (Keyboard::isKeyPressed(Keyboard::Up)) { //������ ���������� � ������������ =>��� �����
				keyUp = 1;
			}
			KeyLeftClick = (Mouse::isButtonPressed(Mouse::Left)) ? 1 : 0;
			Vector2i pixelPos = Mouse::getPosition(window); // �������� ���������� �������
			Vector2f pos = window.mapPixelToCoords(pixelPos); 
			posMouse.x = pos.x;
			posMouse.y = pos.y;
			packet << keyRight << keyLeft << keyDown << keyUp << KeyLeftClick << posMouse.x << posMouse.y;		//������ �������� ��������� � �����
			socket.send(packet);	//�������� ������
			packet.clear();			//������ �����
		}
		socket.receive(packet2);
		entities3.clear();
		PacketProcessing(packet2);
		packet2.clear();

		window.clear();
		lvl.Draw(window);
		for (it3 = entities3.begin(); it3 != entities3.end(); it3++){
			ReceiveEntity *b = *it3;
			window.draw(b->sprite);
			window.draw(b->sprite2);
		}
		window.display();
	}
	return 0;
}
