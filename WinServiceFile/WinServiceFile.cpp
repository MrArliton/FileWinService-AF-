
#include "windows.h"
#include "fstream"
#include "Config.h"
#include "string.h"
#include "thread"
SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE serviceStatusHandle;
LPWSTR serviceName = (LPWSTR)L"FileManagerAF";
Config* configuration; // Конфигурация для работы службы
using namespace std;
int InitService() {
	try {
		configuration = new Config();
		configuration->loadConfig();
	}
	catch (exception) {
		return 0;
	}
	return 1;
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
	thread th = thread(InitService);
	error = 1;
		th.join();
		th.~thread();
	if (!error) {
		serviceStatus.dwCurrentState = SERVICE_STOPPED;
		serviceStatus.dwWin32ExitCode = -1;
		SetServiceStatus(serviceStatusHandle, &serviceStatus);
		return;
		return;
	}

	serviceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(serviceStatusHandle, &serviceStatus);


	while (serviceStatus.dwCurrentState == SERVICE_RUNNING)
	{

	}

	// stop
	serviceStatus.dwCurrentState = SERVICE_STOPPED;
	serviceStatus.dwWin32ExitCode = -1;
	SetServiceStatus(serviceStatusHandle, &serviceStatus);
	return;
}

int main()
{
	SERVICE_TABLE_ENTRY ServiceTable[1];
	ServiceTable[0].lpServiceName = serviceName;
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

	StartServiceCtrlDispatcher(ServiceTable);
}