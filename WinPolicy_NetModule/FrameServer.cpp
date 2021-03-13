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

#include "FrameServer.h"
#include "NetModuleBase.h"
#include "PwdMgr.h"
#include "../NMService/NMServiceBase.h"
#include "WPNP.h"
#include <string.h>

BEGIN_EVENT_TABLE(FrameServer, wxFrame)
	EVT_SOCKET(ID_SERVER_EVENT, FrameServer::OnServerEvent)
	EVT_SOCKET(ID_SOCKET_EVENT, FrameServer::OnSocketEvent)
	EVT_SOCKET(ID_DGRAM_EVENT, FrameServer::OnDatagramEvent)
	EVT_TIMER(ID_TIMER, FrameServer::OnTimer)
	EVT_TIMER(ID_TIMER_REG, FrameServer::OnTimerReg)
	EVT_CLOSE(FrameServer::OnClose)
END_EVENT_TABLE()

// Construtor do frame
FrameServer::FrameServer(bool demo)
	:wxFrame(NULL, wxID_ANY, wxT("WinPolicy NetModule - janela de depura��o"), wxDefaultPosition, wxSize(300, 300))
{
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	m_demo = demo;

	// Timers
	timer = new wxTimer(this, ID_TIMER);		// Verifica se o NMService.exe est� em execu��o
	timer->Start(1000);

	timer_reg = new wxTimer(this, ID_TIMER_REG);// Fecha o registro

	// Cria��o dos widgets
	lb_text = new wxStaticText(this, wxID_ANY, wxT("Mensagens recebidas:"));
	txt_debug = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, \
		wxTE_MULTILINE | wxTE_READONLY | wxTE_BESTWRAP);

	// Posicionamento dos widgets
	sizer->Add(lb_text, 0, wxALL, 5);
	sizer->Add(txt_debug, 1, wxALL | wxEXPAND, 5);
	SetSizer(sizer);

	// Cria��o do socket
	wxIPV4address addr;
	if (!demo)
		addr.AnyAddress();
	else
		addr.LocalHost();
	addr.Service(SERVER_PORT);

	socket = new wxSocketServer(addr);
	socket->SetEventHandler(*this, ID_SERVER_EVENT);
	socket->SetNotify(wxSOCKET_CONNECTION_FLAG);
	socket->Notify(true);
	socket->SetTimeout(SOCK_TIMEOUT);

	// Cria��o do socket UDP
	addr_client = new wxIPV4address();
	//if (!demo)
		addr_client->AnyAddress();
	//else
	//	addr_client->LocalHost();
	addr_client->Service(DGRAM_PORT);

	dgram = new wxDatagramSocket(*addr_client, wxSOCKET_NOWAIT);
	const int optval = 1;
	dgram->SetOption(SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));
	dgram->SetEventHandler(*this, ID_DGRAM_EVENT);
	dgram->SetNotify(wxSOCKET_INPUT_FLAG);
	dgram->Notify(true);

	// Envia mensagem de on-line para broadcast
	if (!demo)
		SendUDPHello();
}

// Conex�o do socket
void FrameServer::OnServerEvent(wxSocketEvent &event)
{
	wxSocketBase *client;

	switch (event.GetSocketEvent())
	{
	case wxSOCKET_CONNECTION:	// Nova conex�o recebida
	{
		// Recebe a conex�o
		client = socket->Accept(false);
		wxIPV4address addr_con;

		// Se n�o houver nenhuma senha configurada, a conex�o � rejeitada
		if (PwdMgr::PasswdExists() == false)
		{
			Debug(wxT("*** Sem senha definida! Nova conex�o recusada."));
			SendData(client, WPNP_NOPWD, wxT("N�o h� senha definida!"));
			client->Destroy();
			break;
		}

		// Aceita a conex�o e envia a chave p�blica RSA
		Debug(wxT("*** Nova conex�o chegou!"));

		SendData(client, WPNP_OK, wxT("Conex�o aceita!"));
		SockData *sdata = new SockData();

		client->SetEventHandler(*this, ID_SOCKET_EVENT);
		client->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
		client->Notify(true);
		client->SetTimeout(SOCK_TIMEOUT);
		client->SetClientData((SockData *)sdata);

		wxString Msg;
		Msg.Printf(wxT("%s,%s"), sdata->GetKey().n, sdata->GetKey().e);
		SendData(client, WPNP_AUTH_KEY, Msg);

		m_connections++;
		break;
	}

	default:	// Evento desconhecido
		Debug("*** Evento desconhecido do servidor!");
	}
}

