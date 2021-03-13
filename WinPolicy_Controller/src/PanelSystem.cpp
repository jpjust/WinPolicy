/* PanelSystem class
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

#include "PanelSystem.h"
#include "ControllerBase.h"
#include <TweaksDefs.h>
#include <WPNP.h>
#include <wx/image.h>

BEGIN_EVENT_TABLE(PanelSystem, wxPanel)
	EVT_BUTTON(ID_PANELSYSTEM_BTN_CHECKALL, PanelSystem::CheckAll)
	EVT_BUTTON(ID_PANELSYSTEM_BTN_UNCHECKALL, PanelSystem::UnCheckAll)
END_EVENT_TABLE()

// Construtor
PanelSystem::PanelSystem(wxWindow *parent)
	:wxScrolledWindow(parent)
{
	wxStaticBox *statbox = new wxStaticBox(this, -1, _("Sistema"));
	wxStaticBoxSizer *sizer_v1 = new wxStaticBoxSizer(statbox, wxVERTICAL);
	wxBoxSizer *sizer_h1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h2 = new wxBoxSizer(wxHORIZONTAL);

	label_title = new wxStaticText(this, -1, _("Use as op��es abaixo para gerenciar as restri��es gerais do\n" \
											 "sistema Windows."));

	wxImage png(ControllerBase::GetAppDir() + wxT("/images/system.png"), wxBITMAP_TYPE_PNG);
#ifdef __WXMSW__
	int osmaj, osmin, osmm;
	wxGetOsVersion(&osmaj, &osmin);
	osmm = osmaj * 100 + osmin;
	if (osmm < 501)
		png.ConvertAlphaToMask();
#endif
	bitmap = new wxStaticBitmap(this, -1, wxBitmap(png));

	// Widgets que podem entrar em foco devem ser criados na ordem correta
	list = new wxCheckListBox(this, -1, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL);
	btn_checkall = new wxButton(this, ID_PANELSYSTEM_BTN_CHECKALL, _("Marcar todos"));
	btn_checknone = new wxButton(this, ID_PANELSYSTEM_BTN_UNCHECKALL, _("Desmarcar todos"));

	// Desenvolvimento do layout
	sizer_h1->Add(bitmap, 0, wxALL, 5);
	sizer_h1->Add(label_title, 0, wxALL, 5);

	sizer_h2->Add(0, 0, 1, wxALL, 5);
	sizer_h2->Add(btn_checkall, 0, wxALL, 5);
	sizer_h2->Add(btn_checknone, 0, wxALL, 5);

	sizer_v1->Add(sizer_h1, 0, wxBOTTOM, 5);
	sizer_v1->Add(list, 1, wxTOP | wxEXPAND, 5);
	sizer_v1->Add(sizer_h2, 0, wxALL | wxEXPAND, 0);

	SetSizer(sizer_v1);

	// Adiciona o conte�do da lista
	int i = 0;
	while (1)
	{
		if (tweaks_system[i][0] == wxT('0'))
			break;
		vector<wxString> parm = Split(wxString(tweaks_system[i++]), wxT('|'), -1);
		list->Append(parm[4]);
		tags.push_back(parm[1]);
	}
	tags.resize(tags.size());

	SetScrollRate(5, 5);
}

// Retorna as op��es marcadas
wxString PanelSystem::GetChecked(void)
{
	wxString Checked;

	for (unsigned int i = 0; i < list->GetCount(); i++)
	{
		if (list->IsChecked(i))
			Checked.Append(tags[i] + wxT(","));
	}

	return Checked;
}

// Retorna as op��es desmarcadas
wxString PanelSystem::GetUnchecked(void)
{
	wxString Unchecked;

	for (unsigned int i = 0; i < list->GetCount(); i++)
	{
		if (!list->IsChecked(i))
			Unchecked.Append(tags[i] + wxT(","));
	}

	return Unchecked;
}

// Marca/desmarca as restri��es de acordo com a string
void PanelSystem::SetChecked(wxString Checks, bool check)
{
	vector<wxString> opt = Split(Checks, wxT(','), -1);

	for (unsigned int i = 0; i < opt.size(); i++)
	{
		for (unsigned int j = 0; j < tags.size(); j++)
		{
			if (tags[j] == opt[i])
			{
				list->Check(j, check);
				j = tags.size();
			}
		}
	}
}

// Marca todas as op��es
void PanelSystem::CheckAll(wxCommandEvent &event)
{
	for (unsigned i = 0; i < list->GetCount(); i++)
		list->Check(i, true);
}

// Desmarca todas as op��es
void PanelSystem::UnCheckAll(wxCommandEvent &event)
{
	for (unsigned i = 0; i < list->GetCount(); i++)
		list->Check(i, false);
}
