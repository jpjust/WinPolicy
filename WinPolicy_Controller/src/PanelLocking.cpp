/* PanelLocking class
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

#include "PanelLocking.h"
#include "ControllerBase.h"
#include <WPNP.h>
#include <wx/image.h>
#include <vector>

using namespace std;

BEGIN_EVENT_TABLE(PanelLocking, wxScrolledWindow)
	EVT_BUTTON(ID_PANELLOCKING_BTN_ADDTIME, PanelLocking::AddTime)
	EVT_BUTTON(ID_PANELLOCKING_BTN_DELTIME, PanelLocking::DelTime)
END_EVENT_TABLE()

// Class constructor
PanelLocking::PanelLocking(wxWindow *parent)
	:wxScrolledWindow(parent)
{
	wxStaticBox *statbox = new wxStaticBox(this, -1, _("Bloqueio"));
	wxStaticBoxSizer *sizer_v1 = new wxStaticBoxSizer(statbox, wxVERTICAL);
	wxBoxSizer *sizer_h1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h5 = new wxBoxSizer(wxHORIZONTAL);

	description = _("Use as op��es abaixo para configurar o sistema de\n" \
				  "bloqueio do WinPolicy e programar os hor�rios de bloqueio.");
	label_title = new wxStaticText(this, -1, description);

	wxImage png(ControllerBase::GetAppDir() + wxT("/images/clock.png"), wxBITMAP_TYPE_PNG);
#ifdef __WXMSW__
	int osmaj, osmin, osmm;
	wxGetOsVersion(&osmaj, &osmin);
	osmm = osmaj * 100 + osmin;
	if (osmm < 501)
		png.ConvertAlphaToMask();
#endif
	bitmap = new wxStaticBitmap(this, -1, wxBitmap(png));

	// Widgets that can get focus are created in the correct order
	chk_enablealock = new wxCheckBox(this, ID_PANELLOCKING_CHK_AUTOLOCK, _("Ativar o Autolock"));
	chk_askpwdtolock = new wxCheckBox(this, -1, _("Solicitar senha para bloquear o computador pelo Autolock"));
	chk_askpwdtoshutdown = new wxCheckBox(this, -1, _("Solicitar senha para desligar o computador quando bloqueado"));
	chk_lockkeys = new wxCheckBox(this, -1, _("Desativar teclas do sistema (CTRL, ALT, TAB, WIN e ESC)"));
	chk_lockonstartup = new wxCheckBox(this, -1, _("Bloquear automaticamente ao iniciar o Windows (95, 98 e Me)"));
	chk_lockonlogon = new wxCheckBox(this, -1, _("Bloquear o computador ap�s o logon"));
	chk_lockontime = new wxCheckBox(this, -1, _("Bloquear automaticamente nos hor�rios abaixo:"));
	txt_time_h = new wxTextCtrl(this, -1, wxT(""), wxDefaultPosition, wxSize(30, -1));
	txt_time_m = new wxTextCtrl(this, -1, wxT(""), wxDefaultPosition, wxSize(30, -1));
	btn_addtime = new wxButton(this, ID_PANELLOCKING_BTN_ADDTIME, _("Adicionar"));
	btn_deltime = new wxButton(this, ID_PANELLOCKING_BTN_DELTIME, _("Remover"));
	list_times = new wxListBox(this, -1, wxDefaultPosition, wxSize(300, 100), 0, NULL, wxLB_HSCROLL | wxLB_EXTENDED);

	// Layout development
	sizer_h1->Add(bitmap, 0, wxALL, 5);
	sizer_h1->Add(label_title, 0, wxALL, 5);

	sizer_h2->Add(chk_enablealock, 0, wxALL, 5);
	sizer_h2->Add(0, 0, 1, wxALL, 0);

	sizer_h3->Add(txt_time_h, 0, wxLEFT | wxTOP | wxBOTTOM, 5);
	sizer_h3->Add(txt_time_m, 0, wxRIGHT | wxTOP | wxBOTTOM, 5);
	sizer_h3->Add(btn_addtime, 0, wxALL, 5);
	sizer_h3->Add(btn_deltime, 0, wxALL, 5);

	sizer_h5->Add(list_times, 0, wxALL | wxEXPAND, 5);

	sizer_v1->Add(sizer_h1, 0, wxALL, 0);
	sizer_v1->Add(sizer_h2, 0, wxALL | wxEXPAND, 0);
	sizer_v1->Add(chk_askpwdtolock, 0, wxALL, 5);
	sizer_v1->Add(chk_askpwdtoshutdown, 0, wxALL, 5);
	sizer_v1->Add(chk_lockkeys, 0, wxALL, 5);
	sizer_v1->Add(chk_lockonstartup, 0, wxALL, 5);
	sizer_v1->Add(chk_lockonlogon, 0, wxALL, 5);
	sizer_v1->Add(chk_lockontime, 0, wxALL, 5);
	sizer_v1->Add(sizer_h3, 0, wxALL | wxEXPAND, 0);
	sizer_v1->Add(sizer_h5, 0, wxALL | wxEXPAND, 0);

	SetSizer(sizer_v1);
	SetScrollRate(5, 5);
}

// Return a string with all checked tweaks
wxString PanelLocking::GetChecked(void)
{
	wxString Checked(wxT(""));

	if (chk_enablealock->IsChecked())		Checked.Append(wxT("0,"));
	if (chk_askpwdtolock->IsChecked())		Checked.Append(wxT("1,"));
	if (chk_askpwdtoshutdown->IsChecked())	Checked.Append(wxT("2,"));
	if (chk_lockkeys->IsChecked())			Checked.Append(wxT("3,"));
	if (chk_lockonstartup->IsChecked())		Checked.Append(wxT("4,"));
	if (chk_lockonlogon->IsChecked())		Checked.Append(wxT("5,"));
	if (chk_lockontime->IsChecked())		Checked.Append(wxT("6,"));

	return Checked;
}

// Return a string with all unchecked tweaks
wxString PanelLocking::GetUnChecked(void)
{
	wxString UnChecked(wxT(""));

	if (!chk_enablealock->IsChecked())		UnChecked.Append(wxT("0,"));
	if (!chk_askpwdtolock->IsChecked())		UnChecked.Append(wxT("1,"));
	if (!chk_askpwdtoshutdown->IsChecked())	UnChecked.Append(wxT("2,"));
	if (!chk_lockkeys->IsChecked())			UnChecked.Append(wxT("3,"));
	if (!chk_lockonstartup->IsChecked())	UnChecked.Append(wxT("4,"));
	if (!chk_lockonlogon->IsChecked())		UnChecked.Append(wxT("5,"));
	if (!chk_lockontime->IsChecked())		UnChecked.Append(wxT("6,"));

	return UnChecked;
}

// Return a string with all times that are set
wxString PanelLocking::GetTimes(void)
{
	int h, m;
	wxString Times(wxT(""));

	for (unsigned int i = 0; i < list_times->GetCount(); i++)
	{
		h = strtol((char *)list_times->GetString(i).BeforeFirst(':').c_str(), NULL, 10);
		m = strtol((char *)list_times->GetString(i).AfterFirst(':').c_str(), NULL, 10);

		Times.Append(wxString::Format(wxT("%d"), h * 60 + m));
	}

	return Times;
}

// Marca/desmarca restri��es de acordo com a string
void PanelLocking::SetChecked(wxString Checks, bool check)
{
	vector<wxString> opt = Split(Checks, wxT(','), -1);

	for (unsigned int i = 0; i < opt.size(); i++)
	{
		switch (atoi(opt[i].mb_str()))
		{
		case 1: chk_askpwdtolock->SetValue(check);		break;
		case 2: chk_askpwdtoshutdown->SetValue(check);	break;
		case 3: chk_lockkeys->SetValue(check);			break;
		case 4: chk_lockonstartup->SetValue(check);		break;
		case 5: chk_lockonlogon->SetValue(check);		break;
		case 6: chk_lockontime->SetValue(check);		break;
		}
	}
}

// Limpa a lista de hor�rios e preenche com a lista nova
void PanelLocking::SetTimes(wxString Times)
{
	vector<wxString> NewTimes = Split(Times, wxT(','), -1);

	list_times->Clear();
	for (unsigned int i = 0; i < NewTimes.size(); i++)
		AddTimeMin(atoi(NewTimes[i].mb_str()));
}

// AddTimeMin method
void PanelLocking::AddTimeMin(int t_minutes)
{
	int time_h, time_m;
	wxString str_time;

	time_h = t_minutes / 60;
	time_m = t_minutes % 60;

	str_time.Printf(wxT("%2d:%2d"), time_h, time_m);
	str_time.Replace(wxT(" "), wxT("0"));

	if (list_times->FindString(str_time) != wxNOT_FOUND)
		wxMessageBox(_("O hor�rio digitado j� est� na lista."), APP_NAME, wxICON_EXCLAMATION);
	else
		list_times->Append(str_time);
}

// AddTime method
void PanelLocking::AddTime(wxCommandEvent &event)
{
	unsigned int time_h, time_m;

	time_h = strtol(txt_time_h->GetValue().mb_str(), NULL, 10);
	time_m = strtol(txt_time_m->GetValue().mb_str(), NULL, 10);

	AddTimeMin(time_h * 60 + time_m);

	txt_time_h->Clear();
	txt_time_m->Clear();
}

// DelTime method
void PanelLocking::DelTime(wxCommandEvent &event)
{
	wxArrayInt sels;
	int i;

	list_times->GetSelections((wxArrayInt&)sels);
	if (sels.GetCount() == 0)
	{
		wxMessageBox(_("Por favor, selecione pelo menos um hor�rio na lista antes de prosseguir."), APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	for (i = sels.GetCount() - 1; i >= 0; i--)
		list_times->Delete(sels.Item(i));
}
