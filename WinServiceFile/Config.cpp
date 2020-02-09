#include "Config.h"
using namespace std;

Config::Config() { // Конструктор нам не потребуется
}
	vector<string> Config::Split(string line, char del) { // Делим строку 
		int i = line.find(del, 0);
		int next = 0;
		int prev = 0;

		vector<string> otvet;
		string buffer;
		while ((next = line.find('-', prev)) != string::npos) {
			buffer = line.substr(prev, next - prev);
			otvet.push_back(buffer);
			prev = next + 1;

		}
		buffer = line.substr(prev, next - prev);
		otvet.push_back(buffer);
		return otvet;
	}
	bool Config::setCfg(string line) { // Записываем конфиг
		vector<string> elem = Split(line, '-');
			if ((elem.size() < 3) & (elem.size() > 1)) { // В 1 строке всегда записан только 1 настройка конфига
				if (elem.at(0)._Equal("automat")) {
					if (elem.at(1)._Equal("1")) {
						automat = true;
					}
					else {
						automat = false;
					}
				}
				else if (elem.at(0)._Equal("notific")) {
					if (elem.at(1)._Equal("1")) {
						notific = true;
					}
					else {
						notific = false;
					}
				}
				else if (elem.at(0)._Equal("timeSyn")) {
					timeSyn = atof(elem.at(1).c_str());
				}
				else if (elem.at(0)._Equal("sizeStor")) {
					sizeStor = atof(elem.at(1).c_str());
				}
				else if (elem.at(0)._Equal("catalog")) {
					catalog = elem.at(1);
				}
				else if (elem.at(0)._Equal("ip")) {
					ip = elem.at(1);
				}
				else if (elem.at(0)._Equal("port")) {
					timeSyn = atoi(elem.at(1).c_str());
				}
			}
		return true;
	}
	int Config::loadConfig() { // Прогружаем конфиг
		string line;
		ifstream file(AddrConfig);
		if (!file.is_open()) { return -1; }
		// Начинаем чтение конфига
		while (getline(file, line)) {
			if (!setCfg(line)) {
				return 0;
			}
		}
		return 1;
	}
	// Геттеры
	bool Config::getAutomat() {
		return automat;
	}
	bool Config::getNotific() {
		return notific;
	}
	float Config::getTimeSyn() {
		return timeSyn;
	}
	float Config::getSizeStor() {
		return sizeStor;
	}
	std::string Config::getIp() {
		return ip;
	}
	int Config::getPort() {
		return port;
	}
	std::string Config::getCatalog() {
		return catalog;
	}
