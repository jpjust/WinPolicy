/* Password management class
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

#include "PwdMgr.h"
#include "jswinreg.h"
#include <rsatool.h>

// GetPasswd method: get the password from the Windows registry
wxString PwdMgr::GetPasswd(void)
{
	return wxString(jsreg_getvalue(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy", "Password3.0", ""));
}

// SetPasswd method: save a new password in the Windows registry
bool PwdMgr::SetPasswd(wxString NewPasswd)
{
	NewPasswd = Text2Code(NewPasswd);

	if (jsreg_savevalue(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy", "Password3.0", NewPasswd.mb_str()) == 0)
		return true;
	else
		return false;
}

// Configura nova senha j� criptografada
bool PwdMgr::SetPasswdCode(wxString NewCode)
{
	if (jsreg_savevalue(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy", "Password3.0", NewCode.mb_str()) == 0)
		return true;
	else
		return false;
}

// CheckPasswd method: check if the given password is correct
bool PwdMgr::CheckPasswd(wxString Passwd)
{
	wxString Passwd_enc = Text2Code(Passwd);

	if (Passwd_enc == GetPasswd())
		return true;
	else
		return false;
}

// PasswdExists method: check if a password was set
bool PwdMgr::PasswdExists(void)
{
	wxString RegPwd(GetPasswd());

	if (RegPwd.Len() > 0)
		return true;
	else
		return false;
}

// ErasePasswd method: erase admin's password
bool PwdMgr::ErasePasswd(void)
{
	return !INT2BOOL(jsreg_delvalue(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy", "Password3.0"));
}

// Converte texto plano para criptografia da senha
wxString PwdMgr::Text2Code(wxString Text)
{
	struct rsa_key_st key;

	key.size = RSA_KEY_SIZE;
	key.n = RSA_KEY_N;
	key.e = RSA_KEY_E;
	key.d = "";

	return wxString(rsa_encrypt((char *)Text.mb_str(), key));
}
