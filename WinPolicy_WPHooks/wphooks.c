/* WinPolicy hooks library
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

#include "wphooks.h"
#include <stdio.h>

/* Keyboard hook filter */
__declspec(dllexport) LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

__declspec(dllexport) LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, KBDLLHOOKSTRUCT *lParam)
{
#ifdef _WINPOLICY_DEBUG_
	char msg[11] = {0};
	sprintf(msg, "%d", (int)lParam->vkCode);

	MessageBox(NULL, msg, "Hook", MB_OK);
#endif

	switch (wParam)
	{
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			return -1;
	}

	switch (lParam->vkCode)
	{
		case 9:		/* TAB */
		case 27:	/* ESC */
		case 46:	/* DEL */
		case 91:	/* Left WIN */
		case 92:	/* Right WIN */
		case 93:	/* Context menu */
		case 162:	/* Left CTRL */
		case 163:	/* Right CTRL */
		case 164:	/* Left ALT */
		case 165:	/* Right ALT */
			return -1;
	}

	return CallNextHookEx(NULL, nCode, wParam, (WPARAM)lParam);
}
