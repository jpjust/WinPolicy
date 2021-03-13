/* Controller base code
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

#include "ControllerBase.h"

wxString ControllerBase::m_appdir = wxT("");

// Configura o diret�rio do programa de acordo com o caminho do execut�vel
void ControllerBase::SetAppDir(wxString AppPath)
{
	m_appdir.Clear();
	m_appdir.Append(AppPath);
#ifdef __WXGTK__
	m_appdir = m_appdir.BeforeLast('/');
#endif
#ifdef __WXMSW__
	m_appdir = m_appdir.BeforeLast('\\');
#endif
}

// Retorna o diret�rio do programa
wxString ControllerBase::GetAppDir(void)
{
	return m_appdir;
}

// Retorna o diret�rio de configura��o
wxString ControllerBase::GetConfDir(void)
{
	wxStandardPaths Path;
	return Path.GetUserDataDir();
}
