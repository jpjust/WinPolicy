/* Rescue disk management class
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

#include "RDiskMgr.h"
#include "PwdMgr.h"
#include "AppBase.h"
#include <wx/string.h>
#include <wx/msgdlg.h>
#include <rsatool.h>
#include <stdio.h>

// SetRescuePwd method: set the rescue disk password
bool RDiskMgr::SetRescuePwd(void)
{
	FILE *fd;
	struct rsa_key_st key;

	key.size = RD_RSA_KEY_SIZE;
	key.n = RD_RSA_KEY_N;
	key.e = RD_RSA_KEY_E;
	key.d = "";

	wxString DiskPwd = wxString(rsa_encrypt((char *)PwdMgr::GetPasswd().mb_str(), key));

start:
	wxMessageBox("Insira um disco vazio na unidade A: e clique em \"OK\". Este ser� seu Disco de Recupera��o.", APP_NAME, wxICON_INFORMATION);

	if ((fd = fopen("a:\\wpolicy.pwd", "w")) == NULL)
	{
		if (wxMessageBox("Erro ao tentar acessar o arquivo \"a:\\wpolicy.pwd\". Deseja tentar novamente?", APP_NAME, wxICON_EXCLAMATION | wxYES_NO) == wxYES)
			goto start;
		else
			return false;
	}

	fputs(DiskPwd.mb_str(), fd);
	fclose(fd);

	return true;
}

// CheckRescuePwd method: check if the rescue disk password is correct
bool RDiskMgr::CheckRescuePwd(bool Verbose = true)
{
	FILE *fd;
	char pwd[1024] = {0};
	wxString RegPwd(PwdMgr::GetPasswd());
	wxString Pwd_enc;
	struct rsa_key_st key;

	key.size = RD_RSA_KEY_SIZE;
	key.n = RD_RSA_KEY_N;
	key.e = RD_RSA_KEY_E;
	key.d = "";

start:
	if (Verbose)
		wxMessageBox("Insira o Disco de Recupera��o na unidade A: e clique em \"OK\".", APP_NAME, wxICON_INFORMATION);

	if ((fd = fopen("a:\\wpolicy.pwd", "r")) == NULL)
	{
		if ((Verbose) && (wxMessageBox("Erro ao tentar acessar o arquivo \"a:\\wpolicy.pwd\". Deseja tentar novamente?", APP_NAME, wxICON_EXCLAMATION | wxYES_NO) == wxYES))
			goto start;
		else
			return false;
	}

	fgets(pwd, 1024, fd);
	fclose(fd);

	Pwd_enc = wxString(rsa_encrypt((char *)RegPwd.mb_str(), key));

	if (Pwd_enc == wxString(pwd))
		return true;
	return false;
}
