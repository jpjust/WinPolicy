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

#ifndef _TWEAKSPROGRAMS_H
#define _TWEAKSPROGRAMS_H

#include "Tweaks.h"
#include "jswinreg.h"
#include <vector>

using namespace std;

class TweaksPrograms : public Tweaks
{
public:
	TweaksPrograms(bool loadall = true);
	void LoadTweaks(void);
	bool SaveTweaks(bool hku, wxArrayString Users = AppBase::GetLoadedUsers(), bool thisuser = true);

	bool LoadFromProfile(wxString filename);
	bool SaveToProfile(wxString filename);

	bool GetTEnabled(void);
	vector<wxString> GetTPrograms(void);
		wxString GetFilename(int index);
	int GetProgramsCount(void);

	void SetTEnabled(bool value);
	void AddFilename(wxString filename);
	void DelFilename(wxString filename);
	void DelFilename(int index);
	void ClearPrograms(void);

private:
	bool t_tweakenabled;
	vector<wxString> t_programs;
};

#endif
