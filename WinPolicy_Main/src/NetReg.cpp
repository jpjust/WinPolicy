/* Class:       NetReg
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

#include "NetReg.h"
#include "AppBase.h"
#include "jswinreg.h"
#include <wx/url.h>
#include <wx/strconv.h>
#include <rsatool.h>
#include <shellapi.h>
#include <string.h>

void *NetReg::Entry(void)
{
#ifdef USE_NETREG
	// Executa o hdsn.exe para obter o n�mero de s�rie
	wxString Cmd = AppBase::GetAppDir() + wxT("\\hdsn.exe");

	ShellExecute(NULL, NULL, Cmd.mb_str(), NULL, NULL, SW_HIDE);
	wxSleep(5);		// Aguarda 5 segundos enquanto o processo est� rodando

	// Pega o resultado no registro, criptografa e compara
	wxString HDSN(jsreg_getvalue(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy", "hdsn", ""));
	if (HDSN.Len() < 1)
		return NULL;

	struct rsa_key_st mykey;

	mykey.size = NETREG_KEYSIZE;
	mykey.n = NETREG_KEY_N;
	mykey.e = NETREG_KEY_E;
	mykey.d = "";

	char *hdsn_c = rsa_encrypt((char *)HDSN.mb_str(), mykey);
	char *hdsn_c_reg = jsreg_getvalue(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy", "hdsn_c", "");

	if (strcmp(hdsn_c, hdsn_c_reg) == 0)	// S�o iguais?
		return NULL;

	// Faz o registro on-line
	wxString Url = wxString(NETREG_URL) + wxString(wxT("?manufacturer=")) + \
		wxString(NETREG_MANUFACTURER) + wxString(wxT("&pcid=")) + wxString(hdsn_c);

	wxMessageBox(wxT("O WinPolicy ir� agora fazer um registro do software no nosso servidor. " \
		"Este registro serve apenas para indicar o n�mero de computadores utilizando o WinPolicy. " \
		"Nenhuma informa��o sobre seu computador ser� enviada para nossos servidores."), APP_NAME, wxICON_INFORMATION);
	wxURL browser(Url);

	wxInputStream *instr = browser.GetInputStream();

	wxSleep(10);	// Time-out de 10 segundos, j� que o InputStrem n�o t� funcionando :(

	if (instr != NULL)
	{
		do
		{
			wxSleep(1);
		} while (instr->CanRead() == false);
	}

	// Acesso conclu�do. Podemos deletar a stream e gravar o resultado no registro.
	delete instr;
	if (jsreg_savevalue(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy", "hdsn_c", hdsn_c) == 0)
		wxMessageBox(wxT("O registro foi efetuado com sucesso! Obrigado por utilizar nosso software."), APP_NAME, wxICON_INFORMATION);
	else
		wxMessageBox(wxT("Ocorreu um erro ao finalizar o registro. Uma nova tentativa ser� feita na pr�xima utiliza��o do software."), APP_NAME, wxICON_ERROR);
#endif

	return NULL;
}
