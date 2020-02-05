#pragma once
#include "fstream"
class Config
{
private:
	bool automat; // �����
	bool notific; // �����������
	float timeSyn; // ���������� ������� ��� ������������� � ��������(������)
	float sizeStor; // ������������ ������ ���������� ������ � �������� 
	std::string catalog; // ������� � ������� ���������� ����������� ���� ���������� ������ � �������
	int port;
	std::string ip;
	const std::string AddrConfig = "C:\\ConfigsAF\\config.txt";
public:
	Config();
	int loadConfig(); // ��������� ������ �������
	//����������� �������
	bool getAutomat();
	bool getNotific();
	float getTimeSyn();
	float getSizeStor();
	std::string getIp();
	int getPort();
	std::string getCatalog();
};

