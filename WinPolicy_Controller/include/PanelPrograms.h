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

const long ID_PANELPROGRAMS_BTN_ADD = wxNewId();
const long ID_PANELPROGRAMS_BTN_DEL = wxNewId();
const long ID_PANELPROGRAMS_BTN_BROWSE = wxNewId();

class PanelPrograms : public wxScrolledWindow
{
public:
	PanelPrograms(wxWindow *parent);

	wxString GetPrograms(void);
	bool GetTweak(void);
	void SetPrograms(wxString Programs);
	void SetTweak(bool enabled);

	void AddFilename(wxString filename);

	DECLARE_EVENT_TABLE()

private:
	void AddProgram(wxCommandEvent &event);
	void DelProgram(wxCommandEvent &event);
	void Browse(wxCommandEvent &event);

	void CheckWinPolicyEXE(void);

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
