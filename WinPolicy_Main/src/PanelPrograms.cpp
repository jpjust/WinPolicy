/* Programs panel
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

#include "PanelPrograms.h"
#include "AppBase.h"
#include "jswinreg.h"
#include <stdio.h>
#include <wx/image.h>

BEGIN_EVENT_TABLE(PanelPrograms, wxPanel)
	EVT_BUTTON(ID_PANELPROGRAMS_BTN_ADD, PanelPrograms::AddProgram)
	EVT_BUTTON(ID_PANELPROGRAMS_BTN_DEL, PanelPrograms::DelProgram)
	EVT_BUTTON(ID_PANELPROGRAMS_BTN_BROWSE, PanelPrograms::Browse)
END_EVENT_TABLE()

// Class constructor
PanelPrograms::PanelPrograms(wxWindow *parent)
	:wxPanel(parent)
{
	wxStaticBox *statbox = new wxStaticBox(this, -1, "Programas");
	wxStaticBoxSizer *sizer_v1 = new wxStaticBoxSizer(statbox, wxVERTICAL);
	wxBoxSizer *sizer_h1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h3 = new wxBoxSizer(wxHORIZONTAL);

	str_mandatory.Add("winpolicy.exe");
	str_mandatory.Add("wplocker.exe");
	str_mandatory.Add("wpservice.exe");
	str_mandatory.Add("autolock.exe");
	str_mandatory.Add("justsoft winpolicy.scr");

	label_title = new wxStaticText(this, -1, "Use as op��es abaixo para especificar quais programas\n" \
											 "poder�o ser executados pelo Windows.");

	bitmap = new wxStaticBitmap(this, -1, AppBase::LoadImage(wxT("programs.png")));

	// Widgets that can get focus are created in the correct order
	chk_blockall = new wxCheckBox(this, -1, "Bloquear todos os programas, exceto os listados abaixo:");
	txt_program = new wxTextCtrl(this, -1);
	btn_addprogram = new wxButton(this, ID_PANELPROGRAMS_BTN_ADD, "Adicionar");
	btn_remprogram = new wxButton(this, ID_PANELPROGRAMS_BTN_DEL, "Remover");
	list_blocked = new wxListBox(this, -1, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL | wxLB_EXTENDED);
	btn_addfromlist = new wxButton(this, ID_PANELPROGRAMS_BTN_BROWSE, "Procurar");

	// Layout development
	sizer_h1->Add(bitmap, 0, wxALL, 5);
	sizer_h1->Add(label_title, 0, wxALL, 5);

	sizer_h2->Add(txt_program, 1, wxALL, 5);
	sizer_h2->Add(btn_addprogram, 0, wxALL, 5);
	sizer_h2->Add(btn_remprogram, 0, wxALL, 5);

	sizer_h3->Add(0, 0, 1, wxALL, 0);
	sizer_h3->Add(btn_addfromlist, 0, wxALL, 5);

	sizer_v1->Add(sizer_h1, 0, wxALL, 0);
	sizer_v1->Add(chk_blockall, 0, wxALL | wxEXPAND, 5);
	sizer_v1->Add(sizer_h2, 0, wxALL | wxEXPAND, 0);
	sizer_v1->Add(list_blocked, 1, wxALL | wxEXPAND, 5);
	sizer_v1->Add(sizer_h3, 0, wxALL | wxEXPAND, 0);

	SetSizer(sizer_v1);

	GetTweaks();
}

// GetTweaks method
void PanelPrograms::GetTweaks(void)
{
	chk_blockall->SetValue(Tweaks.GetTEnabled());

	list_blocked->Clear();
	for (int i = 0; i < Tweaks.GetProgramsCount(); i++)
		list_blocked->Append(Tweaks.GetFilename(i));

	CheckWinPolicyEXE();
}

// ApplyTweaks method
void PanelPrograms::ApplyTweaks(void)
{
	Tweaks.SetTEnabled(chk_blockall->GetValue());

	Tweaks.ClearPrograms();
	for (unsigned int i = 0; i < list_blocked->GetCount(); i++)
		Tweaks.AddFilename(list_blocked->GetString(i));
}

// SaveTweaks method
bool PanelPrograms::SaveTweaks(bool hku, wxArrayString Users, bool thisuser)
{
	ApplyTweaks();
	return Tweaks.SaveTweaks(hku, Users, thisuser);
}

// SaveProfile method
bool PanelPrograms::SaveProfile(wxString filename)
{
	ApplyTweaks();
	return Tweaks.SaveToProfile(filename);
}

// LoadProfile method
bool PanelPrograms::LoadProfile(wxString filename)
{
	if (Tweaks.LoadFromProfile(filename) == false)
		return false;

	GetTweaks();

	return true;
}

// AddFilename method
void PanelPrograms::AddFilename(wxString filename)
{
	if (filename.Len() == 0)
		return;

	for (unsigned i = 0; i < list_blocked->GetCount(); i++)
	{
		if (list_blocked->GetString(i).Lower() == filename.Lower())
		{
			wxMessageBox("O aplicativo \"" + filename + "\" j� existe na lista.", APP_NAME, wxICON_EXCLAMATION);
			return;
		}
	}

	list_blocked->Append(filename);
}

// AddProgram method
void PanelPrograms::AddProgram(wxCommandEvent &event)
{
	if (txt_program->GetValue().Length() == 0)
	{
		wxMessageBox("Digite o nome de um execut�vel na caixa de texto ao lado ou clique no bot�o \"Procurar\" para navegar nas pastas dos seus discos r�gidos.", APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	AddFilename(txt_program->GetValue());
	txt_program->Clear();
}

// DelProgram method
void PanelPrograms::DelProgram(wxCommandEvent &event)
{
	wxArrayInt sels;
	int i;

	list_blocked->GetSelections((wxArrayInt&)sels);
	if (sels.GetCount() == 0)
	{
		wxMessageBox("Por favor, selecione pelo menos um programa na lista antes de prosseguir.", APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	for (i = sels.GetCount() - 1; i >= 0; i--)
	{
		if (str_mandatory.Index(list_blocked->GetString(sels.Item(i)), false) != wxNOT_FOUND)
		{
			wxMessageBox("O execut�vel \"" + list_blocked->GetString(sels.Item(i)) + "\" n�o pode ser removido por um aplicativo do WinPolicy.", APP_NAME, wxICON_EXCLAMATION);
			continue;
		}
		list_blocked->Delete(sels.Item(i));
	}
}

// Browse method
void PanelPrograms::Browse(wxCommandEvent &event)
{
	wxFileDialog dialog(this, "Bloquear programa", dialog_path, "", \
		"Todos os tipos v�lidos (*.exe; *.com; *.bat; *.pif)|*.exe; *.com; *.bat; *.pif|Todos os arquivos (*.*)|*.*", \
		wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
	wxArrayString filenames;
	wxString filename;
	unsigned int i;

	if (dialog.ShowModal() == wxID_OK)
	{
		dialog.GetFilenames(filenames);
		for (i = 0; i < filenames.GetCount(); i++)
		{
			filename = filenames.Item(i).AfterLast('\\');
			AddFilename(filename);
		}
		dialog_path = dialog.GetPath();
	}
}

// CheckWinPolicyEXE method
void PanelPrograms::CheckWinPolicyEXE(void)
{
	unsigned int i;

	for (i = 0; i < str_mandatory.GetCount(); i++)
	{
		if (list_blocked->FindString(str_mandatory.Item(i)) == wxNOT_FOUND)
			list_blocked->Append(str_mandatory.Item(i));
	}

	ApplyTweaks();
}
