/* TweaksWindows class
 *
 * Copyright (c) 2005 João Paulo Just Peixoto <just1982@gmail.com>.
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

#include "TweaksWindows.h"
#include "TweaksDefs.h"
#include <wx/fileconf.h>

// Class constructor
TweaksWindows::TweaksWindows(bool getall, bool loadall)
{
	Tweak *aux;
	int i = 0, j = 0;

	tweaks.clear();

	while (tweaks_windows[i][0] != '0')
	{
		aux = new Tweak(tweaks_windows[i++]);

		// Check if this restriction works for my Windows version
		if ((AppBase::GetAllVer() == true) || (getall == true)) // If the user choosed to see all restrictions, let's get everything
		{
			if (aux->GetWinVer().Contains(AppBase::GetWinVerStr()))
				aux->SetDescription("Windows " + AppBase::GetWinVerStr().Upper() + ": " + aux->GetDescription());
		}
		else if ((!aux->GetWinVer().Contains(AppBase::GetWinVerStr())) && (!aux->GetWinVer().Contains(wxT("all"))))
		{
			delete aux;
			continue;
		}

		tweaks.push_back(*aux);
		j++;
		delete aux;
	};

	if (loadall == true)
		LoadTweaks();
}

// Load tweaks from the registry
void TweaksWindows::LoadTweaks(void)
{
	unsigned int i;
	wxString key;

	for (i = 0; i < tweaks.size(); i++)
	{
		key = tweaks[i].GetKey();
		// Verifica se o valor est� em HKEY_LOCAL_MACHINE ou HKEY_CURRENT_USER
		if (tweaks[i].GetWinVer().c_str()[0] == '!')
			tweaks[i].SetData(jsreg_getvalue_int(HKEY_LOCAL_MACHINE, key.mb_str(), tweaks[i].GetValue(), 0));
		else
			tweaks[i].SetData(jsreg_getvalue_int(HKEY_CURRENT_USER, key.mb_str(), tweaks[i].GetValue(), 0));
	}
}

// Aplica tweaks no registro
bool TweaksWindows::SaveTweaks(bool hku, wxArrayString Users, bool thisuser)
{
	unsigned int i;
	wxString key;

	// Apaga valores atuais
	jsreg_delkey(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer");
	jsreg_delkey(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\ActiveDesktop");

	AppBase::RegDelUserKey("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", hku, Users, thisuser);
	AppBase::RegDelUserKey("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\ActiveDesktop", hku, Users, thisuser);

	// Salva novos valores
	for (i = 0; i < tweaks.size(); i++)
	{
		if (tweaks[i].GetData() == 0)
			continue;

		key = tweaks[i].GetKey();

		// Verifica se deve ser salvo em HKEY_LOCAL_MACHINE ou HKEY_USERS
		if (tweaks[i].GetWinVer().c_str()[0] == '!')
		{
			if (jsreg_savevalue_int(HKEY_LOCAL_MACHINE, key.c_str(), tweaks[i].GetValue().c_str(), tweaks[i].GetData()) > 0)
				return false;
		}
		else
		{
			if (AppBase::RegSaveUserValue(key, tweaks[i].GetValue(), tweaks[i].GetData(), hku, Users, thisuser) != 0)
				return false;
		}
	}

	return true;
}

// Load tweaks from profile
bool TweaksWindows::LoadFromProfile(wxString filename)
{
	wxFileConfig fc(wxEmptyString, wxEmptyString, filename, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
	wxString profile_str, item_str;
	unsigned int i;

	if (fc.Read("/Tweaks/Windows", &profile_str) == false)
		return false;

	// Clear all tweaks
	for (i = 0; i < tweaks.size(); i++)
		tweaks[i].SetData(0);

	// Set tweaks according to profile
	while (profile_str.Len() > 0)
	{
		item_str = profile_str.BeforeFirst(',');

		if (item_str.Len() > 0)
			for (i = 0; i < tweaks.size(); i++)
				if (tweaks[i].GetTag() == item_str)
					tweaks[i].SetData(1);

		profile_str.Remove(0, item_str.Len() + 1);
	}

	return true;
}

// Save tweaks to profile
bool TweaksWindows::SaveToProfile(wxString filename)
{
	wxFileConfig fc(wxEmptyString, wxEmptyString, filename, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
	wxString profile_str;
	int error = 0;
	unsigned int i;

	for (i = 0; i < tweaks.size(); i++)
	{
		if (tweaks[i].GetData() == 1)
			profile_str.Append(tweaks[i].GetTag() + ",");
	}

	error += BOOL2INT(!fc.Write("/Tweaks/Windows", profile_str));
	error += BOOL2INT(!fc.Flush());

	if (error > 0)
		return false;
	else
		return true;
}

// Return the number of tweaks (instances of Tweak)
int TweaksWindows::GetTweaksCount(void)
{
	return tweaks.size();
}

// Return the Tweak instance
Tweak TweaksWindows::GetTweak(int index)
{
	return tweaks[index];
}

// Set the Tweak instance
void TweaksWindows::SetTweak(int index, Tweak NewTweak)
{
	tweaks[index] = NewTweak;
}
