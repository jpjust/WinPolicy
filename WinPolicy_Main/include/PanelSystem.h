/* System panel
 *
 * Copyright (c) 2004 - 2006 João Paulo Just Peixoto <just1982@gmail.com>.
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

#ifndef _PANELSYSTEM_H
#define _PANELSYSTEM_H

#include <wx/wx.h>
#include <wx/fileconf.h>
#include "TweaksSystem.h"

#define ID_PANELSYSTEM_BTN_CHECKALL 0
#define ID_PANELSYSTEM_BTN_UNCHECKALL 1

using namespace std;

class PanelSystem : public wxPanel
{
public:
	PanelSystem(wxWindow *parent);

	void GetTweaks(void);
	void ApplyTweaks(void);
	bool SaveTweaks(bool hku, wxArrayString Users = AppBase::GetLoadedUsers(), bool thisuser = true);
	bool SaveProfile(wxString filename);
	bool LoadProfile(wxString filename);
	wxCheckListBox *GetListBox(void);
	bool RestrictRegedit(void);

	DECLARE_EVENT_TABLE()

private:
	void CheckAll(wxCommandEvent &event);
	void UnCheckAll(wxCommandEvent &event);

	TweaksSystem	Tweaks;

	wxStaticBitmap	*bitmap;

	wxStaticText	*label_title;
	wxCheckListBox	*list;

	wxButton		*btn_checkall;
	wxButton		*btn_checknone;
};

#endif
