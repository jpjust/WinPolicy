/* TweaksAutoLock class
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

#include "TweaksAutoLock.h"
#include <wx/fileconf.h>

// Class constructor
TweaksAutoLock::TweaksAutoLock(bool loadall)
{
	t_alockenabled = false;
	t_pwdtolock = false;
	t_pwdtoshutdown = false;
	t_lockkeys = false;
	t_lockonstartup = false;
	t_lockonlogon = false;
	t_lockontime = false;

	// Instala WPServices.exe e faz o servi�o interativo
	AppBase::InstallService();
	AppBase::MakeServiceInteractive();

	if (loadall == true)
		LoadTweaks();
}

// Load tweaks from the registry
void TweaksAutoLock::LoadTweaks(void)
{
	char **values;
	int i, cvalues;
	unsigned int time_h, time_m;
	wxString str_time;

	// Boolean restrictions
	t_alockenabled = INT2BOOL(jsreg_getvalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Lock", "AutoLock", 0));
	t_pwdtolock = INT2BOOL(jsreg_getvalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Lock", "PwdToLock", 0));
	t_pwdtoshutdown = INT2BOOL(jsreg_getvalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Lock", "PwdToShutdown", 0));
	t_lockkeys = INT2BOOL(jsreg_getvalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Lock", "NoCtrlAltDel", 0));
	t_lockonstartup = INT2BOOL(jsreg_getvalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Lock", "LoadLock", 0));
	t_lockonlogon = INT2BOOL(jsreg_getvalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Lock", "BlockLogon", 0));
	t_lockontime = INT2BOOL(jsreg_getvalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Lock", "TimeLock", 0));

	// Times
	t_times.clear();
	values = jsreg_getkeyvalues(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Lock\\Times", &cvalues);
	for (i = 0; i < cvalues; i++)
	{
		str_time = wxString(jsreg_getvalue(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Lock\\Times", values[i], ""));
		if (str_time.Len() == 0)
			continue;

		time_h = strtol(str_time.mb_str(), NULL, 10) / 60;
		time_m = strtol(str_time.mb_str(), NULL, 10) % 60;

		t_times.push_back(wxDateTime(time_h, time_m));
	}
	t_times.resize(t_times.size());

	for (i = 0; i < cvalues; i++)
		if (values[i] != NULL)
			free(values[i]);
	if (values != NULL)
		free(values);

	RefreshService();
	RefreshLockingInit();
}

// Aplica os tweaks no registro
bool TweaksAutoLock::SaveTweaks(void)
{
	int error = 0, time_h, time_m;
	unsigned int i;
	wxString value, key;

	error += jsreg_savevalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Lock", "AutoLock", BOOL2INT(t_alockenabled));
	error += jsreg_savevalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Lock", "PwdToLock", BOOL2INT(t_pwdtolock));
	error += jsreg_savevalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Lock", "PwdToShutdown", BOOL2INT(t_pwdtoshutdown));
	error += jsreg_savevalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Lock", "NoCtrlAltDel", BOOL2INT(t_lockkeys));
	error += jsreg_savevalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Lock", "LoadLock", BOOL2INT(t_lockonstartup));
	error += jsreg_savevalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Lock", "BlockLogon", BOOL2INT(t_lockonlogon));
	error += jsreg_savevalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Lock", "TimeLock", BOOL2INT(t_lockontime));

	jsreg_delallvalues(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Lock\\Times");
	for (i = 0; i < t_times.size(); i++)
	{
		time_h = t_times[i].GetHour();
		time_m = t_times[i].GetMinute();

		value.Printf("%d", time_h * 60 + time_m);
		key.Printf("%d", i);

		error += jsreg_savevalue(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Lock\\Times", key.mb_str(), value.mb_str());
	}

	RefreshService();
	RefreshLockingInit();

	if (error > 0)
		return false;
	else
		return true;
}

// Load tweaks from a profile
bool TweaksAutoLock::LoadFromProfile(wxString filename)
{
	wxFileConfig fc(wxEmptyString, wxEmptyString, filename, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
	wxString str_profile, str_time;
	unsigned int time_h, time_m;

	fc.Read("/Locking/AutoLock", &t_alockenabled);
	fc.Read("/Locking/PwdToLock", &t_pwdtolock);
	fc.Read("/Locking/PwdToShutdown", &t_pwdtoshutdown);
	fc.Read("/Locking/LockKeys", &t_lockkeys);
	fc.Read("/Locking/LockOnStartup", &t_lockonstartup);
	fc.Read("/Locking/LockOnLogon", &t_lockonlogon);
	fc.Read("/Locking/LockOnTime", &t_lockontime);

	t_times.clear();
	if (fc.Read("/Locking/Times", &str_profile) == true)
	{
		while (str_profile.Len() > 0)
		{
			str_time = str_profile.BeforeFirst(',');

			if (str_time.Len() > 0)
			{
				time_h = strtol(str_time.mb_str(), NULL, 10) / 60;
				time_m = strtol(str_time.mb_str(), NULL, 10) % 60;

				t_times.push_back(wxDateTime(time_h, time_m));
			}

			str_profile.Remove(0, str_time.Len() + 1);
		}
		t_times.resize(t_times.size());
	}
	else
		return false;

	return true;
}

// Save tweaks to a profile
bool TweaksAutoLock::SaveToProfile(wxString filename)
{
	wxFileConfig fc(wxEmptyString, wxEmptyString, filename, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
	wxString str_time, str_profile;
	int error = 0;
	unsigned int i;

	error += BOOL2INT(!fc.Write("/Locking/AutoLock", t_alockenabled));
	error += BOOL2INT(!fc.Write("/Locking/PwdToLock", t_pwdtolock));
	error += BOOL2INT(!fc.Write("/Locking/PwdToShutdown", t_pwdtoshutdown));
	error += BOOL2INT(!fc.Write("/Locking/LockKeys", t_lockkeys));
	error += BOOL2INT(!fc.Write("/Locking/LockOnStartup", t_lockonstartup));
	error += BOOL2INT(!fc.Write("/Locking/LockOnLogon", t_lockonlogon));
	error += BOOL2INT(!fc.Write("/Locking/LockOnTime", t_lockontime));

	for (i = 0; i < t_times.size(); i++)
		str_profile.Append(wxString::Format("%d,", t_times[i].GetHour() * 60 + t_times[i].GetMinute()));

	error += BOOL2INT(!fc.Write("/Locking/Times", str_profile));
	error += BOOL2INT(!fc.Flush());

	if (error > 0)
		return false;
	else
		return true;
}

// Atualiza o status do servi�o do AutoLock
bool TweaksAutoLock::RefreshService(void)
{
	int error = 0;

	if (GetTALockEnabled())
	{
		error += BOOL2INT(!AppBase::ActivateService(true));
		error += BOOL2INT(!AppBase::RunAutoLockService());
	}
	else
	{
		error += BOOL2INT(!AppBase::ActivateService(false));
		AppBase::StopAutoLockService();
	}

	if (error > 0)
		return false;
	else
		return true;
}

// Atualiza inicializa��o do WPLocker
int TweaksAutoLock::RefreshLockingInit(void)
{
	int error = 0;

	if (GetTLockOnStartup())
		error += jsreg_savevalue(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\RunServices", "WinPolicy", (char *)wxString("WPLocker.exe").c_str());
	else
	{
		error += jsreg_savevalue(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\RunServices", "WinPolicy", "");
		jsreg_delvalue(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\RunServices", "WinPolicy");
	}

	if (GetTLockOnLogon())
		error += jsreg_savevalue(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", "WinPolicy", (char *)wxString("WPLocker.exe").c_str());
	else
	{
		error += jsreg_savevalue(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", "WinPolicy", "");
		jsreg_delvalue(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", "WinPolicy");
	}

	return error;
}


/////////////////////////////////////////////////
// Return restrictions values
bool TweaksAutoLock::GetTALockEnabled(void)
{
	return t_alockenabled;
}

bool TweaksAutoLock::GetTPwdToLock(void)
{
	return t_pwdtolock;
}

bool TweaksAutoLock::GetTPwdToShutdown(void)
{
	return t_pwdtoshutdown;
}

bool TweaksAutoLock::GetTLockKeys(void)
{
	return t_lockkeys;
}

bool TweaksAutoLock::GetTLockOnStartup(void)
{
	return t_lockonstartup;
}

bool TweaksAutoLock::GetTLockOnLogon(void)
{
	return t_lockonlogon;
}

bool TweaksAutoLock::GetTLockOnTime(void)
{
	return t_lockontime;
}

vector<wxDateTime> TweaksAutoLock::GetTTimes(void)
{
	return t_times;
}

wxDateTime TweaksAutoLock::GetTime(int index)
{
	return t_times[index];
}

int TweaksAutoLock::GetTimeCount(void)
{
	return t_times.size();
}
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Set restrictions values
void TweaksAutoLock::SetTALockEnabled(bool value)
{
	t_alockenabled = value;
}

void TweaksAutoLock::SetTPwdToLock(bool value)
{
	t_pwdtolock = value;
}

void TweaksAutoLock::SetTPwdToShutdown(bool value)
{
	t_pwdtoshutdown = value;
}

void TweaksAutoLock::SetTLockKeys(bool value)
{
	t_lockkeys = value;
}

void TweaksAutoLock::SetTLockOnStartup(bool value)
{
	t_lockonstartup = value;
}

void TweaksAutoLock::SetTLockOnLogon(bool value)
{
	t_lockonlogon = value;
}

void TweaksAutoLock::SetTLockOnTime(bool value)
{
	t_lockontime = value;
}

void TweaksAutoLock::SetTTimes(vector<wxDateTime> new_vector)
{
	t_times = new_vector;
}

void TweaksAutoLock::SetTime(int index, wxDateTime new_time)
{
	t_times[index] = new_time;
}

void TweaksAutoLock::AddTime(wxDateTime new_time)
{
	t_times.push_back(new_time);
	t_times.resize(t_times.size());
}

void TweaksAutoLock::ClearTimes(void)
{
	t_times.clear();
}
/////////////////////////////////////////////////
