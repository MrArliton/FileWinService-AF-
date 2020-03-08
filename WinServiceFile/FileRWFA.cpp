#include "FileRWFA.h"
#include "windows.h"
//debug
#include <iostream>
using namespace std;

FileRWFA::FileRWFA(const string& catalog,const string& baseName) {
	this->catalog = catalog;
	FileRWFA::baseName = baseName;
	exc.push_back(ConvertStringToWstring(baseName));
}
// Две функции конвертации строк
std::string ConvertWStringToString(const std::wstring& wstr)
{
	std::string strTo;
	char* szTo = new char[wstr.length() + 1];
	szTo[wstr.size()] = '\0';
	WideCharToMultiByte(CP_ACP, NULL, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
	strTo = szTo;
	delete[] szTo;
	return strTo;
}
wstring FileRWFA::ConvertStringToWstring(const string& str)
{
	if (str.empty())
	{
		return wstring();
	}
	int num_chars = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, str.c_str(), str.length(), NULL, 0);
	wstring wstrTo;
	if (num_chars)
	{
		wstrTo.resize(num_chars);
		if (MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, str.c_str(), str.length(), &wstrTo[0], num_chars))
		{
			return wstrTo;
		}
	}
	return wstring();
}
base_s FileRWFA::readBase(fstream& file) {// Исполльзуется для чтения базы данных
	base_s info;
	string buff;
	char charb;
	file >> charb;
	if (charb != '.') {
		while (1) { // Считываем значение файла по номеру
			if (isdigit(charb)) {
				buff.push_back(charb);
			}
			else {
				break;
			}
			file >> charb;
		}
		info.number = atoi(buff.c_str());
		buff.clear();
		if (charb == ':') {
			file >> charb;
			if (charb == 'c') {
				info.catalog = 0;
			}
			else {
				info.catalog = 1;
			}
			file >> charb;
			if (charb == '\"') { // Считываем имя файла
				file >> charb;
				while (charb != '\"') {
					buff.push_back(charb);
					file >> charb;
				}
				info.name = buff;
				buff.clear();
			}
			file >> charb;
			if (charb != '=') {
				info.number = -1;
			}
			if (info.catalog) {
				file >> charb;
				while (charb != '.') {
					if (isdigit(charb)) // Проверяем  цифра ли  
					{
						buff.push_back(charb);
					}
					file >> charb;
				}
				info.bytes = atoi(buff.c_str()); // Преобразуем строку в число

			}
		}
		else {
			info.number = -1;
		}
	}
	else {
		info.number = -1;
		info.catalog = 3;
	}
	return info;
}
void FileRWFA::compareFiles(const wstring& path, fstream& file) { // Цикличная функция 
	vector<wstring> start = getCF(path + L"\\*"); // Тут берём основные катологи и работаем с ними
	if (start.size() > 0) {
		for (int i = 0; i < start.size(); i++) {
			bool type = typeFile(path+L"\\"+start.at(i));
			file << i << ":";
			if (!type) {
				file << "c";
			}
			else {
				file << "f";
			}
			file << "\"" << ConvertWStringToString(start.at(i)) << "\"=";
			if (!type) { // Если каталог
				compareFiles(path + L"\\" + start.at(i), file);
				file << ".";
			}
			else { //Если файл
				fstream bufferFile = openFile(path+L"\\"+start.at(i), false);
				bufferFile.seekg(0, ios_base::end);
				file << bufferFile.tellg() << ".";
				closeFile(bufferFile);
			}
		}
	}
}
int FileRWFA::typeFile(const wstring& file) { // Узнаем файл это или каталог
	wstring str = ConvertStringToWstring(catalog)+ L"\\" + file;
	WIN32_FIND_DATA data;
	FindFirstFile(str.c_str(), &data);
	if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		return 0;
	}
	else {
		return 1;
	}

}	
int FileRWFA::isEqualWStrMas(std::wstring str, std::vector<std::wstring> vector) {
	for (int i = 0;i<vector.size(); i++) {
		if (vector.at(i)._Equal(str)) {
			return i;
	}
	}
	return -1;
}
int FileRWFA::isEqualStrMas(std::string str, std::vector<std::wstring> vector) {
	for (int i = 0; i < vector.size(); i++) {
		if (vector.at(i)._Equal(ConvertStringToWstring(str))) { // Производим конвертацию строки в Юникод
			return i;
		}
	}
	return -1;
}
//
int FileRWFA::addBaseFile(const string& path) {

}
//
bool FileRWFA::execCommands(queue<vector<string>> qCommands) { // Выполняем изменения в базе данных
	while (!qCommands.empty()) {
		vector<string> str = qCommands.front();
		qCommands.pop();
		if (str.at(0) == "create") {
			if (!addBaseFile(str.at(1))) {
				return false;
			}
		}
		else if (str.at(0) == "update") {
			if (!updateBytesFile(str.at(1))) {
				return false;
			}
		}
		else if (str.at(0) == "delete") {
			if (!deleteBaseFile(str.at(1))) {
				return false;
			}
		}
		else { // Если комманда не обнаружена

		}

	}
	return true;
}
void FileRWFA::compareFC(const string& path, fstream& file, queue<vector<string>>& qCommands) { // Производим сравнения каталогов
	vector<wstring> start = getCF(path + "\\*"); // Тут берём основные катологи и работаем с ними
	base_s bs;
	vector<int> baseF;
	vector<string> arg;
	// Начинаем сравнение
	bs = readBase(file);
	while (bs.catalog != 3) {
		int id = isEqualStrMas(bs.name, start);
		if (id > -1) {
			baseF.push_back(id);
			if (!bs.catalog) { // Каталог
				compareFC(path+"\\"+bs.name, file, qCommands); // Подаём на сравнение каталога
			}
			else {
				fstream fb = openFile(path+"\\"+bs.name, false);
				fb.seekg(0, ios_base::end);
				if (bs.bytes != fb.tellg()) { // Если количество байтов не равно
					arg.push_back("update");
					arg.push_back(path+"\\"+bs.name);
					qCommands.push(arg);
					arg.clear();
				}
				closeFile(fb);
			}
		}
		else { // Если нет файла который есть в базе то значит он удалён
			arg.push_back("delete");
			arg.push_back(path+"\\"+bs.name);
			qCommands.push(arg);
			arg.clear();
		}
		bs = readBase(file);
	}
	for (int i = 0; i < start.size(); i++) {
		bool b = true;
		for (int o = 0; o < baseF.size(); o++) {
			if (baseF.at(o) == i) {
				b = false;
			}
		}
		if (b) { // Если такого ид нету то этот файл создан
				 //Подаём команду о создании новох файлов
			// тут же делаем  изменения в базе данных
			arg.push_back("create");
			arg.push_back(path+"\\"+ConvertWStringToString(start.at(i)));
			qCommands.push(arg);
			arg.clear();
		}
	}

}
void FileRWFA::synchronizeCatalogs(queue<vector<string>>& qCommands) { // qCommands - это очередь команд которые выполняет клиент общаясь с сервером
	fstream file = openFileT(baseName, false);
	vector<wstring> start = getCF("*"); // Тут берём основные катологи и работаем с ними
	if (start.size() > 0) {
		if (file) {
			file.seekg(0, ios_base::end);
			int len = file.tellg();
			if (len > 1) { // База создана производим сравнение
				// Если при сравннении база повреждена производим её пересоздание
				file.seekg(0,ios_base::beg); // Перемещаем точку чтения
				base_s bs;
				vector<int> baseF;
				vector<string> arg;
				// Начинаем сравнение
				bs = readBase(file);
				while(bs.catalog!=3){
					int id = isEqualStrMas(bs.name, start);
					if (id > -1) {
						baseF.push_back(id);
						if (!bs.catalog) { // Каталог
							compareFC(bs.name, file, qCommands); // Подаём на сравнение каталога
						}
						else {
							fstream fb = openFile(bs.name, false);
							fb.seekg(0, ios_base::end);
							if (bs.bytes != fb.tellg()) { // Если количество байтов не равно
								arg.push_back("update");
								arg.push_back(bs.name);
								qCommands.push(arg);
								arg.clear();
							}
							closeFile(fb);
						}
					}
					else { // Если нет файла который есть в базе то значит он удалён
						arg.push_back("delete");
						arg.push_back(bs.name);
						qCommands.push(arg);
						arg.clear();
					}
					bs = readBase(file);
					}
				// Тут производим обозначени о новых добавленных файлах по ид
				for (int i = 0; i < start.size();i++) {
					bool b = true;
					for (int o = 0; o < baseF.size();o++) {
						if (baseF.at(o) == i) {
							b = false;
						}
					}
					if (b) { // Если такого ид нету то этот файл создан
						//Подаём команду о создании новох файлов
						// тут же делаем  изменения в базе данных
						arg.push_back("create");
						arg.push_back(ConvertWStringToString(start.at(i)));
						qCommands.push(arg);
						arg.clear();
					}
				}

				}
			else { // База не создана или удалена ,начинаем создание новой базы
				// А также сообщаем серверу о требуемой синхронизации
				for (int i = 0; i < start.size(); i++) {
					bool type = typeFile(start.at(i));
					file << i << ":";
					if (!type) {
						file << "c";
					}
					else {
						file << "f";
					}
					file << "\"" << ConvertWStringToString(start.at(i)) << "\"=";
					if (!type) { // Если является каталогом
						compareFiles(start.at(i), file);
						file << ".";
					}
					else { // Если является файлом то открываем его и узнаем размер
						fstream bufferFile = openFile(start.at(i), false);
						bufferFile.seekg(0, ios_base::end);
						file << bufferFile.tellg() << ".";
						closeFile(bufferFile);
					}

				}
				file << ".";
			}
		}
		else {
			errorFA("Can't open file");
		}
	}
	closeFile(file);
}
int FileRWFA::isExc(wstring str) {
	for (int i = 0; i < exc.size(); i++) {
		if (exc.at(i)._Equal(str))
			return 0;
	}
	return 1;
}
vector<wstring> FileRWFA::getCF(const string& path) { // Возвращает названия катологов
	WIN32_FIND_DATA data;
	wstring buff = ConvertStringToWstring(catalog + "\\" + path);
	HANDLE handle = FindFirstFile(buff.c_str(),&data);
	vector<wstring> catalogs;
	if(isExc(data.cFileName))
	catalogs.push_back(data.cFileName);
	lastInfoCF = 1;
	while (FindNextFile(handle, &data) != 0) {
		if (isExc(data.cFileName)) {
			catalogs.push_back(data.cFileName);
			lastInfoCF++;
		}
			
	}
	return catalogs;
}

