#include "headers.h"
#include <iostream>
#include "queue"
SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE serviceStatusHandle;
LPWSTR serviceName = (LPWSTR)L"FileManagerAF";

Config* configuration; // Конфигурация для работы службы

Connector* connector; // Подключеение
FileRWFA* fileman; // Файловый менеджер
std::queue<std::vector<std::string>> qCommands; // Очередь команд , если команду нельзя выполнить перемещает в конец очереди
using namespace std;
void InitService() { // Инициализируем класс конфига, подключения , менеджера файлов
		configuration = new Config();
		configuration->loadConfig();
		fileman = new FileRWFA(configuration->getCatalog(),"FilesData.base");
}


void ControlHandler(DWORD request) {
	switch (request)
	{
	case SERVICE_CONTROL_STOP:

		serviceStatus.dwCurrentState = SERVICE_STOPPED;
		serviceStatus.dwWin32ExitCode = -1;
		SetServiceStatus(serviceStatusHandle, &serviceStatus);
		return;

	case SERVICE_CONTROL_SHUTDOWN:

		serviceStatus.dwCurrentState = SERVICE_STOPPED;
		serviceStatus.dwWin32ExitCode = -1;
		SetServiceStatus(serviceStatusHandle, &serviceStatus);
		return;

	default:
		break;
	}



	return;
}
void MainThread() { // Главный поток
	// Создаём экзампляр подключения
	connector = new Connector(configuration->getIp(), configuration->getPort(),configuration->getCatalog());
	queue<vector<string>> qCommands;
	while (true) {
		Sleep(configuration->getTimeSyn()); // Спящий режим
		if (connector->isConnect()) {
			fileman->synchronizeCatalogs(qCommands);
			if (fileman->execCommands(qCommands)) {
				if (connector->execCommands(qCommands)) { 
					connector->synchronized();// Синхронизируем файлы базы сервера с файлами
				}
				else { // Если проивзошла ошибка  при исполнении команды
 				
				}
			}
		}
		else {
			connector->connection(); // Подключение
		}
	}

}
void ServiceMain(int argc, char** argv) {

	serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	serviceStatus.dwCurrentState = SERVICE_START_PENDING;
	serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	serviceStatus.dwWin32ExitCode = 0;
	serviceStatus.dwServiceSpecificExitCode = 0;
	serviceStatus.dwCheckPoint = 0;
	serviceStatus.dwWaitHint = 0;

	serviceStatusHandle = RegisterServiceCtrlHandler(serviceName, (LPHANDLER_FUNCTION)ControlHandler);
	if (serviceStatusHandle == (SERVICE_STATUS_HANDLE)0) {
		return;
	}
	thread th = thread(InitService);
		th.join();
		th.~thread();

	serviceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(serviceStatusHandle, &serviceStatus);

	thread one = thread(MainThread);
	one.join();
	HANDLE han = one.native_handle();
	WaitForSingleObject(han, INFINITY); // Если поток завершится значит програма прервана и службу можно вырубать
	serviceStatus.dwCurrentState = SERVICE_STOPPED;
	serviceStatus.dwWin32ExitCode = -1;
	SetServiceStatus(serviceStatusHandle, &serviceStatus);
	delete(connector);
	delete(fileman);
	delete(configuration);
	return;
}

int main()
{
	/*SERVICE_TABLE_ENTRY ServiceTable[1];
	ServiceTable[0].lpServiceName = serviceName;
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

	StartServiceCtrlDispatcher(ServiceTable);*/

	//debug
	//FileRWFA file("C:\\Users\\MrArl\\Desktop\\TEST","FilesData.base");

	//cout << qCommands.size();
	//file.synchronizeCatalogs(qCommands);
	//cout << qCommands.size();
	//while (!qCommands.empty()) {
	//	cout << qCommands.front().at(0) << "\n";
	//	cout << qCommands.front().at(1) << "-\n";
	//	qCommands.pop();
	//}
	//////
	configuration = new Config();
	configuration->loadConfig();
	connector = new Connector(configuration->getIp(), 1000, configuration->getCatalog());
	cout <<connector->connection()<<"\n";
	cout << connector->connectFinish("123gds__") << "\n";
	fstream fs = fstream("C:\\Users\\MrArl\\Desktop\\TEST\\Dionnisiy123.pptx" ,ios_base::in | ios_base::out | ios_base::binary | ios_base::ate);
	cout << connector->sendFile(fs, "Dionnisiy123.pptx") << "-";
	cin.get();
}