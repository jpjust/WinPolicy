/* About panel
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

#ifndef _PANELABOUT_H
#define _PANELABOUT_H

#include <wx/wx.h>

#define ID_PANELABOUT_BTN_LICENSE 1

class PanelAbout : public wxPanel
{
public:
	PanelAbout(wxWindow *parent);

	DECLARE_EVENT_TABLE()

private:
	void ShowLicense(wxCommandEvent &event);

	wxStaticText	*label_appname;
	wxStaticText	*label_copyright;
	wxStaticText	*label_legalnotice;
	wxStaticText	*label_url;
	wxStaticText	*label_mscopyright;

	wxTextCtrl		*txt_softwares;

	wxStaticBitmap	*bitmap;
	wxButton		*btn_license;
};

#endif
