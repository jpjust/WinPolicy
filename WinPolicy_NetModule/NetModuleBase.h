/* NetModule base class
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

#ifndef _NETMODULEBASE_H
#define _NETMODULEBASE_H

#include <wx/string.h>
#include <wx/socket.h>
#include <rsatool.h>

#define NETMODULE_MUTEX "lka87398ahlfsp9287idhfwkl2"
#define APP_NM_NAME wxT("Justsoft WinPolicy NetModule")
#define NETMODULE_VERSION wxT("1.0")

class SockData
{
public:
	SockData(void);

	void SetAuth(bool auth);

	struct rsa_key_st GetKey(void);
	bool GetAuth(void);

private:
	rsa_key_st key;
	bool auth_ok;
};

class NetModuleBase
{
public:
	static void ExecuteCommand(wxString Command, wxSocketBase *client);
};

#endif
