/* Operating System selection dialog
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

#ifndef _DIALOGOS_H
#define _DIALOGOS_H

#include <wx/wx.h>

#define ID_DIALOGOS_BTN_OK 0

class DialogOS : public wxDialog
{
public:
	DialogOS(void);
	void SaveSettings(wxCommandEvent& event);

	DECLARE_EVENT_TABLE();

private:
	wxStaticText	*label_title;
	wxRadioBox		*radiobox;
	wxCheckBox		*chk_all;
	wxButton		*btn_ok;
};

#endif
