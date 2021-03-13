/* NMService base class
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

#include "NMServiceBase.h"
#include "AppBase.h"
#include "jswinreg.h"
#include <windows.h>
#include <shellapi.h>
#include <wx/wx.h>

// Instala o servi�o
bool InstallService(void)
{
	wxString str_path = AppBase::GetAppDir() + "\\NMService.exe";
	SC_HANDLE schSCManager;

	if ((schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
		return false;

	if (CreateService(
		schSCManager,                  // SCManager database
		"NetModule",                   // name of service
		"WinPolicy NetModule",         // service name to display
		SERVICE_ALL_ACCESS,            // desired access
		SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, // service type
		SERVICE_DEMAND_START,          // start type
		SERVICE_ERROR_NORMAL,          // error control type
		(char *)str_path.mb_str(), // path to service's binary
		NULL,                          // no load ordering group
		NULL,                          // no tag identifier
		NULL,                          // no dependencies
		NULL,                          // LocalSystem account
		NULL) == NULL)                 // no password
		{
			CloseServiceHandle(schSCManager);
			return false;
		}

	CloseServiceHandle(schSCManager);

	return true;
}

// Ativa o servi�o do NetModule
bool ActivateService(bool activate)
{
	SC_HANDLE schSCManager, schService;
    SC_LOCK sclLock;

	// Open SCM
	if ((schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
	{
		if (activate)
			jsreg_savevalue(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", "NetModule", (char *)wxString(AppBase::GetAppDir() + "\\NetModule.exe").mb_str());
		else
			jsreg_delvalue(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", "NetModule");
		return false;
	}

	// Lock database
	if ((sclLock = LockServiceDatabase(schSCManager)) == NULL)
	{
		CloseServiceHandle(schSCManager);
		return false;
	}

	// Open service data... something like that :)
	if ((schService = OpenService(schSCManager, "NetModule", SERVICE_CHANGE_CONFIG | SERVICE_START)) == NULL)
	{
		UnlockServiceDatabase(sclLock);
		CloseServiceHandle(schSCManager);
		return false;
	}

	wxString str_path = AppBase::GetAppDir() + "\\NMService.exe";

	if (activate)
	{
		// Configure AutoLock service to start automatically and don't forget to
		// make sure it is interactive
		if (ChangeServiceConfig(schService, SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
			SERVICE_AUTO_START,	SERVICE_NO_CHANGE, (char *)str_path.mb_str(), NULL, NULL, NULL,
			NULL, NULL, NULL) == 0)
			{
				CloseServiceHandle(schService);
				UnlockServiceDatabase(sclLock);
				CloseServiceHandle(schSCManager);
				return false;
			}
	}
	else
	{
		// Configure AutoLock service to not start automatically
		if (ChangeServiceConfig(schService, SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
			SERVICE_DEMAND_START, SERVICE_NO_CHANGE, (char *)str_path.mb_str(), NULL, NULL, NULL,
			NULL, NULL, NULL) == 0)
			{
				CloseServiceHandle(schService);
				UnlockServiceDatabase(sclLock);
				CloseServiceHandle(schSCManager);
				return false;
			}
	}

	// Close service handle and others
	CloseServiceHandle(schService);
	UnlockServiceDatabase(sclLock);
	CloseServiceHandle(schSCManager);

	return true;
}

// Inicia o servi�o
bool RunService(void)
{
	SC_HANDLE schSCManager, schService;

	// Open SCM
	if ((schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
	{
		// Maybe we are in a Windows 9x environment...
		RunNetModule();
		return false;
	}

	// Open service data... something like that :)
	if ((schService = OpenService(schSCManager, "NetModule", SERVICE_ALL_ACCESS)) == NULL)
	{
		CloseServiceHandle(schSCManager);
		return false;
	}

	if (!StartService(schService, 0, NULL))
	{
		CloseServiceHandle(schService);
		CloseServiceHandle(schSCManager);
		return false;
	}

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);

	return true;
}

// Finaliza o servi�o
bool StopService(void)
{
	SC_HANDLE schSCManager, schService;

	// Open SCM
	if ((schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
		return false;

	// Open service data... something like that :)
	if ((schService = OpenService(schSCManager, "NetModule", SERVICE_STOP)) == NULL)
	{
		CloseServiceHandle(schSCManager);
		return false;
	}

	// Send SERVICE_CONTROL_STOP OpCode
	SERVICE_STATUS ssStatus;
	if (!ControlService(schService, SERVICE_CONTROL_STOP, &ssStatus))
	{
		CloseServiceHandle(schService);
		CloseServiceHandle(schSCManager);
		return false;
	}

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);

	return true;
}

// Executa o NetModule
void RunNetModule(void)
{
	wxString path = "\"" + AppBase::GetAppDir() + "\\NetModule.exe\"";
	ShellExecute(NULL, NULL, (char *)path.mb_str(), NULL, NULL, SW_SHOW);
}
