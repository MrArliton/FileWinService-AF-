#pragma once
#include "fstream"
#include <string>
#include "vector"
class Config
{
private:
	bool automat; // �����
	bool notific; // �����������
	float timeSyn; // ���������� ������� ��� ������������� � ��������(������)
	float sizeStor; // ������������ ������ ���������� ������ � �������� (���������)
	std::string catalog; // ������� � ������� ���������� ����������� ���� ���������� ������ � �������
	int port;
	std::string ip;
	bool setCfg(std::string line);
	std::vector<std::string> Split(std::string str,char del); // ������� ���������� ������ �� ��������� �����
	const std::string AddrConfig = "C:\\ConfigsFA\\config.txt";
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

