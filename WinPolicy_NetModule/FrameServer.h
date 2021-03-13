/* Main NetModule frame
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

#ifndef _FRAMESERVER_H
#define _FRAMESERVER_H

#include <wx/wx.h>
#include <wx/socket.h>

const long ID_SERVER_EVENT = wxNewId();
const long ID_SOCKET_EVENT = wxNewId();
const long ID_DGRAM_EVENT = wxNewId();
const long ID_TIMER = wxNewId();
const long ID_TIMER_REG = wxNewId();

class FrameServer : public wxFrame
{
public:
	FrameServer(bool demo = false);

	void OnServerEvent(wxSocketEvent &event);
	void OnSocketEvent(wxSocketEvent &event);
	void OnDatagramEvent(wxSocketEvent &event);

	void OnTimer(wxTimerEvent &event);
	void OnTimerReg(wxTimerEvent &event);
	void OnClose(wxCloseEvent &event);

	void SendUDP(int code, wxString Data = wxT(""), wxString Address = wxT("255.255.255.255"));
	void SendUDPHello(void);

private:
	void Debug(wxString Text);
	bool m_demo;

	wxSocketServer	*socket;
	wxDatagramSocket	*dgram;
	wxIPV4address	*addr_client;

	wxStaticText	*lb_text;
	wxTextCtrl		*txt_debug;

	wxTimer			*timer;
	wxTimer			*timer_reg;

	unsigned int m_connections;

	DECLARE_EVENT_TABLE()
};

#endif
