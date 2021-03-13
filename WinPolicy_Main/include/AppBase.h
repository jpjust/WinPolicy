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

#ifndef _APPBASE_H
#define _APPBASE_H

// Defini��es de vers�o
//#define _WINPOLICY_FREEWARE_

#include <wx/wx.h>
#include <windows.h>

// Application versioning
#define VERSION_MAJOR 3
#define VERSION_MINOR 2
#define VERSION_REVISION 0
#define VERSION_MAJOR_STR "3"
#define VERSION_MINOR_STR "2"
#define VERSION_REVISION_STR "0"

#ifdef _WINPOLICY_FREEWARE_
# define VERSION_NAME wxT(" Freeware")
#else
# define VERSION_NAME wxT(" Enterprise")
#endif

#define APP_NAME wxString(wxT("")) + wxT("Justsoft WinPolicy 3.2") + VERSION_NAME

#define USERSREG wxT("Software\\Justsoft\\WinPolicy\\UsersReg")

// Windows versions
#define WINVER_ALL 100
#define WINVER_95 101
#define WINVER_98 102
#define WINVER_ME 103
#define WINVER_NT 104
#define WINVER_2K 105
#define WINVER_2K3 105
#define WINVER_XP 106

#define RWINVER_95 400
#define RWINVER_NT 400
#define RWINVER_98 410
#define RWINVER_ME 490
#define RWINVER_2K 500
#define RWINVER_XP 501
#define RWINVER_2K3 502
#define RWINVER_VIS 600

// Global events
#define EVT_PASSWDOK 200
#define EVT_PROFILE_LOAD 201
#define EVT_PROFILE_SAVE 202

// Macros
#define INT2BOOL(x) ((x == 0) ? false : true)
#define BOOL2INT(x) (x ? 1 : 0)

// Tipos de shutdown
enum ShutdownType
{
	WP_SHUTDOWN = EWX_POWEROFF,
	WP_REBOOT = EWX_REBOOT,
	WP_LOGOFF = EWX_LOGOFF
};

class AppBase
{
public:
	static wxLogStderr Logger;
	static void ShowError(DWORD error);

	// Diret�rios e imagens
	static wxString GetAppDir(void);
	static wxString GetImgDir(void);
	static void SetAppDir(wxString str_argv0);
	static wxBitmap LoadImage(wxString Image);

	// Vers�o
	static bool CheckPrevInstance(wxString str_mutex, HANDLE *hl_mutex);
	static int GetRealWinVer(void);
	static int GetWinVer(bool raw);
	static wxString GetWinVerStr(void);
	static bool SetWinVer(int newver);
	static bool GetAllVer(void);
	static bool SetAllVer(bool allver);

	// Status de acesso
	static bool GetAccessStatus(void);
	static void SetAccessStatus(bool NewStatus);

	// Usu�rios
	static wxArrayString LoadUsersReg(void);
	static bool UnLoadUsersReg(wxArrayString Users = m_users);
	static wxArrayString GetLoadedUsers(void);
	static wxString GetUsersDir(void);
	static wxArrayString GetUsers(void);

	// Registro
	static bool RegDelUserKey(wxString SubKey, bool hku = true, wxArrayString Users = m_users, bool thisuser = true);
	static int RegSaveUserValue(wxString SubKey, wxString ValueName, wxString Value, bool hku = true, wxArrayString Users = m_users, bool thisuser = true);
	static int RegSaveUserValue(wxString SubKey, wxString ValueName, int Value, bool hku = true, wxArrayString Users = m_users, bool thisuser = true);

	// Servi�o
	static bool InstallService(void);
	static bool ActivateService(bool activate);
	static bool MakeServiceInteractive(void);

	// Bloqueios
	static bool DisableKeyboard(bool Disable);
	static void RunWinPolicy(void);
	static bool LockNow(bool async);
	static void RunAutoLock(void);
	static bool RunAutoLockService(void);
	static bool StopAutoLockService(void);

	// Outros
	static bool ShutdownWindows(ShutdownType stype = WP_SHUTDOWN);
	static void ShowHelp(void);

	// Constants
	static const wxString StrWinVersion[];
	static const wxString StrWinVer[];

private:
	static wxString AppDir;
	static bool AccessStatus;
	static bool KeysLocked;

	static wxArrayString m_users;
	static wxSemaphore m_semareg;

	static HINSTANCE hinstDLL;
	static HOOKPROC hkprcSysMsg;
	static HHOOK hhookSysMsg;
};

wxString pixmap_path(wxString pixmap);

#endif
