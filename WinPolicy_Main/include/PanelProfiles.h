/* Profiles panel
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

#ifndef _PANELPROFILES_H
#define _PANELPROFILES_H

#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/filedlg.h>

#define ID_PANELPROFILE_BTN_VERSION 0
#define ID_PANELPROFILE_BTN_LOAD 1
#define ID_PANELPROFILE_BTN_SAVE 2
#define PROFILE_NOACTION 0
#define PROFILE_NEEDLOAD 1
#define PROFILE_NEEDSAVE 2

class PanelProfiles : public wxPanel
{
public:
	PanelProfiles(wxWindow *parent);
	void LoadProfileClick(wxCommandEvent& event);
	void SaveProfileClick(wxCommandEvent& event);
	void OSDialog(wxCommandEvent& event);

	wxString GetProfileFilename(void);
	void SetProfileFilename(wxString filename);

	DECLARE_EVENT_TABLE();

private:
	wxString		dialog_path;
	wxString		profile_filename;

	wxStaticText	*label_title;
	wxStaticText	*label_verdesc;
	wxStaticText	*label_verdesc2;
	wxStaticText	*label_winver;
	wxStaticText	*label_allver;

	wxStaticBitmap	*bitmap;
	wxStaticLine	*line;

	wxButton		*btn_load;
	wxButton		*btn_save;
	wxButton		*btn_version;
};

#endif
