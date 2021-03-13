/* WinPolicy ScreenSaver
 *
 * Copyright (c) 2005 João Paulo Just Peixoto <just1982@gmail.com>.
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

/* From MSDN (ScreenSaver command-line arguments):
 *
 * ScreenSaver           - Show the Settings dialog box.
 * ScreenSaver /c        - Show the Settings dialog box, modal to the
 *                         foreground window.
 * ScreenSaver /p <HWND> - Preview Screen Saver as child of window <HWND>.
 * ScreenSaver /s        - Run the Screen Saver.
 *
 * In addition, Windows 95 Screen Savers must handle:
 *
 * ScreenSaver /a <HWND> - change password, modal to window <HWND>
 */

#include "WPSSaver.h"
#include "AppBase.h"
#include "jswinreg.h"
#include <windows.h>
#include <string.h>

// OnInit method: the main method
bool WPSSaver::OnInit(void)
{
	// Program initialization
	wxString WPPath(jsreg_getvalue(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy", "Path", ""));
	AppBase::SetAppDir(WPPath + "\\foo");
	wxInitAllImageHandlers();

#ifdef _WINPOLICY_DEBUG_
	wxString str_arglist;

	for (int i = 0; i < argc; i++)
		str_arglist.Append(wxString::Format("%d: %s\n", i, argv[i]));

	wxMessageBox(str_arglist, APP_NAME, wxICON_INFORMATION);
#endif

	for (int i = 1; i < argc; i++)
	{
		if ((strcmp(argv[i], "/c") == 0) || (strcmp(argv[i], "/a") == 0) || (argc == 1))
			AppBase::RunWinPolicy();

		if (strcmp(argv[i], "/s") == 0)
			AppBase::LockNow(false);
	}

	return false;
}
