#include "Clients.h"

Clients client1, client2;
IpAddress ip = IpAddress::getLocalAddress();	//Локальный ip Адресс
char buffer[2000];
size_t received;	//??
string text = "connect to: ";
TcpListener listener;

sf::Packet& operator <<(sf::Packet& packet, Entity*& b)
{
	if(b->name=="Enemy" || b->name=="Player"){
		return packet << b->name << b->x << b->y << b->name_type << b->name_type2 << b->sprite.getRotation() << b->sprite2.getRotation();
	}
	if(b->name=="Explosion"){
		return packet << b->name << b->x << b->y << b->num;
	}
	return packet << b->name << b->x << b->y << b->name_type << b->sprite.getRotation();
}

Clients::Clients(){
	KeyRight, KeyLeft, KeyDown, KeyUp = 0;
}

void Clients::init(){
	listener.accept(socket);    //который будет содержать новое соединение
	socket.send(text.c_str(), text.length() + 1);
	socket.receive(buffer, sizeof(buffer), received);
}

void Clients::update(){
	socket.receive(packetReceive);				//Команда которая ожидает данных в виде пакета от клиентской части
	if(packetReceive >> KeyRight >> KeyLeft >> KeyDown >> KeyUp >> KeyLeftClick >> posMouse.x >> posMouse.y ){			//вытаскиваем значение из пакета в переменную x и у (действие уже происходит)
		p->posMouse.x = posMouse.x;
		p->posMouse.y = posMouse.y;
		p->LeftClick = KeyLeftClick;
		p->Left = KeyLeft;
		p->Right = KeyRight;
		if(KeyUp) p->move = Up;
		else if(KeyDown) p->move = Down;
		else p->move = None;
		//cout << x << ":" << y<< endl; 	//..и если все прошло успешно то выводим её координаты	
	}
	//packetSend.clear();
	//packetSend << static_cast<sf::Uint32>(entities.size());
	//for (std::list<Entity*>::const_iterator it = entities.begin(); it != entities.end(); ++it){
	//	Entity* b = *it;
	//	packetSend << b;
	//}
	socket.send(packetSend);	//Отправка данных
	//packetSend.clear();			//Чистим пакет
}
