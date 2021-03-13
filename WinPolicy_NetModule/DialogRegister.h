/* DialogRegister class
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

#ifndef DIALOGREGISTER_H_
#define DIALOGREGISTER_H_

#include <wx/wx.h>
#include <rsatool.h>

// Chave RSA
#define KEY_SIZE 128
#define KEY_N "9eh2ekj7gv3kijqmk1tdd1rl7"
#define KEY_E "9eh2ekj7gv3kcdydq5t2tfgod"
#define KEY_D "34tosuuehmd6s4ngkpxoxt5k5"

const long ID_BTNREGISTER = wxNewId();
const long ID_BTNCLOSE = wxNewId();
const long ID_BTNWEB = wxNewId();

// Classe da janela
class DialogRegister: public wxDialog
{
public:
	DialogRegister(void);

	void OnRegisterClick(wxCommandEvent &event);
	void OnCloseClick(wxCommandEvent &event);
	void OnWebClick(wxCommandEvent &event);
	void OnClose(wxCloseEvent &event);

	static bool SerialCompare(wxString Doc, wxString PCID, wxString Serial);
	static wxString GetPCID(void);

	DECLARE_EVENT_TABLE()

private:
	wxButton	*btn_register;
	wxButton	*btn_close;
	wxButton	*btn_web;

	wxStaticText	*lb_title;
	wxStaticText	*lb_doc;
	wxStaticText	*lb_pcid;
	wxStaticText	*lb_serial;

	wxTextCtrl	*txt_doc;
	wxTextCtrl	*txt_pcid;
	wxTextCtrl	*txt_serial;
};

#endif /*DIALOGREGISTER_H_*/
