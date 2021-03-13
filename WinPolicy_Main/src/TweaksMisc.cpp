/* TweaksMisc class
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

#include "TweaksMisc.h"
#include <wx/fileconf.h>
#include <windows.h>
#include <math.h>

// Class constructor
TweaksMisc::TweaksMisc(bool loadall)
{
	t_noiestartpage = false;
	t_nodosrun = false;
	t_nodosmode = false;
	t_hiddendrives = 0;
	t_denieddrives = 0;
	t_nof8 = false;
	t_forcelogon = false;
	t_requirepwd = false;

	if (loadall == true)
		LoadTweaks();
}

// Load tweaks from the registry
void TweaksMisc::LoadTweaks(void)
{
	t_noiestartpage = INT2BOOL(jsreg_getvalue_int(HKEY_CURRENT_USER, "Software\\Policies\\Microsoft\\Internet Explorer\\Control Panel", "HomePage", 0));
	t_nodosrun = INT2BOOL(jsreg_getvalue_int(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\WinOldApp", "Disabled", 0));
	t_nodosmode = INT2BOOL(jsreg_getvalue_int(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\WinOldApp", "NoRealMode", 0));

	t_hiddendrives = jsreg_getvalue_int(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoDrives", 0);
	t_denieddrives = jsreg_getvalue_int(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoViewOnDrive", 0);

	// F5/F8 menu (Windows 95 and 98 only)
	wxFileConfig fc(wxEmptyString, wxEmptyString, "C:\\MSDOS.SYS", wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
	long BootKeys = 0;
	fc.Read("/Options/BootKeys", &BootKeys);
	t_nof8 = !INT2BOOL(BootKeys);

	// Windows 95, 98 and Me only
	if (strcmp(jsreg_getvalue(HKEY_USERS, ".Default\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", "NoLogon", ""), "") != 0)
		t_forcelogon = true;
	else
		t_forcelogon = false;
	t_requirepwd = INT2BOOL(jsreg_getvalue_int(HKEY_LOCAL_MACHINE, "Network\\Logon", "MustBeValidated", 0));
}

// Aplica os tweaks no registro
bool TweaksMisc::SaveTweaks(bool hku, wxArrayString Users, bool thisuser)
{
	int error = 0;

	// P�gina inicial do IE e restri��es do MS-DOS
	error += AppBase::RegSaveUserValue("Software\\Policies\\Microsoft\\Internet Explorer\\Control Panel", "HomePage", BOOL2INT(t_noiestartpage), hku, Users, thisuser);
	error += AppBase::RegSaveUserValue("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\WinOldApp", "Disabled", BOOL2INT(t_nodosrun), hku, Users, thisuser);
	error += AppBase::RegSaveUserValue("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\WinOldApp", "NoRealMode", BOOL2INT(t_nodosmode), hku, Users, thisuser);

	// Drives
	error += AppBase::RegSaveUserValue("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoDrives", (int)t_hiddendrives, hku, Users, thisuser);
	error += AppBase::RegSaveUserValue("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoViewOnDrive", (int)t_denieddrives, hku, Users, thisuser);

	// Menu F5/F8 menu (Windows 95 e 98 somente)
	if ((AppBase::GetRealWinVer() == RWINVER_95) || (AppBase::GetRealWinVer() == RWINVER_98))
	{
		SetFileAttributes("C:\\MSDOS.SYS", FILE_ATTRIBUTE_NORMAL);
		wxFileConfig fc(wxEmptyString, wxEmptyString, "C:\\MSDOS.SYS", wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
		error += BOOL2INT(!fc.Write("/Options/BootKeys", BOOL2INT(!t_nof8)));
		error += BOOL2INT(!fc.Flush());
		SetFileAttributes("C:\\MSDOS.SYS", FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY);
	}

	// For�ar logon (Win9x)
	if (AppBase::GetRealWinVer() < RWINVER_2K)
	{
		if (t_forcelogon)
			error += jsreg_savevalue(HKEY_USERS, ".Default\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", "NoLogon", "rundll32 user32,ExitWindowsEx 0");
		else
		{
			error += jsreg_savevalue(HKEY_USERS, ".Default\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", "NoLogon", "");
			jsreg_delvalue(HKEY_USERS, ".Default\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", "NoLogon");
		}

		// Requerer senha v�lida de rede (Win9x)
		error += jsreg_savevalue_int(HKEY_LOCAL_MACHINE, "Network\\Logon", "MustBeValidated", BOOL2INT(t_requirepwd));
	}

	return true;
}

// Carrega tweaks do perfil
bool TweaksMisc::LoadFromProfile(wxString filename)
{
	wxFileConfig fc(wxEmptyString, wxEmptyString, filename, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);

	fc.Read("/Misc/LockStartPage", &t_noiestartpage);
	fc.Read("/Misc/DisallowDOSProgram", &t_nodosrun);
	fc.Read("/Misc/DisallowDOSMode", &t_nodosmode);
	fc.Read("/Misc/RestrictF8Menu", &t_nof8);
	fc.Read("/Misc/ForceLogon", &t_forcelogon);
	fc.Read("/Misc/RequirePwd", &t_requirepwd);

	fc.Read("/Misc/HideDisks", &t_hiddendrives);
	fc.Read("/Misc/DenyDisks", &t_denieddrives);

	return true;
}

// Save tweaks to profile
bool TweaksMisc::SaveToProfile(wxString filename)
{
	wxFileConfig fc(wxEmptyString, wxEmptyString, filename, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
	int error = 0;

	error += BOOL2INT(!fc.Write("/Misc/LockStartPage", t_noiestartpage));
	error += BOOL2INT(!fc.Write("/Misc/DisallowDOSProgram", t_nodosrun));
	error += BOOL2INT(!fc.Write("/Misc/DisallowDOSMode", t_nodosmode));
	error += BOOL2INT(!fc.Write("/Misc/RestrictF8Menu", t_nof8));
	error += BOOL2INT(!fc.Write("/Misc/ForceLogon", t_forcelogon));
	error += BOOL2INT(!fc.Write("/Misc/RequirePwd", t_requirepwd));

	error += BOOL2INT(!fc.Write("/Misc/HideDisks", t_hiddendrives));
	error += BOOL2INT(!fc.Write("/Misc/DenyDisks", t_denieddrives));

	error += BOOL2INT(!fc.Flush());

	if (error > 0)
		return false;
	else
		return true;
}

// GetDrivesInt method
double TweaksMisc::GetDrivesNum(wxString *str_drives)
{
	int i;
	double drives = 0;
	short letter;
	char letters[26] = {0};

	str_drives->MakeUpper();

	// Remove invalid characters
	for (i = 0; i < (int)str_drives->Len(); i++)
	{
		letter = str_drives->GetChar(i) - 65;
		if (letter < 0)
			letter = 26;

		if ((letters[letter] == 1) || (letter > 25))
		{
			str_drives->Remove(i, 1);
			i--;
		}
		else
			letters[letter] = 1;
	}

	// Calculate the representative integer
	for (i = 0; i < (int)str_drives->Len(); i++)
		drives += pow(2, str_drives->GetChar(i) - 65);

	return drives;
}

// GetDrivesStr method
wxString TweaksMisc::GetDrivesStr(double drives)
{
	int i;
	char drive[2] = {0};
	wxString str_result;

	for (i = 0; i < 26; i++)
	{
		if (drives >= pow(2, 25 - i))
		{
			drives -= pow(2, 25 - i);
			drive[0] = 65 + 25 - i;
			str_result.Prepend(wxString(drive));
		}
	}

	return str_result;
}

/////////////////////////////////////////////////
// Return the restrictions values
bool TweaksMisc::GetTNoIEStartPage(void)
{
	return t_noiestartpage;
}

bool TweaksMisc::GetTNoDOSRun(void)
{
	return t_nodosrun;
}

bool TweaksMisc::GetTNoDOSMode(void)
{
	return t_nodosmode;
}

double TweaksMisc::GetTHiddenDrives(void)
{
	return t_hiddendrives;
}

double TweaksMisc::GetTDeniedDrives(void)
{
	return t_denieddrives;
}

bool TweaksMisc::GetTNoF8(void)
{
	return t_nof8;
}

bool TweaksMisc::GetTForceLogon(void)
{
	return t_forcelogon;
}

bool TweaksMisc::GetTRequirePassword(void)
{
	return t_requirepwd;
}
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Set restrictions values
void TweaksMisc::SetTNoIEStartPage(bool value)
{
	t_noiestartpage = value;
}

void TweaksMisc::SetTNoDOSRun(bool value)
{
	t_nodosrun = value;
}

void TweaksMisc::SetTNoDOSMode(bool value)
{
	t_nodosmode = value;
}

void TweaksMisc::SetTHiddenDrives(double value)
{
	t_hiddendrives = value;
}

void TweaksMisc::SetTDeniedDrives(double value)
{
	t_denieddrives = value;
}

void TweaksMisc::SetTNoF8(bool value)
{
	t_nof8 = value;
}

void TweaksMisc::SetTForceLogon(bool value)
{
	t_forcelogon = value;
}

void TweaksMisc::SetTRequirePassword(bool value)
{
	t_requirepwd = value;
}
