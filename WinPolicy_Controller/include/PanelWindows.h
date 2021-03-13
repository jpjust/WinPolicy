/* Windows panel
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

#ifndef _PANELWINDOWS_H
#define _PANELWINDOWS_H

#include <wx/wx.h>
#include <vector>

const long ID_PANELWINDOWS_BTN_CHECKALL = wxNewId();
const long ID_PANELWINDOWS_BTN_UNCHECKALL = wxNewId();

using namespace std;

class PanelWindows: public wxScrolledWindow
{
public:
	PanelWindows(wxWindow *parent);

	wxString GetChecked(void);
	wxString GetUnchecked(void);
	void SetChecked(wxString Checks, bool check);

	DECLARE_EVENT_TABLE()

private:
	void CheckAll(wxCommandEvent &event);
	void UnCheckAll(wxCommandEvent &event);

	vector<wxString> tags;

	wxString		description;
	wxStaticBitmap	*bitmap;

	wxStaticText	*label_title;
	wxCheckListBox	*list;

	wxButton		*btn_checkall;
	wxButton		*btn_checknone;
};

#endif
