#pragma once
#include "string"
class Connector
{
private:
	void readThread(); // ��� ������ � �������
	std::string ip;
	int port;
public:
	Connector(std::string ip,int port);
	

};

