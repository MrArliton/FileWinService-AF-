#pragma once
#include "string"
class Connector
{
private:
	void readThread(); // Для потока с чтением
	std::string ip;
	int port;
public:
	Connector(std::string ip,int port);
	

};

