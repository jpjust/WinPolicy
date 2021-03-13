/* Main WinPolicy frame
 *
 * Copyright (c) 2004 - 2007 João Paulo Just Peixoto <just1982@gmail.com>.
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

#ifndef _FRAMEWINPOLICY_H
#define _FRAMEWINPOLICY_H

#include <wx/wx.h>
#include <wx/listctrl.h>

#include "PanelSecurity.h"
#include "PanelWinUsers.h"
#include "PanelUsers.h"
#include "PanelProfiles.h"
#include "PanelSystem.h"
#include "PanelWindows.h"
#include "PanelNetwork.h"
#include "PanelPrograms.h"
#include "PanelLocking.h"
#include "PanelMisc.h"
#include "PanelAbout.h"
#include "NetReg.h"

#define ID_FRAMEWINPOLICY_BTN_OK 0
#define ID_FRAMEWINPOLICY_BTN_CANCEL 1
#define ID_FRAMEWINPOLICY_BTN_APPLY 2
#define ID_FRAMEWINPOLICY_MENU 3

class FrameWinPolicy: public wxFrame
{
public:
	FrameWinPolicy(void);

	void Quit(wxCommandEvent& event);
	void MenuClick(wxListEvent& event);
	void SaveSettings(wxCommandEvent& event);
	void OnLoadProfile(wxCommandEvent& event);
	void OnSaveProfile(wxCommandEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnPasswdOK(wxCommandEvent& event);
	void OnClose(wxCloseEvent &event);
	void OnCheckboxClick(wxCommandEvent &event);

	void HideAllPanels(void);
	void LoadProfile(wxString filename, bool loadusers, bool allusers, bool silent, bool savetweaks);

	DECLARE_EVENT_TABLE();

private:
	NetReg			*netreg;

	bool			profile_loadusers;
	bool			profile_allusers;
	bool			profile_silent;
	bool			profile_savetweaks;

	wxBoxSizer		*sizer_v1;
	wxBoxSizer		*sizer_v2;
	wxBoxSizer		*sizer_v3;
	wxBoxSizer		*sizer_h1;
	wxBoxSizer		*sizer_h2;

	wxListCtrl		*list;
	wxImageList		*img_menu;

	wxButton		*btnOK;
	wxButton		*btnCancel;
	wxButton		*btnApply;

	PanelSecurity	*panel_security;
	PanelWinUsers	*panel_winusers;
	PanelUsers		*panel_users;
	PanelProfiles	*panel_profiles;
	PanelSystem		*panel_system;
	PanelWindows	*panel_windows;
	PanelNetwork	*panel_network;
	PanelPrograms	*panel_programs;
	PanelLocking	*panel_locking;
	PanelMisc		*panel_misc;
	PanelAbout		*panel_about;
};

#endif
