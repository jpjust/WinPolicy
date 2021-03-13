/* Misc panel
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

#ifndef _PANELMISC_H
#define _PANELMISC_H

#include <wx/wx.h>

class PanelMisc : public wxScrolledWindow
{
public:
	PanelMisc(wxWindow *parent);

	wxString GetChecked(void);
	wxString GetUnChecked(void);
	wxString GetHiddenDrv(void);
	wxString GetDeniedDrv(void);
	void SetChecked(wxString Checks, bool check);
	void SetHiddenDrv(wxString Drives);
	void SetDeniedDrv(wxString Drives);

private:
	wxStaticBitmap	*bitmap;

	wxStaticText	*label_title;
	wxStaticText	*label_hidedisks;
	wxStaticText	*label_denydisks;
	wxStaticText	*label_win9xonly;
	wxStaticText	*label_win9xmeonly;

	wxCheckBox		*chk_startpage;
	wxCheckBox		*chk_dosprog;
	wxCheckBox		*chk_dosmode;
	wxCheckBox		*chk_f8menu;
	wxCheckBox		*chk_forcelogon;
	wxCheckBox		*chk_requirepwd;

	wxTextCtrl		*txt_hidedisks;
	wxTextCtrl		*txt_denydisks;
};

#endif
