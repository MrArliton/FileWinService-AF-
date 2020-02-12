#pragma once
#include "string"
#include "winsock.h"
class Connector
{
private:
	void readThread(); // ��� ������ � ������� 
	bool readingTh = false; // ����� �� ������ ������ � �������
	sockaddr_in addr;
	WSADATA ws;
	SOCKET sock; // �����
	bool init = false;
	int sendData(char* chars);
	int recvData(char* buffer);
	bool sendFile(std::fstream& file); // ���������� ���� ��� ����� �������� 
	// ������ �����������
	bool connectFinish(std::string pass); // ����� ��������
	bool connectsIs(); // �������� ���������� ��  
	void filesSynchronized(); // ���������� ������������� ������ � ����� �������� ��������� ����� ���������(�������� ������� � ���� ��������/�������� ������ ����� �� ��� ���������) � ���������� ������� � ������� ��� � ��������� ���������� 
	// ����������
	//���� �����������
	/* ��� ������� ������� ������� ����� ��������� ������ ���� �������*/
	bool getFile(std::fstream& file, int sizefile); // ��������� ���� �� ������� ��� ������ ������� �� ������� � �����
	//
public:
	Connector(const std::string& ip,int port);
	int synchronized(); // �������������� � �������� � ���������� ����� 
	/*
	��� ������� ��������� �������
	*/
	int connection();
	bool isConnect();
	int disconnect();
	//
};

