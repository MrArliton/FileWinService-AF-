#pragma once
#include "fstream"
#include <string>
#include "vector"
class Config
{
private:
	bool automat; // Режим
	bool notific; // Уведомления
	float timeSyn; // Промежуток времени для синхронизации с сервером(минуты)
	float sizeStor; // Максимальный размер занимаемой памяти в каталоге (гигабайты)
	std::string catalog; // Каталог в который просиходит копирование либо скачивание файлов с сервера
	int port;
	std::string ip;
	bool setCfg(std::string line);
	std::vector<std::string> Split(std::string str,char del); // Функция разделения строки на несколько строк
	const std::string AddrConfig = "C:\\ConfigsFA\\config.txt";
public:
	Config();
	int loadConfig(); // Обновляет данные конфига
	//Стандартные геттеры
	bool getAutomat();
	bool getNotific();
	float getTimeSyn();
	float getSizeStor();
	std::string getIp();
	int getPort();
	std::string getCatalog();
};

