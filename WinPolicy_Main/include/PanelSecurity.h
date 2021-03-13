/* Security panel
 *
 * Copyright (c) 2004 - 2005 João Paulo Just Peixoto <just1982@gmail.com>.
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

#ifndef _PANELSECURITY_H
#define _PANELSECURITY_H

#include <wx/wx.h>

#define ID_PANELSECURITY_BTN_RESCUE 0
#define ID_PANELSECURITY_BTN_SAVEPWD 1
#define ID_PANELSECURITY_BTN_DELPWD 2
#define ID_PANELSECURITY_BTN_OK 3
#define ID_PANELSECURITY_PWDPROMPT 4

class PanelSecurity : public wxPanel
{
public:
	PanelSecurity(wxWindow *parent);
	void ShowPwdWidgets(bool show);

	void SavePasswd(wxCommandEvent& event);
	void DelPasswd(wxCommandEvent& event);
	void CheckPasswd(wxCommandEvent& event);

	void RescueDisk(wxCommandEvent& event);

	DECLARE_EVENT_TABLE();

private:
	wxString		description;
	wxStaticBitmap	*bitmap;

	wxStaticText	*label_title;
	wxStaticText	*label_pwd1;
	wxStaticText	*label_pwd2;
	wxStaticText	*label_pwd3;

	wxTextCtrl		*text_pwd1;
	wxTextCtrl		*text_pwd2;
	wxTextCtrl		*text_pwd3;

	wxButton		*btn_savepwd;
	wxButton		*btn_erasepwd;
	wxButton		*btn_recovery;
	wxButton		*btn_ok;
};

#endif
