/* Class:       DialogAbout
 * Description: "About" window.
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

#ifndef DIALOGABOUT_H_
#define DIALOGABOUT_H_

#include <wx/wx.h>

const long ID_DIALOGABOUT_BTN_WEB = wxNewId();
const long ID_DIALOGABOUT_BTN_LICENSE = wxNewId();
const long ID_DIALOGABOUT_BTN_CLOSE = wxNewId();

class DialogAbout: public wxDialog
{
public:
	DialogAbout(void);

	DECLARE_EVENT_TABLE()

private:
	void ShowLicense(wxCommandEvent &event);
	void OpenWeb(wxCommandEvent &event);

	wxStaticText	*label_appname;
	wxStaticText	*label_copyright;
	wxStaticText	*label_legalnotice;
	wxStaticText	*label_mscopyright;

	wxTextCtrl		*txt_softwares;

	wxStaticBitmap	*bitmap;
	wxButton		*btn_web;
	wxButton		*btn_license;
};

#endif /*DIALOGABOUT_H_*/
