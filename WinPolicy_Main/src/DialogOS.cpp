/* Operating System selection dialog
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

#include "DialogOS.h"
#include "AppBase.h"
#include "jswinreg.h"

BEGIN_EVENT_TABLE(DialogOS, wxDialog)
	EVT_BUTTON(ID_DIALOGOS_BTN_OK, DialogOS::SaveSettings)
END_EVENT_TABLE()

// Class constructor
DialogOS::DialogOS(void)
	:wxDialog(NULL, -1, "Sistema Operacional", wxDefaultPosition, wxDefaultSize)
{
	wxBoxSizer *sizer_v1 = new wxBoxSizer(wxVERTICAL);
	wxArrayString radio_array;

	radio_array.Add("Windows 95");
	radio_array.Add("Windows 98");
	radio_array.Add("Windows Me");
	radio_array.Add("Windows NT");
	radio_array.Add("Windows 2000/2003");
	radio_array.Add("Windows XP/Vista");

	label_title = new wxStaticText(this, -1, "Use esta janela para selecionar o sistema " \
											 "operacional onde o WinPolicy est� instalado.");
	btn_ok = new wxButton(this, ID_DIALOGOS_BTN_OK, "OK");
	radiobox = new wxRadioBox(this, -1, "Op��es", wxDefaultPosition, wxDefaultSize, radio_array, 2);
	chk_all = new wxCheckBox(this, -1, "Exibir op��es de todas as vers�es");

	label_title->Wrap(radiobox->GetSize().GetWidth());

	// Layout development
	sizer_v1->Add(label_title, 0, wxALL | wxEXPAND, 5);
	sizer_v1->Add(radiobox, 1, wxALL | wxEXPAND, 5);
	sizer_v1->Add(chk_all, 0, wxALL | wxEXPAND, 5);
	sizer_v1->Add(btn_ok, 0, wxALL | wxCENTER, 5);

	SetSizerAndFit(sizer_v1);

	// Gets the selected Windows version
	int winver = AppBase::GetWinVer(false) - WINVER_95;

	if ((winver < 0) || (winver > 5))
		winver = 0;

	radiobox->SetSelection(winver);
	chk_all->SetValue(AppBase::GetAllVer());
}

// Saves the settings
void DialogOS::SaveSettings(wxCommandEvent& event)
{
	int winver, error = 0;

	winver = radiobox->GetSelection() + WINVER_95;

	error += BOOL2INT(!AppBase::SetWinVer(winver));
	error += BOOL2INT(!AppBase::SetAllVer(chk_all->IsChecked()));

	if (error > 0)
	{
		wxMessageBox("Erro ao salvar a configura��o!", APP_NAME, wxICON_ERROR);
		return;
	}

	wxMessageBox("� preciso reiniciar o WinPolicy para que a altera��o fa�a efeito.", APP_NAME, wxICON_INFORMATION);

	Close(true);
}
