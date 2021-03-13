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

#ifndef _TWEAKSMISC_H
#define _TWEAKSMISC_H

#include "Tweaks.h"
#include "AppBase.h"
#include "jswinreg.h"

using namespace std;

class TweaksMisc : public Tweaks
{
public:
	TweaksMisc(bool loadall = true);
	void LoadTweaks(void);
	bool SaveTweaks(bool hku, wxArrayString Users = AppBase::GetLoadedUsers(), bool thisuser = true);

	bool LoadFromProfile(wxString filename);
	bool SaveToProfile(wxString filename);

	static double GetDrivesNum(wxString *str_drives);
	static wxString GetDrivesStr(double drives);

	bool GetTNoIEStartPage(void);
	bool GetTNoDOSRun(void);
	bool GetTNoDOSMode(void);
	double GetTHiddenDrives(void);
	double GetTDeniedDrives(void);
	bool GetTNoF8(void);
	bool GetTForceLogon(void);
	bool GetTRequirePassword(void);

	void SetTNoIEStartPage(bool value);
	void SetTNoDOSRun(bool value);
	void SetTNoDOSMode(bool value);
	void SetTHiddenDrives(double value);
	void SetTDeniedDrives(double value);
	void SetTNoF8(bool value);
	void SetTForceLogon(bool value);
	void SetTRequirePassword(bool value);

private:
	bool t_noiestartpage;
	bool t_nodosrun;
	bool t_nodosmode;
	double t_hiddendrives;
	double t_denieddrives;
	bool t_nof8;
	bool t_forcelogon;
	bool t_requirepwd;
};

#endif
