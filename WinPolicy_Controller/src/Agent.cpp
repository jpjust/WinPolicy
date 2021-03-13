/* Agent class
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

#include "Agent.h"
#include <rsatool.h>
#include <vector>

#ifdef __LINUX__
#include <unistd.h>
#endif

using namespace std;

BEGIN_EVENT_TABLE(Agent, wxEvtHandler)
	EVT_SOCKET(ID_SOCKET_EVENT, Agent::OnSocketEvent)
END_EVENT_TABLE()

unsigned int Agent::m_totalmsg = 0;

/////////////////////////////
// Thread
// Configura o agente pai da thread
void AgentThread::SetParent(Agent *parent)
{
	m_parent = parent;
}

// Ponto de entrada da thread
void *AgentThread::Entry(void)
{
	char *buffer = (char *)malloc(sizeof(char) * (BUF_SIZE + 1));

	while (1)
	{
		// Se (n�o exister mensagens para enviar) e (n�o for poss�vel ler do socket ou o socket n�o est� conectado):
		// fica no while, aguardando que alguma dessas condi��es seja poss�vel
		while ((m_parent->CountMessages() == 0) &&
			((!m_parent->GetSocket()->WaitForRead(0)) || (!m_parent->GetSocket()->IsConnected())))
		{
			if (TestDestroy())
			{
				free(buffer);
				return NULL;
			}
			Sleep(THREAD_INT);
		}

		// Verifica se h� tarefas a serem feitas
		while (m_parent->CountMessages() > 0)
		{
			struct sock_message msg = m_parent->PopMessage();
			m_parent->SetPassword(msg.Password);
			m_parent->SendCommand(msg.code, msg.Data);
		}

		// Verifica se h� dados a serem recebidos
		while (m_parent->GetSocket()->WaitForRead(0))
		{
			memset(buffer, 0, (BUF_SIZE + 1) * sizeof(char));
			m_parent->GetSocket()->ReadMsg(buffer, BUF_SIZE);	// Caso sobre dados na fila
			if (m_parent->GetSocket()->Error())
				break;
			m_parent->ExecuteCommand(wxString(buffer, wxConvUTF8));
		}
	}

	free(buffer);
	return NULL;
};

/////////////////////////////
// Agente
// Inicializa��o
Agent::Agent(void)
{
	// Cria o socket
	m_sock = new wxSocketClient();
	m_sock->SetEventHandler(*this, ID_SOCKET_EVENT);
	m_sock->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	m_sock->Notify(true);
	m_sock->SetTimeout(SOCK_TIMEOUT);
	m_sock->SetFlags(wxSOCKET_WAITALL);

	// Sem�foros
	sm_connection = new wxSemaphore(1);
	sm_messages = new wxSemaphore(1);

	// Atributos
	m_password = wxT("");
	m_ip = wxT("");
	m_authok = -1;

	// Threads
	thread = new AgentThread();
	thread->SetParent(this);
	thread->Create();
	thread->Run();
}

// Limpa os objetos
Agent::~Agent(void)
{
	RemoveMessages();

	thread->Delete();
	m_sock->Destroy();

	delete m_sock;
	delete sm_connection;
	delete sm_messages;
}

// Ajusta a senha do agente
void Agent::SetPassword(wxString Password)
{
	m_password.Clear();
	m_password.Append(Password);
}

// Configura os ponteiros dos pain�is
void Agent::SetPanels(PanelSystem *psys, PanelWindows *pwin, PanelNetwork *pnet, PanelPrograms *pprog, PanelLocking *plock, PanelMisc *pmisc, PanelUsers *pusers)
{
	m_panelsystem = psys;
	m_panelwindows = pwin;
	m_panelnetwork = pnet;
	m_panelprograms = pprog;
	m_panellocking = plock;
	m_panelmisc = pmisc;
	m_panelusers = pusers;
}

// Configura o endere�o IP do agente
void Agent::SetIP(wxString IPaddress)
{
	m_ip.Clear();
	m_ip.Append(IPaddress);
}

// Retorna o endere�o IP do agente
wxString Agent::GetIP(void)
{
	return m_ip;
}

// Ajusta o nome do agente
void Agent::SetName(wxString Name)
{
	m_name = Name;
}

// Obt�m o nome do agente
wxString Agent::GetName(void)
{
	return m_name;
}

// Configura um ponteiro para a janela principal
void Agent::SetOwner(wxEvtHandler *owner)
{
	m_owner = owner;
}

// Retorna o socket do agente
wxSocketClient *Agent::GetSocket(void)
{
	return m_sock;
}

// Retorna o estado da autentica��o
bool Agent::GetAuth(void)
{
	if (m_authok == 1)
		return true;
	else
		return false;
}

// Connecta no cliente
bool Agent::Connect(void)
{
	sm_connection->Wait();

	// Retorna true se j� estiver conectado
	if (IsConnected())
	{
		sm_connection->Post();
		return true;
	}

	m_authok = -1;

	// Tenta conectar
	wxIPV4address addr;

	addr.Hostname(m_ip);
	addr.Service(SERVER_PORT);

	m_sock->Connect(addr, false);
	m_sock->WaitOnConnect(10);

	if (m_sock->IsConnected())
		SetLastMsg(_("Conectado."), DBG_INFO);
	else
	{
		SetLastMsg(_("Falha na conex�o!"), DBG_ERR);
		unsigned int removed = RemoveMessages();
		if (removed > 0)
			SetLastMsg(wxString::Format(_("Removendo tarefas deste agente... %u removida(s)."), removed), DBG_ERR);

		m_sock->Close();
		sm_connection->Post();
		return false;
	}

	// Aguarda a autentica��o para ser poss�vel executar outras tarefas
	char *buffer = (char *)malloc(sizeof(char) * (BUF_SIZE + 1));
	while (m_sock->WaitForRead(AUTH_TIMEOUT))
	{
		memset(buffer, 0, (BUF_SIZE + 1) * sizeof(wxChar));
		m_sock->ReadMsg(buffer, BUF_SIZE);	// Caso sobre dados na fila, outro evento ser� gerado
		if (m_sock->Error())
			break;
		ExecuteCommand(wxString(buffer, wxConvUTF8));
		if (GetAuth() == true)
			break;
	}
	free(buffer);

	sm_connection->Post();
	return GetAuth();
}

// Disconecta do cliente
bool Agent::Disconnect(void)
{
	return m_sock->Close();
}

// Returna o estado da conex�o
bool Agent::IsConnected(void)
{
	return m_sock->IsConnected() && GetAuth();
}

// Adiciona uma nova mensagem a ser enviada
void Agent::PushMessage(struct sock_message newmsg)
{
	sm_messages->Wait();
	messages.push_back(newmsg);
	messages.resize(messages.size());
	m_totalmsg++;
	sm_messages->Post();

	wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_MSGCOUNT_EVT);
	wxPostEvent(m_owner, event);
}
void Agent::PushMessage(int code, wxString Data, wxString Password)
{
	sm_messages->Wait();
	struct sock_message newmsg;
	newmsg.code = code;
	newmsg.Data = Data;
	newmsg.Password = Password;
	messages.push_back(newmsg);
	messages.resize(messages.size());
	m_totalmsg++;
	sm_messages->Post();

	wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_MSGCOUNT_EVT);
	wxPostEvent(m_owner, event);
}

// Puxa uma mensagem da lista
struct sock_message Agent::PopMessage(void)
{
	sm_messages->Wait();
	struct sock_message msg = messages.front();
	messages.pop_front();
	messages.resize(messages.size());
	m_totalmsg--;
	sm_messages->Post();

	wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_MSGCOUNT_EVT);
	wxPostEvent(m_owner, event);

	return msg;
}

// Conta as mensagens na lista
unsigned int Agent::CountMessages(void)
{
	return messages.size();
}

// Retorna o n�mero total de mensagens de todos os agentes
unsigned int Agent::CountTotalMessages(void)
{
	return m_totalmsg;
}

// Remove todas as mensagens da lista
unsigned int Agent::RemoveMessages(void)
{
	sm_messages->Wait();
	unsigned int count = messages.size();
	m_totalmsg -= count;
	messages.clear();
	messages.resize(messages.size());
	sm_messages->Post();

	wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_MSGCOUNT_EVT);
	wxPostEvent(m_owner, event);

	return count;
}

// Envia um comando WPNP para o cliente
void Agent::SendCommand(int code, wxString data)
{
	if (Connect() == false)
		goto Agent_SendCommand_error;

	if (!SendData(m_sock, code, data))
		goto Agent_SendCommand_error;

	return;

Agent_SendCommand_error:
	SetLastMsg(_("Erro ao enviar mensagem!"), DBG_ERR);
	unsigned int removed = RemoveMessages();
	if (removed > 0)
		SetLastMsg(wxString::Format(_("Removendo tarefas deste agente... %u removida(s)."), removed), DBG_ERR);
}

/////////////////////////////
// Eventos privados
// SetLastMsg: envia mensagem para a janela principal
void Agent::SetLastMsg(wxString Msg, int code)
{
	if (!m_owner)
		return;

	int style;

	switch (code)
	{
	case DBG_OK:
	case WPNP_OK:
		style = DBG_OK;
		break;

	case DBG_ERR:
	case WPNP_FAIL:
	case WPNP_AUTH_FAIL:
		style = DBG_ERR;
		break;

	case DBG_HIGH:
	case WPNP_NOPWD:
		style = DBG_HIGH;
		break;

	case DBG_INFO:
	case WPNP_AUTH_OK:
		style = DBG_INFO;
		break;

	default:
		style = DBG_OK;
	}

	wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, AGENT_EVT_FINISH);
	event.SetInt(style);
	event.SetString(GetName() + wxT(" [") + GetIP() + wxT("]: ") + Msg);
	wxPostEvent(m_owner, event);
}

// OnSocketEvent: executado quando um evento do socket acontece
void Agent::OnSocketEvent(wxSocketEvent &event)
{
	switch(event.GetSocketEvent())
	{
	case wxSOCKET_INPUT:	// Dados chegaram pela rede
		break;

	case wxSOCKET_LOST:		// Conex�o perdida
	{
		SetLastMsg(_("Conex�o perdida."), DBG_ERR);
		unsigned int removed = RemoveMessages();
		if (removed > 0)
			SetLastMsg(wxString::Format(_("Removendo tarefas deste agente... %u removida(s)."), removed), DBG_ERR);

		m_sock->Close();
		break;
	}

	default:				// Evento desconhecido
		break;
	}
}

// ExecuteCommand: interpreta o comando passado e o executa
void Agent::ExecuteCommand(wxString Command)
{
#ifdef _DEBUG_
	wxString DbgCommand = Command;
#else
	wxString DbgCommand = Command.AfterFirst(' ');
#endif

	vector<wxString> parm;
	wxString Code = Command.BeforeFirst(' ');
	int codeint = atoi((char *)Code.c_str());
	Command.Remove(0, Code.Len() + 1);
	parm = Split(Command, ',', -1);

	switch (codeint)
	{
	///////////////////////// Autentica��o //////////////////////////
	case WPNP_AUTH_KEY:
	{
		if (parm.size() != 2) goto end;

		struct rsa_key_st rk;
		rk.size = KEY_SIZE;
		rk.n = (char *)parm[0].c_str();
		rk.e = (char *)parm[1].c_str();
		rk.d = 0;

		char *pwd = rsa_encrypt((char *)m_password.c_str(), rk);

		SendData(m_sock, WPNP_AUTH, wxString((wxChar *)pwd));
		free(pwd);

		SetLastMsg(_("Enviando senha..."));
		break;
	}

	case WPNP_AUTH_FAIL:
	{
		m_authok = 0;
		SetLastMsg(_("Senha inv�lida!"), DBG_ERR);
//		RemoveMessages();
		Disconnect();
		break;
	}

	case WPNP_AUTH_OK:
	{
		m_authok = 1;
		SetLastMsg(_("Senha OK!"), DBG_INFO);
		break;
	}

	///////////////////////// Resultados ////////////////////////////
	case WPNP_OK:
	{
		//DbgCommand->Printf(wxT("%s"), parm[0].mb_str());
		break;
	}

	case WPNP_FAIL:
	{
		//DbgCommand->Printf(wxT("%s"), parm[0].mb_str());
		break;
	}

	case WPNP_NOPWD:
	{
		SetLastMsg(_("Nenhuma senha definida no agente, imposs�vel conectar."), DBG_HIGH);
		Disconnect();
		break;
	}

	///////////////////////// Usu�rios //////////////////////////////
	case WPNP_USER_GLIST:
	{
		if (parm.size() < 1) break;

		m_panelusers->ClearUsers();

		for (unsigned int i = 0; i < parm.size(); i++)
		{
			vector<wxString> userdata = Split(parm[i], ':', -1);
			m_panelusers->AddUser(userdata[0]);
			m_panelusers->ChangePwd(userdata[0], userdata[1]);
		}
		break;
	}

	case WPNP_USER_GPWD:
	{
		if (parm.size() < 2) break;
		m_panelusers->ChangePwd(parm[0], parm[1]);
		break;
	}

	case WPNP_USER_GTIME:
	{
		if (parm.size() < 2) break;
		for (unsigned int i = 1; i < parm.size(); i++)
			m_panelusers->AddTime(parm[0], parm[i]);
		break;
	}

	case WPNP_USER_GPERM:
	{
		if (parm.size() < 2) break;
		if (parm[1] == wxString(wxT("1")))
			m_panelusers->SetPerm(parm[0], true);
		else
			m_panelusers->SetPerm(parm[0], false);
		break;
	}

	///////////////////////// Sistema ///////////////////////////////
	case WPNP_SYS_GTON:
	case WPNP_SYS_GTOFF:
	{
		if (codeint == WPNP_SYS_GTON)
			m_panelsystem->SetChecked(Command, true);
		else
			m_panelsystem->SetChecked(Command, false);
		break;
	}

	///////////////////////// Windows Explorer //////////////////////
	case WPNP_WIN_GTON:
	case WPNP_WIN_GTOFF:
	{
		if (codeint == WPNP_WIN_GTON)
			m_panelwindows->SetChecked(Command, true);
		else
			m_panelwindows->SetChecked(Command, false);
		break;
	}

	///////////////////////// Rede //////////////////////////////////
	case WPNP_NET_GTON:
	case WPNP_NET_GTOFF:
	{
		if (codeint == WPNP_NET_GTON)
			m_panelnetwork->SetChecked(Command, true);
		else
			m_panelnetwork->SetChecked(Command, false);
		break;
	}

	case WPNP_NET_GPWD:
		m_panelnetwork->SetPwdLen(Command);
		break;

	///////////////////////// Programas /////////////////////////////
	case WPNP_PROG_GSET:
		m_panelprograms->SetTweak(INT2BOOL(atoi(Command.mb_str())));
		break;

	case WPNP_PROG_GLIST:
		m_panelprograms->SetPrograms(Command);
		break;

	///////////////////////// Bloqueio //////////////////////////////
	case WPNP_LOCK_GTON:
	case WPNP_LOCK_GTOFF:
	{
		bool enable;
		if (codeint == WPNP_LOCK_GTON)
			enable = true;
		else
			enable = false;

		m_panellocking->SetChecked(Command, enable);
		break;
	}

	case WPNP_LOCK_GTIMES:
		m_panellocking->SetTimes(Command);
		break;

	///////////////////////// Outros ////////////////////////////////
	case WPNP_MISC_GTON:
	case WPNP_MISC_GTOFF:
	{
		bool enable;
		if (codeint == WPNP_MISC_GTON)
			enable = true;
		else
			enable = false;

		m_panelmisc->SetChecked(Command, enable);
		break;
	}

	case WPNP_MISC_GHIDE:
		m_panelmisc->SetHiddenDrv(Command);
		break;

	case WPNP_MISC_GDENY:
		m_panelmisc->SetDeniedDrv(Command);
		break;
	}

	end:
#ifdef _DEBUG_
	SetLastMsg(wxT("DEBUG: ") + DbgCommand);
#else
	if ((codeint == WPNP_OK) || (codeint == WPNP_FAIL))
		SetLastMsg(DbgCommand);
#endif
	return;
}
