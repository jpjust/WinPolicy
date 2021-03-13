/* Class:       PanelWinUsers
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

#include "PanelWinUsers.h"
#include "AppBase.h"
#include <wx/utils.h>

// Construtor
PanelWinUsers::PanelWinUsers(wxWindow *parent)
	:wxPanel(parent)
{
	wxStaticBox *statbox = new wxStaticBox(this, wxID_ANY, wxT("Usu�rios do Sistema"));
	wxStaticBoxSizer *sizer_v1 = new wxStaticBoxSizer(statbox, wxVERTICAL);
	wxBoxSizer *sizer_h1 = new wxBoxSizer(wxHORIZONTAL);

	bitmap = new wxStaticBitmap(this, wxID_ANY, AppBase::LoadImage(wxT("users_sys.png")));

	label_desc = new wxStaticText(this, wxID_ANY, wxT("Use esta janela para configurar os usu�rios do sistema que\n" \
													"ir�o receber as restri��es aplicadas. Por padr�o, apenas o\n" \
													"o usu�rio logado no momento recebe as restri��es."));

#ifndef _WINPOLICY_ENTERPRISE_
	label_notice = new wxStaticText(this, wxID_ANY, wxT("Fun��es exclusivas do WinPolicy Enterprise"));
	label_notice->SetOwnForegroundColour(wxColour(255, 0, 0));
	label_notice->SetOwnFont(wxFont(-1, wxDEFAULT, -1, wxBOLD));
#endif

	// Widgets que recebem foco s�o criados na ordem do TAB
	chk_list = new wxCheckBox(this, wxID_ANY, wxT("Aplicar nos usu�rios selecionados na lista:"));
	list_users = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_MULTIPLE | wxLB_SORT);
	chk_allusers = new wxCheckBox(this, wxID_ANY, wxT("Aplicar em todos os usu�rios do sistema"));
	chk_thisuser = new wxCheckBox(this, wxID_ANY, wxT("Aplicar no usu�rio atual (") + wxGetUserName() + wxT(")"));

	// Preenchimento da lista
	list_users->InsertItems(AppBase::LoadUsersReg(), 0);

	// Posicionamento dos widgets
	sizer_h1->Add(bitmap, 0, wxALL, 5);
	sizer_h1->Add(label_desc, 0, wxALL, 5);

	sizer_v1->Add(sizer_h1, 0, wxALL, 0);
	sizer_v1->Add(chk_list, 0, wxALL, 5);
	sizer_v1->Add(list_users, 1, wxALL | wxEXPAND, 5);
	sizer_v1->Add(chk_allusers, 0, wxALL, 5);
	sizer_v1->Add(chk_thisuser, 0, wxALL, 5);

#ifndef _WINPOLICY_ENTERPRISE_
	sizer_v1->Add(label_notice, 0, wxALL | wxALIGN_CENTRE, 5);
	chk_list->Disable();
	list_users->Disable();
	chk_allusers->Disable();
	chk_thisuser->Disable();
	chk_allusers->SetValue(true);
	chk_thisuser->SetValue(true);
#endif

	SetSizer(sizer_v1);
}

// Retorna o valor da checkbox chk_allusers
bool PanelWinUsers::GetAllUsers(void)
{
	return chk_allusers->GetValue();
}

// Retorna o valor da checkbox chk_thisuser
bool PanelWinUsers::GetThisUser(void)
{
	return chk_thisuser->GetValue();
}

// Retorna uma lista com os usu�rios selecionados na lista
wxArrayString PanelWinUsers::GetUsers(void)
{
	wxArrayInt Indexes;
	wxArrayString List;

	if (chk_list->GetValue())
	{
		list_users->GetSelections(Indexes);
		for (unsigned int i = 0; i < Indexes.GetCount(); i++)
			List.Add(list_users->GetString(Indexes.Item(i)));
	}

	return List;
}

// Modifica o valor da caixa chk_allusers
void PanelWinUsers::SetAllUsers(bool value)
{
	chk_allusers->SetValue(value);
}
