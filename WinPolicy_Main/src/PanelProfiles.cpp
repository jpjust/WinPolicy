/* Profiles panel
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

#include "PanelProfiles.h"
#include "AppBase.h"
#include "DialogOS.h"
#include "jswinreg.h"
#include <wx/image.h>
#include <wx/fileconf.h>

BEGIN_EVENT_TABLE(PanelProfiles, wxPanel)
	EVT_BUTTON(ID_PANELPROFILE_BTN_LOAD, PanelProfiles::LoadProfileClick)
	EVT_BUTTON(ID_PANELPROFILE_BTN_SAVE, PanelProfiles::SaveProfileClick)
	EVT_BUTTON(ID_PANELPROFILE_BTN_VERSION, PanelProfiles::OSDialog)
END_EVENT_TABLE()

// Class constructor
PanelProfiles::PanelProfiles(wxWindow *parent)
	:wxPanel(parent)
{
	wxStaticBox *statbox = new wxStaticBox(this, -1, "Perfis");
	wxStaticBoxSizer *sizer_v1 = new wxStaticBoxSizer(statbox, wxVERTICAL);
	wxBoxSizer *sizer_v2 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *sizer_h1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h3 = new wxBoxSizer(wxHORIZONTAL);

	label_title = new wxStaticText(this, -1, "Aqui voc� pode salvar a configura��o atual para usar em\n" \
											 "outro computador ou carregar um perfil j� salvo\n" \
											 "anteriormente. Se voc� administra v�rios computadores,\n" \
											 "recomendamos salvar os Perfis para uso posterior.");
	label_verdesc = new wxStaticText(this, -1, "O WinPolicy est� configurado para a vers�o do Windows listada\nabaixo:");
	label_winver = new wxStaticText(this, -1, "Linux :)");
	label_winver->SetOwnFont(wxFont(-1, wxDEFAULT, -1, wxBOLD));
	label_allver = new wxStaticText(this, -1, "(Restri��es para outras vers�es do Windows\n" \
											  "tamb�m est�o sendo exibidas)");
	label_allver->SetOwnFont(wxFont(-1, wxDEFAULT, -1, wxBOLD));
	label_allver->Show(false);
	label_verdesc2 = new wxStaticText(this, -1, "Apenas as restri��es referentes a esta vers�o do Windows est�o\n" \
												"sendo mostradas. Caso queira alterar esta configura��o, clique\n" \
												"no bot�o abaixo:");

	bitmap = new wxStaticBitmap(this, -1, AppBase::LoadImage(wxT("profiles.png")));

	line = new wxStaticLine(this, -1);

	// Widgets that can get focus are created in the correct order
	btn_load = new wxButton(this, ID_PANELPROFILE_BTN_LOAD, "Carregar Perfil");
	btn_save = new wxButton(this, ID_PANELPROFILE_BTN_SAVE, "Salvar Perfil");
	btn_version = new wxButton(this, ID_PANELPROFILE_BTN_VERSION, "Selecionar vers�o");

	// Layout development
	sizer_h1->Add(bitmap, 0, wxALL, 5);
	sizer_h1->Add(label_title, 1, wxALL | wxEXPAND, 5);

	sizer_h2->Add(btn_load, 1, wxALL, 5);
	sizer_h2->Add(btn_save, 1, wxALL, 5);

	sizer_v2->Add(label_winver, 0, wxALL, 5);
	sizer_v2->Add(label_allver, 0, wxALL, 5);

	sizer_h3->Add(20, 0, 0, wxALL, 5);
	sizer_h3->Add(sizer_v2, 0, wxALL, 5);

	sizer_v1->Add(sizer_h1, 0, wxBOTTOM, 5);
	sizer_v1->Add(sizer_h2, 0, wxTOP | wxCENTRE, 5);
	sizer_v1->Add(line, 0, wxALL | wxEXPAND, 5);
	sizer_v1->Add(label_verdesc, 0, wxALL, 5);
	sizer_v1->Add(sizer_h3, 0, wxALL, 0);
	sizer_v1->Add(label_verdesc2, 0, wxALL, 5);
	sizer_v1->Add(btn_version, 0, wxALL | wxCENTRE, 5);

	SetSizer(sizer_v1);

	// Initialization
	label_winver->SetLabel(AppBase::StrWinVersion[AppBase::GetWinVer(false) - WINVER_95]);
	label_allver->Show(AppBase::GetAllVer());
}

// LoadProfileClick method: show a file loading dialog to load a profile
void PanelProfiles::LoadProfileClick(wxCommandEvent& event)
{
	wxFileDialog dialog(this, "Carregar Perfil", dialog_path, "", \
		"Perfil do WinPolicy (*.wpp)|*.wpp|Todos os arquivos (*.*)|*.*", \
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (dialog.ShowModal() == wxID_OK)
	{
		profile_filename = dialog.GetPath();
		dialog_path = dialog.GetDirectory();

		wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, EVT_PROFILE_LOAD);
		wxPostEvent(this, event);
	}
}

// SaveProfileClick method: show a file loading dialog to save the profile
void PanelProfiles::SaveProfileClick(wxCommandEvent& event)
{
	wxFileDialog dialog(this, "Salvar Perfil", dialog_path, "", \
		"Perfil do WinPolicy (*.wpp)|*.wpp|Todos os arquivos (*.*)|*.*", \
		wxSAVE | wxOVERWRITE_PROMPT);

	if (dialog.ShowModal() == wxID_OK)
	{
		profile_filename = dialog.GetPath();
		dialog_path = dialog.GetDirectory();

		// Erase contents before saving
		wxFileConfig fc(wxEmptyString, wxEmptyString, profile_filename, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
		fc.DeleteAll();
		fc.Flush();

		wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, EVT_PROFILE_SAVE);
		wxPostEvent(this, event);
	}
}

// OSDialog method: show the OS selection dialog
void PanelProfiles::OSDialog(wxCommandEvent& event)
{
	DialogOS *dialog = new DialogOS();
	dialog->ShowModal();
	delete dialog;
}

// GetProfileFilename method: return the profile filename
wxString PanelProfiles::GetProfileFilename(void)
{
	return profile_filename;
}

// SetProfileFilename method: set the profile filename
void PanelProfiles::SetProfileFilename(wxString filename)
{
	profile_filename = filename;
}
