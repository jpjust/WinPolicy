/* PanelUsers class
 *
 * Copyright (c) 2004-2006 João Paulo Just Peixoto <just1982@gmail.com>.
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

#include "PanelUsers.h"
#include "ControllerBase.h"
#include <stdlib.h>
#include <wx/image.h>

BEGIN_EVENT_TABLE(PanelUsers, wxPanel)
	EVT_BUTTON(ID_PANELUSERS_BTN_ADD, PanelUsers::OnAddUser)
	EVT_BUTTON(ID_PANELUSERS_BTN_DEL, PanelUsers::OnDelUser)
	EVT_BUTTON(ID_PANELUSERS_BTN_CHPWD, PanelUsers::OnChangePwd)
	EVT_BUTTON(ID_PANELUSERS_BTN_ADDTIME, PanelUsers::OnAddTime)
	EVT_BUTTON(ID_PANELUSERS_BTN_DELTIME, PanelUsers::OnDelTime)
	EVT_COMBOBOX(ID_PANELUSERS_BTN_COMBOUSERS, PanelUsers::OnComboUsersChange)
	EVT_CHECKBOX(ID_PANELUSERS_CHK_ALLOWSHUTDOWN, PanelUsers::SaveChanges)
END_EVENT_TABLE()

// Class constructor
PanelUsers::PanelUsers(wxWindow *parent)
	:wxScrolledWindow(parent)
{
	wxStaticBox *statbox = new wxStaticBox(this, -1, _("Usu�rios"));
	wxStaticBoxSizer *sizer_v1 = new wxStaticBoxSizer(statbox, wxVERTICAL);
	wxBoxSizer *sizer_h1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h4 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h5 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h6 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h7 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h8 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h9 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_v2 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *sizer_v3 = new wxBoxSizer(wxVERTICAL);

	wxImage png(ControllerBase::GetAppDir() + wxT("/images/users.png"), wxBITMAP_TYPE_PNG);
#ifdef __WXMSW__
	int osmaj, osmin, osmm;
	wxGetOsVersion(&osmaj, &osmin);
	osmm = osmaj * 100 + osmin;
	if (osmm < 501)
		png.ConvertAlphaToMask();
#endif
	bitmap = new wxStaticBitmap(this, -1, wxBitmap(png));

	label_desc = new wxStaticText(this, -1, _("Use esta janela para configurar os usu�rios que poder�o\n" \
											"desbloquear o computador para utiliz�-lo. Voc� tamb�m\n" \
											"pode especificar os hor�rios em que o computador poder�\n" \
											"ser utilizado por eles."));
	label_new = new wxStaticText(this, -1, _("Novo usu�rio:"));
	label_user = new wxStaticText(this, -1, _("Usu�rio:"));
	label_addtime = new wxStaticText(this, -1, _("Adicionar hor�rio"));
	label_days = new wxStaticText(this, -1, _("Dias da semana:"));
	label_times = new wxStaticText(this, -1, _("Hor�rio (hh:mm):"));
	label_allowed = new wxStaticText(this, -1, _("Hor�rios permitidos:"));
	label_from = new wxStaticText(this, -1, _("De:"));
	label_to = new wxStaticText(this, -1, _("at�:"));

	// Widgets that can get focus are created in the correct order
	text_username = new wxTextCtrl(this, -1);
	btn_add = new wxButton(this, ID_PANELUSERS_BTN_ADD, _("Adicionar"));
	combo_users = new wxComboBox(this, ID_PANELUSERS_BTN_COMBOUSERS, wxT(""), wxDefaultPosition, wxSize(40, -1), 0, NULL, wxCB_READONLY | wxCB_SORT);
	btn_changepwd = new wxButton(this, ID_PANELUSERS_BTN_CHPWD, _("Alterar senha"));
	btn_deluser = new wxButton(this, ID_PANELUSERS_BTN_DEL, _("Excluir"));
	chk_day[0] = new wxCheckBox(this, -1, _("Dom"));
	chk_day[1] = new wxCheckBox(this, -1, _("Seg"));
	chk_day[2] = new wxCheckBox(this, -1, _("Ter"));
	chk_day[3] = new wxCheckBox(this, -1, _("Qua"));
	chk_day[4] = new wxCheckBox(this, -1, _("Qui"));
	chk_day[5] = new wxCheckBox(this, -1, _("Sex"));
	chk_day[6] = new wxCheckBox(this, -1, _("S�b"));
	text_from_h = new wxTextCtrl(this, -1, wxT(""), wxDefaultPosition, wxSize(30, -1));
	text_from_m = new wxTextCtrl(this, -1, wxT(""), wxDefaultPosition, wxSize(30, -1));
	text_to_h = new wxTextCtrl(this, -1, wxT(""), wxDefaultPosition, wxSize(30, -1));
	text_to_m = new wxTextCtrl(this, -1, wxT(""), wxDefaultPosition, wxSize(30, -1));
	list_times = new wxListBox(this, -1, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL | wxLB_EXTENDED);
	btn_addtime = new wxButton(this, ID_PANELUSERS_BTN_ADDTIME, _("Adicionar"));
	btn_deltime = new wxButton(this, ID_PANELUSERS_BTN_DELTIME, _("Remover"));
	chk_allowshutdown = new wxCheckBox(this, ID_PANELUSERS_CHK_ALLOWSHUTDOWN, _("Este usu�rio pode desligar o computador"));

	// Layout development
	sizer_h1->Add(bitmap, 0, wxALL, 5);
	sizer_h1->Add(label_desc, 0, wxALL, 5);

	sizer_h2->Add(label_new, 0, wxALL, 5);
	sizer_h2->Add(text_username, 1, wxALL, 5);
	sizer_h2->Add(btn_add, 0, wxALL, 5);

	sizer_h3->Add(label_user, 0, wxALL, 5);
	sizer_h3->Add(combo_users, 1, wxALL, 5);
	sizer_h3->Add(btn_changepwd, 0, wxALL, 5);
	sizer_h3->Add(btn_deluser, 0, wxALL, 5);

	sizer_h5->Add(chk_day[0], 0, wxALL, 5);
	sizer_h5->Add(chk_day[1], 0, wxALL, 5);
	sizer_h5->Add(chk_day[2], 0, wxALL, 5);
	sizer_h6->Add(chk_day[3], 0, wxALL, 5);
	sizer_h6->Add(chk_day[4], 0, wxALL, 5);
	sizer_h6->Add(chk_day[5], 0, wxALL, 5);
	sizer_h7->Add(chk_day[6], 0, wxALL, 5);

	sizer_h8->Add(label_from, 0, wxALL, 5);
	sizer_h8->Add(text_from_h, 0, wxALL, 0);
	sizer_h8->Add(text_from_m, 0, wxALL, 0);
	sizer_h8->Add(label_to, 0, wxALL, 5);
	sizer_h8->Add(text_to_h, 0, wxALL, 0);
	sizer_h8->Add(text_to_m, 0, wxALL, 0);

	sizer_v2->Add(label_addtime, 0, wxALL, 5);
	sizer_v2->Add(label_days, 0, wxALL, 5);
	sizer_v2->Add(sizer_h5, 0, wxALL, 0);
	sizer_v2->Add(sizer_h6, 0, wxALL, 0);
	sizer_v2->Add(sizer_h7, 0, wxALL, 0);
	sizer_v2->Add(label_times, 0, wxALL, 5);
	sizer_v2->Add(sizer_h8, 0, wxALL, 0);

	sizer_h9->Add(0, 0, 1, wxALL, 5);
	sizer_h9->Add(btn_addtime, 0, wxALL, 5);
	sizer_h9->Add(btn_deltime, 0, wxALL, 5);

	sizer_v3->Add(label_allowed, 0, wxALL, 5);
	sizer_v3->Add(list_times, 1, wxALL | wxEXPAND, 5);
	sizer_v3->Add(sizer_h9, 0, wxALL | wxEXPAND, 0);

	sizer_h4->Add(sizer_v2, 0, wxALL, 0);
	sizer_h4->Add(sizer_v3, 1, wxALL | wxEXPAND, 0);

	sizer_v1->Add(sizer_h1, 0, wxBOTTOM, 5);
	sizer_v1->Add(sizer_h2, 0, wxTOP | wxEXPAND, 5);
	sizer_v1->Add(sizer_h3, 0, wxALL | wxEXPAND, 0);
	sizer_v1->Add(sizer_h4, 0, wxALL | wxEXPAND, 0);
	sizer_v1->Add(chk_allowshutdown, 0, wxALL, 5);

	SetSizer(sizer_v1);
	SetScrollRate(5, 5);
}

// Get users list
vector<UserMgr> PanelUsers::GetUsers(void)
{
	return users;
}

// Refresh combobox
void PanelUsers::RefreshUsernames(void)
{
	combo_users->Clear();
	for (unsigned int i = 0; i < users.size(); i++)
		combo_users->Append(users[i].GetUsername());
}

// SaveChanges method
void PanelUsers::SaveChanges(wxCommandEvent &event)
{
	for (unsigned int i = 0; i < users.size(); i++)
	{
		if (users[i].GetUsername() == combo_users->GetStringSelection())
			users[i].SetCanShutdown(chk_allowshutdown->IsChecked());
	}
}

// LoadChanges method
void PanelUsers::LoadChanges(void)
{
	for (unsigned int i = 0; i < users.size(); i++)
	{
		if (users[i].GetUsername() == combo_users->GetStringSelection())
		{
			chk_allowshutdown->SetValue(users[i].GetCanShutdown());
		}
	}
}

// Limpa a lista de usu�rios
void PanelUsers::ClearUsers(void)
{
	users.clear();
	time_table.clear();
	combo_users->Clear();
}

// Adiciona um novo usu�rio
void PanelUsers::AddUser(wxString Username)
{
	if (Username.Len() == 0)
		return;

	// Verifica se o usu�rio j� existe
	for (unsigned int i = 0; i < users.size(); i++)
		if (users[i].GetUsername().Lower() == Username.Lower())
			return;

	// Cria o novo usu�rio
	UserMgr User(Username);
	users.push_back(User);
	users.resize(users.size());

	RefreshUsernames();
}

// Muda a senha criptografada do usu�rio
void PanelUsers::ChangePwd(wxString Username, wxString Code)
{
	// Define a senha
	for (unsigned int i = 0; i < users.size(); i++)
	{
		if (users[i].GetUsername() == Username)
			users[i].SetPasswordCode(Code);
	}
}

// Adiciona hor�rio para um usu�rio
void PanelUsers::AddTime(wxString Username, wxString Time)
{
	UserMgrTime NewTime = UserMgrTime((char *)Time.c_str());

	for (unsigned int i = 0; i < users.size(); i++)
	{
		if (users[i].GetUsername() == Username)
			users[i].AddTime(NewTime);
	}
}

// Configura permiss�o de desligamento do computador
void PanelUsers::SetPerm(wxString Username, bool perm)
{
	for (unsigned int i = 0; i < users.size(); i++)
	{
		if (users[i].GetUsername() == Username)
			users[i].SetCanShutdown(perm);
	}
}

// AddUser method
void PanelUsers::OnAddUser(wxCommandEvent& event)
{
	if (text_username->GetValue().Len() == 0)
	{
		wxMessageBox(_("Por favor, digite um nome de usu�rio na caixa ao lado."), APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	// Check if the user already exists
	for (unsigned int i = 0; i < users.size(); i++)
	{
		if (users[i].GetUsername().Lower() == text_username->GetValue().Lower())
		{
			wxMessageBox(_("Este usu�rio j� existe."), APP_NAME, wxICON_EXCLAMATION);
			return;
		}
	}

	// Create the new user
	UserMgr User(text_username->GetValue());
	users.push_back(User);
	users.resize(users.size());

	RefreshUsernames();
}

// DelUser method
void PanelUsers::OnDelUser(wxCommandEvent& event)
{
	if (combo_users->GetStringSelection().Len() == 0)
	{
		wxMessageBox(_("Por favor, selecione um usu�rio na lista ao lado."), APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	if (wxMessageBox(_("Tem certeza que deseja excluir o usu�rio \"") + combo_users->GetStringSelection() + _("\"?"), APP_NAME, wxICON_QUESTION | wxYES_NO) == wxNO)
		return;

	// Remove user
	unsigned int i = 0;

	while (i < users.size())
	{
		if (users[i].GetUsername() == combo_users->GetStringSelection())
			users.erase(users.begin() + i);
		else
			i++;
	}
	users.resize(users.size());

	RefreshUsernames();
}

// ChangePwd method
void PanelUsers::OnChangePwd(wxCommandEvent& event)
{
	wxString Pwd[2];

	if (combo_users->GetStringSelection().Len() == 0)
	{
		wxMessageBox(_("Por favor, selecione um usu�rio na lista ao lado."), APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	Pwd[0] = wxGetPasswordFromUser(_("Digite uma senha para o usu�rio \"") + combo_users->GetStringSelection() + _("\":"), APP_NAME);
	Pwd[1] = wxGetPasswordFromUser(_("Repita a senha do usu�rio:"), APP_NAME);

	if (Pwd[0] != Pwd[1])
	{
		wxMessageBox(_("As senhas digitadas n�o coincidem. Tente novamente."), APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	// Set password
	for (unsigned int i = 0; i < users.size(); i++)
	{
		if (users[i].GetUsername() == combo_users->GetStringSelection())
			users[i].SetPassword(Pwd[0]);
	}
}

// AddTime method
void PanelUsers::OnAddTime(wxCommandEvent& event)
{
	unsigned int i;
	unsigned int from_h, from_m, to_h, to_m, fromtime, totime;
	bool weekdays[7], wd_yes = false;
	wxCommandEvent event2(wxEVT_COMMAND_COMBOBOX_SELECTED, ID_PANELUSERS_BTN_COMBOUSERS);
	wxDateTime TimeFrom(0, 0), TimeUntil(0, 0);
	UserMgrTime NewTime;

	if (combo_users->GetStringSelection().Len() == 0)
	{
		wxMessageBox(_("Por favor, selecione um usu�rio na lista de usu�rios antes de prosseguir."), APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	from_h = strtol((char *)text_from_h->GetValue().c_str(), NULL, 10);
	from_m = strtol((char *)text_from_m->GetValue().c_str(), NULL, 10);
	to_h = strtol((char *)text_to_h->GetValue().c_str(), NULL, 10);
	to_m = strtol((char *)text_to_m->GetValue().c_str(), NULL, 10);

	if ((from_h > 23) || (from_m > 59) || (to_h > 23) || (to_m > 59))
	{
		wxMessageBox(_("Voc� digitou um hor�rio inv�lido. Verifique os n�meros e tente novamente."), APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	fromtime = 60 * from_h + from_m;
	totime = 60 * to_h + to_m;

	TimeFrom.SetHour(from_h);
	TimeFrom.SetMinute(from_m);
	TimeUntil.SetHour(to_h);
	TimeUntil.SetMinute(to_m);

	if (totime <= fromtime)
	{
		wxMessageBox(_("O hor�rio final n�o pode ser igual ou inferior ao hor�rio de in�cio."), APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	for (i = 0; i < 7; i++)
	{
		if (chk_day[i]->IsChecked() == true)
		{
			weekdays[i] = true;
			wd_yes = true;
		}
		else
			weekdays[i] = false;
	}

	if (wd_yes == false)
	{
		wxMessageBox(_("Voc� n�o selecionou um dia da semana para o usu�rio."), APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	NewTime = UserMgrTime(TimeFrom, TimeUntil, weekdays);

	for (unsigned int i = 0; i < users.size(); i++)
	{
		if (users[i].GetUsername() == combo_users->GetStringSelection())
			users[i].AddTime(NewTime);
	}

	OnComboUsersChange(event2);
}

// DelTime method
void PanelUsers::OnDelTime(wxCommandEvent& event)
{
	if (combo_users->GetStringSelection().Len() == 0)
	{
		wxMessageBox(_("Por favor, selecione um usu�rio antes de prosseguir."), APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	wxArrayInt sels;
	int j, csels, item;
	unsigned int i;
	UserMgrTime Time;

	csels = list_times->GetSelections((wxArrayInt&)sels);
	if (csels == 0)
	{
		wxMessageBox(_("Por favor, selecione pelo menos um hor�rio na lista antes de prosseguir."), APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	for (i = 0; i < users.size(); i++)
	{
		if (users[i].GetUsername() == combo_users->GetStringSelection())
			break;
	}

	for (j = 0; j < csels; j++)
	{
		item = sels.Item(j);
		Time = time_table[item];
		users[i].DelTime(Time);
	}

	wxCommandEvent event2(wxEVT_COMMAND_COMBOBOX_SELECTED, ID_PANELUSERS_BTN_COMBOUSERS);
	OnComboUsersChange(event2);
}

// ComboUsersChange method
void PanelUsers::OnComboUsersChange(wxCommandEvent& event)
{
	if (combo_users->GetStringSelection().Len() < 1)
		return;

	int j;
	unsigned int i, u;

	for (u = 0; u < users.size(); u++)
	{
		if (users[u].GetUsername() == combo_users->GetStringSelection())
			break;
	}

	wxString str_key, str_fulltime, str_time, str_item;

	list_times->Clear();
	time_table.clear();
	time_table = users[u].GetTimes();

	LoadChanges();

	for (i = 0; i < time_table.size(); i++)
	{
		str_item.Clear();
		for (j = 0; j < 7; j++)
			if (time_table[i].GetDaysOfWeek()[j] == true)
				str_item.Append(chk_day[j]->GetLabel() + wxT(","));

		str_item.Append(wxString::Format(wxT("%2d:%2d-%2d:%2d"), \
			time_table[i].GetTimeFrom().GetHour(), time_table[i].GetTimeFrom().GetMinute(), \
			time_table[i].GetTimeUntil().GetHour(), time_table[i].GetTimeUntil().GetMinute()));
		str_item.Replace(wxT(" "), wxT("0"));
		list_times->Append(str_item);
	}
}
