/* DialogAbout class
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

#include "DialogAbout.h"
#include "ControllerBase.h"
#include "DialogLicense.h"
#include <wx/image.h>

BEGIN_EVENT_TABLE(DialogAbout, wxDialog)
	EVT_BUTTON(ID_DIALOGABOUT_BTN_WEB, DialogAbout::OpenWeb)
	EVT_BUTTON(ID_DIALOGABOUT_BTN_LICENSE, DialogAbout::ShowLicense)
END_EVENT_TABLE()

// Class constructor
DialogAbout::DialogAbout(void)
	:wxDialog(NULL, wxID_ANY, APP_NAME, wxDefaultPosition, wxDefaultSize)
{
	wxBoxSizer *sizer_v1 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *sizer_v2 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *sizer_h1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h2 = new wxBoxSizer(wxHORIZONTAL);

	label_appname = new wxStaticText(this, -1, APP_NAME);
	label_copyright = new wxStaticText(this, -1, _("Copyright (c) 2006 - 2007 Justsoft Inform�tica Ltda.\n" \
												     "Todos os direitos reservados."));
	label_legalnotice = new wxStaticText(this, -1, "O Justsoft WinPolicy Controller est� licenciado sob os termos do seu\n" \
												   "contrato de licen�a de uso que foi exibido durante sua instala��o.\n" \
												   "Voc� deve estar de acordo com este contrato para utilizar o software.");
	label_mscopyright = new wxStaticText(this, -1, _("MS-DOS�, Windows�, Internet Explorer� e MSN Messenger� s�o\n" \
												       "marcas registradas da Microsoft Corp."));

	wxImage png(ControllerBase::GetAppDir() + wxT("/images/about.png"), wxBITMAP_TYPE_PNG);
#ifdef __WXMSW__
	int osmaj, osmin, osmm;
	wxGetOsVersion(&osmaj, &osmin);
	osmm = osmaj * 100 + osmin;
	if (osmm < 501)
		png.ConvertAlphaToMask();
#endif
	bitmap = new wxStaticBitmap(this, -1, wxBitmap(png));

	btn_web = new wxButton(this, ID_DIALOGABOUT_BTN_WEB, wxT("http://www.justsoft.com.br/winpolicy/"));
	btn_license = new wxButton(this, ID_DIALOGABOUT_BTN_LICENSE, _("Contrato de Licen�a Justsoft"));
	txt_softwares = new wxTextCtrl(this, -1, \
	_("O Justsoft WinPolicy usa os seguintes softwares:\n" \
		"\n" \
		"1. wxWidgets 2.8.2\n" \
		"wxWidgets � uma framework Open Source e multiplataforma para cria��o de interfaces " \
		"gr�ficas em C++.\n" \
		"http://www.wxwidgets.org/\n" \
		"\n" \
		"2. GNU Multiple Precision Arithmetic Library 4.2.1\n" \
		"GMP � uma biblioteca livre para aritim�tica de precis�o arbitr�ria, operando em inteiros " \
		"com sinal, n�meros racionais e pontos flutuantes. N�o h� limites para a precis�o, a n�o " \
		"ser a quantidade de mem�ria da m�quina.\n" \
		"http://www.swox.com/gmp\n" \
		"\n" \
		"3. RSA Tool 0.6\n" \
		"RSA Tool � uma biblioteca livre utilizada para criptografia RSA. Desenvolvida por Jo�o Paulo " \
		"Just.\n" \
		"jp@justsoft.com.br"), \
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_BESTWRAP);

	// Layout development
	sizer_v2->Add(label_appname, 0, wxALL, 5);
	sizer_v2->Add(label_copyright, 0, wxALL, 5);

	sizer_h1->Add(bitmap, 0, wxALL, 5);
	sizer_h1->Add(sizer_v2, 0, wxALL, 0);

	sizer_h2->Add(btn_web, 0, wxALL, 5);
	sizer_h2->Add(btn_license, 0, wxALL, 5);

	sizer_v1->Add(sizer_h1, 0, wxALL, 5);
	sizer_v1->Add(label_legalnotice, 0, wxALL, 5);
	sizer_v1->Add(sizer_h2, 0, wxALL | wxCENTRE, 5);
	sizer_v1->Add(txt_softwares, 1, wxALL | wxCENTRE | wxEXPAND, 5);
	sizer_v1->Add(label_mscopyright, 0, wxALL, 5);

	SetSizerAndFit(sizer_v1);
}

// Abre a p�gina do WinPolicy
void DialogAbout::OpenWeb(wxCommandEvent &event)
{
	wxLaunchDefaultBrowser(wxT("http://www.justsoft.com.br/winpolicy/"));
}

// Exibe a janela com a licen�a freeware
void DialogAbout::ShowLicense(wxCommandEvent &event)
{
	DialogLicense *dialog = new DialogLicense();
	dialog->ShowModal();
	delete dialog;
}
