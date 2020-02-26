#include "headers.h"
#include <iostream>
SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE serviceStatusHandle;
LPWSTR serviceName = (LPWSTR)L"FileManagerAF";

Config* configuration; // Конфигурация для работы службы

Connector* connector; // Подключеение
FileRWFA* fileman; // Файловый менеджер
using namespace std;
void InitService() { // Инициализируем класс конфига, подключения , менеджера файлов
		configuration = new Config();
		configuration->loadConfig();
		fileman = new FileRWFA(configuration->getCatalog());
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
	connector = new Connector(configuration->getIp(), configuration->getPort());
	while (true) {
		Sleep(configuration->getTimeSyn()); // Спящий режим
		if (connector->isConnect()) {
			connector->synchronized();// Синхронизируем файлы
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
	Connector connect("192.168.1.101",1000);
	cout << connect.connection() << "\n";
	cout << connect.connectFinish("123gds__");
	cin.get();
}