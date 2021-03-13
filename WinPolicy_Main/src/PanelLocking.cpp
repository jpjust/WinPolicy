/* Locking panel
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

#include "PanelLocking.h"
#include "AppBase.h"
#include "jswinreg.h"
#include <windows.h>
#include <stdlib.h>
#include <wx/image.h>

BEGIN_EVENT_TABLE(PanelLocking, wxScrolledWindow)
	EVT_CHECKBOX(ID_PANELLOCKING_CHK_AUTOLOCK, PanelLocking::AutoLockClick)
	EVT_BUTTON(ID_PANELLOCKING_BTN_LOCKNOW, PanelLocking::LockNow)
	EVT_BUTTON(ID_PANELLOCKING_BTN_ADDTIME, PanelLocking::AddTime)
	EVT_BUTTON(ID_PANELLOCKING_BTN_DELTIME, PanelLocking::DelTime)
END_EVENT_TABLE()

// Class constructor
PanelLocking::PanelLocking(wxWindow *parent)
	:wxScrolledWindow(parent)
{
	wxStaticBox *statbox = new wxStaticBox(this, -1, "Bloqueio");
	wxStaticBoxSizer *sizer_v1 = new wxStaticBoxSizer(statbox, wxVERTICAL);
	wxBoxSizer *sizer_h1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h5 = new wxBoxSizer(wxHORIZONTAL);

	description = "Use as op��es abaixo para configurar o sistema de\n" \
				  "bloqueio do WinPolicy e programar os hor�rios de bloqueio.";
	label_title = new wxStaticText(this, -1, description);

	bitmap = new wxStaticBitmap(this, -1, AppBase::LoadImage(wxT("clock.png")));

	label_ssaver = new wxStaticText(this, -1, "Voc� tamb�m pode configurar o\n" \
											  "WinPolicy para bloquear o Windows\n" \
											  "automaticamente caso o computador\n" \
											  "esteja ocioso. Para fazer isto, basta\n" \
											  "abrir as Propriedades de V�deo no\n" \
											  "Painel de Controle e configurar a\n" \
											  "Prote��o de Tela do WinPolicy.");

	// Widgets that can get focus are created in the correct order
	chk_enablealock = new wxCheckBox(this, ID_PANELLOCKING_CHK_AUTOLOCK, "Ativar o Autolock");
	btn_locknow = new wxButton(this, ID_PANELLOCKING_BTN_LOCKNOW, "Bloquear o computador agora");
	chk_askpwdtolock = new wxCheckBox(this, -1, "Solicitar senha para bloquear o computador pelo Autolock");
	chk_askpwdtoshutdown = new wxCheckBox(this, -1, "Solicitar senha para desligar/efetuar logoff quando bloqueado");
	chk_lockkeys = new wxCheckBox(this, -1, "Desativar teclas do sistema (CTRL, ALT, TAB, WIN e ESC)");
	chk_lockonstartup = new wxCheckBox(this, -1, "Bloquear automaticamente ao iniciar o Windows (95, 98 e Me)");
	chk_lockonlogon = new wxCheckBox(this, -1, "Bloquear o computador ap�s o logon");
	chk_lockontime = new wxCheckBox(this, -1, "Bloquear automaticamente nos hor�rios abaixo:");
	txt_time_h = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxSize(30, -1));
	txt_time_m = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxSize(30, -1));
	btn_addtime = new wxButton(this, ID_PANELLOCKING_BTN_ADDTIME, "Adicionar");
	btn_deltime = new wxButton(this, ID_PANELLOCKING_BTN_DELTIME, "Remover");
	list_times = new wxListBox(this, -1, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL | wxLB_EXTENDED);

	// Layout development
	sizer_h1->Add(bitmap, 0, wxALL, 5);
	sizer_h1->Add(label_title, 0, wxALL, 5);

	sizer_h2->Add(chk_enablealock, 0, wxALL, 5);
	sizer_h2->Add(0, 0, 1, wxALL, 0);
	sizer_h2->Add(btn_locknow, 0, wxALL, 5);

	sizer_h3->Add(txt_time_h, 0, wxLEFT | wxTOP | wxBOTTOM, 5);
	sizer_h3->Add(txt_time_m, 0, wxRIGHT | wxTOP | wxBOTTOM, 5);
	sizer_h3->Add(btn_addtime, 0, wxALL, 5);
	sizer_h3->Add(btn_deltime, 0, wxALL, 5);

	sizer_h5->Add(list_times, 1, wxALL | wxEXPAND, 5);
	sizer_h5->Add(label_ssaver, 0, wxALL, 5);

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

	GetTweaks();
}

// GetTweaks method
void PanelLocking::GetTweaks(void)
{
	int i, time_h, time_m;
	wxString str_time, list_item;

	// When getting tweaks, the AutoLockClick message box must not be shown
	GettingTweak = true;

	list_times->Clear();

	chk_enablealock->SetValue(Tweaks.GetTALockEnabled());
	chk_askpwdtolock->SetValue(Tweaks.GetTPwdToLock());
	chk_askpwdtoshutdown->SetValue(Tweaks.GetTPwdToShutdown());
	chk_lockkeys->SetValue(Tweaks.GetTLockKeys());
	chk_lockonstartup->SetValue(Tweaks.GetTLockOnStartup());
	chk_lockonlogon->SetValue(Tweaks.GetTLockOnLogon());
	chk_lockontime->SetValue(Tweaks.GetTLockOnTime());

	for (i = 0; i < Tweaks.GetTimeCount(); i++)
	{
		time_h = Tweaks.GetTime(i).GetHour();
		time_m = Tweaks.GetTime(i).GetMinute();

		list_item.Printf("%2d:%2d", time_h, time_m);
		list_item.Replace(" ", "0");
		list_times->Append(list_item);
	}

	// Execute the AutoLockClick event to refresh the checkboxes status
	wxCommandEvent event(wxEVT_COMMAND_CHECKBOX_CLICKED, ID_PANELLOCKING_CHK_AUTOLOCK);
	AutoLockClick(event);

	GettingTweak = false;
}

// ApplyTweaks method
void PanelLocking::ApplyTweaks(void)
{
	unsigned int time_h, time_m;
	wxString str_time, key, value;

	Tweaks.SetTALockEnabled(chk_enablealock->IsChecked());
	Tweaks.SetTPwdToLock(chk_askpwdtolock->IsChecked());
	Tweaks.SetTPwdToShutdown(chk_askpwdtoshutdown->IsChecked());
	Tweaks.SetTLockKeys(chk_lockkeys->IsChecked());
	Tweaks.SetTLockOnStartup(chk_lockonstartup->IsChecked());
	Tweaks.SetTLockOnLogon(chk_lockonlogon->IsChecked());
	Tweaks.SetTLockOnTime(chk_lockontime->IsChecked());

	Tweaks.ClearTimes();
	for (unsigned i = 0; i < list_times->GetCount(); i++)
	{
		str_time = list_times->GetString(i);
		time_h = strtol(str_time.BeforeFirst(':').mb_str(), NULL, 10);
		time_m = strtol(str_time.AfterLast(':').mb_str(), NULL, 10);

		Tweaks.AddTime(wxDateTime(time_h, time_m));
	}
}

// SaveTweaks method
bool PanelLocking::SaveTweaks(void)
{
	int error = 0;

	ApplyTweaks();
	error += BOOL2INT(!Tweaks.SaveTweaks());

	if (error > 0)
		return false;
	else
		return true;
}

// SaveProfile method
bool PanelLocking::SaveProfile(wxString filename)
{
	ApplyTweaks();
	return Tweaks.SaveToProfile(filename);
}

// LoadProfile method
bool PanelLocking::LoadProfile(wxString filename)
{
	if (Tweaks.LoadFromProfile(filename) == false)
		return false;

	GetTweaks();

	return true;
}

// AutoLockClick method
void PanelLocking::AutoLockClick(wxCommandEvent &event)
{
	if (chk_enablealock->IsChecked())
	{
		chk_askpwdtolock->Enable(true);
		chk_askpwdtoshutdown->Enable(true);
		chk_lockkeys->Enable(true);
		chk_lockontime->Enable(true);
	}
	else
	{
		if (GettingTweak == false)
		{
			if (wxMessageBox("Desativando o �cone do Autolock, os bloqueios programados n�o ter�o " \
				"efeito. Se quiser que o computador seja bloqueado nos hor�rios programados (ou na " \
				"inicializa��o, se configurado), voc� deve manter o �cone do Autolock.\n\n" \
				"Tem certeza que deseja remov�-lo?", APP_NAME, wxYES_NO | wxICON_QUESTION) == wxNO)
			{
				chk_enablealock->SetValue(true);
				return;
			}
		}

		chk_askpwdtolock->Enable(false);
		chk_askpwdtoshutdown->Enable(false);
		chk_lockkeys->Enable(false);
		chk_lockontime->Enable(false);
	}
}

// LockNow method
void PanelLocking::LockNow(wxCommandEvent &event)
{
	AppBase::LockNow(true);
}

// AddTimeMin method
void PanelLocking::AddTimeMin(int t_minutes)
{
	int time_h, time_m;
	wxString str_time;

	time_h = t_minutes / 60;
	time_m = t_minutes % 60;

	str_time.Printf("%2d:%2d", time_h, time_m);
	str_time.Replace(" ", "0");

	if (list_times->FindString(str_time) != wxNOT_FOUND)
		wxMessageBox("O hor�rio digitado j� est� na lista.", APP_NAME, wxICON_EXCLAMATION);
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
		wxMessageBox("Por favor, selecione pelo menos um hor�rio na lista antes de prosseguir.", APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	for (i = sels.GetCount() - 1; i >= 0; i--)
		list_times->Delete(sels.Item(i));
}
