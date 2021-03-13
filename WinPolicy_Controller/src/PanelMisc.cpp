/* PanelMisc class
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

#include "PanelMisc.h"
#include "ControllerBase.h"
#include <WPNP.h>
#include <wx/image.h>
#include <vector>

using namespace std;

// Class constructor
PanelMisc::PanelMisc(wxWindow *parent)
	:wxScrolledWindow(parent)
{
	wxStaticBox *statbox = new wxStaticBox(this, -1, _("Outros"));
	wxStaticBoxSizer *sizer_v1 = new wxStaticBoxSizer(statbox, wxVERTICAL);
	wxBoxSizer *sizer_h1 = new wxBoxSizer(wxHORIZONTAL);

	label_title = new wxStaticText(this, -1, _("Use as caixas abaixo para habilitar outras op��es de\n" \
											 "restri��o do WinPolicy."));

	wxImage png(ControllerBase::GetAppDir() + wxT("/images/misc.png"), wxBITMAP_TYPE_PNG);
#ifdef __WXMSW__
	int osmaj, osmin, osmm;
	wxGetOsVersion(&osmaj, &osmin);
	osmm = osmaj * 100 + osmin;
	if (osmm < 501)
		png.ConvertAlphaToMask();
#endif
	bitmap = new wxStaticBitmap(this, -1, wxBitmap(png));

	label_hidedisks = new wxStaticText(this, -1, _("Ocultar unidades de disco (digite as letras, ex.: ACDF)"));
	label_denydisks = new wxStaticText(this, -1, _("Negar acesso � unidades de disco (Windows Me, 2000 e XP)"));
	label_win9xonly = new wxStaticText(this, -1, _("Apenas Windows 95 e 98:"));
	label_win9xonly->SetOwnForegroundColour(wxColour(255, 0, 0));
	label_win9xonly->SetOwnFont(wxFont(-1, wxDEFAULT, -1, wxBOLD));
	label_win9xmeonly = new wxStaticText(this, -1, _("Apenas Windows 95, 98 e Me:"));
	label_win9xmeonly->SetOwnForegroundColour(wxColour(255, 0, 0));
	label_win9xmeonly->SetOwnFont(wxFont(-1, wxDEFAULT, -1, wxBOLD));

	chk_startpage = new wxCheckBox(this, -1, _("Impedir a mudan�a da p�gina inicial do Internet Explorer"));
	chk_dosprog = new wxCheckBox(this, -1, _("Impedir a execu��o de programas baseados no MS-DOS"));
	chk_dosmode = new wxCheckBox(this, -1, _("Impedir a reinicializa��o no modo MS-DOS"));
	chk_f8menu = new wxCheckBox(this, -1, _("Impedir o menu das teclas F5 e F8 na inicializa��o"));
	chk_forcelogon = new wxCheckBox(this, -1, _("For�ar o usu�rio a efetuar logon local"));
	chk_requirepwd = new wxCheckBox(this, -1, _("Requerer senha de rede v�lida para acessar o Windows"));

	txt_hidedisks = new wxTextCtrl(this, -1);
	txt_denydisks = new wxTextCtrl(this, -1);

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
}

// Return a string with all checked tweaks
wxString PanelMisc::GetChecked(void)
{
	wxString Checked(wxT(""));

	if (chk_startpage->IsChecked())		Checked.Append(wxT("0,"));
	if (chk_dosprog->IsChecked())		Checked.Append(wxT("1,"));
	if (chk_dosmode->IsChecked())		Checked.Append(wxT("2,"));
	if (chk_f8menu->IsChecked())		Checked.Append(wxT("3,"));
	if (chk_forcelogon->IsChecked())	Checked.Append(wxT("4,"));
	if (chk_requirepwd->IsChecked())	Checked.Append(wxT("5,"));

	return Checked;
}

// Return a string with all unchecked tweaks
wxString PanelMisc::GetUnChecked(void)
{
	wxString UnChecked(wxT(""));

	if (!chk_startpage->IsChecked())	UnChecked.Append(wxT("0,"));
	if (!chk_dosprog->IsChecked())		UnChecked.Append(wxT("1,"));
	if (!chk_dosmode->IsChecked())		UnChecked.Append(wxT("2,"));
	if (!chk_f8menu->IsChecked())		UnChecked.Append(wxT("3,"));
	if (!chk_forcelogon->IsChecked())	UnChecked.Append(wxT("4,"));
	if (!chk_requirepwd->IsChecked())	UnChecked.Append(wxT("5,"));

	return UnChecked;
}

// Return the hidden drives number
wxString PanelMisc::GetHiddenDrv(void)
{
	return txt_hidedisks->GetValue();
}

// Return the denied drives number
wxString PanelMisc::GetDeniedDrv(void)
{
	return txt_denydisks->GetValue();
}

// Marca/desmarca restri��es de acordo com a string
void PanelMisc::SetChecked(wxString Checks, bool check)
{
	vector<wxString> opt = Split(Checks, wxT(','), -1);

	for (unsigned int i = 0; i < opt.size(); i++)
	{
		switch (atoi(opt[i].mb_str()))
		{
		case 0: chk_startpage->SetValue(check);		break;
		case 1: chk_dosprog->SetValue(check);		break;
		case 2: chk_dosmode->SetValue(check);		break;
		case 3: chk_f8menu->SetValue(check);		break;
		case 4: chk_forcelogon->SetValue(check);	break;
		case 5: chk_requirepwd->SetValue(check);	break;
		}
	}
}

// Preenche a caixa de texto dos drives ocultos
void PanelMisc::SetHiddenDrv(wxString Drives)
{
	txt_hidedisks->SetValue(Drives);
}

// Preenche a caixa de texto dos drives restritos
void PanelMisc::SetDeniedDrv(wxString Drives)
{
	txt_denydisks->SetValue(Drives);
}
