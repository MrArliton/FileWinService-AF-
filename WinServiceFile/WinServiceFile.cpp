
#include "windows.h"
#include "fstream"
#include "Config.h"
#include "string.h"

SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE serviceStatusHandle;
LPWSTR serviceName = (LPWSTR)L"FileManagerAF";
Config* configuration; // Конфигурация для работы службы
using namespace std;
int InitService() {
	configuration = new Config();
	//string line;
	//ifstream file("C:\\ConfigsAF\\config.txt");
	//if (!file.is_open()) { return 0; }

	return 1;
}
void ShutDownService() {
	serviceStatus.dwCurrentState = SERVICE_STOPPED;
	serviceStatus.dwWin32ExitCode = -1;
	SetServiceStatus(serviceStatusHandle, &serviceStatus);
	return;
}
void ControlHandler(DWORD request) {
	switch (request)
	{
	case SERVICE_CONTROL_STOP:

		ShutDownService();
		return;

	case SERVICE_CONTROL_SHUTDOWN:

		ShutDownService();
		return;

	default:
		break;
	}

	SetServiceStatus(serviceStatusHandle, &serviceStatus);

	return;
}
void ServiceMain(int argc, char** argv) {
	int error;

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

	error = InitService();
	if (!error) {
		ShutDownService();
		return;
	}

	serviceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(serviceStatusHandle, &serviceStatus);

	while (serviceStatus.dwCurrentState == SERVICE_RUNNING)
	{


	}

	return;
}

int main()
{
	SERVICE_TABLE_ENTRY ServiceTable[1];
	ServiceTable[0].lpServiceName = serviceName;
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

	StartServiceCtrlDispatcher(ServiceTable);
}