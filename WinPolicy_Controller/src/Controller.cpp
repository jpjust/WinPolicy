/* Main WinPolicy Controller class
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

#include "Controller.h"
#include "FrameController.h"
#include "ControllerBase.h"
#include <wx/image.h>
#include <wx/fileconf.h>

// Main application method
bool Controller::OnInit(void)
{
	ControllerBase::SetAppDir(argv[0]);
	SetAppName(wxT("WPController"));

	// Cria diret�rio de configura��o
	if (!wxDirExists(ControllerBase::GetConfDir()))
		wxMkdir(ControllerBase::GetConfDir());

	// Idioma
	int lang;
	wxFileConfig fc(wxEmptyString, wxEmptyString, CONF_FILENAME, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
	if (!fc.Read(wxT("/Configuration/Language"), &lang))
//		lang = wxLANGUAGE_ENGLISH_US;
		lang = wxLANGUAGE_UNKNOWN;

	m_locale.Init(lang);
	m_locale.AddCatalogLookupPathPrefix(ControllerBase::GetAppDir());
	m_locale.AddCatalog(wxT("messages"));

	wxSocketBase::Initialize();
	wxInitAllImageHandlers();
	FrameController *frame = new FrameController();
	frame->Show(true);
	SetTopWindow(frame);

	return true;
}
