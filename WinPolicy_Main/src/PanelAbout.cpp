/* About panel
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

#include "PanelAbout.h"
#include "AppBase.h"
#include "DialogLicense.h"
#include <wx/image.h>

BEGIN_EVENT_TABLE(PanelAbout, wxPanel)
	EVT_BUTTON(ID_PANELABOUT_BTN_LICENSE, PanelAbout::ShowLicense)
END_EVENT_TABLE()

// Class constructor
PanelAbout::PanelAbout(wxWindow *parent)
	:wxPanel(parent)
{
	wxStaticBox *statbox = new wxStaticBox(this, -1, "Sobre");
	wxStaticBoxSizer *sizer_v1 = new wxStaticBoxSizer(statbox, wxVERTICAL);
	wxBoxSizer *sizer_v2 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *sizer_h1 = new wxBoxSizer(wxHORIZONTAL);

	label_appname = new wxStaticText(this, -1, APP_NAME);
	label_copyright = new wxStaticText(this, -1, "Copyright (c) 2001 - 2007 Justsoft Inform�tica Ltda.e\n" \
												 "Todos os direitos reservados.");

#ifdef _WINPOLICY_FREEWARE_
	label_legalnotice = new wxStaticText(this, -1, "O Justsoft WinPolicy Freeware � um software de distribui��o gratuita\n" \
												   "(Freeware). Voc� pode distribu�-lo e instal�-lo em um n�mero infinito\n" \
												   "de computadores, bastando apenas obedecer �s leis de copyright que o\n" \
												   "protegem.");
#else
	label_legalnotice = new wxStaticText(this, -1, "O Justsoft WinPolicy est� licenciado sob os termos do seu contrato de\n" \
												   "licen�a de uso que foi exibido durante sua instala��o. Voc� deve estar\n" \
												   "de acordo com este contrato para utilizar o software.");
#endif

	label_url = new wxStaticText(this, -1, "http://www.justsoft.com.br/winpolicy/");
	label_url->SetOwnForegroundColour(wxColour(0, 0, 255));
	label_url->SetOwnFont(wxFont(-1, wxDEFAULT, -1, wxBOLD));
	label_mscopyright = new wxStaticText(this, -1, "MS-DOS�, Windows�, Internet Explorer� e MSN Messenger� s�o\n" \
												   "marcas registradas da Microsoft Corp.");

	bitmap = new wxStaticBitmap(this, -1, AppBase::LoadImage(wxT("about.png")));

	btn_license = new wxButton(this, ID_PANELABOUT_BTN_LICENSE, "Contrato de Licen�a Justsoft");
	txt_softwares = new wxTextCtrl(this, -1, "O Justsoft WinPolicy usa os seguintes softwares:\n" \
		"\n" \
		"1. wxWidgets 2.8.6\n" \
		"wxWidgets � uma framework Open Source e multiplataforma para cria��o de interfaces " \
		"gr�ficas em C++.\n" \
		"http://www.wxwidgets.org/\n" \
		"\n" \
		"2. GNU Multiple Precision Arithmetic Library 4.2.2\n" \
		"GMP � uma biblioteca livre para aritim�tica de precis�o arbitr�ria, operando em inteiros " \
		"com sinal, n�meros racionais e pontos flutuantes. N�o h� limites para a precis�o, a n�o " \
		"ser a quantidade de mem�ria da m�quina.\n" \
		"http://www.swox.com/gmp\n" \
		"\n" \
		"3. RSA Tool 0.6\n" \
		"RSA Tool � uma biblioteca livre utilizada para criptografia RSA. Desenvolvida por Jo�o Paulo " \
		"Just.\n" \
		"http://jpjust.blogspot.com/\n" \
		"\n" \
		"4. Nuvola Icon Theme for KDE 3.x 1.0\n" \
		"Nuvola � uma cole��o de �cones em formato PNG, inicialmente desenvolvidos para o KDE 3, por David Vignoni.\n" \
		"http://www.icon-king.com/\n", \
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_BESTWRAP);

	// Layout development
	sizer_v2->Add(label_appname, 0, wxALL, 5);
	sizer_v2->Add(label_copyright, 0, wxALL, 5);

	sizer_h1->Add(bitmap, 0, wxALL, 5);
	sizer_h1->Add(sizer_v2, 0, wxALL, 0);

	sizer_v1->Add(sizer_h1, 0, wxBOTTOM, 5);
	sizer_v1->Add(label_legalnotice, 0, wxTOP, 5);
	sizer_v1->Add(label_url, 0, wxALL, 5);
	sizer_v1->Add(btn_license, 0, wxALL | wxCENTRE, 5);
	sizer_v1->Add(txt_softwares, 1, wxALL | wxCENTRE | wxEXPAND, 5);
	sizer_v1->Add(label_mscopyright, 0, wxALL, 5);

	SetSizer(sizer_v1);
}

// ShowLicense method
void PanelAbout::ShowLicense(wxCommandEvent &event)
{
	DialogLicense *dialog = new DialogLicense();
	dialog->ShowModal();
	delete dialog;
}
