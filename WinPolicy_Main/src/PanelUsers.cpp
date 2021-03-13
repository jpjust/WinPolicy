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

#include "PanelUsers.h"
#include "AppBase.h"
#include "jswinreg.h"
#include <stdlib.h>
#include <wx/image.h>

BEGIN_EVENT_TABLE(PanelUsers, wxPanel)
	EVT_BUTTON(ID_PANELUSERS_BTN_ADD, PanelUsers::AddUser)
	EVT_BUTTON(ID_PANELUSERS_BTN_DEL, PanelUsers::DelUser)
	EVT_BUTTON(ID_PANELUSERS_BTN_CHPWD, PanelUsers::ChangePwd)
	EVT_BUTTON(ID_PANELUSERS_BTN_ADDTIME, PanelUsers::AddTime)
	EVT_BUTTON(ID_PANELUSERS_BTN_DELTIME, PanelUsers::DelTime)
	EVT_COMBOBOX(ID_PANELUSERS_BTN_COMBOUSERS, PanelUsers::ComboUsersChange)
	EVT_CHECKBOX(ID_PANELUSERS_CHK_ALLOWSHUTDOWN, PanelUsers::SaveChanges)
END_EVENT_TABLE()

// Class constructor
PanelUsers::PanelUsers(wxWindow *parent)
	:wxPanel(parent)
{
	wxStaticBox *statbox = new wxStaticBox(this, -1, "Usu�rios");
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

	bitmap = new wxStaticBitmap(this, -1, AppBase::LoadImage(wxT("users.png")));

	label_desc = new wxStaticText(this, -1, "Use esta janela para configurar os usu�rios que poder�o\n" \
											"desbloquear o computador para utiliz�-lo. Voc� tamb�m\n" \
											"pode especificar os hor�rios em que o computador poder�\n" \
											"ser utilizado por eles.");
	label_new = new wxStaticText(this, -1, "Novo usu�rio:");
	label_user = new wxStaticText(this, -1, "Usu�rio:");
	label_addtime = new wxStaticText(this, -1, "Adicionar hor�rio");
	label_days = new wxStaticText(this, -1, "Dias da semana:");
	label_times = new wxStaticText(this, -1, "Hor�rio (hh:mm):");
	label_allowed = new wxStaticText(this, -1, "Hor�rios permitidos:");
	label_from = new wxStaticText(this, -1, "De:");
	label_to = new wxStaticText(this, -1, "at�:");

	// Widgets that can get focus are created in the correct order
	text_username = new wxTextCtrl(this, -1);
	btn_add = new wxButton(this, ID_PANELUSERS_BTN_ADD, "Adicionar");
	combo_users = new wxComboBox(this, ID_PANELUSERS_BTN_COMBOUSERS, "", wxDefaultPosition, wxSize(40, -1), 0, NULL, wxCB_READONLY | wxCB_SORT);
	btn_changepwd = new wxButton(this, ID_PANELUSERS_BTN_CHPWD, "Alterar senha");
	btn_deluser = new wxButton(this, ID_PANELUSERS_BTN_DEL, "Excluir");
	chk_day[0] = new wxCheckBox(this, -1, "Dom");
	chk_day[1] = new wxCheckBox(this, -1, "Seg");
	chk_day[2] = new wxCheckBox(this, -1, "Ter");
	chk_day[3] = new wxCheckBox(this, -1, "Qua");
	chk_day[4] = new wxCheckBox(this, -1, "Qui");
	chk_day[5] = new wxCheckBox(this, -1, "Sex");
	chk_day[6] = new wxCheckBox(this, -1, "S�b");
	text_from_h = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxSize(30, -1));
	text_from_m = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxSize(30, -1));
	text_to_h = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxSize(30, -1));
	text_to_m = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxSize(30, -1));
	list_times = new wxListBox(this, -1, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL | wxLB_EXTENDED);
	btn_addtime = new wxButton(this, ID_PANELUSERS_BTN_ADDTIME, "Adicionar");
	btn_deltime = new wxButton(this, ID_PANELUSERS_BTN_DELTIME, "Remover");
	chk_allowshutdown = new wxCheckBox(this, ID_PANELUSERS_CHK_ALLOWSHUTDOWN, "Este usu�rio pode desligar o computador");

	// Add usernames to the combobox
	RefreshUsernames();

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
}

