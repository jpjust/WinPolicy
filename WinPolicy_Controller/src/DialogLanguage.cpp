/* DialogLanguage class
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

#include <wx/fileconf.h>
#include "DialogLanguage.h"
#include "ControllerBase.h"

BEGIN_EVENT_TABLE(DialogLanguage, wxDialog)
	EVT_BUTTON(ID_DIALOGLANGUAGE_BTN_OK, DialogLanguage::OnOKClick)
END_EVENT_TABLE()

// Construtor
DialogLanguage::DialogLanguage(void)
	:wxDialog(NULL, wxID_ANY, APP_NAME)
{
	wxBoxSizer *sizer_v1 = new wxBoxSizer(wxVERTICAL);

	lb_title = new wxStaticText(this, wxID_ANY,
		_("Escolha o seu idioma na caixa de sele��o\n" \
		  "abaixo e clique em \"OK\"."));

	combo_languages = new wxComboBox(this, wxID_ANY);
	combo_languages->Append(wxT("Portugu�s"));
	combo_languages->Append(wxT("English"));

	btn_ok = new wxButton(this, ID_DIALOGLANGUAGE_BTN_OK, wxT("OK"));

	// Sizer
	sizer_v1->Add(lb_title, 0, wxALL | wxEXPAND, 5);
	sizer_v1->Add(combo_languages, 0, wxALL | wxALIGN_CENTER, 5);
	sizer_v1->Add(btn_ok, 0, wxALL | wxALIGN_CENTER, 5);

	SetSizerAndFit(sizer_v1);
}

// Clique no bot�o OK
void DialogLanguage::OnOKClick(wxCommandEvent &event)
{
	int lang;

	switch (combo_languages->GetSelection())
	{
	case 0:
		lang = wxLANGUAGE_UNKNOWN;
		break;

	case 1:
		lang = wxLANGUAGE_ENGLISH_US;
		break;

	default:
		lang = wxLANGUAGE_ENGLISH_US;
		break;
	}

	wxFileConfig fc(wxEmptyString, wxEmptyString, CONF_FILENAME, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
	fc.Write(wxT("/Configuration/Language"), lang);
	fc.Flush();

	wxMessageBox(_("O novo idioma entrar� em vigor na pr�xima vez que voc� iniciar o Controller."), APP_NAME, wxICON_INFORMATION);
	Close();
}
