/* UserMgr class
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
#include <rsatool.h>

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
	wxString str_fulltime((wxChar *)rawtime);
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
	raw_time = strtol((char *)str_time.c_str(), NULL, 10);
	from_h = raw_time / 60;
	from_m = raw_time % 60;

	str_time = str_fulltime.Mid(11, 4);
	raw_time = strtol((char *)str_time.c_str(), NULL, 10);
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
			rawstring.Append(wxT("1"));
		else
			rawstring.Append(wxT("0"));
	}

	rawstring.Append(wxString::Format(wxT("%4d"), t_from.GetHour() * 60 + t_from.GetMinute()));
	rawstring.Append(wxString::Format(wxT("%4d"), t_until.GetHour() * 60 + t_until.GetMinute()));
	rawstring.Replace(wxT(" "), wxT("0"));

	strncpy(rawchar, (char *)rawstring.c_str(), 16);
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
UserMgr::UserMgr(void)
{
	m_shutdown = false;
}

UserMgr::UserMgr(wxString Username)
{
	m_username = Username;
	m_shutdown = false;
}

// Set username
void UserMgr::SetUsername(wxString Username)
{
	m_username = Username;
}

// Set password
void UserMgr::SetPassword(wxString Password)
{
	struct rsa_key_st rk;
	rk.size = USERS_RSA_KEY_SIZE;
	rk.n = USERS_RSA_KEY_N;
	rk.e = USERS_RSA_KEY_E;
	rk.d = "";
	m_password = wxString((wxChar *)rsa_encrypt((char *)Password.c_str(), rk));
}

// Define a senha do usu�rio com o c�digo j� criptografado
void UserMgr::SetPasswordCode(wxString Code)
{
	m_password = Code;
}

// Add a new time
void UserMgr::AddTime(UserMgrTime NewTime)
{
	m_times.push_back(NewTime);
	m_times.resize(m_times.size());
}

// Delete a time
void UserMgr::DelTime(UserMgrTime Time)
{
	unsigned int i = 0;

	while (i < m_times.size())
	{
		if (m_times[i] == Time)
			m_times.erase(m_times.begin() + i);
		else
			i++;
	}
	m_times.resize(m_times.size());
}

// Set if user can shutdown computer
void UserMgr::SetCanShutdown(bool shutdown)
{
	m_shutdown = shutdown;
}

// Get username
wxString UserMgr::GetUsername(void)
{
	return m_username;
}

// Get user's password
wxString UserMgr::GetPassword(void)
{
	return m_password;
}

// Get user's times
vector<UserMgrTime> UserMgr::GetTimes(void)
{
	return m_times;
}

// Return if user can shutdown computer
bool UserMgr::GetCanShutdown(void)
{
	return m_shutdown;
}
