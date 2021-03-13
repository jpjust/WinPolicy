/* FrameController class
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

#include <wx/sysopt.h>
#include <wx/fileconf.h>
#include <rsatool.h>
#include "FrameController.h"
#include "Agent.h"
#include "DialogAbout.h"

BEGIN_EVENT_TABLE(FrameController, wxFrame)
	EVT_TOOL_RANGE(ID_TBAR_ADDPC, ID_TBAR_STOPALL, FrameController::OnToolbarClick)
	EVT_LISTBOX(ID_LISTDYN, FrameController::OnListClick)
	EVT_LISTBOX(ID_LISTSTAT, FrameController::OnListClick)
	EVT_LISTBOX_DCLICK(ID_LISTDYN, FrameController::OnListDClick)
	EVT_LISTBOX_DCLICK(ID_LISTSTAT, FrameController::OnListDClick)
	EVT_BUTTON(ID_BTN_PCALL, FrameController::OnListBtnClick)
	EVT_BUTTON(ID_BTN_PCNONE, FrameController::OnListBtnClick)
	EVT_BUTTON(ID_BTN_CHPWD, FrameController::OnChPwdClick)
	EVT_BUTTON(AGENT_EVT_FINISH, FrameController::OnAgentEvent)
	EVT_BUTTON(ID_MSGCOUNT_EVT, FrameController::OnMsgCountEvent)
	EVT_MENU(ID_MENU_EXIT, FrameController::OnMenuExit)
	EVT_MENU(ID_MENU_ADDPC, FrameController::OnMenuAddPC)
	EVT_MENU(ID_MENU_DELPC, FrameController::OnMenuDelPC)
	EVT_MENU(ID_MENU_UPDATE, FrameController::OnMenuUpdate)
	EVT_MENU(ID_MENU_LOCK, FrameController::OnMenuLockPC)
	EVT_MENU(ID_MENU_SDOWN, FrameController::OnMenuShutdownPC)
	EVT_MENU(ID_MENU_REBOOT, FrameController::OnMenuShutdownPC)
	EVT_MENU(ID_MENU_LOGOFF, FrameController::OnMenuShutdownPC)
	EVT_MENU(ID_MENU_RUN, FrameController::OnMenuRun)
	EVT_MENU(ID_MENU_SEND, FrameController::OnMenuSend)
	EVT_MENU(ID_MENU_GET, FrameController::OnMenuGet)
	EVT_MENU(ID_MENU_STOP, FrameController::OnMenuStop)
	EVT_MENU(ID_MENU_STOPALL, FrameController::OnMenuStopAll)
	EVT_MENU(ID_MENU_HELP_MANUAL, FrameController::OnMenuHelp)
	EVT_MENU(ID_MENU_HELP_WINPOLICY, FrameController::OnMenuHelp)
	EVT_MENU(ID_MENU_HELP_WEB, FrameController::OnMenuHelp)
	EVT_MENU(ID_MENU_HELP_ACTIVATION, FrameController::OnMenuHelp)
	EVT_MENU(ID_MENU_HELP_LANGUAGE, FrameController::OnMenuHelp)
	EVT_MENU(ID_MENU_ABOUT, FrameController::OnMenuAbout)
	EVT_TIMER(ID_TIMER_UDP, FrameController::OnTimerUDP)
	EVT_SOCKET(ID_DGRAM_EVENT, FrameController::OnDatagramEvent)
	EVT_CLOSE(FrameController::OnClose)
END_EVENT_TABLE()

// Construtor
FrameController::FrameController(void)
	:wxFrame(NULL, wxID_ANY, APP_NAME)
{
	wxBoxSizer *sizerh_main = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizerv_main = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *sizerv_list = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *sizerh_list = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizerh_pwd = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizerv_notebook = new wxBoxSizer(wxVERTICAL);

	// Configura o timer de broadcast
	timer_udp = new wxTimer(this, ID_TIMER_UDP);
	timer_udp->Start(300000);	// Executar a cada 5 minutos
	wxTimerEvent udpevent(ID_TIMER_UDP, wxEVT_TIMER);
	wxPostEvent(this, udpevent);

	// Configura o socket para mensagens em broadcast
	addr_client = new wxIPV4address();
	addr_bcast = new wxIPV4address();

	addr_client->AnyAddress();
	addr_client->Service(DGRAM_PORT_SENDER);
	addr_bcast->Hostname(wxT("255.255.255.255"));
	addr_bcast->Service(DGRAM_PORT);

	dgram = new wxDatagramSocket(*addr_client, wxSOCKET_NOWAIT);
	const int optval = 1;
	dgram->SetOption(SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));
	dgram->SetEventHandler(*this, ID_DGRAM_EVENT);
	dgram->SetNotify(wxSOCKET_INPUT_FLAG);
	dgram->Notify(true);

	// �cones, background e sem�foros
#ifdef __WXMSW__
	SetIcons(wxIconBundle(wxICON(wxICON)));
#endif
	SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	allusers = 1;
	sm_debug = new wxSemaphore(1);

	// Cria��o da barra de menus
	mbar = new wxMenuBar();
	wxMenu *mnu_list = new wxMenu();
	wxMenu *mnu_control = new wxMenu();
	wxMenu *mnu_help = new wxMenu();
	mnu_misc = new wxMenu();

	mnu_list->Append(ID_MENU_ADDPC,		_("&Adicionar PC\tCTRL+A"));
	mnu_list->Append(ID_MENU_DELPC,		_("&Remover PC\tCTRL+D"));
	mnu_list->Append(ID_MENU_UPDATE,	_("A&tualizar lista\tF5"));
	mnu_list->AppendSeparator();
	mnu_list->Append(ID_MENU_EXIT,		_("&Sair"));

	mnu_misc->Append(ID_MENU_LOCK,		_("&Bloquear PCs\tCTRL+L"));
	mnu_misc->AppendSeparator();
	mnu_misc->Append(ID_MENU_SDOWN,		_("&Desligar PCs\tCTRL+S"));
	mnu_misc->Append(ID_MENU_REBOOT,	_("&Reiniciar PCs\tCTRL+R"));
	mnu_misc->Append(ID_MENU_LOGOFF,	_("&Logoff remoto\tCTRL+O"));
	mnu_misc->AppendSeparator();
	mnu_misc->Append(ID_MENU_RUN,		_("&Executar programa remotamente\tCTRL+X"));
//	mnu_misc->Append(ID_MENU_SETTIME,	_("&Ajustar cron�metro\tCTRL+T"));

	mnu_control->Append(ID_MENU_GET,	_("&Obter restri��es\tF7"));
	mnu_control->Append(ID_MENU_SEND,	_("&Enviar restri��es\tF8"));
	mnu_control->Append(wxID_ANY, _("Outros"), mnu_misc);
	mnu_control->AppendSeparator();
	mnu_control->Append(ID_MENU_STOP,	_("&Parar tarefas"));
	mnu_control->Append(ID_MENU_STOPALL,_("Parar &todas as tarefas"));

	mnu_help->Append(ID_MENU_HELP_MANUAL, _("&Manual de utiliza��o\tF1"));
	mnu_help->Append(ID_MENU_HELP_WINPOLICY, _("Ajuda do W&inPolicy\tF3"));
	mnu_help->AppendSeparator();
	mnu_help->Append(ID_MENU_HELP_WEB,	_("WinPolicy na &Web\tF4"));
	mnu_help->Append(ID_MENU_HELP_ACTIVATION, _("&Ativar NetModule\tF5"));
//	mnu_help->AppendSeparator();
//	mnu_help->Append(ID_MENU_HELP_LANGUAGE,	_("&Selecionar idioma\tF6"));
	mnu_help->AppendSeparator();
	mnu_help->Append(ID_MENU_ABOUT,		_("&Sobre\tF12"));

	mbar->Append(mnu_list,				_("&Lista"));
	mbar->Append(mnu_control,			_("&Controle"));
	mbar->Append(mnu_help,				_("&Ajuda"));

	// Cria��o da barra de ferramentas
	wxString ImgDir = ControllerBase::GetAppDir() + wxT("/images/");

	// Configura a Toolbar para fundo transparente
#ifdef __WXMSW__
	int osmaj, osmin, osmm;
	wxGetOsVersion(&osmaj, &osmin);
	osmm = osmaj * 100 + osmin;
	if ((wxDisplayDepth() >= 32) && (osmm >= 501))
		wxSystemOptions::SetOption(wxT("msw.remap"), 2);
	else
		wxSystemOptions::SetOption(wxT("msw.remap"), 0);
#endif

	tbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_HORIZONTAL | wxTB_TEXT);
	tbar->SetToolBitmapSize(wxSize(32, 32));
	tbar->AddTool(ID_TBAR_ADDPC,	_("Adicionar PC"),		AppBase::LoadImage(wxT("addpc.png")),		wxNullBitmap, wxITEM_NORMAL, wxEmptyString, _("Adiciona um agente na lista est�tica"));
	tbar->AddTool(ID_TBAR_DELPC,	_("Remover PC"),		wxBitmap(ImgDir + wxT("delpc.png"), wxBITMAP_TYPE_PNG),		wxNullBitmap, wxITEM_NORMAL, wxEmptyString, _("Remove o(s) agente(s) selecionado(s)"));
	tbar->AddTool(ID_TBAR_UPDATE,	_("Atualizar lista"),	wxBitmap(ImgDir + wxT("refresh.png"), wxBITMAP_TYPE_PNG),	wxNullBitmap, wxITEM_NORMAL, wxEmptyString, _("Atualiza a lista din�mica com os agentes na rede"));
	tbar->AddSeparator();
	tbar->AddTool(ID_TBAR_MISC,		_("Outras op��es"),		wxBitmap(ImgDir + wxT("more.png"), wxBITMAP_TYPE_PNG),	wxNullBitmap, wxITEM_NORMAL, wxEmptyString, _("Exibe um menu com outras op��es dispon�veis"));
	tbar->AddSeparator();
	tbar->AddTool(ID_TBAR_SEND,		_("Enviar restri��es"),	wxBitmap(ImgDir + wxT("send.png"), wxBITMAP_TYPE_PNG),		wxNullBitmap, wxITEM_NORMAL, wxEmptyString, _("Envia restri��es para o(s) agente(s) selecionado(s)"));
	tbar->AddTool(ID_TBAR_GET,		_("Obter restri��es"),	wxBitmap(ImgDir + wxT("get.png"), wxBITMAP_TYPE_PNG),		wxNullBitmap, wxITEM_NORMAL, wxEmptyString, _("Obt�m restri��es do agente selecionado"));
	tbar->AddSeparator();
	tbar->AddTool(ID_TBAR_STOP,		_("Parar"),				wxBitmap(ImgDir + wxT("stop.png"), wxBITMAP_TYPE_PNG),		wxNullBitmap, wxITEM_NORMAL, wxEmptyString, _("P�ra tarefas em espera do(s) agente(s) selecionado(s)"));
	tbar->AddTool(ID_TBAR_STOPALL,	_("Parar todas"),		wxBitmap(ImgDir + wxT("stopall.png"), wxBITMAP_TYPE_PNG),	wxNullBitmap, wxITEM_NORMAL, wxEmptyString, _("P�ra tarefas em espera de todos os agentes"));
	tbar->Realize();

	// Cria��o da status bar
	stbar = new wxStatusBar(this, ID_STATUSBAR);
	stbar->SetFieldsCount(2);
	stbar->PushStatusText(_("Tarefas restantes: 0"), 1);

	// Cria��o da lista de agentes
	lb_dynlist = new wxStaticText(this, wxID_ANY, _("Lista din�mica"));
	lb_statlist = new wxStaticText(this, wxID_ANY, _("Lista est�tica"));
	ag_list = new wxCheckListBox(this, ID_LISTDYN, wxDefaultPosition, wxSize(200, 100), 0, NULL, wxLB_HSCROLL);
	ag_list_stat = new wxCheckListBox(this, ID_LISTSTAT, wxDefaultPosition, wxSize(200, 100), 0, NULL, wxLB_HSCROLL);
	//ag_tree = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1));//, wxTR_HAS_BUTTONS | wxTR_MULTIPLE);
	//agt_root = ag_tree->AddRoot(_("Detec��o autom�tica"));

	// Bot�es da lista de agentes
	btn_pcall = new wxButton(this, ID_BTN_PCALL, _("Todos"));
	btn_pcnone = new wxButton(this, ID_BTN_PCNONE, _("Nenhum"));

	// Campo de senha
	lb_pwd = new wxStaticText(this, wxID_ANY, _("Senha de conex�o dos agentes:"));
	txt_pwd = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150, -1), wxTE_PASSWORD);

	//lb_newpwd[0] = new wxStaticText(this, wxID_ANY, _("Nova senha dos agentes:"));
	//txt_newpwd[0] = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150, -1), wxTE_PASSWORD);

	//lb_newpwd[1] = new wxStaticText(this, wxID_ANY, _("Confirme a senha:"));
	//txt_newpwd[1] = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150, -1), wxTE_PASSWORD);

	btn_chpwd = new wxButton(this, ID_BTN_CHPWD, _("Alterar senha"));

	// Cria��o das abas
	tabs = new wxNotebook(this, wxID_ANY);
	panel_system = new PanelSystem(tabs);
	panel_windows = new PanelWindows(tabs);
	panel_network = new PanelNetwork(tabs);
	panel_programs = new PanelPrograms(tabs);
	panel_locking = new PanelLocking(tabs);
	panel_misc = new PanelMisc(tabs);
	panel_users = new PanelUsers(tabs);
	tabs->AddPage(panel_system, _("Sistema"));
	tabs->AddPage(panel_windows, _("Windows"));
	tabs->AddPage(panel_network, _("Rede"));
	tabs->AddPage(panel_programs, _("Programas"));
	tabs->AddPage(panel_locking, _("Bloqueio"));
	tabs->AddPage(panel_misc, _("Outros"));
	tabs->AddPage(panel_users, _("Usu�rios"));

	// Caixa de mensagens
	txt_msg = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(-1, 100), \
		wxTE_MULTILINE | wxTE_READONLY | wxTE_BESTWRAP | wxTE_RICH2);

	// Posicionamento dos widgets
	sizerh_list->Add(btn_pcall, 0, wxALL | wxALIGN_CENTER, 5);
	sizerh_list->Add(btn_pcnone, 0, wxALL | wxALIGN_CENTER, 5);
	sizerv_list->Add(lb_dynlist, 0, wxALL, 5);
	sizerv_list->Add(ag_list, 1, wxALL | wxEXPAND, 5);
	sizerv_list->Add(lb_statlist, 0, wxALL, 5);
	sizerv_list->Add(ag_list_stat, 1, wxALL | wxEXPAND, 5);
	sizerv_list->Add(sizerh_list, 0, wxALL | wxEXPAND | wxALIGN_CENTER, 0);

	sizerh_pwd->Add(lb_pwd, 0, wxALL, 5);
	sizerh_pwd->Add(txt_pwd, 0, wxALL, 5);
	sizerh_pwd->Add(btn_chpwd, 0, wxALL, 5);

	sizerv_notebook->Add(sizerh_pwd, 0, wxALL | wxEXPAND, 0);
	sizerv_notebook->Add(tabs, 1, wxALL | wxEXPAND, 5);

	sizerh_main->Add(sizerv_list, 0, wxALL | wxEXPAND, 0);
	sizerh_main->Add(sizerv_notebook, 1, wxALL | wxEXPAND, 0);

	sizerv_main->Add(sizerh_main, 1, wxALL | wxEXPAND, 0);
	sizerv_main->Add(txt_msg, 0, wxALL | wxEXPAND, 5);

	// Finaliza��o
	SetMenuBar(mbar);
	SetToolBar(tbar);
	SetStatusBar(stbar);
	SetSizerAndFit(sizerv_main);
	//TaskMgr::SetOwner(this);

	// Preenchimento da lista de agentes
	wxFileConfig fc(wxEmptyString, wxEmptyString, LIST_FILENAME, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
	wxString AgentName, AgentIP;
	int i = 0;
	do
	{
		wxString AgentEntry;
		AgentName = wxEmptyString;
		AgentIP = wxEmptyString;
		fc.Read(wxString::Format(wxT("/AgentsList/%d"), i++), &AgentEntry);

		AgentName = AgentEntry.BeforeLast('|');
		AgentIP = AgentEntry.AfterLast('|');

		if ((AgentName.Len() > 0) && (AgentIP.Len() > 0))
			AddPC(AgentIP, AgentName, true);
	} while ((AgentName.Len() > 0) && (AgentIP.Len() > 0));
}

// Destrutor
FrameController::~FrameController(void)
{
	for (unsigned int i = 0; i < agents.size(); i++)
		delete agents[i];
	for (unsigned int i = 0; i < agents_stat.size(); i++)
		delete agents_stat[i];
	agents.clear();
	agents_stat.clear();

	dgram->Destroy();

	delete sm_debug;
	delete addr_client;
	delete addr_bcast;

}

// Processa eventos da barra de ferramentas
void FrameController::OnToolbarClick(wxCommandEvent &event)
{
	switch (event.GetId())
	{
	case ID_TBAR_ADDPC:
		AddPC(wxEmptyString, wxEmptyString, true);
		break;

	case ID_TBAR_DELPC:
		DelPC();
		break;

	case ID_TBAR_UPDATE:
		UpdateList();
		break;

	case ID_TBAR_MISC:
		PopupMenu(mnu_misc);
		break;

	case ID_TBAR_SEND:
		SendConf();
		break;

	case ID_TBAR_GET:
		GetConf();
		break;

	case ID_TBAR_STOP:
		StopTasks(false);
		break;

	case ID_TBAR_STOPALL:
		StopTasks();
		break;

	default:
		break;
	}
}

// Processa cliques nas listas de agentes
void FrameController::OnListClick(wxCommandEvent &event)
{
	if (event.GetId() == ID_LISTDYN)
		ag_list_stat->Deselect(0);
	else
		ag_list->Deselect(0);
}

// Processa cliques duplos nas listas de agentes
void FrameController::OnListDClick(wxCommandEvent &event)
{
	GetConf();
}

// Processa eventos dos bot�es da lista de agentes
void FrameController::OnListBtnClick(wxCommandEvent &event)
{
	if (event.GetId() == ID_BTN_PCALL)
	{
		for (unsigned int i = 0; i < ag_list->GetCount(); i++)
			ag_list->Check(i, true);
		for (unsigned int i = 0; i < ag_list_stat->GetCount(); i++)
			ag_list_stat->Check(i, true);
	}
	else
	{
		for (unsigned int i = 0; i < ag_list->GetCount(); i++)
			ag_list->Check(i, false);
		for (unsigned int i = 0; i < ag_list_stat->GetCount(); i++)
			ag_list_stat->Check(i, false);
	}
}

// Mudan�a da senha dos agentes
void FrameController::OnChPwdClick(wxCommandEvent &event)
{
	if (txt_pwd->GetValue().Len() < 1)
	{
		wxMessageBox(_("Antes de enviar as restri��es, digite a senha dos agentes no campo \"Senha de conex�o dos agentes\"."), APP_NAME, wxICON_EXCLAMATION);
		txt_pwd->SetFocus();
		return;
	}

	wxString NewPwd[2];

	NewPwd[0] = wxGetPasswordFromUser(_("Digite a nova senha para os agentes:"), APP_NAME);
	if (NewPwd[0].Len() == 0)
		return;

	NewPwd[1] = wxGetPasswordFromUser(_("Confirme a nova senha:"), APP_NAME);
	if (NewPwd[1].Len() == 0)
		return;

	if (NewPwd[0] != NewPwd[1])
	{
		wxMessageBox(_("As senhas digitadas n�o coincidem."), APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	// Criptografa a senha
	struct rsa_key_st key;

	key.size = RSA_KEY_SIZE;
	key.n = RSA_KEY_N;
	key.e = RSA_KEY_E;
	key.d = "";

	wxString PwdCode((wxChar *)rsa_encrypt((char *)NewPwd[0].c_str(), key));

	SendToAgents(WPNP_SETPWD, PwdCode);
}

/////////////////////////////
// Eventos dos menus
void FrameController::OnMenuExit(wxCommandEvent &event) { Close(); }
void FrameController::OnMenuAddPC(wxCommandEvent &event) { AddPC(wxEmptyString, wxEmptyString, true); }
void FrameController::OnMenuDelPC(wxCommandEvent &event) { DelPC(); }
void FrameController::OnMenuUpdate(wxCommandEvent &event) { UpdateList(); }
void FrameController::OnMenuLockPC(wxCommandEvent &event) { LockPC(); }
void FrameController::OnMenuShutdownPC(wxCommandEvent &event) { ShutdownPC(event.GetId()); }
void FrameController::OnMenuSend(wxCommandEvent &event) { SendConf(); }
void FrameController::OnMenuGet(wxCommandEvent &event) { GetConf(); }
void FrameController::OnMenuStop(wxCommandEvent &event) { StopTasks(false); }
void FrameController::OnMenuStopAll(wxCommandEvent &event) { StopTasks(); }
void FrameController::OnMenuHelp(wxCommandEvent &event)
{
	switch (event.GetId())
	{
	case ID_MENU_HELP_MANUAL:
	{
		wxString str_path = ControllerBase::GetAppDir() + wxT("\\help\\manual_enterprise.pdf");
		ShellExecute(NULL, NULL, (char *)str_path.mb_str(), NULL, NULL, SW_SHOW);
		break;
	}

	case ID_MENU_HELP_WINPOLICY:
	{
		wxString str_path = ControllerBase::GetAppDir() + wxT("\\help\\index.html");
		ShellExecute(NULL, NULL, (char *)str_path.mb_str(), NULL, NULL, SW_SHOW);
		break;
	}

	case ID_MENU_HELP_WEB:
		wxLaunchDefaultBrowser(wxT("http://www.justsoft.com.br/winpolicy/"));
		break;

	case ID_MENU_HELP_ACTIVATION:
		wxLaunchDefaultBrowser(wxT("http://www.justsoft.com.br/winpolicy/activate/"));
		break;

	case ID_MENU_HELP_LANGUAGE:
	{
		DialogLanguage *dialog = new DialogLanguage();
		dialog->ShowModal();
		dialog->Destroy();
		break;
	}

	default:
		break;
	}
}
void FrameController::OnMenuAbout(wxCommandEvent &event)
{
	DialogAbout *dialog = new DialogAbout();
	dialog->ShowModal();
	delete dialog;
}
void FrameController::OnMenuRun(wxCommandEvent &event)
{
	if (txt_pwd->GetValue().Len() < 1)
	{
		wxMessageBox(_("Antes de executar uma a��o, digite a senha dos agentes no campo \"Senha de conex�o dos agentes\"."), APP_NAME, wxICON_EXCLAMATION);
		txt_pwd->SetFocus();
		return;
	}

	wxString Command = wxGetTextFromUser(
		_("Digite o comando, caminho do programa ou arquivo que deseja executar no computador remoto.\n\n" \
		"Ex.: notepad.exe ou \"C:\\Arquivos de Programas\\Justsoft WinPolicy\\WinPolicy.exe\"\n\n" \
		"Se quiser executar um programa ou arquivo neste computador, copie o execut�vel ou arquivo para\n" \
		"uma pasta compartilhada e execute como no exemplo abaixo:\n\n" \
		"\\\\Computador\\compartilhamento\\programa.exe"), APP_NAME);

	if (Command.Len() > 0)
		SendToAgents(WPNP_RUN, Command);
}
/////////////////////////////

// Evento de fechamento da janela
void FrameController::OnClose(wxCloseEvent &event)
{
	wxFileConfig fc(wxEmptyString, wxEmptyString, LIST_FILENAME, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);

	fc.DeleteGroup(wxT("/AgentsList"));
	for (unsigned int i = 0; i < agents_stat.size(); i++)
		fc.Write(wxString::Format(wxT("/AgentsList/%d"), i), agents_stat[i]->GetName() + wxT("|") + agents_stat[i]->GetIP());

	fc.Flush();
	Destroy();
}

// Adiciona um computador na lista
void FrameController::AddPC(wxString Address, wxString Desc, bool stat)
{
	Agent *PC;
	wxString Name;

	if (Address.Len() == 0)
	{
		wxIPV4address addr;
		bool tryagain;

		do
		{
			tryagain = false;

			Address = wxGetTextFromUser(_("Digite o endere�o IP do computador:"), APP_NAME);
			if (Address.Len() == 0)
				return;

			if (tryagain = !addr.Hostname(Address))
				wxMessageBox(_("O endere�o digitado � inv�lido ou n�o p�de ser encontrado."), APP_NAME, wxICON_EXCLAMATION);
		} while (tryagain);

		Desc = wxEmptyString;
		Desc = wxGetTextFromUser(_("Digite um nome ou descri��o para este agente:"), APP_NAME);
		if (Desc == wxEmptyString)
			return;
	}

	if ((Address.Len() == 0) || (Desc.Len() == 0))
		return;

	if (!stat)
	{
		Name = Desc.BeforeFirst('[');
		Name.RemoveLast();
	}
	else
		Name = Desc;

	// A verifica��o de agente j� existente est� demorando quando tentamos inserir centenas de agentes
	for (unsigned int i = 0; i < agents_stat.size(); i++)
	{
		if (Address == agents_stat[i]->GetIP())
			return;
	}

	for (unsigned int i = 0; i < agents.size(); i++)
	{
		if (Address == agents[i]->GetIP())
			return;
	}

	PC = new Agent();
	PC->SetPanels(panel_system, panel_windows, panel_network, panel_programs, panel_locking, panel_misc, panel_users);
	PC->SetIP(Address);
	PC->SetName(Name);
	PC->SetOwner(this);

	if (stat)
	{
		ag_list_stat->Check(ag_list_stat->Append(Desc + wxT(" [") + Address + wxT("]")), true);
		agents_stat.push_back(PC);
		agents_stat.resize(agents_stat.size());
	}
	else
	{
		ag_list->Check(ag_list->Append(Desc), false);
		agents.push_back(PC);
		agents.resize(agents.size());
	}
}

// Remove um computador da lista
void FrameController::DelPC(void)
{
	// Lista din�mica
	for (int i = 0; i < (int)ag_list->GetCount(); i++)
	{
		if (ag_list->IsChecked(i))
		{
			Agent *ag = agents[i];
			agents.erase(agents.begin() + i);
			ag_list->Delete(i);
			delete ag;
			i = -1;		// Volta para o in�cio do for
		}
	}

	// Lista est�tica
	for (int i = 0; i < (int)ag_list_stat->GetCount(); i++)
	{
		if (ag_list_stat->IsChecked(i))
		{
			ag_list_stat->Delete(i);
			Agent *ag = agents_stat[i];
			agents_stat.erase(agents_stat.begin() + i);
			delete ag;
			i = -1;		// Volta para o in�cio do for
		}
	}

	agents.resize(agents.size());
	agents_stat.resize(agents_stat.size());
}

// Atualiza a lista de agentes
void FrameController::UpdateList(void)
{
	//if (TaskMgr::CountTasks() > 0)
	//{
	//	wxMessageBox(_("Existem tarefas a serem executadas. A remo��o de computadores da lista n�o pode ser feita enquanto existir tarefas em execu��o."), APP_NAME, wxICON_INFORMATION);
	//	return;
	//}

//	ag_list->Clear();
//
//	for (unsigned int i = 0; i < agents.size(); i++)
//		delete agents[i];
//	agents.clear();

	timer_udp->Start();
	wxTimerEvent udpevent(ID_TIMER_UDP, wxEVT_TIMER);
	wxPostEvent(this, udpevent);
}

// Bloqueia os agentes selecionados
void FrameController::LockPC(void)
{
	if (txt_pwd->GetValue().Len() < 1)
	{
		wxMessageBox(_("Antes de executar uma a��o, digite a senha dos agentes no campo \"Senha de conex�o dos agentes\"."), APP_NAME, wxICON_EXCLAMATION);
		txt_pwd->SetFocus();
		return;
	}

	SendToAgents(WPNP_LOCK);
}

// Desliga os agentes selecionados
void FrameController::ShutdownPC(int id)
{
	if (txt_pwd->GetValue().Len() < 1)
	{
		wxMessageBox(_("Antes de executar uma a��o, digite a senha dos agentes no campo \"Senha de conex�o dos agentes\"."), APP_NAME, wxICON_EXCLAMATION);
		txt_pwd->SetFocus();
		return;
	}

	switch (id)
	{
	case ID_MENU_SDOWN:
		SendToAgents(WPNP_SHUTDOWN);
		break;

	case ID_MENU_REBOOT:
		SendToAgents(WPNP_REBOOT);
		break;

	case ID_MENU_LOGOFF:
		SendToAgents(WPNP_LOGOFF);
		break;
	}
}

// Envia as configura��es para os agentes
void FrameController::SendConf(void)
{
	if (txt_pwd->GetValue().Len() < 1)
	{
		wxMessageBox(_("Antes de enviar as restri��es, digite a senha dos agentes no campo \"Senha de conex�o dos agentes\"."), APP_NAME, wxICON_EXCLAMATION);
		txt_pwd->SetFocus();
		return;
	}

	wxString Data, Tmp;

	// Pain�l "Sistema"
	Data.Printf(wxT("%d,"), allusers);
	Data.Append(panel_system->GetChecked());
	if (Data.Len() > 2)
		SendToAgents(WPNP_SYS_TON, Data);
	Data.Printf(wxT("%d,"), allusers);
	Data.Append(panel_system->GetUnchecked());
	if (Data.Len() > 2)
		SendToAgents(WPNP_SYS_TOFF, Data);

	// Pain�l "Windows"
	Data.Printf(wxT("%d,"), allusers);
	Data.Append(panel_windows->GetChecked());
	if (Data.Len() > 2)
		SendToAgents(WPNP_WIN_TON, Data);
	Data.Printf(wxT("%d,"), allusers);
	Data.Append(panel_windows->GetUnchecked());
	if (Data.Len() > 2)
		SendToAgents(WPNP_WIN_TOFF, Data);

	// Pain�l "Rede"
	Data.Printf(wxT("%d,"), allusers);
	Data.Append(panel_network->GetChecked());
	if (Data.Len() > 2)
		SendToAgents(WPNP_NET_TON, Data);
	Data.Printf(wxT("%d,"), allusers);
	Data.Append(panel_network->GetUnchecked());
	if (Data.Len() > 2)
		SendToAgents(WPNP_NET_TOFF, Data);
	SendToAgents(WPNP_NET_PWDLEN, panel_network->GetPwdLen());

	// Pain�l "Programas"
	Data.Printf(wxT("%d,"), allusers);
	Data.Append(panel_programs->GetPrograms());
	if (Data.Len() > 2)
		SendToAgents(WPNP_PROG_LIST, Data);
	Data.Printf(wxT("%d,%d"), allusers, panel_programs->GetTweak());
	SendToAgents(WPNP_PROG_SET, Data);

	// Pain�l "Bloqueio"
	Data = panel_locking->GetChecked();
	if (Data.Len() > 0)
		SendToAgents(WPNP_LOCK_TON, Data);
	Data = panel_locking->GetUnChecked();
	if (Data.Len() > 0)
		SendToAgents(WPNP_LOCK_TOFF, Data);
	Data = panel_locking->GetTimes();
	SendToAgents(WPNP_LOCK_TIMES, Data);

	// Pain�l "Outros"
	Data.Printf(wxT("%d,"), allusers);
	Data.Append(panel_misc->GetChecked());
	if (Data.Len() > 2)
		SendToAgents(WPNP_MISC_TON, Data);
	Data.Printf(wxT("%d,"), allusers);
	Data.Append(panel_misc->GetUnChecked());
	if (Data.Len() > 2)
		SendToAgents(WPNP_MISC_TOFF, Data);
	Data.Printf(wxT("%d,"), allusers);
	Data.Append(panel_misc->GetHiddenDrv());
	if (Data.Len() > 2)
		SendToAgents(WPNP_MISC_HIDE, Data);
	Data.Printf(wxT("%d,"), allusers);
	Data.Append(panel_misc->GetDeniedDrv());
	if (Data.Len() > 2)
		SendToAgents(WPNP_MISC_DENY, Data);

	// Pain�l "Usu�rios"
	vector<UserMgr> users = panel_users->GetUsers();
	SendToAgents(WPNP_USER_DELA);

	for (unsigned int i = 0; i < users.size(); i++)
	{
		SendToAgents(WPNP_USER_ADD, users[i].GetUsername());
		SendToAgents(WPNP_USER_PWD, users[i].GetUsername() + wxT(",") + users[i].GetPassword());

		Data = users[i].GetUsername();
		Data.Append(wxT(","));
		for (unsigned int j = 0; j < users[i].GetTimes().size(); j++)
			Data.Append(wxString::Format(wxT("%s,"), users[i].GetTimes()[j].GetRawTime()));
		SendToAgents(WPNP_USER_TIMES, Data);

		Data = users[i].GetUsername() + wxString::Format(wxT(",%d"), BOOL2INT(users[i].GetCanShutdown()));
		SendToAgents(WPNP_USER_SDOWN, Data);
	}
}

// Eventos do socket UDP
void FrameController::OnDatagramEvent(wxSocketEvent &event)
{
	char buffer[BUF_SIZE] = {0};
	dgram->RecvFrom(*addr_client, buffer, BUF_SIZE);
	wxString Buffer(buffer, wxConvUTF8);
	wxString Code = Buffer.BeforeFirst(' ');
	wxString Description = Buffer.Remove(0, Code.Len() + 1);
	wxString RemoteIP = addr_client->IPAddress();

	int code = atoi((char *)Code.c_str());

	// Trata as mensagens
	switch (code)
	{
	case WPNP_IMHERE:
	{
		vector<wxString> parm = Split(Description, '|', -1);
		if (parm.size() < 3)
		{
			wxString Msg;
			Msg.Append(_("Agente incompat�vel: "));
			Msg.Append(RemoteIP);
			Debug(Msg, DBG_ERR);
			return;
		}

		// Desmonta a mensagem
		wxString Hostname = parm[0];
		wxString NMVersion = parm[1];
		wxString OSDesc = parm[2];

		AddPC(RemoteIP, Hostname + wxT(" [") + RemoteIP + wxT("]: NetModule ") + NMVersion + wxT(", ") + OSDesc);
		break;
	}

	case WPNP_IMLEAVING:
	{
		for (unsigned int i = 0; i < agents.size(); i++)
		{
			if (agents[i]->GetIP() == RemoteIP)
			{
				ag_list->Check(i, false);
				break;
			}
		}
		break;
	}

	default:
		Debug(_("Mensagem UDP desconhecida"), DBG_ERR);
		break;
	}
}

// Obt�m as restri��es do agente selecionado
void FrameController::GetConf(void)
{
	if (txt_pwd->GetValue().Len() < 1)
	{
		wxMessageBox(_("Antes de obter as restri��es, digite a senha dos agentes no campo \"Senha de conex�o dos agentes\"."), APP_NAME, wxICON_EXCLAMATION);
		txt_pwd->SetFocus();
		return;
	}

	int sel, res;
	wxCheckListBox *list_ptr;
	vector<Agent *> ag_aux;

	if ((sel = ag_list->GetSelection()) != wxNOT_FOUND)
	{
		list_ptr = ag_list;
		ag_aux = agents;
	}
	else
	{
		sel = ag_list_stat->GetSelection();
		list_ptr = ag_list_stat;
		ag_aux = agents_stat;
	}

	if (sel == wxNOT_FOUND)
	{
		wxMessageBox(_("Antes de continuar, selecione com o mouse um computador na lista."), APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	res = wxMessageBox(_("As marca��es dos pain�is ser�o substitu�das pelas restri��es do computador \"") + \
		list_ptr->GetStringSelection() + _("\". Deseja continuar?"), APP_NAME, wxICON_QUESTION | wxYES_NO);

	if (res == wxYES)
	{
		// Sistema
		ag_aux[sel]->PushMessage(WPNP_SYS_GTON, wxT(""), txt_pwd->GetValue());
		ag_aux[sel]->PushMessage(WPNP_SYS_GTOFF, wxT(""), txt_pwd->GetValue());

		// Windows Explorer
		ag_aux[sel]->PushMessage(WPNP_WIN_GTON, wxT(""), txt_pwd->GetValue());
		ag_aux[sel]->PushMessage(WPNP_WIN_GTOFF, wxT(""), txt_pwd->GetValue());

		// Rede
		ag_aux[sel]->PushMessage(WPNP_NET_GTON, wxT(""), txt_pwd->GetValue());
		ag_aux[sel]->PushMessage(WPNP_NET_GTOFF, wxT(""), txt_pwd->GetValue());
		ag_aux[sel]->PushMessage(WPNP_NET_GPWD, wxT(""), txt_pwd->GetValue());

		// Programas
		ag_aux[sel]->PushMessage(WPNP_PROG_GSET, wxT(""), txt_pwd->GetValue());
		ag_aux[sel]->PushMessage(WPNP_PROG_GLIST, wxT(""), txt_pwd->GetValue());

		// Bloqueio
		ag_aux[sel]->PushMessage(WPNP_LOCK_GTON, wxT(""), txt_pwd->GetValue());
		ag_aux[sel]->PushMessage(WPNP_LOCK_GTOFF, wxT(""), txt_pwd->GetValue());
		ag_aux[sel]->PushMessage(WPNP_LOCK_GTIMES, wxT(""), txt_pwd->GetValue());

		// Outros
		ag_aux[sel]->PushMessage(WPNP_MISC_GTON, wxT(""), txt_pwd->GetValue());
		ag_aux[sel]->PushMessage(WPNP_MISC_GTOFF, wxT(""), txt_pwd->GetValue());
		ag_aux[sel]->PushMessage(WPNP_MISC_GHIDE, wxT(""), txt_pwd->GetValue());
		ag_aux[sel]->PushMessage(WPNP_MISC_GDENY, wxT(""), txt_pwd->GetValue());

		// Usu�rios
		ag_aux[sel]->PushMessage(WPNP_USER_GLIST, wxT(""), txt_pwd->GetValue());

		// Vers�o do NetModule
		ag_aux[sel]->PushMessage(WPNP_NM_VER, wxT(""), txt_pwd->GetValue());
	}
}

// Aborta as tarefas em execu��o
void FrameController::StopTasks(bool all)
{
	// Mensagem
	if (all)
		Debug(_("Interrompendo todas as tarefas..."));
	else
		Debug(_("Interrompendo tarefas dos agentes selecionados..."));

	// Lista din�mica
	for (unsigned int i = 0; i < ag_list->GetCount(); i++)
		if ((ag_list->IsChecked(i)) || (all))
			agents[i]->RemoveMessages();

	// Lista est�tica
	for (unsigned int i = 0; i < ag_list_stat->GetCount(); i++)
		if ((ag_list_stat->IsChecked(i)) || (all))
			agents_stat[i]->RemoveMessages();
}

// Envia mensagem em broadcast para procurar agentes na rede
void FrameController::OnTimerUDP(wxTimerEvent& event)
{
	wxString Msg;
	Msg.Printf(wxT("%d"), WPNP_WHOISTHERE);
	dgram->SendTo(*addr_bcast, Msg, Msg.Len());
}

// Processa os eventos enviados pelos agentes
void FrameController::OnAgentEvent(wxCommandEvent &event)
{
	if (event.GetString().Len() > 0)
		Debug(event.GetString(), event.GetInt());
}

// Contabiliza as mensagens dos agentes
void FrameController::OnMsgCountEvent(wxCommandEvent &event)
{
	wxString Text;

	Text.Printf(_("Tarefas restantes: %u"), Agent::CountTotalMessages());
	stbar->PushStatusText(Text, 1);
}

// Adiciona texto na caixa de mensagens
void FrameController::Debug(wxString Text, int color_code)
{
	wxColour tcolor;
	wxDateTime TimeNow = wxDateTime::Now();

	switch (color_code)
	{
	case DBG_OK:
		tcolor = wxColour(0, 0, 0);
		break;

	case DBG_ERR:
		tcolor = wxColour(255, 0, 0);
		break;

	case DBG_INFO:
		tcolor = wxColour(0, 127, 0);
		break;

	case DBG_HIGH:
		tcolor = wxColour(0, 0, 127);
		break;

	default:
		tcolor = wxColour(0, 0, 0);
	}

	wxFont tfont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New"));
	wxTextAttr tattr(tcolor, wxNullColour, tfont);

	sm_debug->Wait();
	long pos = txt_msg->GetLastPosition();
	txt_msg->AppendText(TimeNow.Format(wxT("[%H:%M:%S] ")) + Text + wxT("\n"));
	txt_msg->SetStyle(pos, txt_msg->GetLastPosition(), tattr);
	sm_debug->Post();
}

// Insere o comando na fila de tarefas para cada agente
void FrameController::SendToAgents(int code, wxString Data, wxString Password)
{
	if (Password.Len() == 0)
		Password = txt_pwd->GetValue();

	// Lista din�mica
	for (unsigned int i = 0; i < ag_list->GetCount(); i++)
		if (ag_list->IsChecked(i))
			agents[i]->PushMessage(code, Data, Password);

	// Lista est�tica
	for (unsigned int i = 0; i < ag_list_stat->GetCount(); i++)
		if (ag_list_stat->IsChecked(i))
			agents_stat[i]->PushMessage(code, Data, Password);
}
