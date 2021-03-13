/* WinPolicy System Service
 *
 * Copyright (c) 2006 João Paulo Just Peixoto <just1982@gmail.com>.
 *
 * This file is part of WinPolicy.
 *
 * WinPolicy is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WinPolicy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WinPolicy.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <windows.h>
#include <process.h>
#include <string.h>
#include "WPServiceBase.h"
#include "AppBase.h"
#include "AutoLockBase.h"

SERVICE_STATUS          WPServiceStatus;
SERVICE_STATUS_HANDLE   WPServiceStatusHandle;

static bool closing = false;

// AutoLock thread. Try to run AutoLock every 15 seconds.
void __cdecl autolock_thread(void *data)
{
	HANDLE mutex;

	while (!closing)
	{
		CloseHandle(mutex);
		if (AppBase::CheckPrevInstance(ALOCK_MUTEX, &mutex) == false)
		{
			while (!CloseHandle(mutex))	// Caso deu erro, tente novamente
				Sleep(0);
			AppBase::RunAutoLock();
		}
		Sleep(15000);
	}
}

// Service configuration
void ConfigureService(void)
{
	SC_HANDLE schSCManager, schService;
    SC_LOCK sclLock;

	// Open SCM
	if ((schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
		return;

	// Lock database
	if ((sclLock = LockServiceDatabase(schSCManager)) == NULL)
	{
		CloseServiceHandle(schSCManager);
		return;
	}

	// Open service data... something like that :)
	if ((schService = OpenService(schSCManager, "AutoLock", SERVICE_CHANGE_CONFIG | SERVICE_START)) == NULL)
	{
		UnlockServiceDatabase(sclLock);
		CloseServiceHandle(schSCManager);
		return;
	}

	// Set the service's description
	SERVICE_DESCRIPTION svcDesc;
	svcDesc.lpDescription = "Ativa o WinPolicy AutoLock. Necess�rio para bloqueios autom�ticos por hor�rio e bloqueio cronometrado.";
	ChangeServiceConfig2(schService, SERVICE_CONFIG_DESCRIPTION, &svcDesc);

	// Configure service's error handling
	SERVICE_FAILURE_ACTIONS svcFail;
	SC_ACTION svcAction[1];

	svcAction[0].Type = SC_ACTION_RESTART;
	svcAction[0].Delay = 1000;

	svcFail.dwResetPeriod = 60;
	svcFail.cActions = 1;
	svcFail.lpsaActions = svcAction;

	ChangeServiceConfig2(schService, SERVICE_CONFIG_FAILURE_ACTIONS, &svcFail);

	// Close service handle and others
	CloseServiceHandle(schService);
	UnlockServiceDatabase(sclLock);
	CloseServiceHandle(schSCManager);
}

// Service control handler
VOID WINAPI WPServiceCtrlHandler(DWORD Opcode)
{
	switch (Opcode)
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		// Do whatever it takes to stop here.
		closing = true;
		WPServiceStatus.dwWin32ExitCode = 0;
		WPServiceStatus.dwCurrentState  = SERVICE_STOPPED;
		WPServiceStatus.dwCheckPoint    = 0;
		WPServiceStatus.dwWaitHint      = 0;
		break;

	case SERVICE_CONTROL_INTERROGATE:
		// Fall through to send current status.
		break;

	default:
		// Nothing here...
		break;
	}

	// Send current status.
	SetServiceStatus(WPServiceStatusHandle, &WPServiceStatus);

	return;
}

// ServiceMain function
VOID WINAPI WPServiceStart(DWORD argc, LPTSTR *argv)
{
//	DWORD status;
//	DWORD specificError;

	WPServiceStatus.dwServiceType        = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS;
	WPServiceStatus.dwCurrentState       = SERVICE_START_PENDING;
	WPServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	WPServiceStatus.dwWin32ExitCode      = 0;
	WPServiceStatus.dwServiceSpecificExitCode = 0;
	WPServiceStatus.dwCheckPoint         = 0;
	WPServiceStatus.dwWaitHint           = 0;

	WPServiceStatusHandle = RegisterServiceCtrlHandler("WPService", WPServiceCtrlHandler);

	if (WPServiceStatusHandle == (SERVICE_STATUS_HANDLE)0)
	{
		MessageBox(NULL, "RegisterServiceCtrlHandler() error!", "Error", MB_OK);
		return;
	}

/* This would be useful if we had something to do before anything
   else (like an initialization code). This is still here just for
   future reference.

	// Initialization code goes here.
	status = 0;
	// ... nothing to do ...

	// Handle error condition
	if (status != NO_ERROR)
	{
		WPServiceStatus.dwCurrentState       = SERVICE_STOPPED;
		WPServiceStatus.dwCheckPoint         = 0;
		WPServiceStatus.dwWaitHint           = 0;
		WPServiceStatus.dwWin32ExitCode      = status;
		WPServiceStatus.dwServiceSpecificExitCode = specificError;

		SetServiceStatus(WPServiceStatusHandle, &WPServiceStatus);
		return;
    }
*/

	// Initialization complete - report running status.
	WPServiceStatus.dwCurrentState = SERVICE_RUNNING;
	WPServiceStatus.dwCheckPoint   = 0;
	WPServiceStatus.dwWaitHint     = 0;

	if (!SetServiceStatus(WPServiceStatusHandle, &WPServiceStatus))
		return;

	// Here is where the service does its work.
	_beginthread(autolock_thread, 0, NULL);

	return;
}

// The Main function
int main(int argc, char *argv[])
{
	AppBase::SetAppDir(argv[0]);

	if (argc > 1)
	{
		// Apenas execute ConfigureService() e caia fora! :)
		if (strcmp(argv[1], "/install") == 0)
		{
			AppBase::InstallService();
			ConfigureService();
			//AppBase::ActivateService(true);
			//AppBase::RunAutoLockService();
			return 0;
		}

		// Ativa o servi�o
		if (strcmp(argv[1], "/enable") == 0)
		{
			AppBase::ActivateService(true);
			AppBase::RunAutoLockService();
			return 0;
		}

		// Desativa o servi�o
		if (strcmp(argv[1], "/disable") == 0)
		{
			AppBase::ActivateService(false);
			AppBase::StopAutoLockService();
			return 0;
		}
	}

	ConfigureService();

	if (AppBase::CheckPrevInstance(WPSERVICE_MUTEX, NULL) == true)
		return 1;

	SERVICE_TABLE_ENTRY DispatchTable[] =
	{
		{"WPService", WPServiceStart},
		{NULL, NULL}
	};

	if (!StartServiceCtrlDispatcher(DispatchTable))
	{
		MessageBox(NULL, "StartServiceCtrlDispatcher() error!", "Error", MB_OK);
		return -1;
	}

	return 0;
}