// Eventos do cliente
void FrameServer::OnSocketEvent(wxSocketEvent &event)
{
	wxSocketBase *client = event.GetSocket();
	char *buffer = (char *)malloc(sizeof(char) * BUF_SIZE);
	memset(buffer, 0, BUF_SIZE);

	switch (event.GetSocketEvent())
	{
	case wxSOCKET_INPUT:	// Dados chegaram
		if (client->WaitForRead())
		{
			if (client->IsData())
			{
				client->ReadMsg(buffer, BUF_SIZE);
				if (client->Error())
				{
					Debug(wxT("*** Erro de leitura!"));
					client->Close();
					m_connections--;
					break;
				}
				Debug(wxString(buffer));
				NetModuleBase::ExecuteCommand(wxString(buffer), client);
				AppBase::LoadUsersReg();
				timer_reg->Start(3000, true);
			}
			else
			{
				Debug(wxT("*** Dados corrompidos!"));
				client->Close();
				m_connections--;
			}
		}
		break;

	case wxSOCKET_LOST:		// Conex�o perdida
		Debug(wxT("*** Conex�o perdida!"));

		client->Destroy();
		m_connections--;
		break;

	default:				// Evento desconhecido
		Debug(wxT("*** Evento desconhecido do cliente!"));
		break;
	}
}

// Eventos do socket UDP
void FrameServer::OnDatagramEvent(wxSocketEvent &event)
{
	char buffer[BUF_SIZE] = {0};
	dgram->RecvFrom(*addr_client, buffer, BUF_SIZE);
	dgram->Discard();
	Debug(wxString::Format(wxT("!!! Pacote UDP recebido de %s: %s"), addr_client->IPAddress().mb_str(), buffer));

	int code = atoi(buffer);

	// Trata as mensagens
	switch (code)
	{
	case WPNP_WHOISTHERE:
	{
		SendUDPHello();
		break;
	}

	default:
		Debug(wxT("!!! Mensagem UDP desconhecida"));
		break;
	}
}

// Evento do timer
void FrameServer::OnTimer(wxTimerEvent &event)
{
#ifndef _NETMODULE_DEBUG_
	// Se estivermos em um ambiente NT, devemos verificar se
	// NMService.exe est� em execu��o. Sen�o, NetModule.exe DEVE MORRER! :)
	if (AppBase::GetRealWinVer() >= 500)
	{
		if (AppBase::CheckPrevInstance(NMSERVICE_MUTEX, NULL) == false)
		{
			wxExit();
		}
	}
#endif
}

// Timer de registro dos usu�rios
void FrameServer::OnTimerReg(wxTimerEvent &event)
{
	AppBase::UnLoadUsersReg();
}

// Evento de fechamento de janela
void FrameServer::OnClose(wxCloseEvent &event)
{
	SendUDP(WPNP_IMLEAVING);
	AppBase::UnLoadUsersReg();
	Destroy();
}

// Insere texto de depura��o em txt_debug
void FrameServer::Debug(wxString Text)
{
#ifdef _NETMODULE_DEBUG_
	txt_debug->AppendText(Text + wxT("\n"));
#endif
}

// Envia datagrama UDP para endere�o espec�fico ou broadcast
void FrameServer::SendUDP(int code, wxString Data, wxString Address)
{
	// Verifica se � um c�digo v�lido de controle
	if ((code < WPNP_WHOISTHERE) || (code > WPNP_IMLEAVING))
		return;

	wxIPV4address addr;
	addr.Hostname(Address);
	addr.Service(DGRAM_PORT_SENDER);

	wxString Msg;
	Msg.Printf(wxT("%d %s"), code, (char *)Data.c_str());
	dgram->SendTo(addr, Msg, Msg.Len());
}

// Envia mensagem UDP WPNP_IMHERE
void FrameServer::SendUDPHello(void)
{
	if (m_demo)
		SendUDP(WPNP_IMHERE, wxGetHostName() + wxT("|") + NETMODULE_VERSION + wxT("|") + wxGetOsDescription(), wxT("127.0.0.1"));
	else
		SendUDP(WPNP_IMHERE, wxGetHostName() + wxT("|") + NETMODULE_VERSION + wxT("|") + wxGetOsDescription(), addr_client->IPAddress());
}