// RefreshUsernames method
void PanelUsers::RefreshUsernames(void)
{
	vector<wxString> users = UserMgr::GetUsersList();

	combo_users->Clear();
	for (unsigned int i = 0; i < users.size(); i++)
		combo_users->Append(users[i]);
}

// SaveProfile method
bool PanelUsers::SaveProfile(wxString filename)
{
	return UserMgr::SaveToProfile(filename);
}

// LoadProfile method
bool PanelUsers::LoadProfile(wxString filename)
{
	if (UserMgr::LoadFromProfile(filename) == false)
		return false;

	wxCommandEvent event2(wxEVT_COMMAND_COMBOBOX_SELECTED, ID_PANELUSERS_BTN_COMBOUSERS);

	RefreshUsernames();
	ComboUsersChange(event2);

	return true;
}

// Adiciona um usu�rio
void PanelUsers::AddUser(wxCommandEvent& event)
{
	wxString SubKey;

	if (text_username->GetValue().Len() == 0)
	{
		wxMessageBox("Por favor, digite um nome de usu�rio na caixa ao lado.", APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	// Verifica se o usu�rio j� existe
	if (UserMgr::FindUser(text_username->GetValue()) == true)
	{
		wxMessageBox("O usu�rio \"" + text_username->GetValue() + "\" j� existe. Escolha outro nome.", APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	// Cria o novo usu�rio
	if (UserMgr::AddUser(text_username->GetValue()) == false)
	{
		wxMessageBox("Ocorreu um erro ao tentar adicionar o usu�rio \"" + text_username->GetValue() + "\". Tente novamente.", APP_NAME, wxICON_EXCLAMATION);
		return;
	}
	else
	{
		combo_users->Append(text_username->GetValue());
		wxMessageBox("O usu�rio \"" + text_username->GetValue() + "\" foi adicionado!", APP_NAME, wxICON_INFORMATION);
	}
}

// DelUser method
void PanelUsers::DelUser(wxCommandEvent& event)
{
	wxString Key, SubKey;

	if (combo_users->GetStringSelection().Len() == 0)
	{
		wxMessageBox("Por favor, selecione um usu�rio na lista ao lado.", APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	if (wxMessageBox("Tem certeza que deseja excluir o usu�rio \"" + combo_users->GetStringSelection() + "\"?", APP_NAME, wxICON_QUESTION | wxYES_NO) == wxNO)
		return;

	// Remove user
	if (UserMgr::DelUser(combo_users->GetStringSelection()) == false)
		wxMessageBox("Ocorreu um erro ao tentar excluir o usu�rio \"" + combo_users->GetStringSelection() + "\". Tente novamente.", APP_NAME, wxICON_EXCLAMATION);
	else
		combo_users->Delete(combo_users->FindString(combo_users->GetStringSelection()));
}

// ChangePwd method
void PanelUsers::ChangePwd(wxCommandEvent& event)
{
	wxString Pwd[2];

	if (combo_users->GetStringSelection().Len() == 0)
	{
		wxMessageBox("Por favor, selecione um usu�rio na lista ao lado.", APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	Pwd[0] = wxGetPasswordFromUser("Digite uma senha para o usu�rio \"" + combo_users->GetStringSelection() + "\":", APP_NAME);
	Pwd[1] = wxGetPasswordFromUser("Repita a senha do usu�rio:", APP_NAME);

	if (Pwd[0] != Pwd[1])
	{
		wxMessageBox("As senhas digitadas n�o coincidem. Tente novamente.", APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	// Set password
	if (UserMgr::SetPassword(combo_users->GetStringSelection(), Pwd[0]) == false)
	{
		wxMessageBox("Ocorreu um erro ao tentar salvar a senha do usu�rio \"" + combo_users->GetStringSelection() + "\". Tente novamente.", APP_NAME, wxICON_ERROR);
		return;
	}
	else
		wxMessageBox("A senha foi salva com �xito.", APP_NAME, wxICON_INFORMATION);
}

// AddTime method
void PanelUsers::AddTime(wxCommandEvent& event)
{
	unsigned int i;
	unsigned int from_h, from_m, to_h, to_m, fromtime, totime;
	bool weekdays[7], wd_yes = false;
	wxCommandEvent event2(wxEVT_COMMAND_COMBOBOX_SELECTED, ID_PANELUSERS_BTN_COMBOUSERS);
	wxDateTime TimeFrom(0, 0), TimeUntil(0, 0);
	UserMgrTime NewTime;

	if (combo_users->GetStringSelection().Len() == 0)
	{
		wxMessageBox("Por favor, selecione um usu�rio na lista de usu�rios antes de prosseguir.", APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	from_h = strtol((char *)text_from_h->GetValue().mb_str(), NULL, 10);
	from_m = strtol((char *)text_from_m->GetValue().mb_str(), NULL, 10);
	to_h = strtol((char *)text_to_h->GetValue().mb_str(), NULL, 10);
	to_m = strtol((char *)text_to_m->GetValue().mb_str(), NULL, 10);

	if ((from_h > 23) || (from_m > 59) || (to_h > 23) || (to_m > 59))
	{
		wxMessageBox("Voc� digitou um hor�rio inv�lido. Verifique os n�meros e tente novamente.", APP_NAME, wxICON_EXCLAMATION);
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
		wxMessageBox("O hor�rio final n�o pode ser igual ou inferior ao hor�rio de in�cio.", APP_NAME, wxICON_EXCLAMATION);
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
		wxMessageBox("Voc� n�o selecionou um dia da semana para o usu�rio.", APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	NewTime = UserMgrTime(TimeFrom, TimeUntil, weekdays);

	if (UserMgr::AddTime(combo_users->GetStringSelection(), NewTime) == false)
	{
		wxMessageBox("N�o foi poss�vel gravar o hor�rio no registro. Certifique-se de que possui privil�gios de administrador no Windows e tente novamente.", APP_NAME, wxICON_ERROR);
		return;
	}

	ComboUsersChange(event2);
}

// DelTime method
void PanelUsers::DelTime(wxCommandEvent& event)
{
	if (combo_users->GetStringSelection().Len() == 0)
	{
		wxMessageBox("Por favor, selecione um usu�rio antes de prosseguir.", APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	wxArrayInt sels;
	int i, csels, item;
	UserMgrTime Time;

	csels = list_times->GetSelections((wxArrayInt&)sels);
	if (csels == 0)
	{
		wxMessageBox("Por favor, selecione pelo menos um hor�rio na lista antes de prosseguir.", APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	for (i = 0; i < csels; i++)
	{
		item = sels.Item(i);
		Time = time_table[item];
		if (UserMgr::DelTime(combo_users->GetStringSelection(), Time) == false)
		{
			wxMessageBox("N�o foi poss�vel remover o(s) hor�rio(s) selecionado(s). Certifique-se de que est� executando o WinPolicy com privil�gios de administrador e tente novamente.", APP_NAME, wxICON_ERROR);
			return;
		}
	}

	wxCommandEvent event2(wxEVT_COMMAND_COMBOBOX_SELECTED, ID_PANELUSERS_BTN_COMBOUSERS);
	ComboUsersChange(event2);
}

// ComboUsersChange method
void PanelUsers::ComboUsersChange(wxCommandEvent& event)
{
	if (combo_users->GetStringSelection().Len() < 1)
		return;

	int j;
	unsigned int i;

	wxString str_key, str_fulltime, str_time, str_item;

	list_times->Clear();
	LoadChanges(combo_users->GetStringSelection());
	time_table.clear();

	time_table = UserMgr::GetTimes(combo_users->GetStringSelection());

	for (i = 0; i < time_table.size(); i++)
	{
		str_item.Clear();
		for (j = 0; j < 7; j++)
			if (time_table[i].GetDaysOfWeek()[j] == true)
				str_item.Append(chk_day[j]->GetLabel() + ",");

		str_item.Append(wxString::Format("%2d:%2d-%2d:%2d", \
			time_table[i].GetTimeFrom().GetHour(), time_table[i].GetTimeFrom().GetMinute(), \
			time_table[i].GetTimeUntil().GetHour(), time_table[i].GetTimeUntil().GetMinute()));
		str_item.Replace(" ", "0");
		list_times->Append(str_item);
	}
}

// SaveChanges method
void PanelUsers::SaveChanges(wxCommandEvent &event)
{
	UserMgr::SetUserCanShutdown(combo_users->GetValue(), chk_allowshutdown->IsChecked());
}

// LoadChanges method
void PanelUsers::LoadChanges(wxString str_username)
{
	if (str_username.Len() == 0)
		return;

	chk_allowshutdown->SetValue(UserMgr::GetUserCanShutdown(str_username));
}
