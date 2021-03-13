/* User management class
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

#include "UserMgr.h"
#include "jswinreg.h"
#include "AppBase.h"
#include <wx/fileconf.h>
#include <string.h>
#include <stdlib.h>

/////////////////////////////////////////////////
// UserMgrTime class
UserMgrTime::UserMgrTime(void)
{
	t_from = 0;
	t_until = 0;

	for (int i = 0; i < 7; i++)
		t_daysofweek[i] = 0;
}

//UserMgrTime::UserMgrTime(unsigned int time_from, unsigned int time_until, bool daysofweek[7])
UserMgrTime::UserMgrTime(wxDateTime time_from, wxDateTime time_until, bool daysofweek[7])
{
	t_from = time_from;
	t_until = time_until;

	for (int i = 0; i < 7; i++)
		t_daysofweek[i] = daysofweek[i];
}

UserMgrTime::UserMgrTime(char rawtime[16])
{
	wxString str_fulltime(rawtime);
	wxString str_item, str_time;
	unsigned int raw_time, from_h, from_m, to_h, to_m;

	for (int i = 0; i < 7; i++)
	{
		if (str_fulltime.GetChar(i) == '0')
			t_daysofweek[i] = false;
		else
			t_daysofweek[i] = true;
	}

	str_time = str_fulltime.Mid(7, 4);
	raw_time = strtol((char *)str_time.mb_str(), NULL, 10);
	from_h = raw_time / 60;
	from_m = raw_time % 60;

	str_time = str_fulltime.Mid(11, 4);
	raw_time = strtol((char *)str_time.mb_str(), NULL, 10);
	to_h = raw_time / 60;
	to_m = raw_time % 60;

	t_from = wxDateTime(from_h, from_m);
	t_until = wxDateTime(to_h, to_m);
}

// Return the attributes in a raw string format (just like the third constructor)
char *UserMgrTime::GetRawTime(void)
{
	wxString rawstring;
	char *rawchar = (char *)malloc(sizeof(char) * 16);

	for (int i = 0; i < 7; i++)
	{
		if (t_daysofweek[i] == true)
			rawstring.Append("1");
		else
			rawstring.Append("0");
	}

	rawstring.Append(wxString::Format("%4d", t_from.GetHour() * 60 + t_from.GetMinute()));
	rawstring.Append(wxString::Format("%4d", t_until.GetHour() * 60 + t_until.GetMinute()));
	rawstring.Replace(" ", "0");

	strncpy(rawchar, (char *)rawstring.mb_str(), 16);
	return rawchar;
}

// Return the days of week array (allowed days = 1, disallowed days = 0)
bool *UserMgrTime::GetDaysOfWeek(void)
{
	return t_daysofweek;
}

// Return the starting of the allowed time
wxDateTime UserMgrTime::GetTimeFrom(void)
{
	return t_from;
}

// Return the ending of the allowed time
wxDateTime UserMgrTime::GetTimeUntil(void)
{
	return t_until;
}

// Set the allowed days of the week
void UserMgrTime::SetDaysOfWeek(bool daysofweek[7])
{
	for (int i = 0; i < 7; i++)
		t_daysofweek[i] = daysofweek[i];
}

// Set the starting of the allowed time
void UserMgrTime::SetTimeFrom(unsigned int new_time)
{
	t_from = new_time;
}

// Set the ending of the allowed time
void UserMgrTime::SetTimeUntil(unsigned int new_time)
{
	t_until = new_time;
}

// Comparison operator
bool UserMgrTime::operator ==(const UserMgrTime& x)
{
	if (x.t_from != t_from)
		return false;

	if (x.t_until != t_until)
		return false;

	for (int i = 0; i < 7; i++)
		if (x.t_daysofweek[i] != t_daysofweek[i])
			return false;

	return true;
}
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// UserMgr class
////
// Return a vector with the registered users
vector<wxString> UserMgr::GetUsersList(void)
{
	int intUsers, i;
	char **strUsername = jsreg_getsubkeys(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Users", &intUsers);
	vector<wxString> users;

	if (strUsername != NULL)
	{
		for (i = 0; i < intUsers; i++)
		{
			if (strUsername[i] != NULL)
			{
				users.push_back(wxString(strUsername[i]));
				free(strUsername[i]);
			}
		}
		if (strUsername != NULL)
			free(strUsername);
	}
	users.resize(users.size());

	return users;
}

// Check if the given username exists
bool UserMgr::FindUser(wxString Username)
{
	Username.MakeLower();

	vector<wxString> users = GetUsersList();

	for (unsigned int i = 0; i < users.size(); i++)
		if (users[i] == Username)
			return true;

	return false;
}

// Adiciona um novo usu�rio
bool UserMgr::AddUser(wxString Username)
{
	// Coloca em caracteres min�sculos
	Username.MakeLower();

	for (unsigned int i = 0; i < Username.Len(); i++)
		if (((Username.c_str()[i] < 'a') || (Username.c_str()[i] > 'z')) && ((Username.c_str()[i] < '0') || (Username.c_str()[i] > '9')))
			return false;

	if (FindUser(Username) == true)
		return false;

	wxString SubKey = "Software\\Justsoft\\WinPolicy\\Users\\" + Username;

	if (jsreg_savevalue(HKEY_LOCAL_MACHINE, SubKey.mb_str(), "", "") != 0)
		return false;
	else
		return true;
}

// Remove an existing user
bool UserMgr::DelUser(wxString Username)
{
	if (FindUser(Username) == false)
		return false;

	int cSubKeys, i;
	wxString Key = "Software\\Justsoft\\WinPolicy\\Users\\" + Username;
	wxString SubKey;
	char **SubKeys = jsreg_getsubkeys(HKEY_LOCAL_MACHINE, Key, &cSubKeys);
	bool res = false;

	for (i = 0; i < cSubKeys; i++)
	{
		SubKey.Printf("%s\\%s", Key.mb_str(), SubKeys[i]);
		if (jsreg_delkey(HKEY_LOCAL_MACHINE, SubKey.mb_str()) != 0)
			goto deluser_end;
	}

	if (jsreg_delkey(HKEY_LOCAL_MACHINE, Key) != 0)
		goto deluser_end;

	res = true;

deluser_end:
	if (SubKeys == NULL)
		return res;

	for (i = 0; i < cSubKeys; i++)
		if (SubKeys[i] != NULL)
			free(SubKeys[i]);
	if (SubKeys != NULL)
		free(SubKeys);

	return res;
}

// Remove all users
bool UserMgr::DelAllUsers(void)
{
	wxString Key = "Software\\Justsoft\\WinPolicy\\Users";

	if (jsreg_delkey(HKEY_LOCAL_MACHINE, (char *)Key.mb_str()) != 0)
		return false;

	if (jsreg_savevalue(HKEY_LOCAL_MACHINE, (char *)Key.mb_str(), "", "") != 0)
		return false;

	return true;
}

// Return the user's password
wxString UserMgr::GetPassword(wxString Username)
{
	Username.MakeLower();

	if (FindUser(Username) == false)
		return wxString("");

	wxString Key = "Software\\Justsoft\\WinPolicy\\Users\\" + Username;
	return wxString(jsreg_getvalue(HKEY_LOCAL_MACHINE, Key.mb_str(), "Password", ""));
}

// Set the user's password
bool UserMgr::SetPassword(wxString Username, wxString Password)
{
	if (FindUser(Username) == false)
		return false;

	wxString SubKey;
	struct rsa_key_st key;

	key.size = USERS_RSA_KEY_SIZE;
	key.n = USERS_RSA_KEY_N;
	key.e = USERS_RSA_KEY_E;
	key.d = "";

	SubKey = "Software\\Justsoft\\WinPolicy\\Users\\" + Username;
	if (jsreg_savevalue(HKEY_LOCAL_MACHINE, SubKey.mb_str(), "Password", rsa_encrypt((char *)Password.mb_str(), key)) != 0)
		return false;
	else
		return true;
}

// Define a senha do usu�rio com a string j� criptografada
bool UserMgr::SetPasswordCode(wxString Username, wxString Code)
{
	if (FindUser(Username) == false)
		return false;

	wxString SubKey = "Software\\Justsoft\\WinPolicy\\Users\\" + Username;
	if (jsreg_savevalue(HKEY_LOCAL_MACHINE, SubKey.mb_str(), "Password", (char *)Code.mb_str()) != 0)
		return false;
	else
		return true;
}

// Validate the user's password
bool UserMgr::CheckPasswd(wxString Username, wxString Password)
{
	Username.MakeLower();

	if (FindUser(Username) == false)
		return false;

	struct rsa_key_st key;

	key.size = USERS_RSA_KEY_SIZE;
	key.n = USERS_RSA_KEY_N;
	key.e = USERS_RSA_KEY_E;
	key.d = "";

	wxString Passwd_enc(rsa_encrypt((char *)Password.mb_str(), key));

	if (Passwd_enc == GetPassword(Username))
		return true;
	else
		return false;
}

// Erase the user's password
bool UserMgr::DelPassword(wxString Username)
{
	if (FindUser(Username) == false)
		return false;

	wxString Key = "Software\\Justsoft\\WinPolicy\\Users\\" + Username;
	if (jsreg_delvalue(HKEY_LOCAL_MACHINE, Key.mb_str(), "Password") != 0)
		return false;
	else
		return true;
}

// Return a vector of allowed times
vector<UserMgrTime> UserMgr::GetTimes(wxString Username)
{
	wxString str_key, str_fulltime, str_item;
	vector<UserMgrTime> times;
	int ctimes;
	char **values;

	str_key = "Software\\Justsoft\\WinPolicy\\Users\\" + Username + "\\AllowedTime";
	values = jsreg_getkeyvalues(HKEY_LOCAL_MACHINE, (char *)str_key.mb_str(), &ctimes);
	if (values == NULL)
		return times;

	for (int i = 0; i < ctimes; i++)
	{
		str_fulltime = wxString(jsreg_getvalue(HKEY_LOCAL_MACHINE, (char *)str_key.mb_str(), values[i], ""));
		if (str_fulltime == "")
			continue;

		times.push_back(UserMgrTime((char *)str_fulltime.mb_str()));
	}
	times.resize(times.size());

	for (int i = 0; i < ctimes; i++)
		if (values[i] != NULL)
			free(values[i]);
	if (values != NULL)
		free(values);

	return times;
}

// Set the user's allowed times
bool UserMgr::SetTimes(wxString Username, vector<UserMgrTime> times)
{
	if (FindUser(Username) == false)
		return false;

	wxString Key = "Software\\Justsoft\\WinPolicy\\Users\\" + Username + "\\AllowedTime";

	// If the key doesn't exist, create it
	jsreg_savevalue(HKEY_LOCAL_MACHINE, Key.mb_str(), "", "");

	if (jsreg_delallvalues(HKEY_LOCAL_MACHINE, Key) != 0)
		return false;

	for (unsigned int i = 0; i < times.size(); i++)
	{
		wxString Value, Num;
		unsigned int raw_time;

		for (int j = 0; j < 7; j++)
		{
			if (times[i].GetDaysOfWeek()[j] == true)
				Value.Append("1");
			else
				Value.Append("0");
		}

		raw_time = times[i].GetTimeFrom().GetHour() * 60 + times[i].GetTimeFrom().GetMinute();
		Value.Append(wxString::Format("%4d", raw_time));

		raw_time = times[i].GetTimeUntil().GetHour() * 60 + times[i].GetTimeUntil().GetMinute();
		Value.Append(wxString::Format("%4d", raw_time));

		Value.Replace(" ", "0");
		Num.Printf("%d", i);

		if (jsreg_savevalue(HKEY_LOCAL_MACHINE, (char *)Key.mb_str(), (char *)Num.mb_str(), (char *)Value.mb_str()) != 0)
			return false;
	}

	return true;
}

// Add an user's allowed time
bool UserMgr::AddTime(wxString Username, UserMgrTime NewTime)
{
	if (FindUser(Username) == false)
		return false;

	vector<UserMgrTime> times = GetTimes(Username);

	times.push_back(NewTime);
	times.resize(times.size());

	return SetTimes(Username, times);
}

// Delete and user's allowed time
bool UserMgr::DelTime(wxString Username, UserMgrTime Time)
{
	if (FindUser(Username) == false)
		return false;

	vector<UserMgrTime> times = GetTimes(Username);
	unsigned int i = 0;

	while (i < times.size())
	{
		if (times[i] == Time)
			times.erase(times.begin() + i);
		else
			i++;
	}
	times.resize(times.size());

	return SetTimes(Username, times);
}

// Clear the allowed times list
bool UserMgr::ClearTimes(wxString Username)
{
	vector<UserMgrTime> times;	// Create an empty list to store
	return SetTimes(Username, times);
}

// Check if the time Time is allowed for the user
bool UserMgr::IsTimeAllowed(wxString Username, wxDateTime Time, wxDateTime *TimeUntil)
{
	if (FindUser(Username) == false)
		return false;

	vector<UserMgrTime> times = GetTimes(Username);

	for (unsigned int i = 0; i < times.size(); i++)
	{
		if (times[i].GetDaysOfWeek()[Time.GetWeekDay()] == false)
			continue;

		if (Time.IsBetween(times[i].GetTimeFrom(), times[i].GetTimeUntil()) == true)
		{
			if (TimeUntil != NULL)
				*TimeUntil = times[i].GetTimeUntil();

			return true;
		}
	}

	return false;
}

// Return true if user can shutdown the computer
bool UserMgr::GetUserCanShutdown(wxString Username)
{
	Username.MakeLower();

	if (FindUser(Username) == false)
		return false;

	return INT2BOOL(jsreg_getvalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Users\\" + Username, "AllowShutdown", 0));
}

// Set if user can shutdown the computer
bool UserMgr::SetUserCanShutdown(wxString Username, bool allow)
{
	if (FindUser(Username) == false)
		return false;

	if (jsreg_savevalue_int(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Users\\" + Username, "AllowShutdown", BOOL2INT(allow)) != 0)
		return false;
	else
		return true;
}

// Load users' data from profile
bool UserMgr::LoadFromProfile(wxString Filename)
{
	wxFileConfig fc(wxEmptyString, wxEmptyString, Filename, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
	wxString str_time, str_times, str_group, str_user, str_users, str_shutdown, str_pwd;
	int error = 0;

	if (fc.Read("/Users/Users", &str_users) == false)
		return false;

	while (str_users.Len() > 0)
	{
		str_user = str_users.BeforeFirst(',');

		if (str_user.Len() > 0)
		{
			if (fc.Read("/Users/" + str_user + "/AllowedTimes", &str_times) == true)
			{
				while (str_times.Len() > 0)
				{
					str_time = str_times.BeforeFirst(',');

					if (str_time.Len() > 0)
					{
						UserMgrTime Time;
						bool weekdays[7];

						for (int i = 0; i < 7; i++)
						{
							if (str_time.GetChar(i) == '1')
								weekdays[i] = true;
							else
								weekdays[i] = false;
						}

						unsigned int t_from = strtol((char *)str_time.Mid(7, 4).mb_str(), NULL, 10);
						unsigned int t_to = strtol((char *)str_time.Mid(11, 4).mb_str(), NULL, 10);

						Time = UserMgrTime(wxDateTime(t_from / 60, t_from % 60), \
							wxDateTime(t_to / 60, t_to % 60), weekdays);

						error += BOOL2INT(!UserMgr::AddTime(str_user, Time));
					}

					str_times.Remove(0, str_time.Len() + 1);
				}
			}

			UserMgr::SetUserCanShutdown(str_user, false);
			if (fc.Read("/Users/" + str_user + "/AllowShutdown", &str_shutdown) == true)
				if (INT2BOOL(strtol((char *)str_shutdown.mb_str(), NULL, 10)) == true)
					error += BOOL2INT(!UserMgr::SetUserCanShutdown(str_user, true));

			if (fc.Read("/Users/" + str_user + "/Password", &str_pwd) == true)
				error += jsreg_savevalue(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Users\\" + str_user, "Password", str_pwd);
		}

		str_users.Remove(0, str_user.Len() + 1);
	}

	if (error > 0)
		return false;
	else
		return true;
}

// Save users' data to profile
bool UserMgr::SaveToProfile(wxString Filename)
{
	wxFileConfig fc(wxEmptyString, wxEmptyString, Filename, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
	wxString str_times, str_users;
	int error = 0;
	vector<wxString> users = GetUsersList();
	vector<UserMgrTime> times;

	for (unsigned int i = 0; i < users.size(); i++)
	{
		str_users.Append(users[i] + ",");

		str_times.Clear();
		times = UserMgr::GetTimes(users[i]);
		for (unsigned int j = 0; j < times.size(); j++)
			str_times.Append(wxString(times[j].GetRawTime()) + ",");

		error += BOOL2INT(!fc.Write("/Users/" + users[i] + "/AllowedTimes", str_times));
		error += BOOL2INT(!fc.Write("/Users/" + users[i] + "/AllowShutdown", UserMgr::GetUserCanShutdown(users[i])));
		error += BOOL2INT(!fc.Write("/Users/" + users[i] + "/Password", UserMgr::GetPassword(users[i])));
	}

	error += BOOL2INT(!fc.Write("/Users/Users", str_users));
	error += BOOL2INT(!fc.Flush());

	if (error > 0)
		return false;
	else
		return true;
}
/////////////////////////////////////////////////
