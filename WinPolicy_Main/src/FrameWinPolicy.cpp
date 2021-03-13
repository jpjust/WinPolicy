/* Main WinPolicy frame
 *
 * Copyright (c) 2004 - 2007 João Paulo Just Peixoto <just1982@gmail.com>.
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

#include "FrameWinPolicy.h"
#include "AppBase.h"
#include "PwdMgr.h"
#include "NetReg.h"
#include <wx/image.h>
#include <wx/utils.h>

const wxString str_menu[] = {"Seguran�a", "Usu�rios do Sistema", "Usu�rios do WinPolicy", "Perfil", "Sistema", "Windows", "Rede", "Programas", "Bloqueio", "Outros", "Sobre", "Ajuda"};

BEGIN_EVENT_TABLE(FrameWinPolicy, wxFrame)
	EVT_BUTTON(ID_FRAMEWINPOLICY_BTN_OK, FrameWinPolicy::SaveSettings)
	EVT_BUTTON(ID_FRAMEWINPOLICY_BTN_APPLY, FrameWinPolicy::SaveSettings)
	EVT_BUTTON(ID_FRAMEWINPOLICY_BTN_CANCEL, FrameWinPolicy::Quit)
	EVT_LIST_ITEM_SELECTED(ID_FRAMEWINPOLICY_MENU, FrameWinPolicy::MenuClick)
	EVT_BUTTON(EVT_PROFILE_LOAD, FrameWinPolicy::OnLoadProfile)
	EVT_BUTTON(EVT_PROFILE_SAVE, FrameWinPolicy::OnSaveProfile)
    EVT_PAINT(FrameWinPolicy::OnPaint)
	EVT_BUTTON(EVT_PASSWDOK, FrameWinPolicy::OnPasswdOK)
	EVT_CLOSE(FrameWinPolicy::OnClose)
END_EVENT_TABLE()

// Class constructor
FrameWinPolicy::FrameWinPolicy(void)
	:wxFrame((wxWindow *)NULL, -1, APP_NAME, wxDefaultPosition, wxDefaultSize, wxCAPTION | wxMINIMIZE_BOX | wxCLOSE_BOX | wxSYSTEM_MENU)
{
	sizer_v1 = new wxBoxSizer(wxVERTICAL);
	sizer_v2 = new wxBoxSizer(wxVERTICAL);
	sizer_v3 = new wxBoxSizer(wxVERTICAL);
	sizer_h1 = new wxBoxSizer(wxHORIZONTAL);
	sizer_h2 = new wxBoxSizer(wxHORIZONTAL);

	// Variable initialization
	profile_loadusers = false;
	profile_allusers = false;
	profile_silent = false;
	profile_savetweaks = false;

	SetIcons(wxIconBundle(wxICON(wxICON)));
	SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	Refresh();

	// Checks if there is a password set
	if (PwdMgr::PasswdExists() == false)
	{
		wxMessageBox("ATEN��O: Voc� ainda n�o definiu uma senha para o WinPolicy. � recomendado que voc� defina uma senha, s� assim usu�rios n�o-autorizados n�o poder�o alterar as restri��es.", APP_NAME, wxICON_EXCLAMATION);
		AppBase::SetAccessStatus(true);
	}
	else
	{
		AppBase::SetAccessStatus(false);
	}

	// Widgets creation
	img_menu = new wxImageList(32, 32, true, 0);
	img_menu->Add(AppBase::LoadImage(wxT("security.png")));
	img_menu->Add(AppBase::LoadImage(wxT("users_sys.png")));
	img_menu->Add(AppBase::LoadImage(wxT("users.png")));
	img_menu->Add(AppBase::LoadImage(wxT("profiles.png")));
	img_menu->Add(AppBase::LoadImage(wxT("system.png")));
	img_menu->Add(AppBase::LoadImage(wxT("windows.png")));
	img_menu->Add(AppBase::LoadImage(wxT("network.png")));
	img_menu->Add(AppBase::LoadImage(wxT("programs.png")));
	img_menu->Add(AppBase::LoadImage(wxT("clock.png")));
	img_menu->Add(AppBase::LoadImage(wxT("misc.png")));
	img_menu->Add(AppBase::LoadImage(wxT("about.png")));
	img_menu->Add(AppBase::LoadImage(wxT("help.png")));

	list = new wxListCtrl(this, ID_FRAMEWINPOLICY_MENU, wxDefaultPosition, wxSize(120, 365), wxLC_ICON | wxLC_SINGLE_SEL);
	list->AssignImageList(img_menu, wxIMAGE_LIST_NORMAL);
	for (int i = 0; i < 12; i++)
		list->InsertItem(i, str_menu[i], i);

	// Widgets that can get focus are created in the correct order
	panel_security	= new PanelSecurity(this);
	panel_winusers	= new PanelWinUsers(this);
	panel_users		= new PanelUsers(this);
	panel_profiles	= new PanelProfiles(this);
	panel_system	= new PanelSystem(this);
	panel_windows	= new PanelWindows(this);
	panel_network	= new PanelNetwork(this);
	panel_programs	= new PanelPrograms(this);
	panel_locking	= new PanelLocking(this);
	panel_misc		= new PanelMisc(this);
	panel_about		= new PanelAbout(this);

	btnOK = new wxButton(this, ID_FRAMEWINPOLICY_BTN_OK, "OK");
	btnCancel = new wxButton(this, ID_FRAMEWINPOLICY_BTN_CANCEL, "Cancelar");
	btnApply = new wxButton(this, ID_FRAMEWINPOLICY_BTN_APPLY, "Aplicar");

	HideAllPanels();
	panel_security->Show(true);

	// Layout development
	sizer_h2->Add(0, 0, 1, wxALL | wxEXPAND, 5);
	sizer_h2->Add(btnOK, 0, wxALL, 5);
	sizer_h2->Add(btnCancel, 0, wxALL, 5);
	sizer_h2->Add(btnApply, 0, wxALL, 5);

	sizer_v2->Add(panel_security, 1, wxALL | wxEXPAND, 0);

	sizer_v3->Add(list, 1, wxALL | wxEXPAND, 5);

	sizer_h1->Add(sizer_v3, 0, wxALL | wxEXPAND, 0);
	sizer_h1->Add(sizer_v2, 1, wxALL | wxEXPAND, 0);

	wxBitmap bitmap = AppBase::LoadImage(wxT("title.png"));
	sizer_v1->Add(bitmap.GetWidth(), bitmap.GetHeight(), 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 0);
	sizer_v1->Add(sizer_h1, 1, wxRIGHT | wxEXPAND, 5);
	sizer_v1->Add(sizer_h2, 0, wxALL | wxEXPAND, 0);

	SetSizerAndFit(sizer_v1);

	// Program initialization
#ifdef _WINPOLICY_DEBUG_
	wxString debug_title;
	debug_title.Printf("Justsoft WinPolicy - Debug mode [%dx%d]", GetSize().GetWidth(), GetSize().GetHeight());
	SetTitle(debug_title);
#endif

	// Carrega os usu�rios do Windows no registro
	AppBase::LoadUsersReg();

	// Executa a thread do registro
	netreg = new NetReg();
	netreg->Create();
	netreg->Run();
}

// OnPaint method
void FrameWinPolicy::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	PrepareDC(dc);

	dc.Clear();
	dc.DrawBitmap(AppBase::LoadImage(wxT("title.png")), 0, 0, false);
}

// OnPasswdOK event
void FrameWinPolicy::OnPasswdOK(wxCommandEvent& event)
{
	if (!panel_system->GetListBox()->IsChecked(0))
	{
		if (wxMessageBox(wxT("OBSERVA��O: O acesso ao registro do Windows n�o foi bloqueado. " \
			"� recomendado que isto seja feito, assim usu�rios mal intencionados " \
			"n�o poder�o editar as restri��es manualmente. Deseja restringir o acesso " \
			"ao registro do Windows agora?"), wxT(APP_NAME), wxYES_NO | wxICON_QUESTION) == wxYES)
		{
			if (panel_system->RestrictRegedit() == false)
				wxMessageBox(wxT("Ocorreu um erro ao tentar restringir o acesso ao registro do Windows. " \
					"Se estiver executando o Windows NT, 2000, 2003 ou XP, certifique-se de que " \
					"tem privil�gios de administrador neste computador."), wxT(APP_NAME), wxICON_ERROR);
		}
	}
}

// Quit method
void FrameWinPolicy::Quit(wxCommandEvent& event)
{
	Close(true);
}

// ToolBarClick method
void FrameWinPolicy::MenuClick(wxListEvent& event)
{
	int selected = event.GetIndex();// - ID_FRAMEWINPOLICY_BTN_MENU;

	if ((selected > 0) && (selected < 10) && (AppBase::GetAccessStatus() == false))
	{
		wxMessageBox("Acesso negado! Digite a senha na se��o \"Seguran�a\" para continuar.", APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	if (selected == 11)
	{
		AppBase::ShowHelp();
		return;
	}

	HideAllPanels();
	sizer_v2->Detach(0);

	switch (selected)
	{
	case 0:
		sizer_v2->Insert(0, panel_security, 1, wxALL | wxEXPAND, 0);
		panel_security->Show(true);
		break;

	case 1:
		sizer_v2->Insert(0, panel_winusers, 1, wxALL | wxEXPAND, 0);
		panel_winusers->Show(true);
		break;

	case 2:
		sizer_v2->Insert(0, panel_users, 1, wxALL | wxEXPAND, 0);
		panel_users->Show(true);
		break;

	case 3:
		sizer_v2->Insert(0, panel_profiles, 1, wxALL | wxEXPAND, 0);
		panel_profiles->Show(true);
		break;

	case 4:
		sizer_v2->Insert(0, panel_system, 1, wxALL | wxEXPAND, 0);
		panel_system->Show(true);
		break;

	case 5:
		sizer_v2->Insert(0, panel_windows, 1, wxALL | wxEXPAND, 0);
		panel_windows->Show(true);
		break;

	case 6:
		sizer_v2->Insert(0, panel_network, 1, wxALL | wxEXPAND, 0);
		panel_network->Show(true);
		break;

	case 7:
		sizer_v2->Insert(0, panel_programs, 1, wxALL | wxEXPAND, 0);
		panel_programs->Show(true);
		break;

	case 8:
		sizer_v2->Insert(0, panel_locking, 1, wxALL | wxEXPAND, 0);
		panel_locking->Show(true);
		break;

	case 9:
		sizer_v2->Insert(0, panel_misc, 1, wxALL | wxEXPAND, 0);
		panel_misc->Show(true);
		break;

	case 10:
		sizer_v2->Insert(0, panel_about, 1, wxALL | wxEXPAND, 0);
		panel_about->Show(true);
		break;
	}

	sizer_v2->Layout();
}

// HideAllPanels function
void FrameWinPolicy::HideAllPanels(void)
{
	panel_security->Show(false);
	panel_winusers->Show(false);
	panel_users->Show(false);
	panel_profiles->Show(false);
	panel_system->Show(false);
	panel_windows->Show(false);
	panel_network->Show(false);
	panel_programs->Show(false);
	panel_locking->Show(false);
	panel_misc->Show(false);
	panel_about->Show(false);
}

// SaveSettings method
void FrameWinPolicy::SaveSettings(wxCommandEvent& event)
{
	wxString Err = wxT("");

	if ((AppBase::GetAccessStatus() == false) && (profile_savetweaks == false))
	{
		wxMessageBox("Acesso negado! Digite a senha na se��o \"Seguran�a\" para continuar.", APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	bool hku			= panel_winusers->GetAllUsers();
	bool thisuser		= panel_winusers->GetThisUser();
	wxArrayString Users	= panel_winusers->GetUsers();

	if (!panel_system->SaveTweaks(hku, Users, thisuser))	Err.Append(wxT("system\n"));
	if (!panel_windows->SaveTweaks(hku, Users, thisuser))	Err.Append(wxT("windows\n"));
	if (!panel_network->SaveTweaks(hku, Users, thisuser))	Err.Append(wxT("network\n"));
	if (!panel_programs->SaveTweaks(hku, Users, thisuser))	Err.Append(wxT("programs\n"));
	if (!panel_locking->SaveTweaks())						Err.Append(wxT("locking\n"));
	if (!panel_misc->SaveTweaks(hku, Users, thisuser))		Err.Append(wxT("misc\n"));

	if ((Err.Len() > 0) && (profile_silent == false))
	{
#ifdef _WINPOLICY_DEBUG_
		wxMessageBox(wxT("Error:\n\n") + Err, APP_NAME, wxICON_ERROR);
#endif

		wxMessageBox("Ocorreu um erro ao tentar salvar as configura��es. Certifique-se " \
			"de que possui privil�gios de administrador no Windows e tente " \
			"novamente.", APP_NAME, wxICON_ERROR);
		return;
	}

	if (event.GetId() == ID_FRAMEWINPOLICY_BTN_OK)
	{
/*		wxMessageBox(wxT("Restri��es aplicadas ") + User + \
			wxT(". Se alguma restri��o ativada ou desativada n�o fizer efeito imediatamente, ") + \
			wxT("ser� preciso reiniciar o computador."), APP_NAME, wxICON_INFORMATION);*/
		Close(true);
	}
}

