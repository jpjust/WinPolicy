/* Security panel
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

#include "PanelSecurity.h"
#include "AppBase.h"
#include "PwdMgr.h"
#include "RDiskMgr.h"
#include "jswinreg.h"
#include <wx/image.h>

BEGIN_EVENT_TABLE(PanelSecurity, wxPanel)
	EVT_BUTTON(ID_PANELSECURITY_BTN_RESCUE, PanelSecurity::RescueDisk)
	EVT_BUTTON(ID_PANELSECURITY_BTN_SAVEPWD, PanelSecurity::SavePasswd)
	EVT_BUTTON(ID_PANELSECURITY_BTN_DELPWD, PanelSecurity::DelPasswd)
	EVT_BUTTON(ID_PANELSECURITY_BTN_OK, PanelSecurity::CheckPasswd)
	EVT_TEXT_ENTER(ID_PANELSECURITY_PWDPROMPT, PanelSecurity::CheckPasswd)
END_EVENT_TABLE()

// Class constructor
PanelSecurity::PanelSecurity(wxWindow *parent)
	:wxPanel(parent)
{
	wxStaticBox *statbox = new wxStaticBox(this, -1, "Seguran�a");
	wxStaticBoxSizer *sizer_v1 = new wxStaticBoxSizer(statbox, wxVERTICAL);
	wxBoxSizer *sizer_v2 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *sizer_h1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h3 = new wxBoxSizer(wxHORIZONTAL);

	bitmap = new wxStaticBitmap(this, -1, AppBase::LoadImage(wxT("security.png")));

	label_title = new wxStaticText(this, -1, "� importante definir uma senha de prote��o do WinPolicy, s�\n" \
											 "assim, usu�rios n�o-autorizados n�o poder�o alterar as\n" \
											 "restri��es impostas pelo WinPolicy.");
	label_pwd1 = new wxStaticText(this, -1, "Para definir uma senha, digite-a abaixo:");
	label_pwd2 = new wxStaticText(this, -1, "Confirme a senha digitada acima:");
	label_pwd3 = new wxStaticText(this, -1, "Digite a senha de acesso do WinPolicy abaixo:");

	// Widgets that can get focus are created in the correct order
	text_pwd1 = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxSize(200, -1), wxTE_PASSWORD);
	text_pwd2 = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxSize(200, -1), wxTE_PASSWORD);
	btn_savepwd = new wxButton(this, ID_PANELSECURITY_BTN_SAVEPWD, "Salvar senha");
	btn_erasepwd = new wxButton(this, ID_PANELSECURITY_BTN_DELPWD, "Apagar senha");
	btn_recovery = new wxButton(this, ID_PANELSECURITY_BTN_RESCUE, "Disco de Recupera��o");
	text_pwd3 = new wxTextCtrl(this, ID_PANELSECURITY_PWDPROMPT, "", wxDefaultPosition, wxSize(200, -1), wxTE_PASSWORD);
	btn_ok = new wxButton(this, ID_PANELSECURITY_BTN_OK, "OK");

	// Layout development
	sizer_v2->Add(label_pwd1, 0, wxALL, 5);
	sizer_v2->Add(text_pwd1, 0, wxALL, 5);
	sizer_v2->Add(label_pwd2, 0, wxALL, 5);
	sizer_v2->Add(text_pwd2, 0, wxALL, 5);

	sizer_h1->Add(bitmap, 0, wxALL, 5);
	sizer_h1->Add(label_title, 1, wxALL | wxEXPAND, 5);

	sizer_h2->Add(sizer_v2, 1, wxALL | wxCENTRE, 0);
	sizer_h2->Add(btn_recovery, 0, wxALL | wxCENTRE, 5);

	sizer_h3->Add(btn_savepwd, 1, wxALL | wxCENTRE, 5);
	sizer_h3->Add(btn_erasepwd, 1, wxALL | wxCENTRE, 5);

	sizer_v1->Add(sizer_h1, 0, wxBOTTOM, 5);
	sizer_v1->Add(sizer_h2, 0, wxTOP | wxCENTRE, 5);
	sizer_v1->Add(sizer_h3, 0, wxALL | wxCENTRE, 0);
	sizer_v1->Add(label_pwd3, 0, wxALL | wxCENTRE, 5);
	sizer_v1->Add(text_pwd3, 0, wxALL | wxCENTRE, 5);
	sizer_v1->Add(btn_ok, 0, wxALL | wxCENTRE, 5);

	SetSizer(sizer_v1);

	// Initialization
	ShowPwdWidgets(AppBase::GetAccessStatus());
	text_pwd3->SetFocus();

	if (AppBase::GetAccessStatus() == true)
	{
		wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, EVT_PASSWDOK);
		wxPostEvent(this, event);
	}
}

// ShowPwdWidgets method
void PanelSecurity::ShowPwdWidgets(bool show)
{
	label_pwd3->Show(!show);
	text_pwd3->Show(!show);
	btn_ok->Show(!show);

	text_pwd1->Enable(show);
	text_pwd2->Enable(show);
	btn_savepwd->Enable(show);
	btn_erasepwd->Enable(show);
}

// RescueDisk method
void PanelSecurity::RescueDisk(wxCommandEvent& event)
{
	if (AppBase::GetAccessStatus() == true)	// Creates a rescue disk
	{
		if (PwdMgr::PasswdExists() == false)
		{
			wxMessageBox("Voc� ainda n�o pode criar um Disco de Recupera��o pois n�o h� nenhuma senha definida. Defina uma senha para o WinPolicy e em seguida crie o disco.", APP_NAME, wxICON_INFORMATION);
			return;
		}

		if (RDiskMgr::SetRescuePwd() == true)
			wxMessageBox("Seu Disco de Recupera��o foi criado com sucesso! Guarde-o em um local seguro.", APP_NAME, wxICON_INFORMATION);
		else
			wxMessageBox("Ocorreu um erro ao tentar criar o Disco de Recupera��o! Tente novamente.", APP_NAME, wxICON_ERROR);
	}
	else	// Uses a rescue disk
	{
		if (RDiskMgr::CheckRescuePwd(true) == true)
		{
			AppBase::SetAccessStatus(true);
			ShowPwdWidgets(true);
			wxMessageBox("Senha aceita!", APP_NAME, wxICON_INFORMATION);
		}
		else
			wxMessageBox("A senha contida no Disco de Recupera��o n�o coincide com a senha definida no WinPolicy. Verifique se est� utilizando o disco correto e tente novamente.", APP_NAME, wxICON_ERROR);
	}
}

// SavePasswd method
void PanelSecurity::SavePasswd(wxCommandEvent& event)
{
	if (text_pwd1->GetValue() != text_pwd2->GetValue())
	{
		wxMessageBox("As senhas digitadas n�o coincidem.", APP_NAME, wxICON_EXCLAMATION);
		return;
	}
	else if (text_pwd1->GetValue().Length() < 3)
	{
		wxMessageBox("O comprimento m�nimo da senha � de 3 caracteres.", APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	if (PwdMgr::SetPasswd(text_pwd1->GetValue()) == true)
	{
		if (wxMessageBox("Senha salva com sucesso! � recomendado criar um disco de recupera��o para ser " \
			"utilizado caso haja a perda da senha.\n\nDeseja criar um disco de recupera��o agora?", APP_NAME, wxICON_QUESTION | wxYES_NO) == wxYES)
		{
			wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_PANELSECURITY_BTN_RESCUE);
			RescueDisk(event);
		}
	}
	else
		wxMessageBox("Ocorreu um erro ao tentar salvar a senha no registro. Tente novamente.", APP_NAME, wxICON_ERROR);

	text_pwd1->SetValue(wxString(""));
	text_pwd2->SetValue(wxString(""));
}

// DelPasswd method
void PanelSecurity::DelPasswd(wxCommandEvent& event)
{
	if (PwdMgr::PasswdExists() == false)
	{
		wxMessageBox("N�o existe nenhuma senha definida.", APP_NAME, wxICON_INFORMATION);
		return;
	}

	if (wxMessageBox("Tem certeza que deseja apagar a senha do Justsoft WinPolicy?\n\nATEN��O: Usu�rios n�o autorizados poder�o alterar as restri��es.", APP_NAME, wxYES_NO | wxICON_QUESTION) == wxYES)
		if (PwdMgr::ErasePasswd() == false)
			wxMessageBox("Ocorreu um erro ao tentar apagar a senha. Tente novamente.", APP_NAME, wxICON_ERROR);
		else
			wxMessageBox("A senha do WinPolicy foi removida!", APP_NAME, wxICON_INFORMATION);
}

// CheckPasswd method
void PanelSecurity::CheckPasswd(wxCommandEvent& event)
{
	if (PwdMgr::CheckPasswd(text_pwd3->GetValue()) == false)
		wxMessageBox("Senha incorreta! Tente novamente.", APP_NAME, wxICON_EXCLAMATION);
	else
	{
		AppBase::SetAccessStatus(true);
		ShowPwdWidgets(true);
		wxMessageBox("Senha aceita!", APP_NAME, wxICON_INFORMATION);

		wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, EVT_PASSWDOK);
		wxPostEvent(this, event);
	}

	text_pwd3->SetValue(wxString(""));
}
