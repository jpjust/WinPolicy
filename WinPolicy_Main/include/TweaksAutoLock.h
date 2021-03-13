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

#ifndef _TWEAKSAUTOLOCK_H
#define _TWEAKSAUTOLOCK_H

#include "Tweaks.h"
#include "jswinreg.h"
#include <vector>
#include <wx/datetime.h>

using namespace std;

class TweaksAutoLock : public Tweaks
{
public:
	TweaksAutoLock(bool loadall = true);
	void LoadTweaks(void);
	bool SaveTweaks(void);

	bool LoadFromProfile(wxString filename);
	bool SaveToProfile(wxString filename);

	bool RefreshService(void);
	int RefreshLockingInit(void);

	bool GetTALockEnabled(void);
	bool GetTPwdToLock(void);
	bool GetTPwdToShutdown(void);
	bool GetTLockKeys(void);
	bool GetTLockOnStartup(void);
	bool GetTLockOnLogon(void);
	bool GetTLockOnTime(void);
	vector<wxDateTime> GetTTimes(void);
		wxDateTime GetTime(int index);
		int GetTimeCount(void);

	void SetTALockEnabled(bool value);
	void SetTPwdToLock(bool value);
	void SetTPwdToShutdown(bool value);
	void SetTLockKeys(bool value);
	void SetTLockOnStartup(bool value);
	void SetTLockOnLogon(bool value);
	void SetTLockOnTime(bool value);
	void SetTTimes(vector<wxDateTime> new_vector);
		void SetTime(int index, wxDateTime new_time);
		void AddTime(wxDateTime new_time);
		void ClearTimes(void);

private:
	bool t_alockenabled;
	bool t_pwdtolock;
	bool t_pwdtoshutdown;
	bool t_lockkeys;
	bool t_lockonstartup;
	bool t_lockonlogon;
	bool t_lockontime;
	vector<wxDateTime> t_times;
};

#endif