vector<wstring> FileRWFA::getCF(const wstring& path) { // Возвращает названия катологов
	WIN32_FIND_DATA data;
	wstring buff = ConvertStringToWstring(catalog + "\\") + path;
	HANDLE handle = FindFirstFile(buff.c_str(), &data);
	vector<wstring> catalogs;
	if (isExc(data.cFileName))
	catalogs.push_back(data.cFileName);
	
	lastInfoCF = 1;
	while (FindNextFile(handle, &data) != 0) {
		if (isExc(data.cFileName))
		catalogs.push_back(data.cFileName);
		lastInfoCF++;
	}
	return catalogs;

}
int FileRWFA::getInfoCF() {
	return lastInfoCF;
}
bool FileRWFA::createCatalog(const string& path) {
	wstring s = ConvertStringToWstring(catalog+"\\"+path);
	return CreateDirectory(s.c_str(), NULL);
}
bool FileRWFA::createCatalog(const wstring& path) {
	wstring s = ConvertStringToWstring(catalog + "\\") + path;
	return CreateDirectory(s.c_str(), NULL);
}
std::fstream FileRWFA::openFile(const string& path,bool clear) {
	fstream file;
	if (!clear) {
		 file = fstream(catalog + "\\" + path, ios_base::in | ios_base::out | ios_base::binary | ios_base::ate);
		 if (!file) {
			 file = fstream(catalog + "\\" + path, ios_base::out |ios_base::binary | ios_base::ate);
		 }
	}
	else {
		file = fstream(catalog + "\\" + path, ios_base::in | ios_base::out | ios_base::binary | ios_base::trunc);
	}
	return file;
}

