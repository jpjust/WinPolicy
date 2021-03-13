/* Programs panel
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

#ifndef _PANELPROGRAMS_H
#define _PANELPROGRAMS_H

#include <wx/wx.h>
#include <wx/fileconf.h>
#include "TweaksPrograms.h"

#define ID_PANELPROGRAMS_BTN_ADD 0
#define ID_PANELPROGRAMS_BTN_DEL 1
#define ID_PANELPROGRAMS_BTN_BROWSE 2

class PanelPrograms : public wxPanel
{
public:
	PanelPrograms(wxWindow *parent);

	void GetTweaks(void);
	void ApplyTweaks(void);
	bool SaveTweaks(bool hku, wxArrayString Users = AppBase::GetLoadedUsers(), bool thisuser = true);
	bool SaveProfile(wxString filename);
	bool LoadProfile(wxString filename);

	void AddFilename(wxString filename);

	DECLARE_EVENT_TABLE()

private:
	void AddProgram(wxCommandEvent &event);
	void DelProgram(wxCommandEvent &event);
	void Browse(wxCommandEvent &event);

	void CheckWinPolicyEXE(void);

	TweaksPrograms	Tweaks;

	wxArrayString	str_mandatory;

	wxString		dialog_path;
	wxStaticBitmap	*bitmap;

	wxStaticText	*label_title;
	wxCheckBox		*chk_blockall;

	wxButton		*btn_addprogram;
	wxButton		*btn_remprogram;
	wxButton		*btn_addfromlist;

	wxTextCtrl		*txt_program;
	wxListBox		*list_blocked;
};

#endif
