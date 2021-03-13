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
#include <wx/datetime.h>
#include <vector>
#include "UserMgr.h"

const long ID_PANELUSERS_BTN_ADD = wxNewId();
const long ID_PANELUSERS_BTN_CHPWD = wxNewId();
const long ID_PANELUSERS_BTN_DEL = wxNewId();
const long ID_PANELUSERS_BTN_ADDTIME = wxNewId();
const long ID_PANELUSERS_BTN_DELTIME = wxNewId();
const long ID_PANELUSERS_BTN_COMBOUSERS = wxNewId();
const long ID_PANELUSERS_CHK_ALLOWSHUTDOWN = wxNewId();

using namespace std;

// Panel class
class PanelUsers: public wxScrolledWindow
{
public:
	PanelUsers(wxWindow *parent);

	vector<UserMgr> GetUsers(void);

	void SaveChanges(wxCommandEvent &event);
	void LoadChanges(void);

	void ClearUsers(void);
	void AddUser(wxString Username);
	void ChangePwd(wxString Username, wxString Code);
	void AddTime(wxString Username, wxString Time);
	void SetPerm(wxString Username, bool perm);

	DECLARE_EVENT_TABLE();

private:
	void RefreshUsernames(void);
	void OnAddUser(wxCommandEvent& event);
	void OnDelUser(wxCommandEvent& event);
	void OnChangePwd(wxCommandEvent& event);
	void OnAddTime(wxCommandEvent& event);
	void OnDelTime(wxCommandEvent& event);
	void OnComboUsersChange(wxCommandEvent& event);

	vector<UserMgr>	users;
	vector<UserMgrTime> time_table;

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
