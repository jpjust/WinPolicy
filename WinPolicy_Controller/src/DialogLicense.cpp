/* DialogLicense class
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

#include "DialogLicense.h"

wxChar justsoft_license[] = wxT("LICEN�A JUSTSOFT PARA SOFTWARE N�O-FREEWARE\n" \
"Vers�o 1.0 (16/08/2006)\n" \
"---------------------------------------------\n" \
"Copyright (c) 2006 Justsoft Inform�tica Ltda.\n" \
"http://www.justsoft.com.br/\n" \
"---------------------------------------------\n" \
"\n" \
"Este software � protegido por leis de Copyright que proibem sua reprodu��o ou c�pia ilegal. Ele foi totalmente produzido pela Justsoft Inform�tica Ltda. sendo que � proibido ao usu�rio usar, distribuir, copiar, vender, publicar ou revender o software como produ��o pr�pria ou usar o nome da Justsoft Inform�tica Ltda. em outros softwares.\n" \
"\n" \
"\n" \
"� permitido ao usu�rio:\n" \
"\n" \
"1) Distribuir o software sem licen�a e na vers�o oficial em disquetes ou qualquer outro tipo de m�dia ou pela Internet e redes, sem modific�-lo em hip�tese alguma;\n" \
"\n" \
"2) Inserir links para vers�o sem licen�a e oficial deste software ou seu Site em Home Page pessoal ou corporativa, sendo que o nome da Justsoft Inform�tica Ltda. seja exibido; e\n" \
"\n" \
"3) Executar a vers�o licenciada deste software apenas no computador ao qual foi licenciado.\n" \
"\n" \
"\n" \
"� proibido ao usu�rio:\n" \
"\n" \
"1) Modificar, traduzir o software ou copiar partes deste para outros softwares sem autoriza��o da Justsoft Inform�tica Ltda.;\n" \
"\n" \
"2) Vender, distribuir, publicar ou revender licen�as falsas ou o software sem permiss�o da Justsoft Inform�tica Ltda.; e\n" \
"\n" \
"3) Fraudar licen�as do software para ativa��o e execu��o ilegal.\n" \
"\n" \
"\n" \
"Este software foi testado para uma boa execu��o de sua tarefa, portanto, a Justsoft Inform�tica Ltda. N�O se responsabiliza por danos causados ao computador por uso indevido do programa. Ao utilizar este software, voc� concorda com todos os termos acima e assume todas as responsabilidades devido ao seu uso incorreto.\n" \
"\n" \
"---------------------------------------------\n" \
"Justsoft Inform�tica Ltda.\n" \
"CNPJ: 03.965.608/0001-49\n" \
"\n" \
"http://www.justsoft.com.br\n" \
"suporte@justsoft.com.br");


BEGIN_EVENT_TABLE(DialogLicense, wxDialog)
	EVT_BUTTON(ID_DIALOGLICENSE_BTN_CLOSE, DialogLicense::CloseDialog)
END_EVENT_TABLE()

// Class constructor
DialogLicense::DialogLicense(void)
	:wxDialog(NULL, -1, _("Licen�a"), wxDefaultPosition, wxSize(400, 400))
{
	wxBoxSizer *sizer_v1 = new wxBoxSizer(wxVERTICAL);

	txt_license = new wxTextCtrl(this, -1, justsoft_license, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_BESTWRAP);
	btn_close = new wxButton(this, ID_DIALOGLICENSE_BTN_CLOSE, _("Fechar"));

	// Layout development
	sizer_v1->Add(txt_license, 1, wxALL | wxEXPAND, 5);
	sizer_v1->Add(btn_close, 0, wxALL | wxALIGN_CENTRE, 5);

	SetSizer(sizer_v1);
}

// CloseDialog method
void DialogLicense::CloseDialog(wxCommandEvent &event)
{
	Close(true);
}