std::fstream FileRWFA::openFileT(const string& path, bool clear) {
	fstream file;
	if (!clear) {
		file = fstream(catalog+"\\"+path, ios_base::in | ios_base::out | ios_base::ate);
		if (!file) {
			file = fstream(catalog + "\\" + path, ios_base::out | ios_base::ate);
		}
	}
	else {
		file = fstream(catalog + "\\" + path, ios_base::in | ios_base::out | ios_base::trunc);
	}
	return file;
}
std::fstream FileRWFA::openFile(const wstring& path, bool clear) {
	fstream file;
	string buff = ConvertWStringToString(path);
	if (!clear) {
		file = fstream(catalog + "\\" + buff, ios_base::in | ios_base::out | ios_base::binary | ios_base::ate);
		if (!file) {
			file = fstream(catalog + "\\" + buff, ios_base::out |ios_base::binary|ios_base::ate);
		}
	}
	else {
		file = fstream(catalog + "\\" + buff, ios_base::in | ios_base::out | ios_base::binary | ios_base::trunc);
	}
	return file;
}

std::fstream FileRWFA::openFileT(const wstring& path, bool clear) {
	fstream file;
	string buff = ConvertWStringToString(path);
	if (!clear) {
		file = fstream(catalog + "\\" + buff, ios_base::in | ios_base::out | ios_base::ate);
		if (!file) {
			file = fstream(catalog + "\\" + buff, ios_base::out | ios_base::ate);
		}
	}
	else {
		file = fstream(catalog + "\\" + buff, ios_base::in | ios_base::out | ios_base::trunc);
	}
	return file;
}
bool FileRWFA::closeFile(std::fstream& file) {
	file.close();
	if(file.is_open())
	return false;
	return true;
}

bool FileRWFA::deleteFile(const string& path) {
	string buff = catalog + "\\" + path;
	if (remove(buff.c_str()) != 0) {
		return false;
	}
	else {
		return true;
	}
}
void FileRWFA::errorFA(const char* buffer) {
	cout << buffer;
}