/*
void playonline()
{

	setlocale(LC_ALL, "");							// ��������� ��������� � ������� Windows
	IpAddress ip = IpAddress::getLocalAddress();	//��������� ip ������
	TcpSocket socket;//����������� ��������� ��� ����������� ������ ������� ����� ����������
	Packet packet;	//��� ������������� �������� �������� ������
	char type;
	char mode = ' ';//��� s- ������, � - ������
	int x = 0;      //���������� ����
	int y = 0;
	char buffer[2000];
	size_t received;	//??
	string text = "connect to: ";

	//***********�����������***************
	cout << ip << endl;
		cout << "������� ��� �����������:  c -������, s -������" << endl;
		cin >> type;
		if(type == 's'){
			TcpListener listener;
			listener.listen(2000);
			listener.accept(socket);    //������� ����� ��������� ����� ����������
			text += "Serwer";
			mode = 's';
 
		} else if(type == 'c'){
			cout << "new ip:";
			cin >> ip;
 
			socket.connect(ip, 2000); //ip � ����
			text += "client";
			mode = 'r';
		}
		cout << "helo" << endl;
		socket.send(text.c_str(), text.length() + 1);
		socket.receive(buffer, sizeof(buffer), received);
		cout << buffer <<endl;



    RenderWindow window(VideoMode(640, 480), "Tanks");
	CircleShape shape(10);     //������� ��� � �������� 50
	Image image, image2;
	image.loadFromFile("images/tank1b_body.png");
	image2.loadFromFile("images/tank1b_dualgun.png");
	Player p(image, image2, 400, 300, 34, 48);

	Clock clock;
	Clock clockGame;

    while (window.isOpen())
    {
		float time = clock.getElapsedTime().asMicroseconds();
		float timeGame = clockGame.getElapsedTime().asSeconds();
		clock.restart();
		time = time / 1900;

		Vector2i pixelPos = Mouse::getPosition(window); // �������� ���������� �������
		Vector2f pos = window.mapPixelToCoords(pixelPos); 
		p.rotation(time, pos);

			if(timeGame>2){
				clockGame.restart();
			}

        Event event;
        while (window.pollEvent(event))
        {
					if (event.type == Event::Closed)
							window.close();

							if(mode == 'r'){
					if (Keyboard::isKeyPressed(Keyboard::Right)) { //������ ���������� � ������������ =>��� �����
						x ++;
					}
					if (Keyboard::isKeyPressed(Keyboard::Left)) { //������ ���������� � ������������ =>��� �����
						x --;
					}
 
					if (Keyboard::isKeyPressed(Keyboard::Down)) { //������ ���������� � ������������ =>��� �����
						y ++;
					}
					if (Keyboard::isKeyPressed(Keyboard::Up)) { //������ ���������� � ������������ =>��� �����
						y --;
					}
 
					packet << x << y;		//������ �������� ��������� � �����
					socket.send(packet);	//�������� ������
					packet.clear();			//������ �����
				}


		}

			if(mode == 's'){
		socket.receive(packet);				//������� ������� ������� ������ � ���� ������ �� ���������� �����
		if(packet >> x >> y){				//����������� �������� �� ������ � ���������� x � � (�������� ��� ����������)
			cout << x << ":" << y<< endl; 	//..� ���� ��� ������ ������� �� ������� � ����������
		}
	}

        window.clear();

		shape.setPosition(x, y);	//���������� ��� �� ����������
		window.draw(shape);			//������������ ���


		window.draw(p.sprite);
		window.draw(p.sprite2);
        window.display();

		sleep(sf::milliseconds(10));//��������
    }
	system("pause");
}
*/