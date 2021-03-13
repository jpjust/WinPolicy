/* NMService class
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
#include "NMServiceBase.h"
#include "AppBase.h"
#include "NetModuleBase.h"

SERVICE_STATUS          NMServiceStatus;
SERVICE_STATUS_HANDLE   NMServiceStatusHandle;

static bool closing = false;

// Thread do NetModule. Tenta executar o NetModule a cada 15 segundos.
void __cdecl netmodule_thread(void *data)
{
	HANDLE mutex;

	while (1)
	{
		CloseHandle(mutex);

		if ((!closing) && (AppBase::CheckPrevInstance(NETMODULE_MUTEX, &mutex) == false))
		{
			while (!CloseHandle(mutex))
				Sleep(0);
			RunNetModule();
		}

		Sleep(15000);
	}
}

// Configura��o do servi�o
void ConfigureService(void)
{
	SC_HANDLE schSCManager, schService;
    SC_LOCK sclLock;

	// Abre o SCM
	if ((schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
		return;

	// Trava a base de dados
	if ((sclLock = LockServiceDatabase(schSCManager)) == NULL)
	{
		CloseServiceHandle(schSCManager);
		return;
	}

	// Abre os dados do servi�o... algo desse tipo :)
	if ((schService = OpenService(schSCManager, "NetModule", SERVICE_CHANGE_CONFIG | SERVICE_START)) == NULL)
	{
		UnlockServiceDatabase(sclLock);
		CloseServiceHandle(schSCManager);
		return;
	}

	// Ajusta a descri��o do servi�o
	SERVICE_DESCRIPTION svcDesc;
	svcDesc.lpDescription = "Ativa o WinPolicy NetModule. O agente para conex�es com o WinPolicy Controller.";
	ChangeServiceConfig2(schService, SERVICE_CONFIG_DESCRIPTION, &svcDesc);

	// Configura o "error handler" do servi�o
	SERVICE_FAILURE_ACTIONS svcFail;
	SC_ACTION svcAction[1];

	svcAction[0].Type = SC_ACTION_RESTART;
	svcAction[0].Delay = 1000;

	svcFail.dwResetPeriod = 60;
	svcFail.cActions = 1;
	svcFail.lpsaActions = svcAction;

	ChangeServiceConfig2(schService, SERVICE_CONFIG_FAILURE_ACTIONS, &svcFail);

	// Fecha o servi�o
	CloseServiceHandle(schService);
	UnlockServiceDatabase(sclLock);
	CloseServiceHandle(schSCManager);
}

// Gerente de controle do servi�o
VOID WINAPI NMServiceCtrlHandler(DWORD Opcode)
{
	switch (Opcode)
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		// Fa�a o que for necess�rio para parar aqui
		closing = true;
		NMServiceStatus.dwWin32ExitCode = 0;
		NMServiceStatus.dwCurrentState  = SERVICE_STOPPED;
		NMServiceStatus.dwCheckPoint    = 0;
		NMServiceStatus.dwWaitHint      = 0;
		break;

	case SERVICE_CONTROL_INTERROGATE:
		// Envia o status atual???
		break;

	default:
		// Nada aqui...
		break;
	}

	// Envia o status atual
	SetServiceStatus(NMServiceStatusHandle, &NMServiceStatus);

	return;
}

// ServiceMain
VOID WINAPI NMServiceStart(DWORD argc, LPTSTR *argv)
{
//	DWORD status;
//	DWORD specificError;

	NMServiceStatus.dwServiceType        = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS;
	NMServiceStatus.dwCurrentState       = SERVICE_START_PENDING;
	NMServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	NMServiceStatus.dwWin32ExitCode      = 0;
	NMServiceStatus.dwServiceSpecificExitCode = 0;
	NMServiceStatus.dwCheckPoint         = 0;
	NMServiceStatus.dwWaitHint           = 0;

	NMServiceStatusHandle = RegisterServiceCtrlHandler("NMService", NMServiceCtrlHandler);

	if (NMServiceStatusHandle == (SERVICE_STATUS_HANDLE)0)
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
		NMServiceStatus.dwCurrentState       = SERVICE_STOPPED;
		NMServiceStatus.dwCheckPoint         = 0;
		NMServiceStatus.dwWaitHint           = 0;
		NMServiceStatus.dwWin32ExitCode      = status;
		NMServiceStatus.dwServiceSpecificExitCode = specificError;

		SetServiceStatus(NMServiceStatusHandle, &NMServiceStatus);
		return;
    }
*/

	// Initialization complete - report running status.
	NMServiceStatus.dwCurrentState = SERVICE_RUNNING;
	NMServiceStatus.dwCheckPoint   = 0;
	NMServiceStatus.dwWaitHint     = 0;

	if (!SetServiceStatus(NMServiceStatusHandle, &NMServiceStatus))
		return;

	// Aqui � onde come�a o trabalho do servi�o
	_beginthread(netmodule_thread, 0, NULL);

	return;
}

// A fun��o "main"
int main(int argc, char *argv[])
{
	AppBase::SetAppDir(argv[0]);

	if (argc > 1)
	{
		// Apenas execute ConfigureService() e caia fora! :)
		if (strcmp(argv[1], "/install") == 0)
		{
			InstallService();
			ConfigureService();
			ActivateService(true);
			RunService();
			return 0;
		}

		// Ativa o servi�o
		if (strcmp(argv[1], "/enable") == 0)
		{
			ActivateService(true);
			RunService();
			return 0;
		}

		// Desativa o servi�o
		if (strcmp(argv[1], "/disable") == 0)
		{
			ActivateService(false);
			StopService();
			return 0;
		}
	}

	ConfigureService();

	if (AppBase::CheckPrevInstance(NMSERVICE_MUTEX, NULL) == true)
		return 1;

	SERVICE_TABLE_ENTRY DispatchTable[] =
	{
		{"NMService", NMServiceStart},
		{NULL, NULL}
	};

	if (!StartServiceCtrlDispatcher(DispatchTable))
	{
		MessageBox(NULL, "StartServiceCtrlDispatcher() error!", "Error", MB_OK);
		return -1;
	}

	return 0;
}
