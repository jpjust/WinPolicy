/* Main AutoLock class
 *
 * Copyright (c) 2005-2006 João Paulo Just Peixoto <just1982@gmail.com>.
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

#include "AutoLock.h"
#include "AutoLockBase.h"
#include "AppBase.h"
#include <windows.h>

// OnInit method: the main method
bool AutoLock::OnInit(void)
{
	if (AppBase::CheckPrevInstance(ALOCK_MUTEX, NULL) == true)
		return false;

	AppBase::SetAppDir(argv[0]);
	wxInitAllImageHandlers();

	// Taskbar Icon
	task_icon = new TaskIconAutoLock(wxIcon("wxICON", wxBITMAP_TYPE_ICO_RESOURCE), "WinPolicy Autolock");

	// For Windows 9x compatibility
	// Hide the process in the taskbar list
	//HINSTANCE hiDLL = LoadLibrary((LPCTSTR)"kernel32.dll");
	//DWORD *RegisterServiceProcess() = (DWORD)GetProcAddress(hiDLL, "RegisterServiceProcess");
	//RegisterServiceProcess(GetCurrentProcessId(), 1);

	return true;
}
