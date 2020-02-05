#pragma once
#include "fstream"
class Config
{
private:
	bool automat; // Режим
	bool notific; // Уведомления
	float timeSyn; // Промежуток времени для синхронизации с сервером(минуты)
	float sizeStor; // Максимальный размер занимаемой памяти в каталоге 
	std::string catalog; // Каталог в который просиходит копирование либо скачивание файлов с сервера
	int port;
	std::string ip;
	const std::string AddrConfig = "C:\\ConfigsAF\\config.txt";
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

