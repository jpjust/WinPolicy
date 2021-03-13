/* WinPolicy's base class and some global constants
 *
 * Copyright (c) 2004 - 2006 João Paulo Just Peixoto <just1982@gmail.com>.
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

#include "AppBase.h"
#include "jswinreg.h"
#include <shellapi.h>
#include <userenv.h>
#include <rsatool.h>
#include <wx/dir.h>
#include <wx/stdpaths.h>

// Initialize the static attributes
wxString AppBase::AppDir = "";
bool AppBase::AccessStatus = true;
bool AppBase::KeysLocked = false;

wxArrayString AppBase::m_users = wxArrayString();
wxSemaphore AppBase::m_semareg(1);

HINSTANCE AppBase::hinstDLL = LoadLibrary((LPCTSTR)"wphooks.dll");
HOOKPROC AppBase::hkprcSysMsg = (HOOKPROC)GetProcAddress(AppBase::hinstDLL, "LowLevelKeyboardProc@12");
HHOOK AppBase::hhookSysMsg = NULL;

const wxString AppBase::StrWinVersion[] = {"Windows 95", "Windows 98", "Windows Me", "Windows NT", "Windows 2000/2003", "Windows XP/Vista"};
const wxString AppBase::StrWinVer[] = {"95", "98", "me", "nt", "2k", "xp"};

// GetAppDir method: return the AppDir's value
wxString AppBase::GetAppDir(void)
{
	return AppDir;
}

// GetImgDir: retorna o diret�rio das imagens
wxString AppBase::GetImgDir(void)
{
	return AppDir + "/images";
}

// LoadImage: carrega um arquivo PNG e retorna como wxBitmap
wxBitmap AppBase::LoadImage(wxString Image)
{
	wxImage img(GetImgDir() + wxT("/") + Image);
	img.SetMaskColour(255, 255, 255);
	return wxBitmap(img, wxBITMAP_TYPE_JPEG);
}

// SetAppDir method: set the AppDir's value
void AppBase::SetAppDir(wxString str_argv0)
{
	AppDir.Clear();

#ifdef _WINPOLICY_DEBUG_
	AppDir.Append("D:\\WinPolicy");
#else
	AppDir.Append(str_argv0);
	AppDir = AppDir.BeforeLast('\\');
#endif
}

// Exibe mensagem de erro do Win32 formatada
void AppBase::ShowError(DWORD error)
{
	LPVOID lpMsgBuf;

	FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL);

	MessageBox(NULL, (LPSTR)lpMsgBuf, "Error", MB_OK);
}

// CheckPrevInstance method: check for an existing mutex
bool AppBase::CheckPrevInstance(wxString str_mutex, HANDLE *hl_mutex)
{
	HANDLE mutex;

	if ((mutex = CreateMutex(NULL, true, (char *)str_mutex.mb_str())) == NULL)
	{
#ifdef _WINPOLICY_DEBUG_
		wxMessageBox("Erro ao criar mutex!", APP_NAME, wxICON_ERROR);
#endif
		return true;
	}

	if ((GetLastError() == ERROR_ALREADY_EXISTS) || (GetLastError() == ERROR_ACCESS_DENIED))
	{
#ifdef _WINPOLICY_DEBUG_
		wxMessageBox("Uma inst�ncia do aplicativo foi detectada.", APP_NAME, wxICON_EXCLAMATION);
#endif
		CloseHandle(mutex);
		return true;
	}

	if (hl_mutex != NULL)
		*hl_mutex = mutex;
	return false;
}

// GetRealWinVer method: return the Windows version, using its API
int AppBase::GetRealWinVer(void)
{
	int winver;
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if (!GetVersionEx(&osvi))
		return 0;

	winver = osvi.dwMajorVersion * 100 + osvi.dwMinorVersion;

	return winver;
}

// GetWinVer method: return the WinVer attribute
int AppBase::GetWinVer(bool raw)
{
	if (raw)
		return jsreg_getvalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy", "WinVer", 0);
	else
		return jsreg_getvalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy", "WinVer", WINVER_95);
}

// GetWinVerStr method: return the WinVer attribute in wxString format
wxString AppBase::GetWinVerStr(void)
{
	return StrWinVer[GetWinVer(false) - WINVER_95];
}

// SetWinVer method: set the WinVer value
bool AppBase::SetWinVer(int newver)
{
	return !(INT2BOOL(jsreg_savevalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy", "WinVer", newver)));
}

// GetAllVer method: return 'true' if 'All Windows Versions' option is selected
bool AppBase::GetAllVer(void)
{
	return INT2BOOL(jsreg_getvalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy", "ShowAllTweaks", 0));
}

// SetAllVer method: set if 'All Windows Versions' option is selected
bool AppBase::SetAllVer(bool allver)
{
	return !(INT2BOOL(jsreg_savevalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy", "ShowAllTweaks", BOOL2INT(allver))));
}

// GetAccessStatus method: get the access status (ok or denied)
bool AppBase::GetAccessStatus(void)
{
	return AccessStatus;
}

// SetAccesssStatus method: set the access status (ok or denied)
void AppBase::SetAccessStatus(bool NewStatus)
{
	AccessStatus = NewStatus;
}

// Obt�m o diret�rio de usu�rios
wxString AppBase::GetUsersDir(void)
{
	wxStandardPaths Paths;
	wxString Result = Paths.GetUserConfigDir();	// Retorna C:\Documents and Settings\username\Application Data
	Result = Result.BeforeLast(wxT('\\'));		// Retorna C:\Documents and Settings\username

	// Windows 9x
	int winver = GetRealWinVer();
	if ((winver >= RWINVER_98) && (winver <= RWINVER_ME))
		return Result + wxT("\\Profiles");		// Termina aqui e retorna C:\WINDOWS\Profiles

	Result = Result.BeforeLast(wxT('\\'));		// Retorna C:\Documents and Settings

	return Result;
}

// Carrega chaves dos usu�rios no registro
wxArrayString AppBase::LoadUsersReg(void)
{
	m_semareg.Wait();

	wxString RegFile;
	wxString UsersDir = GetUsersDir();
	wxArrayString Array = GetUsers();

	if (m_users.Count() > 0)
	{
		m_semareg.Post();
		return m_users;
	}

	if ((AppBase::GetRealWinVer() < RWINVER_2K) && (AppBase::GetRealWinVer() != RWINVER_NT))
		RegFile = wxT("user.dat");		// Windows 9x
	else
		RegFile = wxT("ntuser.dat");	// Windows NT

	// Carrega cada usu�rio
	for (unsigned int i = 0; i < Array.GetCount(); i++)
	{
		wxString Filename = UsersDir + wxT("\\") + Array[i] + wxT("\\") + RegFile;
		if (jsreg_loadkey(HKEY_USERS, (char *)Array[i].c_str(), (char *)Filename.c_str()) == 0)
			m_users.Add(Array[i]);
	}

	m_semareg.Post();
	return m_users;
}

// Descarrega as chaves dos usu�rios do registro
bool AppBase::UnLoadUsersReg(wxArrayString Users)
{
	m_semareg.Wait();

	int error = 0;

	for (unsigned int i = 0; i < Users.GetCount(); i++)
		error += jsreg_unloadkey(HKEY_USERS, (char *)Users[i].c_str());

	m_users.Clear();
	m_semareg.Post();
	return INT2BOOL(error);
}

// Retorna uma lista com os usu�rios carregados
wxArrayString AppBase::GetLoadedUsers(void)
{
	return m_users;
}

// Obt�m array de usu�rios
wxArrayString AppBase::GetUsers(void)
{
	wxArrayString Array;
	wxDir DirList;

	if (!DirList.Open(GetUsersDir()))
		return Array;

	wxString Dir;

	if (DirList.GetFirst(&Dir, wxEmptyString, wxDIR_DIRS))
	{
		do
		{
			Array.Add(Dir);
		} while (DirList.GetNext(&Dir));
	}

	return Array;
}

// Apaga a chave no registro dos usu�rios
bool AppBase::RegDelUserKey(wxString SubKey, bool hku, wxArrayString Users, bool thisuser)
{
	int error = 0;

	// Apaga chaves nos registros dos usu�rios da array
	for (unsigned int i = 0; i < Users.GetCount(); i++)
	{
		wxString UserKey = wxString(USERSREG) + wxT("\\") + Users[i] + wxT("\\") + SubKey;
		error += jsreg_delkey(HKEY_LOCAL_MACHINE, (char *)UserKey.mb_str());
	}

	// Apaga chaves nos usu�rios em HKEY_USERS
	if (hku)
	{
		int ckeys;
		char **keys = jsreg_getsubkeys(HKEY_USERS, "", &ckeys);
		for (int i = 0; i < ckeys; i++)
		{
			wxString Key = wxString(keys[i]) + wxT("\\") + SubKey;
			error += jsreg_delkey(HKEY_USERS, (char *)Key.mb_str());
			free(keys[i]);
		}
		if (ckeys > 0)
			free(keys);
	}

	// Apaga no usu�rio atual
	if (thisuser)
		error += jsreg_delkey(HKEY_CURRENT_USER, (char *)SubKey.mb_str());

	return INT2BOOL(error);
}

// RegSaveUserValue methods: save a value in the user's registry area
int AppBase::RegSaveUserValue(wxString SubKey, wxString ValueName, wxString Value, bool hku, wxArrayString Users, bool thisuser)
{
	int error = 0;

	// Salva nas chaves em HKEY_USERS
	if (hku)
	{
		int ckeys;
		char **keys = jsreg_getsubkeys(HKEY_USERS, "", &ckeys);
		for (int i = 0; i < ckeys; i++)
		{
			wxString Key = wxString(keys[i]) + wxT("\\") + SubKey;
			error += jsreg_savevalue(HKEY_USERS, (char *)Key.c_str(), (char *)ValueName.c_str(), (char *)Value.mb_str());
			free(keys[i]);
		}
		if (ckeys > 0)
			free(keys);
	}

	// Salva no usu�rio atual
	if (thisuser)
		error += jsreg_savevalue(HKEY_CURRENT_USER, (char *)SubKey.c_str(), (char *)ValueName.c_str(), (char *)Value.mb_str());

	// Salva nos usu�rios da array
	for (unsigned int i = 0; i < Users.GetCount(); i++)
	{
		wxString UserKey = Users[i] + wxT("\\") + SubKey;
		error += jsreg_savevalue(HKEY_USERS, (char *)UserKey.mb_str(), (char *)ValueName.mb_str(), (char *)Value.mb_str());
	}

	return error;
}

int AppBase::RegSaveUserValue(wxString SubKey, wxString ValueName, int Value, bool hku, wxArrayString Users, bool thisuser)
{
	int error = 0;

	// Salva nas chaves em HKEY_USERS
	if (hku)
	{
		int ckeys;
		char **keys = jsreg_getsubkeys(HKEY_USERS, "", &ckeys);
		for (int i = 0; i < ckeys; i++)
		{
			wxString Key = wxString(keys[i]) + wxT("\\") + SubKey;
			error += jsreg_savevalue_int(HKEY_USERS, (char *)Key.c_str(), (char *)ValueName.c_str(), Value);
			free(keys[i]);
		}
		if (ckeys > 0)
			free(keys);
	}

	// Salva no usu�rio atual
	if (thisuser)
		error += jsreg_savevalue_int(HKEY_CURRENT_USER, (char *)SubKey.c_str(), (char *)ValueName.c_str(), Value);

	// Salva nos usu�rios da array
	for (unsigned int i = 0; i < Users.GetCount(); i++)
	{
		wxString UserKey = Users[i] + wxT("\\") + SubKey;
		error += jsreg_savevalue_int(HKEY_USERS, (char *)UserKey.mb_str(), (char *)ValueName.mb_str(), Value);
	}

	return error;
}

// InstallService method: install WPService.exe as a Windows NT service
bool AppBase::InstallService(void)
{
	wxString str_alockpath = GetAppDir() + "\\WPService.exe";
	SC_HANDLE schSCManager;

	if ((schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
		return false;

	if (CreateService(
		schSCManager,                  // SCManager database
		"AutoLock",                    // name of service
		"WinPolicy AutoLock",          // service name to display
		SERVICE_ALL_ACCESS,            // desired access
		SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, // service type
		SERVICE_DEMAND_START,          // start type
		SERVICE_ERROR_NORMAL,          // error control type
		(char *)str_alockpath.mb_str(), // path to service's binary
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

// ActivateService method: activate/deactivate and start/stop WPService.exe service
bool AppBase::ActivateService(bool activate)
{
	SC_HANDLE schSCManager, schService;
    SC_LOCK sclLock;

	// Open SCM
	if ((schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
	{
		if (activate)
			jsreg_savevalue(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", "AutoLock", (char *)wxString(GetAppDir() + "\\AutoLock.exe").mb_str());
		else
			jsreg_delvalue(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", "AutoLock");
		return false;
	}

	// Lock database
	if ((sclLock = LockServiceDatabase(schSCManager)) == NULL)
	{
		CloseServiceHandle(schSCManager);
		return false;
	}

	// Open service data... something like that :)
	if ((schService = OpenService(schSCManager, "AutoLock", SERVICE_CHANGE_CONFIG | SERVICE_START)) == NULL)
	{
		UnlockServiceDatabase(sclLock);
		CloseServiceHandle(schSCManager);
		return false;
	}

	wxString str_alockpath = AppBase::GetAppDir() + "\\WPService.exe";

	if (activate)
	{
		// Configure AutoLock service to start automatically and don't forget to
		// make sure it is interactive
		if (ChangeServiceConfig(schService, SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
			SERVICE_AUTO_START,	SERVICE_NO_CHANGE, (char *)str_alockpath.mb_str(), NULL, NULL, NULL,
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
			SERVICE_DEMAND_START, SERVICE_NO_CHANGE, (char *)str_alockpath.mb_str(), NULL, NULL, NULL,
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

// MakeServiceInteractive method: make sure the WPService.exe service is interactive
bool AppBase::MakeServiceInteractive(void)
{
	SC_HANDLE schSCManager, schService;
    SC_LOCK sclLock;

	// Open SCM
	if ((schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
		return false;

	// Lock database
	if ((sclLock = LockServiceDatabase(schSCManager)) == NULL)
	{
		CloseServiceHandle(schSCManager);
		return false;
	}

	// Open service data... something like that :)
	if ((schService = OpenService(schSCManager, "AutoLock", SERVICE_CHANGE_CONFIG)) == NULL)
	{
		UnlockServiceDatabase(sclLock);
		CloseServiceHandle(schSCManager);
		return false;
	}

	wxString str_alockpath = AppBase::GetAppDir() + "\\WPService.exe";

	if (ChangeServiceConfig(schService, SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
		SERVICE_NO_CHANGE, SERVICE_NO_CHANGE, (char *)str_alockpath.mb_str(), NULL, NULL, NULL,
		NULL, NULL, NULL) == 0)
		{
			CloseServiceHandle(schService);
			UnlockServiceDatabase(sclLock);
			CloseServiceHandle(schSCManager);
			return false;
		}

	CloseServiceHandle(schService);
	UnlockServiceDatabase(sclLock);
	CloseServiceHandle(schSCManager);

	return true;
}

// Ativa ou desativa o filtro de teclas do WPHooks.dll
bool AppBase::DisableKeyboard(bool Disable)
{
	// Disable = true e KeysLocked = false (primeira chamada para ativar o filtro)
	if ((Disable) && (!KeysLocked))
	{
		// Compatibilidade com Windows 9x
		SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, TRUE, 0, 0);

		// Windows NT
		if ((hhookSysMsg = SetWindowsHookEx(WH_KEYBOARD_LL, hkprcSysMsg, hinstDLL, 0)) == NULL)
			return false;

		KeysLocked = true;
		return true;
	}
	// Disable = false e KeysLocked = true (primeira chamada para desativar o filtro)
	else if ((!Disable) && (KeysLocked))
	{
		// Compatibilidade com Windows 9x
		SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, FALSE, 0, 0);

		// Windows NT
		if (UnhookWindowsHookEx(hhookSysMsg) == true)
		{
			KeysLocked = false;
			return true;
		}
		else
			return false;
	}
	else
		return false;	// Algo incoerente...
}

// RunWinPolicy method: run WinPolicy.exe
void AppBase::RunWinPolicy(void)
{
	wxString wp = "\"" + AppBase::GetAppDir() + "\\winpolicy.exe\"";
	wxExecute(wxString(wp), wxEXEC_ASYNC, NULL);
}

// LockNow method: lock the workstation with WPLocker
bool AppBase::LockNow(bool async)
{
	wxString wplocker = "wplocker.exe";
	bool err;

	if (async)
	{
		if (wxExecute(wxString(wplocker), wxEXEC_ASYNC, NULL) == 0)
			err = false;
		else
			err = true;
	}
	else
	{
		if (wxExecute(wxString(wplocker), wxEXEC_SYNC, NULL) == 0)
			err = true;
		else
			err = false;
	}

	return err;
}

// RunAutoLock method: run AutoLock.exe
void AppBase::RunAutoLock(void)
{
	wxString alock = "\"" + AppBase::GetAppDir() + "\\autolock.exe\"";
	ShellExecute(NULL, NULL, (char *)alock.mb_str(), NULL, NULL, SW_SHOW);
	//wxExecute(wxString(alock), wxEXEC_ASYNC, NULL);
	//wxShell(wxString(alock));
}

// RunAutoLockService method: run WPService.exe as a Windows Service
bool AppBase::RunAutoLockService(void)
{
	SC_HANDLE schSCManager, schService;

	// Open SCM
	if ((schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
	{
		// Maybe we are in a Windows 9x environment...
		RunAutoLock();
		return false;
	}

	// Open service data... something like that :)
	if ((schService = OpenService(schSCManager, "AutoLock", SERVICE_ALL_ACCESS)) == NULL)
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

// StopAutoLockService method: stop WPService.exe
bool AppBase::StopAutoLockService(void)
{
	SC_HANDLE schSCManager, schService;

	// Open SCM
	if ((schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
		return false;

	// Open service data... something like that :)
	if ((schService = OpenService(schSCManager, "AutoLock", SERVICE_STOP)) == NULL)
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

// ShutdownWindows method: shutdown the computer
bool AppBase::ShutdownWindows(ShutdownType stype)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	if (AppBase::GetRealWinVer() >= 500)	// Windows 2000 or above
	{
		// Get a token for this process.
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		{
#ifdef _WINPOLICY_DEBUG_
			wxMessageBox("OpenProcessToken() error", APP_NAME, wxICON_ERROR);
#endif
			return false;
		}

		// Get the LUID for the shutdown privilege.
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

		tkp.PrivilegeCount = 1;	// one privilege to set
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		// Get the shutdown privilege for this process.
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

		if (GetLastError() != ERROR_SUCCESS)
		{
#ifdef _WINPOLICY_DEBUG_
			wxMessageBox("AdjustTokenPrivileges() error", APP_NAME, wxICON_ERROR);
#endif
			return false;
		}

		if (!ExitWindowsEx((UINT)stype | EWX_FORCE, 0x00040000))
		{
#ifdef _WINPOLICY_DEBUG_
			wxMessageBox("ExitWindowsEx((UINT)stype | EWX_FORCE, 0x00040000) error", APP_NAME, wxICON_ERROR);
#endif
			return false;
		}
	}
	else	// Windows Me or below
	{
		switch (stype)
		{
		case WP_SHUTDOWN:
			ShellExecute(NULL, NULL, "RUNDLL.EXE", "SHELL32.DLL,SHExitWindowsEx 1", NULL, SW_SHOWDEFAULT);
			break;

		case WP_REBOOT:
			ShellExecute(NULL, NULL, "RUNDLL.EXE", "SHELL32.DLL,SHExitWindowsEx 2", NULL, SW_SHOWDEFAULT);
			break;

		case WP_LOGOFF:
			ShellExecute(NULL, NULL, "RUNDLL.EXE", "SHELL32.DLL,SHExitWindowsEx", NULL, SW_SHOWDEFAULT);
			break;
		}
	}
	return true;
}

// ShowHelp method: show WinPolicy help page
void AppBase::ShowHelp(void)
{
	wxString str_path = AppBase::GetAppDir() + "\\help\\index.html";
	ShellExecute(NULL, NULL, (char *)str_path.mb_str(), NULL, NULL, SW_SHOW);
}
