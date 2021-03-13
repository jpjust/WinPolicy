/* Network panel
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

#include "PanelNetwork.h"
#include "AppBase.h"
#include "jswinreg.h"
#include <wx/image.h>

BEGIN_EVENT_TABLE(PanelNetwork, wxPanel)
	EVT_BUTTON(ID_PANELNETWORK_BTN_CHECKALL, PanelNetwork::CheckAll)
	EVT_BUTTON(ID_PANELNETWORK_BTN_UNCHECKALL, PanelNetwork::UnCheckAll)
END_EVENT_TABLE()
;

// Class constructor
PanelNetwork::PanelNetwork(wxWindow *parent)
	:wxPanel(parent)
{
	wxStaticBox *statbox = new wxStaticBox(this, -1, "Rede");
	wxStaticBoxSizer *sizer_v1 = new wxStaticBoxSizer(statbox, wxVERTICAL);
	wxBoxSizer *sizer_h1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h3 = new wxBoxSizer(wxHORIZONTAL);

	label_title = new wxStaticText(this, -1, "Use as op��es abaixo para gerenciar as restri��es ao\n" \
											 "acesso das propriedades da rede do Windows.");
	label_pwdlen1 = new wxStaticText(this, -1, "Comprimento m�nimo das senhas:");
	label_pwdlen2 = new wxStaticText(this, -1, "caracteres.");

	bitmap = new wxStaticBitmap(this, -1, AppBase::LoadImage(wxT("network.png")));

	// Widgets that can get focus are created in the correct order
	list = new wxCheckListBox(this, -1, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL);
	txt_pwdlen = new wxTextCtrl(this, -1);
	btn_checkall = new wxButton(this, ID_PANELNETWORK_BTN_CHECKALL, "Marcar todos");
	btn_checknone = new wxButton(this, ID_PANELNETWORK_BTN_UNCHECKALL, "Desmarcar todos");

	// Layout development
	sizer_h1->Add(bitmap, 0, wxALL, 5);
	sizer_h1->Add(label_title, 0, wxALL, 5);

	sizer_h2->Add(label_pwdlen1, 0, wxALL, 5);
	sizer_h2->Add(txt_pwdlen, 0, wxALL, 5);
	sizer_h2->Add(label_pwdlen2, 0, wxALL, 5);

	sizer_h3->Add(0, 0, 1, wxALL, 5);
	sizer_h3->Add(btn_checkall, 0, wxALL, 5);
	sizer_h3->Add(btn_checknone, 0, wxALL, 5);

	sizer_v1->Add(sizer_h1, 0, wxBOTTOM, 5);
	sizer_v1->Add(list, 1, wxTOP | wxEXPAND, 5);
	sizer_v1->Add(sizer_h2, 0, wxALL | wxEXPAND, 0);
	sizer_v1->Add(sizer_h3, 0, wxALL | wxEXPAND, 0);

	SetSizer(sizer_v1);

	// Add the list contents
	for (int i = 0; i < Tweaks.GetTweaksCount(); i++)
		list->Append(Tweaks.GetTweak(i).GetDescription());

	GetTweaks();
}

// CheckAll method
void PanelNetwork::CheckAll(wxCommandEvent &event)
{
	for (unsigned i = 0; i < list->GetCount(); i++)
		list->Check(i, true);
}

// UnCheckAll method
void PanelNetwork::UnCheckAll(wxCommandEvent &event)
{
	for (unsigned i = 0; i < list->GetCount(); i++)
		list->Check(i, false);
}

// GetTweaks method
void PanelNetwork::GetTweaks(void)
{
	int pwdlen;
	wxString str;

	for (int i = 0; i < Tweaks.GetTweaksCount(); i++)
		list->Check(i, (bool)Tweaks.GetTweak(i).GetData());

	pwdlen = Tweaks.GetPwdLen();
	str.Printf("%d", pwdlen);
	txt_pwdlen->SetValue(str);
}

// ApplyTweaks method
void PanelNetwork::ApplyTweaks(void)
{
	for (int i = 0; i < Tweaks.GetTweaksCount(); i++)
	{
		Tweak aux = Tweaks.GetTweak(i);

		aux.SetData(BOOL2INT(list->IsChecked(i)));
		Tweaks.SetTweak(i, aux);
	}
}

// SaveTweaks method
bool PanelNetwork::SaveTweaks(bool hku, wxArrayString Users, bool thisuser)
{
	ApplyTweaks();
	return Tweaks.SaveTweaks(hku, Users, thisuser);
}

// SaveProfile method
bool PanelNetwork::SaveProfile(wxString filename)
{
	ApplyTweaks();
	return Tweaks.SaveToProfile(filename);
}

// LoadProfile method
bool PanelNetwork::LoadProfile(wxString filename)
{
	if (Tweaks.LoadFromProfile(filename) == false)
		return false;

	wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_PANELNETWORK_BTN_UNCHECKALL);
	UnCheckAll(event);

	GetTweaks();

	return true;
}
