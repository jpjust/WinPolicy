/* Tweaks class
 *
 * Copyright (c) 2005 - 2006 João Paulo Just Peixoto <just1982@gmail.com>.
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

#ifndef _TWEAKS_H
#define _TWEAKS_H

#include <wx/string.h>
#include "AppBase.h"

class Tweak
{
public:
	Tweak(const char *tweak_data);

	wxString GetWinVer(void);
	wxString GetTag(void);
	wxString GetKey(void);
	wxString GetValue(void);
	wxString GetDescription(void);
	void SetDescription(wxString Description);
	int GetData(void);
	void SetData(int data);

private:
	wxString t_winver;
	wxString t_tag;
	wxString t_key;
	wxString t_value;
	wxString t_desc;
	int t_data;
};

class Tweaks
{
public:
	bool LoadTweaks(void);
	bool SaveTweaks(bool hku, wxArrayString Users = AppBase::GetLoadedUsers(), bool thisuser = true);
	Tweak GetTweaks(void);
	bool LoadFromProfile(wxString filename);
	bool SaveToProfile(wxString filename);
};

#endif
