#pragma once
#include "string"
#include "WinSock.h"
#include <vector>
#include <queue>
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
	std::string catalog;
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
	int deleteFileServer(const std::string& path); // ��������� ��������� �������� ������� 
	int updateFile(const std::string& path);
	// ������ �����������
	int codingBytes(char* buffer);
	int uncodingBytes(char* buffer);
	int copyBytes(char_m& buffer,const char* input);
	int equalBytes(const char* buffer1,const char* buffer2);
	void filesSynchronized(); // ���������� ������������� ������ � ����� �������� ��������� ����� ���������(�������� ������� � ���� ��������/�������� ������ ����� �� ��� ���������) � ���������� ������� � ������� ��� � ��������� ���������� 
	// ����������
	//���� �����������
	/* ��� ������� ������� ������� ����� ��������� ������ ���� �������*/
	int getFile(std::fstream& file, int sizefile,const std::string& path); // ��������� ���� �� ������� ��� ������ ������� �� ������� � �����
	int sendFile(std::fstream& file,const std::string& path); // ���������� ���� ��� ����� �������� 
	//
public:
	Connector(const std::string& ip,int port,std::string& catalog);
	bool execCommands(std::queue<std::vector<std::string>> qCommand); // ��������� �������
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

