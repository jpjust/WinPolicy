/* TweaksPrograms class
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

#include "TweaksPrograms.h"
#include <wx/fileconf.h>

// Class constructor
TweaksPrograms::TweaksPrograms(bool loadall)
{
	t_tweakenabled = false;

	if (loadall == true)
		LoadTweaks();
}

// Load tweaks from the registry
void TweaksPrograms::LoadTweaks(void)
{
	char **values;
	int i, cvalues;

	t_tweakenabled = INT2BOOL(jsreg_getvalue_int(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "RestrictRun", 0));

	values = jsreg_getkeyvalues(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\RestrictRun", &cvalues);
	t_programs.clear();
	for (i = 0; i < cvalues; i++)
		t_programs.push_back(wxString(jsreg_getvalue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\RestrictRun", values[i], "")));
	t_programs.resize(t_programs.size());

	for (i = 0; i < cvalues; i++)
		if (values[i] != NULL)
			free(values[i]);
	if (values != NULL)
		free(values);
}

// Aplica tweaks no registro
bool TweaksPrograms::SaveTweaks(bool hku, wxArrayString Users, bool thisuser)
{
	unsigned int i;
	wxString value;

	AppBase::RegSaveUserValue("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "RestrictRun", BOOL2INT(t_tweakenabled), hku, Users, thisuser);

	AppBase::RegDelUserKey("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\RestrictRun", hku, Users, thisuser);
	for (i = 0; i < t_programs.size(); i++)
	{
		value.Printf("%d", i);
		AppBase::RegSaveUserValue("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\RestrictRun", value, t_programs[i], hku, Users, thisuser);
	}

	return true;
}

// Load tweaks from profile
bool TweaksPrograms::LoadFromProfile(wxString filename)
{
	wxFileConfig fc(wxEmptyString, wxEmptyString, filename, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
	wxString str_profile, str_item;

	if (fc.Read("/Programs/Deny", &t_tweakenabled) == false)
		return false;

	if (fc.Read("/Programs/Allow", &str_profile) == false)
		return false;

	t_programs.clear();
	while (str_profile.Len() > 0)
	{
		str_item = str_profile.BeforeFirst(',');

		if (str_item.Len() > 0)
			t_programs.push_back(str_item);

		str_profile.Remove(0, str_item.Len() + 1);
	}
	t_programs.resize(t_programs.size());

	return true;
}

// Save tweaks to profile
bool TweaksPrograms::SaveToProfile(wxString filename)
{
	wxFileConfig fc(wxEmptyString, wxEmptyString, filename, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
	int error = 0;
	unsigned int i;
	wxString str_profile;

	for (i = 0; i < t_programs.size(); i++)
		str_profile.Append(t_programs[i] + ",");

	error += BOOL2INT(!fc.Write("/Programs/Deny", t_tweakenabled));
	error += BOOL2INT(!fc.Write("/Programs/Allow", str_profile));
	error += BOOL2INT(!fc.Flush());

	if (error > 0)
		return false;
	else
		return true;
}

/////////////////////////////////////////////////
// Return restrictions values
bool TweaksPrograms::GetTEnabled(void)
{
	return t_tweakenabled;
}

vector<wxString> TweaksPrograms::GetTPrograms(void)
{
	return t_programs;
}

wxString TweaksPrograms::GetFilename(int index)
{
	return t_programs[index];
}

int TweaksPrograms::GetProgramsCount(void)
{
	return t_programs.size();
}
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Set restrictions values
void TweaksPrograms::SetTEnabled(bool value)
{
	t_tweakenabled = value;
}

void TweaksPrograms::AddFilename(wxString filename)
{
	t_programs.push_back(filename);
	t_programs.resize(t_programs.size());
}

void TweaksPrograms::DelFilename(wxString filename)
{
	unsigned int i = 0;

	while (i < t_programs.size())
	{
		if (t_programs[i] == filename)
			t_programs.erase(t_programs.begin() + i);
		else
			i++;
	}

	t_programs.resize(t_programs.size());
}

void TweaksPrograms::DelFilename(int index)
{
	t_programs.erase(t_programs.begin() + index);
	t_programs.resize(t_programs.size());
}

void TweaksPrograms::ClearPrograms(void)
{
	t_programs.clear();
}
/////////////////////////////////////////////////
