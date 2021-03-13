/* PanelWindows class
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

#include "PanelWindows.h"
#include "ControllerBase.h"
#include <TweaksDefs.h>
#include <WPNP.h>
#include <wx/image.h>

BEGIN_EVENT_TABLE(PanelWindows, wxPanel)
	EVT_BUTTON(ID_PANELWINDOWS_BTN_CHECKALL, PanelWindows::CheckAll)
	EVT_BUTTON(ID_PANELWINDOWS_BTN_UNCHECKALL, PanelWindows::UnCheckAll)
END_EVENT_TABLE()

// Class constructor
PanelWindows::PanelWindows(wxWindow *parent)
	:wxScrolledWindow(parent)
{
	wxStaticBox *statbox = new wxStaticBox(this, wxID_ANY, _("Windows"));
	wxStaticBoxSizer *sizer_v1 = new wxStaticBoxSizer(statbox, wxVERTICAL);
	wxBoxSizer *sizer_h1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h2 = new wxBoxSizer(wxHORIZONTAL);

	label_title = new wxStaticText(this, -1, _("Use as op��es abaixo para gerenciar as restri��es do\n" \
											 "Windows Explorer e do Menu Iniciar do Windows."));

	wxImage png(ControllerBase::GetAppDir() + wxT("/images/windows.png"), wxBITMAP_TYPE_PNG);
#ifdef __WXMSW__
	int osmaj, osmin, osmm;
	wxGetOsVersion(&osmaj, &osmin);
	osmm = osmaj * 100 + osmin;
	if (osmm < 501)
		png.ConvertAlphaToMask();
#endif
	bitmap = new wxStaticBitmap(this, -1, wxBitmap(png));

	// Widgets that can get focus are created in the correct order
	list = new wxCheckListBox(this, -1, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL);
	btn_checkall = new wxButton(this, ID_PANELWINDOWS_BTN_CHECKALL, _("Marcar todos"));
	btn_checknone = new wxButton(this, ID_PANELWINDOWS_BTN_UNCHECKALL, _("Desmarcar todos"));

	// Layout development
	sizer_h1->Add(bitmap, 0, wxALL, 5);
	sizer_h1->Add(label_title, 0, wxALL, 5);

	sizer_h2->Add(0, 0, 1, wxALL, 5);
	sizer_h2->Add(btn_checkall, 0, wxALL, 5);
	sizer_h2->Add(btn_checknone, 0, wxALL, 5);

	sizer_v1->Add(sizer_h1, 0, wxBOTTOM, 5);
	sizer_v1->Add(list, 1, wxTOP | wxEXPAND, 5);
	sizer_v1->Add(sizer_h2, 0, wxALL | wxEXPAND, 0);

	SetSizer(sizer_v1);

	// Add the list contents
	int i = 0;
	while (1)
	{
		if (tweaks_windows[i][0] == wxT('0'))
			break;
		vector<wxString> parm = Split(wxString(tweaks_windows[i++]), wxT('|'), -1);
		list->Append(parm[4]);
		tags.push_back(parm[1]);
	}
	tags.resize(tags.size());

	SetScrollRate(5, 5);
}

// Get checked options
wxString PanelWindows::GetChecked(void)
{
	wxString Checked;

	for (unsigned int i = 0; i < list->GetCount(); i++)
	{
		if (list->IsChecked(i))
			Checked.Append(tags[i] + wxT(","));
	}

	return Checked;
}

// Get unchecked options
wxString PanelWindows::GetUnchecked(void)
{
	wxString Unchecked;

	for (unsigned int i = 0; i < list->GetCount(); i++)
	{
		if (!list->IsChecked(i))
			Unchecked.Append(tags[i] + wxT(","));
	}

	return Unchecked;
}

// Marca/desmarca restri��es de acordo com a string
void PanelWindows::SetChecked(wxString Checks, bool check)
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

// CheckAll method
void PanelWindows::CheckAll(wxCommandEvent &event)
{
	for (unsigned i = 0; i < list->GetCount(); i++)
		list->Check(i, true);
}

// UnCheckAll method
void PanelWindows::UnCheckAll(wxCommandEvent &event)
{
	for (unsigned i = 0; i < list->GetCount(); i++)
		list->Check(i, false);
}
