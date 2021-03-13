/* AutoLock Tasbar Icon class
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

#ifndef _TASKICONAUTOLOCK_H
#define _TASKICONAUTOLOCK_H

#include <wx/wx.h>
#include <wx/taskbar.h>
#include <wx/datetime.h>
#include <wx/numdlg.h>

#define ID_TASKBAR_MENU_RUNWP 0
#define ID_TASKBAR_MENU_EXIT 1
#define ID_TASKBAR_MENU_SETCLOCK 2
#define ID_TASKBAR_MENU_LOCKNOW 3
#define ID_TASKBAR_MENU_SHUTDOWN 6
#define ID_TASKBAR_MENU_LOGOFF 7
#define ID_TASKBAR_TIMER_ALOCK 4
#define ID_TASKBAR_TIMER_CLOCK 5

class TaskIconAutoLock : public wxTaskBarIcon
{
public:
	TaskIconAutoLock(wxIcon icon, wxString tooltip);

	virtual wxMenu *CreatePopupMenu();

	void RunWinPolicy(wxCommandEvent &event);
	void SetClock(wxCommandEvent &event);
	void LockNow(wxCommandEvent &event);
	void Shutdown(wxCommandEvent &event);
	void QuitProgram(wxCommandEvent &event);

	void OnTimer(wxTimerEvent &event);
	void OnTimerClock(wxTimerEvent &event);

	DECLARE_EVENT_TABLE()

private:
	void RefreshTooltip(void);

	wxDateTime	prev_time;
	long		clock_time;
	bool		warn;

	wxTimer		*timer_alock;
	wxTimer		*timer_clock;
};

#endif