// OnLoadProfile method
void FrameWinPolicy::OnLoadProfile(wxCommandEvent& event)
{
	int msgbox = 0, error = 0;

	if (profile_silent == false)
		msgbox = wxMessageBox("Deseja importar tamb�m os dados dos usu�rios cadastrados?", APP_NAME, wxYES_NO | wxICON_QUESTION);

	if ((msgbox == wxYES) || (profile_loadusers == true))
		error += BOOL2INT(!panel_users->LoadProfile(panel_profiles->GetProfileFilename()));

	error += BOOL2INT(!panel_system->LoadProfile(panel_profiles->GetProfileFilename()));
	error += BOOL2INT(!panel_windows->LoadProfile(panel_profiles->GetProfileFilename()));
	error += BOOL2INT(!panel_network->LoadProfile(panel_profiles->GetProfileFilename()));
	error += BOOL2INT(!panel_programs->LoadProfile(panel_profiles->GetProfileFilename()));
	error += BOOL2INT(!panel_locking->LoadProfile(panel_profiles->GetProfileFilename()));
	error += BOOL2INT(!panel_misc->LoadProfile(panel_profiles->GetProfileFilename()));

	if (profile_silent == false)
	{
		if (error == 0)
			wxMessageBox("Perfil carregado com sucesso!", APP_NAME, wxICON_INFORMATION);
		else
			wxMessageBox("Ocorreu um erro ao tentar carregar o perfil. Certifique-se " \
				"de que tem permiss�o para ler o arquivo e se este � um arquivo v�lido.", \
				APP_NAME, wxICON_ERROR);
	}

	if (profile_savetweaks)
	{
		wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_FRAMEWINPOLICY_BTN_APPLY);
		SaveSettings(event);
	}

	if (profile_silent)
		Close(true);

	profile_loadusers = false;
	profile_allusers = false;
	profile_silent = false;
	profile_savetweaks = false;
}

