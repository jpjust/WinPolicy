/* Controller base header
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

#ifndef _CONTROLLERBASE_H
#define _CONTROLLERBASE_H

#include <wx/string.h>
#include <wx/utils.h>
#include <wx/stdpaths.h>

// Informa��es da vers�o
#define VERSION_MAJOR 1
#define VERSION_MINOR 1
#define VERSION_REVISION 0
#define VERSION_MAJOR_STR wxT("1")
#define VERSION_MINOR_STR wxT("1")
#define VERSION_REVISION_STR wxT("")
#define ALFA_BETA wxT("")

// Nome do produto
#define APP_NAME wxString(wxString(wxT("")) + wxT("Justsoft WinPolicy Controller ") + VERSION_MAJOR_STR + wxT(".") + VERSION_MINOR_STR + VERSION_REVISION_STR + ALFA_BETA)

// Macros
#define INT2BOOL(x) ((x == 0) ? false : true)
#define BOOL2INT(x) (x ? 1 : 0)

// Constantes globais ficam aqui
#define LIST_FILENAME ControllerBase::GetConfDir() + wxT("/aglist.ini")
#define CONF_FILENAME ControllerBase::GetConfDir() + wxT("/conf.ini")

enum
{
	// Cores do texto de debug
	DBG_OK,
	DBG_ERR,
	DBG_INFO,
	DBG_HIGH,

	// Eventos dos agentes
	ID_MSGCOUNT_EVT,
	AGENT_EVT_FINISH
};

// Classe ControllerBase
class ControllerBase
{
public:
	static void SetAppDir(wxString AppPath);
	static wxString GetAppDir(void);
	static wxString GetConfDir(void);

private:
	static wxString m_appdir;
};

#endif
