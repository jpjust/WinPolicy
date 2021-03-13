/* Tweaks class
 *
 * Copyright (c) 2005 - 2006 João Paulo Just Peixoto <just1982@gmail.com>.
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

#include "Tweaks.h"

// Class constructor
Tweak::Tweak(const char *tweak_str)
{
	wxString tweak_data;
	tweak_data.Printf("%s", tweak_str);

	if (tweak_data.Len() == 0)
	{
		t_winver.Clear();
		t_tag.Clear();
		t_key.Clear();
		t_value.Clear();
		t_desc.Clear();
		return;
	}

	t_winver = tweak_data.BeforeFirst('|');
	tweak_data.Remove(0, t_winver.Len() + 1);

	t_tag = tweak_data.BeforeFirst('|');
	tweak_data.Remove(0, t_tag.Len() + 1);

	t_key = tweak_data.BeforeFirst('|');
	tweak_data.Remove(0, t_key.Len() + 1);

	t_value = tweak_data.BeforeFirst('|');
	tweak_data.Remove(0, t_value.Len() + 1);

	t_desc = tweak_data.BeforeFirst('|');
	t_data = 0;
}

// GetWinVer method
wxString Tweak::GetWinVer(void)
{
	return t_winver;
}

// GetTag method
wxString Tweak::GetTag(void)
{
	return t_tag;
}

// GetKey method
wxString Tweak::GetKey(void)
{
	return t_key;
}

// GetValue method
wxString Tweak::GetValue(void)
{
	return t_value;
}

// GetDescription method
wxString Tweak::GetDescription(void)
{
	return t_desc;
}

// SetDescription method
void Tweak::SetDescription(wxString Description)
{
	t_desc = Description;
}

// GetData method
int Tweak::GetData(void)
{
	return t_data;
}

// SetData method
void Tweak::SetData(int data)
{
	t_data = data;
}
