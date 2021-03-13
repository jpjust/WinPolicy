/* Class:       PanelWinUsers
 * Description: Windows users setup panel.
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

#ifndef PANELWINUSERS_H_
#define PANELWINUSERS_H_

#include <wx/wx.h>

class PanelWinUsers: public wxPanel
{
public:
	PanelWinUsers(wxWindow *parent);

	bool GetAllUsers(void);
	bool GetThisUser(void);
	wxArrayString GetUsers(void);

	void SetAllUsers(bool value);

private:
	wxStaticBitmap	*bitmap;

	wxStaticText	*label_desc;

	wxListBox		*list_users;

	wxCheckBox		*chk_list;
	wxCheckBox		*chk_allusers;
	wxCheckBox		*chk_thisuser;

#ifndef _WINPOLICY_ENTERPRISE_
	wxStaticText	*label_notice;
#endif
};

#endif /*PANELWINUSERS_H_*/
