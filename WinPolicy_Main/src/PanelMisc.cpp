/* Misc panel
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

#include "PanelMisc.h"
#include "AppBase.h"
#include "jswinreg.h"
#include <wx/image.h>

// Class constructor
PanelMisc::PanelMisc(wxWindow *parent)
	:wxScrolledWindow(parent)
{
	wxStaticBox *statbox = new wxStaticBox(this, -1, "Outros");
	wxStaticBoxSizer *sizer_v1 = new wxStaticBoxSizer(statbox, wxVERTICAL);
	wxBoxSizer *sizer_h1 = new wxBoxSizer(wxHORIZONTAL);

	label_title = new wxStaticText(this, -1, "Use as caixas abaixo para habilitar outras op��es de\n" \
											 "restri��o do WinPolicy.");

	bitmap = new wxStaticBitmap(this, -1, AppBase::LoadImage(wxT("misc.png")));

	label_hidedisks = new wxStaticText(this, -1, "Ocultar unidades de disco (digite as letras, ex.: ACDF)");
	label_denydisks = new wxStaticText(this, -1, "Negar acesso � unidades de disco (Windows Me, 2000 e XP)");
	label_win9xonly = new wxStaticText(this, -1, "Apenas Windows 95 e 98:");
	label_win9xonly->SetOwnForegroundColour(wxColour(255, 0, 0));
	label_win9xonly->SetOwnFont(wxFont(-1, wxDEFAULT, -1, wxBOLD));
	label_win9xmeonly = new wxStaticText(this, -1, "Apenas Windows 95, 98 e Me:");
	label_win9xmeonly->SetOwnForegroundColour(wxColour(255, 0, 0));
	label_win9xmeonly->SetOwnFont(wxFont(-1, wxDEFAULT, -1, wxBOLD));

	chk_startpage = new wxCheckBox(this, -1, "Impedir a mudan�a da p�gina inicial do Internet Explorer");
	chk_dosprog = new wxCheckBox(this, -1, "Impedir a execu��o de programas baseados no MS-DOS");
	chk_dosmode = new wxCheckBox(this, -1, "Impedir a reinicializa��o no modo MS-DOS");
	chk_f8menu = new wxCheckBox(this, -1, "Impedir o menu das teclas F5 e F8 na inicializa��o");
	chk_forcelogon = new wxCheckBox(this, -1, "For�ar o usu�rio a efetuar logon local");
	chk_requirepwd = new wxCheckBox(this, -1, "Requerer senha de rede v�lida para acessar o Windows");

	txt_hidedisks = new wxTextCtrl(this, -1);
	txt_denydisks = new wxTextCtrl(this, -1);

	// Enable/disable some options according to Windows version
	if (AppBase::GetAllVer() == false)
	{
		if (!wxString("me2kxp").Contains(AppBase::GetWinVerStr()))
			txt_denydisks->Enable(false);

		if (!wxString("9598").Contains(AppBase::GetWinVerStr()))
			chk_f8menu->Enable(false);

		if (!wxString("9598me").Contains(AppBase::GetWinVerStr()))
		{
			chk_forcelogon->Enable(false);
			chk_requirepwd->Enable(false);
		}
	}

	// Layout development
	sizer_h1->Add(bitmap, 0, wxALL, 5);
	sizer_h1->Add(label_title, 0, wxALL, 5);

	sizer_v1->Add(sizer_h1, 0, wxALL, 0);
	sizer_v1->Add(chk_startpage, 0, wxALL, 5);
	sizer_v1->Add(chk_dosprog, 0, wxALL, 5);
	sizer_v1->Add(chk_dosmode, 0, wxALL, 5);
	sizer_v1->Add(label_hidedisks, 0, wxALL, 5);
	sizer_v1->Add(txt_hidedisks, 0, wxALL | wxEXPAND, 5);
	sizer_v1->Add(label_denydisks, 0, wxALL, 5);
	sizer_v1->Add(txt_denydisks, 0, wxALL | wxEXPAND, 5);
	sizer_v1->Add(label_win9xonly, 0, wxALL, 5);
	sizer_v1->Add(chk_f8menu, 0, wxALL, 5);
	sizer_v1->Add(label_win9xmeonly, 0, wxALL, 5);
	sizer_v1->Add(chk_forcelogon, 0, wxALL, 5);
	sizer_v1->Add(chk_requirepwd, 0, wxALL, 5);

	SetSizer(sizer_v1);
	SetScrollRate(5, 5);

	GetTweaks();
}

// GetTweaks method
void PanelMisc::GetTweaks(void)
{
	chk_startpage->SetValue(Tweaks.GetTNoIEStartPage());
	chk_dosprog->SetValue(Tweaks.GetTNoDOSRun());
	chk_dosmode->SetValue(Tweaks.GetTNoDOSMode());

	txt_hidedisks->SetValue(Tweaks.GetDrivesStr(Tweaks.GetTHiddenDrives()));

	if (txt_denydisks->IsEnabled())
		txt_denydisks->SetValue(Tweaks.GetDrivesStr(Tweaks.GetTDeniedDrives()));

	if (chk_f8menu->IsEnabled())
		chk_f8menu->SetValue(Tweaks.GetTNoF8());

	if (chk_forcelogon->IsEnabled())
		chk_forcelogon->SetValue(Tweaks.GetTForceLogon());

	if (chk_requirepwd->IsEnabled())
		chk_requirepwd->SetValue(Tweaks.GetTRequirePassword());
}

// ApplyTweaks method
void PanelMisc::ApplyTweaks(void)
{
	Tweaks.SetTNoIEStartPage(chk_startpage->GetValue());
	Tweaks.SetTNoDOSRun(chk_dosprog->GetValue());
	Tweaks.SetTNoDOSMode(chk_dosmode->GetValue());

	wxString strDrives = txt_hidedisks->GetValue();
	double drives = Tweaks.GetDrivesNum(&strDrives);
	txt_hidedisks->SetValue(strDrives);
	Tweaks.SetTHiddenDrives(drives);

	if (txt_denydisks->IsEnabled())
	{
		strDrives = txt_denydisks->GetValue();
		drives = Tweaks.GetDrivesNum(&strDrives);
		txt_denydisks->SetValue(strDrives);
		Tweaks.SetTDeniedDrives(drives);
	}

	if (chk_f8menu->IsEnabled())
		Tweaks.SetTNoF8(chk_f8menu->GetValue());

	if (chk_forcelogon->IsEnabled())
		Tweaks.SetTForceLogon(chk_forcelogon->GetValue());

	if (chk_requirepwd->IsEnabled())
		Tweaks.SetTRequirePassword(chk_requirepwd->GetValue());
}

// SaveTweaks method
bool PanelMisc::SaveTweaks(bool hku, wxArrayString Users, bool thisuser)
{
	ApplyTweaks();
	return Tweaks.SaveTweaks(hku, Users, thisuser);
}

// SaveProfile method
bool PanelMisc::SaveProfile(wxString filename)
{
	ApplyTweaks();
	return Tweaks.SaveToProfile(filename);
}

// LoadProfile method
bool PanelMisc::LoadProfile(wxString filename)
{
	if (Tweaks.LoadFromProfile(filename) == false)
		return false;

	GetTweaks();

	return true;
}
