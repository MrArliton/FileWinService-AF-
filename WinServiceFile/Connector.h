#pragma once
#include "string"
#include "WinSock.h"
#pragma comment (lib,"Ws2_32.lib")
class Connector
{
typedef	struct charM {
		char* chars;
		int length;
	}char_m;
private:
	//Const
	const size_t buffer_pass = 32;
	//
	void readThread(); // ��� ������ � ������� 
	bool readingTh = false; // ����� �� ������ ������ � �������
	bool connected = false;
	sockaddr_in addr;
	WSADATA ws;
	SOCKET sock; // �����
	bool init = false;
	int sendData(char_m& chars);
	int recvData(char_m& buffer);
	// ������ �����������
	int codingBytes(char* buffer);
	int uncodingBytes(char* buffer);
	int copyBytes(char_m& buffer,const char* input);
	int equalBytes(const char* buffer1,const char* buffer2);
	void filesSynchronized(); // ���������� ������������� ������ � ����� �������� ��������� ����� ���������(�������� ������� � ���� ��������/�������� ������ ����� �� ��� ���������) � ���������� ������� � ������� ��� � ��������� ���������� 
	// ����������
	//���� �����������
	/* ��� ������� ������� ������� ����� ��������� ������ ���� �������*/
	int getFile(std::fstream& file, int sizefile); // ��������� ���� �� ������� ��� ������ ������� �� ������� � �����
	int sendFile(std::fstream& file); // ���������� ���� ��� ����� �������� 
	//
public:
	Connector(const std::string& ip,int port);
	int synchronized(); // �������������� � �������� � ���������� ����� 
	/*
	��� ������� ��������� �������
	*/
	void errorC(int error);
	int connection();
	int connectFinish(const std::string& pass); // ����� ��������
	bool isConnect();
	int disconnect();
	//
};

