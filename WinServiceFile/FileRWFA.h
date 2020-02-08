#pragma once
#include <string>
#include <fstream>
class FileRWFA
{
private:
	
public:
	FileRWFA(std::string catalog); // В конструкторе происходит инициализация католога
	std::string getCF(std::string path); //catalog + "\\" + path  Формат
	bool createCatalog(std::string path); //    ^ имя католога указывается в пути
	std::fstream openFile(std::string path); // ^ Открывает файл всегда в бинарном виде
	bool closeFile(std::fstream file); // Закрывает файл 
	bool deleteFile(std::string path); // Удаление файла

	
};

