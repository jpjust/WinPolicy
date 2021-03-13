/* Class:       Agent
 * Description: Represents a WinPolicy client.
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

#ifndef _AGENT_H
#define _AGENT_H

#define AUTH_TIMEOUT 10		// Time-out da autentical��o (segundos)
#define OPER_TIMEOUT 10		// Time-out de opera��o (segundos)
#define THREAD_INT 10		// Intervalo de execu��o da thread (milisegundos)

#include <wx/event.h>
#include <wx/app.h>
#include <wx/string.h>
#include <wx/socket.h>
#include <wx/msgdlg.h>
#include <WPNP.h>
#include <list>
#include "ControllerBase.h"

// Controle dos pain�is
#include "PanelSystem.h"
#include "PanelWindows.h"
#include "PanelNetwork.h"
#include "PanelPrograms.h"
#include "PanelLocking.h"
#include "PanelMisc.h"
#include "PanelUsers.h"

using namespace std;

const long ID_SOCKET_EVENT = wxNewId();

// Estrutura de mensagens
struct sock_message
{
	int code;
	wxString Data;
	wxString Password;
};

class Agent;

// Classe: AgentThread
class AgentThread: public wxThread
{
public:
	void SetParent(Agent *parent);
private:
	virtual void *Entry(void);
	Agent *m_parent;
};

// Classe: Agent
class Agent: public wxEvtHandler
{
public:
	Agent(void);
	~Agent(void);

	void SetPassword(wxString Password);
	void SetPanels(PanelSystem *psys, PanelWindows *pwin, PanelNetwork *pnet, PanelPrograms *pprog, PanelLocking *plock, PanelMisc *pmisc, PanelUsers *pusers);

	void SetIP(wxString IPaddress);
	wxString GetIP(void);
	void SetName(wxString Name);
	wxString GetName(void);
	void SetOwner(wxEvtHandler *owner);
	wxSocketClient *GetSocket(void);
	bool GetAuth(void);

	bool Connect(void);
	bool Disconnect(void);
	bool IsConnected(void);

	// A��es
	void PushMessage(struct sock_message newmsg);
	void PushMessage(int code, wxString Data, wxString Password);
	struct sock_message PopMessage(void);
	unsigned int CountMessages(void);
	static unsigned int CountTotalMessages(void);
	unsigned int RemoveMessages(void);
	void SendCommand(int code, wxString data);
	void SetLastMsg(wxString Msg, int code = DBG_OK);

	void ExecuteCommand(wxString Command);

private:
	list<struct sock_message> messages;

	AgentThread		*thread;

	wxSemaphore		*sm_connection;
	wxSemaphore		*sm_messages;
	wxEvtHandler	*m_owner;
	char			m_authok;

	wxString		m_ip;
	wxString		m_name;
	wxString		m_password;
	wxSocketClient	*m_sock;

	static unsigned int m_totalmsg;

	void OnSocketEvent(wxSocketEvent &event);

	// Controle dos pain�is
	PanelSystem		*m_panelsystem;
	PanelWindows	*m_panelwindows;
	PanelNetwork	*m_panelnetwork;
	PanelPrograms	*m_panelprograms;
	PanelLocking	*m_panellocking;
	PanelMisc		*m_panelmisc;
	PanelUsers		*m_panelusers;

	DECLARE_EVENT_TABLE()
};

#endif
