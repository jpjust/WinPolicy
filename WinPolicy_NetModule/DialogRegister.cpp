/* DialogRegister class
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

#include <windows.h>
#include <iptypes.h>
#include <iphlpapi.h>
#include "DialogRegister.h"
#include "NetModuleBase.h"
#include "jswinreg.h"

BEGIN_EVENT_TABLE(DialogRegister, wxDialog)
	EVT_BUTTON(ID_BTNREGISTER,	DialogRegister::OnRegisterClick)
	EVT_BUTTON(ID_BTNCLOSE,		DialogRegister::OnCloseClick)
	EVT_BUTTON(ID_BTNWEB,		DialogRegister::OnWebClick)
	EVT_CLOSE(DialogRegister::OnClose)
END_EVENT_TABLE()

// Construtor da classe
DialogRegister::DialogRegister(void)
	:wxDialog(NULL, wxID_ANY, wxT("Ativa��o do produto - WinPolicy NetModule"), wxDefaultPosition, wxDefaultSize)
{
	wxBoxSizer *sizer_v1 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *sizer_h1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer_h4 = new wxBoxSizer(wxHORIZONTAL);

	// Cria��o dos widgets
	lb_title = new wxStaticText(this, wxID_ANY, wxString(wxT("")) + \
		wxT("Para poder utilizar este produto, ele precisa ser ativado.\n") + \
		wxT("Preencha os campos abaixo de acordo com os dados do seu registro.\n\n") + \
		wxT("Caso possua d�vidas, entre em contato com nosso suporte no endere�o\n") + \
		wxT("suporte@justsoft.com.br"));

	lb_doc = new wxStaticText(this, wxID_ANY, \
		wxT("CPF/CNPJ:"));

	lb_pcid = new wxStaticText(this, wxID_ANY, \
		wxT("PC ID:"));

	lb_serial = new wxStaticText(this, wxID_ANY, \
		wxT("C�digo de ativa��o:"));

	// Widgets interativos, devem ser criados na ordem correta por causa dos TABs
	txt_doc = new wxTextCtrl(this, wxID_ANY);
	txt_pcid = new wxTextCtrl(this, wxID_ANY, wxEmptyString, \
		wxDefaultPosition, wxSize(100, -1), wxTE_READONLY);
	txt_serial = new wxTextCtrl(this, wxID_ANY);

	btn_register = new wxButton(this, ID_BTNREGISTER, wxT("Registrar"));
	btn_close = new wxButton(this, ID_BTNCLOSE, wxT("Demonstra��o"));
	btn_web = new wxButton(this, ID_BTNWEB, wxT("P�gina de ativa��o"));

	// PCID
	txt_pcid->SetValue(GetPCID());

	// Posicionamento dos widgets
	sizer_h1->Add(lb_doc, 1, wxALL, 15);
	sizer_h1->Add(txt_doc, 2, wxALL, 15);
	sizer_h2->Add(lb_pcid, 1, wxALL, 15);
	sizer_h2->Add(txt_pcid, 2, wxALL, 15);
	sizer_h3->Add(lb_serial, 1, wxALL, 15);
	sizer_h3->Add(txt_serial, 2, wxALL, 15);

	sizer_h4->Add(0, 0, 1, wxALL, 0);
	sizer_h4->Add(btn_register, 0, wxALL, 15);
	sizer_h4->Add(btn_close, 0, wxALL, 15);
	sizer_h4->Add(btn_web, 0, wxALL, 15);
	sizer_h4->Add(0, 0, 1, wxALL, 0);

	sizer_v1->Add(lb_title, 0, wxALL, 15);
	sizer_v1->Add(sizer_h1, 0, wxALL | wxEXPAND, 0);
	sizer_v1->Add(sizer_h2, 0, wxALL | wxEXPAND, 0);
	sizer_v1->Add(sizer_h3, 0, wxALL | wxEXPAND, 0);
	sizer_v1->Add(sizer_h4, 0, wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL, 0);

	SetSizerAndFit(sizer_v1);
}

// Efetua a ativa��o
void DialogRegister::OnRegisterClick(wxCommandEvent &event)
{
	if (SerialCompare(txt_doc->GetValue(), txt_pcid->GetValue(), txt_serial->GetValue()) == true)
	{
		jsreg_savevalue(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Activation", "doc", txt_doc->GetValue().c_str());
		jsreg_savevalue(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Activation", "serial", txt_serial->GetValue().c_str());
		EndModal(0);
	}
	else
		wxMessageBox(wxT("O c�digo de ativa��o est� incorreto."), APP_NM_NAME, wxICON_EXCLAMATION);
}

// Fecha janela
void DialogRegister::OnCloseClick(wxCommandEvent &event)
{
	EndModal(1);
}

void DialogRegister::OnWebClick(wxCommandEvent &event)
{
	wxLaunchDefaultBrowser(wxT("http://www.justsoft.com.br/winpolicy/activate/"));
}

// Evento de fechamento da janela
void DialogRegister::OnClose(wxCloseEvent &event)
{
	EndModal(1);
}

// Verifica se o serial est� OK
bool DialogRegister::SerialCompare(wxString Doc, wxString PCID, wxString Serial)
{
	struct rsa_key_st rsakey;
	wxString Key, GenSerial;

	// A chave inicial 'key' � o CPF/CNPJ do usu�rio com o PCID
	Key = Doc + wxT(" ") + PCID;

	// Criptografa a chave inicial e obt�m o n�mero serial
	rsakey.size = KEY_SIZE;
	rsakey.n = KEY_N;
	rsakey.e = KEY_E;
	rsakey.d = KEY_D;

	GenSerial = wxString(rsa_encrypt((char *)Key.c_str(), rsakey));

	// Compara
	if (Serial == GenSerial)
		return true;
	else
		return false;
}

// Obt�m o PCID
wxString DialogRegister::GetPCID(void)
{
	wxString Result;
	IP_ADAPTER_INFO AdapterInfo[16];       // Allocate information for up to 16 NICs
	DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer

	DWORD dwStatus = GetAdaptersInfo(      // Call GetAdapterInfo
		AdapterInfo,                       // [out] buffer to receive data
		&dwBufLen);                        // [in] size of receive data buffer

	if (dwStatus != ERROR_SUCCESS)
		return wxT("");

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to current adapter info

	// Gets MAC address for first adapter
	Result.Printf("%2x%2x%2x%2x%2x%2x",
		pAdapterInfo->Address[0],
		pAdapterInfo->Address[1],
		pAdapterInfo->Address[2],
		pAdapterInfo->Address[3],
		pAdapterInfo->Address[4],
		pAdapterInfo->Address[5]);

	Result.Replace(wxT(" "), wxT("0"), true);
	return Result;
}
