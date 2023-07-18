#include <iostream>
#include <SFML/Network.hpp>
#include <string>
#include <thread>

void InitServer(unsigned short port);
void InitClient(std::string& ipstr, unsigned short port);
void SendPackets(sf::TcpSocket* socket, sf::Packet& packet);
void RecievePackets(sf::TcpSocket* socket);


std::string username;
bool isConnected;

int main(){
	char networkType;
	unsigned short port;
	std::string ipstr;

	std::cout << "Chose (s) for server or (c) for client: ";
	std::cin >> networkType;
	std::cout << "Enter a port: ";
	std::cin >> port;
	std::cout << "Please enter a username: ";
	std::cin >> username;

	//Get the port and ip
	
	if (networkType == 's')
	{
		InitServer(port);
	}
	else{
		InitClient(ipstr, port);
	}

	return 0;
}

void InitServer(unsigned short port) {
	sf::TcpListener listener;
	if(listener.listen(port) != sf::TcpSocket::Status::Done) std::cout << "Error while trying to listen to port: " << port << "\n";
	
	sf::TcpSocket socket;
	if(listener.accept(socket) != sf::TcpSocket::Status::Done) 
		std::cout << "Error while trying to accept the socket \n";
	else
		isConnected = true; 

	
	//thread
	if(isConnected){
		std::cout << "Connected";
		std::thread recieve_thread(RecievePackets, &socket);
		//send packets
		while (true) {
			std::string input;
			std::getline(std::cin, input);
			if(input.length() < 1) continue;

			sf::Packet send_packet;
			send_packet << username << input;
			SendPackets(&socket, send_packet);
		
		}
	}
}

void InitClient(std::string& ipstr, unsigned short port) {
	std::cout << "Please enter an ip to connect to: ";
	std::cin >> ipstr;

	sf::TcpSocket socket;
	sf::TcpSocket::Status status = socket.connect(ipstr, port);

	if(status == sf::TcpSocket::Status::Done) isConnected = true;
	
	//thread
	if(isConnected){
		std::cout << "Connected";
		std::thread recieve_thread(RecievePackets, &socket);
		//send packets
		while (true) {
			std::string input;
			std::getline(std::cin, input);
			if(input.length() < 1) continue;

			sf::Packet send_packet;
			send_packet << username << input;
			SendPackets(&socket, send_packet);
		
		}
	}	
}


void SendPackets(sf::TcpSocket* socket, sf::Packet& packet){
	sf::TcpSocket::Status status = socket -> send(packet);
	if (status != sf::TcpSocket::Status::Done) std::cout << "Error while trying to send packets \n";
}

void RecievePackets(sf::TcpSocket* socket){
	while (true){
		sf::Packet Rpacket;
		if (socket -> receive(Rpacket) != sf::TcpSocket::Done) 
		{	std::cout << "Error while trying to recieve packets \n";
			continue;
		}

		std::string Rusername;
		std::string Rinput;
		
		Rpacket >> Rusername >> Rinput;
		std::cout << Rusername << ": " << Rinput << "\n";
		
		std::this_thread::sleep_for((std::chrono::milliseconds) 300);
	}
}
