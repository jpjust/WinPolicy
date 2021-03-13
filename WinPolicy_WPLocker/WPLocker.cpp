/* Main WPLocker class
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

#include "WPLocker.h"
#include "FrameBackground.h"
#include "AppBase.h"
#include <windows.h>

// OnInit method: the main method
bool WPLocker::OnInit(void)
{
	HANDLE mutex;

	if (AppBase::CheckPrevInstance("hsajUUGSajs7223AJSgsjhak", &mutex) == true)
		return false;

	AppBase::SetAppDir(argv[0]);
	wxInitAllImageHandlers();

	FrameBackground *frame = new FrameBackground(mutex);
	frame->ShowFullScreen(true);
	SetTopWindow(frame);
	return true;
}
