/* Locking panel
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

#ifndef _PANELLOCKING_H
#define _PANELLOCKING_H

#include <wx/wx.h>

const long ID_PANELLOCKING_CHK_AUTOLOCK = wxNewId();
const long ID_PANELLOCKING_BTN_ADDTIME = wxNewId();
const long ID_PANELLOCKING_BTN_DELTIME = wxNewId();

class PanelLocking : public wxScrolledWindow
{
public:
	PanelLocking(wxWindow *parent);

	wxString GetChecked(void);
	wxString GetUnChecked(void);
	wxString GetTimes(void);
	void SetChecked(wxString Checks, bool check);
	void SetTimes(wxString Times);

	DECLARE_EVENT_TABLE()

private:
	void AddTimeMin(int t_minutes);
	void AddTime(wxCommandEvent &event);
	void DelTime(wxCommandEvent &event);

	bool			GettingTweak;

	wxWindow		*my_parent;

	wxString		description;
	wxStaticBitmap	*bitmap;

	wxStaticText	*label_title;

	wxButton		*btn_addtime;
	wxButton		*btn_deltime;

	wxTextCtrl		*txt_time_h;
	wxTextCtrl		*txt_time_m;
	wxListBox		*list_times;

	wxCheckBox		*chk_enablealock;
	wxCheckBox		*chk_askpwdtolock;
	wxCheckBox		*chk_askpwdtoshutdown;
	wxCheckBox		*chk_lockkeys;
	wxCheckBox		*chk_lockonstartup;
	wxCheckBox		*chk_lockonlogon;
	wxCheckBox		*chk_lockontime;
};

#endif