// OnSaveProfile method
void FrameWinPolicy::OnSaveProfile(wxCommandEvent& event)
{
	int error = 0;

	wxFileConfig fc(wxEmptyString, wxEmptyString, panel_profiles->GetProfileFilename(), wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
	error += BOOL2INT(!fc.Write("/WinPolicy/WinPolicyVersion", wxString(VERSION_MAJOR_STR) + "." + VERSION_MINOR_STR));
	error += BOOL2INT(!fc.Write("/WinPolicy/FileVersion", "3"));
	error += BOOL2INT(!fc.Flush());

	if (wxMessageBox("Deseja salvar tamb�m os dados dos usu�rios cadastrados?", APP_NAME, wxYES_NO | wxICON_QUESTION) == wxYES)
		error += BOOL2INT(!panel_users->SaveProfile(panel_profiles->GetProfileFilename()));

	error += BOOL2INT(!panel_system->SaveProfile(panel_profiles->GetProfileFilename()));
	error += BOOL2INT(!panel_windows->SaveProfile(panel_profiles->GetProfileFilename()));
	error += BOOL2INT(!panel_network->SaveProfile(panel_profiles->GetProfileFilename()));
	error += BOOL2INT(!panel_programs->SaveProfile(panel_profiles->GetProfileFilename()));
	error += BOOL2INT(!panel_locking->SaveProfile(panel_profiles->GetProfileFilename()));
	error += BOOL2INT(!panel_misc->SaveProfile(panel_profiles->GetProfileFilename()));

	if (error == 0)
		wxMessageBox("Perfil salvo com sucesso!", APP_NAME, wxICON_INFORMATION);
	else
		wxMessageBox("Ocorreu um erro ao tentar salvar o perfil. Certifique-se " \
			"de que tem permiss�o para gravar no arquivo e tente novamente.", \
			APP_NAME, wxICON_ERROR);
}

// Evento de fechamento da janela
void FrameWinPolicy::OnClose(wxCloseEvent &event)
{
	// Salva altera��es no registro e fecha
	AppBase::UnLoadUsersReg();
	Destroy();
}

// LoadProfile method
void FrameWinPolicy::LoadProfile(wxString filename, bool loadusers, bool allusers, bool silent, bool savetweaks)
{
	panel_profiles->SetProfileFilename(filename);
	profile_loadusers = loadusers;
	profile_silent = silent;
	profile_allusers = allusers;
	profile_savetweaks = savetweaks;

	wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, EVT_PROFILE_SAVE);
	OnLoadProfile(event);
}
