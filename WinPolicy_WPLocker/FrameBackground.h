/* Background frame
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

#ifndef _FRAMEBACKGROUND_H
#define _FRAMEBACKGROUND_H

#include <wx/wx.h>
#include <wx/datetime.h>
#include "DialogWarning.h"

#define ID_FRAMEBACKGROUND_BTN_OK 0
#define ID_FRAMEBACKGROUND_BTN_RECOVERY 1
#define ID_FRAMEBACKGROUND_BTN_SHUTDOWN 2
#define ID_FRAMEBACKGROUND_PWDPROMPT 3
#define ID_FRAMEBACKGROUND_TIMER 4
#define ID_FRAMEBACKGROUND_TIMER_ENDTIME 5
#define ID_FRAMEBACKGROUND_LOGINPROMPT 6

class FrameBackground : public wxFrame
{
public:
	FrameBackground(HANDLE mutex);

	DECLARE_EVENT_TABLE()

private:
	void CheckPwd(wxCommandEvent &event);
	void RecoveryDisk(wxCommandEvent &event);
	void Shutdown(wxCommandEvent &event);
	void OnTimer(wxTimerEvent &event);
	void OnTimerEndTime(wxTimerEvent &event);
	void ChangeFocusPwd(wxCommandEvent &event);
	void OnPaint(wxPaintEvent &event);
	void OnTryClose(wxCloseEvent &event);

	HANDLE			proc_mutex;
	wxDateTime		end_time;
	bool			warn;

	wxStaticBox		*statbox;

	wxStaticText	*label_prompt;
	wxStaticText	*label_login;
	wxStaticText	*label_pwd;
	wxStaticText	*label_recovery;

	wxTextCtrl		*txt_login;
	wxTextCtrl		*txt_pwd;

	wxButton		*btn_ok;
	wxButton		*btn_recovery;
	wxButton		*btn_shutdown;

	wxTimer			*timer_winpos;
	wxTimer			*timer_endtime;
};

__cdecl void recdisk_thread(void *param);

#endif
