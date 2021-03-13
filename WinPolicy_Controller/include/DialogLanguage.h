/* Class:       DialogLanguage
 * Description: Language selection dialog window.
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

#ifndef DIALOGLANGUAGE_H_
#define DIALOGLANGUAGE_H_

#include <wx/wx.h>

enum
{
	ID_DIALOGLANGUAGE_BTN_OK
};

class DialogLanguage: public wxDialog
{
public:
	DialogLanguage(void);

	void OnOKClick(wxCommandEvent &event);

	DECLARE_EVENT_TABLE()

private:
	wxStaticText	*lb_title;
	wxComboBox		*combo_languages;
	wxButton		*btn_ok;
};

#endif /*DIALOGLANGUAGE_H_*/
