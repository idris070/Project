#pragma once
#ifndef CLIENTS_H
#define CLIENTS_H
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <list>
#include "../src/Entity.h"

sf::Packet& operator <<(sf::Packet& packet, Entity*& b);
class Clients;

using namespace std;
using namespace sf;

extern int x,y,p1_x, p1_y;

extern Clients client1, client2;
extern IpAddress ip;	//��������� ip ������
extern char buffer[2000];
extern size_t received;	//??
extern string text;
extern TcpListener listener;

class Clients {
public:
	int KeyRight, KeyLeft, KeyDown, KeyUp, KeyLeftClick;
	Vector2f posMouse;
	Entity *p;

	TcpSocket socket;//����������� ��������� ��� ����������� ������ ������� ����� ����������nh
	Packet packetReceive;	//��� ������������� �������� �������� ������
	Packet packetSend;	//��� ������������� �������� �������� ������
	Clients();
	
	void init();
	void update();
};
#endif CLIENTS_H