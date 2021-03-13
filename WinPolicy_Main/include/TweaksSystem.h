/* TweaksSystem class
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

#ifndef _TWEAKSSYSTEM_H
#define _TWEAKSSYSTEM_H

#include "Tweaks.h"
#include "jswinreg.h"
#include <wx/string.h>
#include <vector>

using namespace std;

class TweaksSystem : public Tweaks
{
public:
	TweaksSystem(bool getall = false, bool loadall = true);
	void LoadTweaks(void);
	bool SaveTweaks(bool hku, wxArrayString Users = AppBase::GetLoadedUsers(), bool thisuser = true);

	bool LoadFromProfile(wxString filename);
	bool SaveToProfile(wxString filename);

	int GetTweaksCount(void);
	Tweak GetTweak(int index);
	void SetTweak(int index, Tweak NewTweak);

	bool RestrictRegedit(void);

private:
	vector<Tweak>	tweaks;
};

#endif
