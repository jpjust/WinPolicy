/* Users panel
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

#ifndef _PANELUSERS_H
#define _PANELUSERS_H

#include <wx/wx.h>
#include <wx/fileconf.h>
#include <vector>
#include "UserMgr.h"

#define ID_PANELUSERS_BTN_ADD 0
#define ID_PANELUSERS_BTN_CHPWD 1
#define ID_PANELUSERS_BTN_DEL 2
#define ID_PANELUSERS_BTN_ADDTIME 3
#define ID_PANELUSERS_BTN_DELTIME 4
#define ID_PANELUSERS_BTN_COMBOUSERS 5
#define ID_PANELUSERS_CHK_ALLOWSHUTDOWN 6

using namespace std;

class PanelUsers : public wxPanel
{
public:
	PanelUsers(wxWindow *parent);
	bool SaveProfile(wxString filename);
	bool LoadProfile(wxString filename);

	DECLARE_EVENT_TABLE();

private:
	void RefreshUsernames(void);
	void AddUser(wxCommandEvent& event);
	void DelUser(wxCommandEvent& event);
	void ChangePwd(wxCommandEvent& event);
	void AddTime(wxCommandEvent& event);
	void DelTime(wxCommandEvent& event);
	void ComboUsersChange(wxCommandEvent& event);
	void SaveChanges(wxCommandEvent &event);
	void LoadChanges(wxString str_username);

	vector<UserMgrTime>	time_table;

	wxStaticBitmap	*bitmap;

	wxStaticText	*label_desc;
	wxStaticText	*label_new;
	wxStaticText	*label_user;
	wxStaticText	*label_addtime;
	wxStaticText	*label_days;
	wxStaticText	*label_times;
	wxStaticText	*label_allowed;
	wxStaticText	*label_from;
	wxStaticText	*label_to;

	wxButton		*btn_add;
	wxButton		*btn_changepwd;
	wxButton		*btn_deluser;
	wxButton		*btn_addtime;
	wxButton		*btn_deltime;

	wxTextCtrl		*text_username;
	wxTextCtrl		*text_from_h;
	wxTextCtrl		*text_from_m;
	wxTextCtrl		*text_to_h;
	wxTextCtrl		*text_to_m;

	wxCheckBox		*chk_day[7];
	wxCheckBox		*chk_allowshutdown;

	wxComboBox		*combo_users;
	wxListBox		*list_times;
};

#endif
