/* Warning dialog
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

#include "AppBase.h"
#include "DialogWarning.h"

BEGIN_EVENT_TABLE(DialogWarning, wxDialog)
	EVT_BUTTON(ID_DIALOGWARNING_BTN_OK, DialogWarning::OK)
END_EVENT_TABLE()

// Class constructor
DialogWarning::DialogWarning(wxWindow *parent)
	:wxDialog(parent, -1, wxString(APP_NAME))
{
	wxBoxSizer *sizer_v1 = new wxBoxSizer(wxVERTICAL);

	bitmap = new wxStaticBitmap(this, -1, wxBITMAP(IMG_SECURITY));
	txt_prompt = new wxStaticText(this, -1, "ATEN��O: Em um minuto, esta esta��o ser� bloqueada pelo WinPolicy.\n");
	btn_ok = new wxButton(this, ID_DIALOGWARNING_BTN_OK, "OK");

	sizer_v1->Add(bitmap, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);
	sizer_v1->Add(txt_prompt, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);
	sizer_v1->Add(btn_ok, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);
	SetSizerAndFit(sizer_v1);
}

// OK method
void DialogWarning::OK(wxCommandEvent &event)
{
	Close(true);
}
