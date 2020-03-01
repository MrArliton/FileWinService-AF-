#pragma once
#include <string>
#include <fstream>
#include "vector"
#include "queue"
class FileRWFA
{
private:
	bool connected;
	std::string catalog;
	std::string baseName;
	std::wstring ConvertStringToWstring(const std::string& str);
	int lastInfoCF;
	void compareFiles(const std::wstring& path,std::fstream& file); // Тут указываем путь до каталога 
	void errorFA(const char* error);
	int typeFile(const std::wstring& name);
	int isExc(std::wstring str);
	std::vector<std::wstring> exc = { L".",L".." }; // Исключения для getCF
public:
	FileRWFA(const std::string& catalog,const std::string& baseName); // В конструкторе происходит инициализация католога
	std::vector<std::wstring> getCF(const std::string& path); //catalog + "\\" + path  Формат
	std::vector<std::wstring> getCF(const std::wstring& path); //
	int getInfoCF();
	void synchronizeCatalogs(std::queue<std::vector<std::string>>& qCommands); // Синхронизирует каталоги с базой данных 
	bool createCatalog(const std::string& path); //    ^ имя католога указывается в пути
	bool createCatalog(const std::wstring& path); //   
	std::fstream openFile(const std::string& path, bool clear); // ^ Открывает файл всегда в бинарном виде
	std::fstream openFileT(const std::string& path,bool clear); // ^ Открывает файл всегда в тестовом виде
	std::fstream openFile(const std::wstring& path, bool clear); 
	std::fstream openFileT(const std::wstring& path, bool clear); 
	bool closeFile(std::fstream& file); // Закрывает файл 
	bool deleteFile(const std::string& path); // Удаление файла
	
};

