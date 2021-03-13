/* Class:       FrameController
 * Description: WinPolicy Controller main window frame.
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

#ifndef _FRAMECONTROLLER_H
#define _FRAMECONTROLLER_H

#include <wx/wx.h>
//#include <wx/treectrl.h>
#include <wx/notebook.h>
#include <vector>
#include "ControllerBase.h"
#include "Agent.h"
#include "PanelSystem.h"
#include "PanelWindows.h"
#include "PanelNetwork.h"
#include "PanelPrograms.h"
#include "PanelLocking.h"
#include "PanelMisc.h"
#include "PanelUsers.h"
#include "DialogLanguage.h"

#define MAX_THREADS 10

#define RSA_KEY_SIZE 1024
#define RSA_KEY_N "u0vxd8dsq1ncsqaklw47q4lubaxspnbzdtjrw2zec79uq1prkgcybmcwhqrzejh2y9kzylw5fyuzfr74sehaq7j17b2ofm4r2ovni6k1jhwqqxz5uamnm5amcpxvk7xu1ppfnvtdt0hqa6acgr2wml7asb2kzzidf1t242drxkysws66hocpybcu8slag1df7n8s97"
#define RSA_KEY_E "u0vxd8dsq1ncsqaklw47q4lubaxspnbzdtjrw2zec79uq1prkgcybmcwhqrzejh2y9kzylw5fyuzfr74sehaq7j17b2ofm4r2otti6bekl68mvbvraeo9hs4xost3zwcuys8du3zeh0aiw8mls35fwrx1v2lglmcs46r2otj5c03wf5fl8a0bwckor8jie5s6fi7wd"
/*#define RSA_KEY_D "a0an4eslkojs9krivapekpja3rn9kjrzslulaozss2fa8okl6tgbfvgatwxbsuhozf6zzjaptbmbt92dlgtrkqicefow57dl0w9xu23suv22vmfyl3gw35xdn89lpbysablesm1bsto3iaqvj9d1taxb0mcv5j7g9de90w9uds01at1t72rc3ys6w92ui4pxe562mt"*/

using namespace std;

// IDs da janela
enum
{
	ID_TBAR_ADDPC,
	ID_TBAR_DELPC,
	ID_TBAR_UPDATE,
	ID_TBAR_MISC,
	ID_TBAR_SEND,
	ID_TBAR_GET,
	ID_TBAR_STOP,
	ID_TBAR_STOPALL,
	ID_STATUSBAR,
	ID_LISTDYN,
	ID_LISTSTAT,
	ID_BTN_PCALL,
	ID_BTN_PCNONE,
	ID_BTN_CHPWD,
	ID_MENU_ADDPC,
	ID_MENU_DELPC,
	ID_MENU_UPDATE,
	ID_MENU_EXIT,
	ID_MENU_LOCK,
	ID_MENU_SDOWN,
	ID_MENU_REBOOT,
	ID_MENU_LOGOFF,
	ID_MENU_RUN,
	ID_MENU_SETTIME,
	ID_MENU_SEND,
	ID_MENU_GET,
	ID_MENU_STOP,
	ID_MENU_STOPALL,
	ID_MENU_HELP_MANUAL,
	ID_MENU_HELP_WINPOLICY,
	ID_MENU_HELP_WEB,
	ID_MENU_HELP_ACTIVATION,
	ID_MENU_HELP_LANGUAGE,
	ID_MENU_ABOUT,
	ID_DGRAM_EVENT,
	ID_TIMER_UDP
};

class FrameController: public wxFrame
{
public:
	FrameController(void);
	~FrameController(void);

	void OnToolbarClick(wxCommandEvent &event);
	void OnListClick(wxCommandEvent &event);
	void OnListDClick(wxCommandEvent &event);
	void OnListBtnClick(wxCommandEvent &event);
	void OnChPwdClick(wxCommandEvent &event);
	void OnMenuExit(wxCommandEvent &event);
	void OnMenuAddPC(wxCommandEvent &event);
	void OnMenuDelPC(wxCommandEvent &event);
	void OnMenuUpdate(wxCommandEvent &event);
	void OnMenuLockPC(wxCommandEvent &event);
	void OnMenuShutdownPC(wxCommandEvent &event);
	void OnMenuRun(wxCommandEvent &event);
	void OnMenuSend(wxCommandEvent &event);
	void OnMenuGet(wxCommandEvent &event);
	void OnMenuStop(wxCommandEvent &event);
	void OnMenuStopAll(wxCommandEvent &event);
	void OnMenuHelp(wxCommandEvent &event);
	void OnMenuAbout(wxCommandEvent &event);
	void OnClose(wxCloseEvent &event);
	void AddPC(wxString Address = wxEmptyString, wxString Desc = wxEmptyString, bool stat = false);
	void DelPC(void);
	void UpdateList(void);
	void LockPC(void);
	void ShutdownPC(int id);
	void SendConf(void);
	void GetConf(void);
	void StopTasks(bool all = true);

	void OnTimerUDP(wxTimerEvent& event);
	void OnAgentEvent(wxCommandEvent &event);
	void OnMsgCountEvent(wxCommandEvent &event);
	void OnDatagramEvent(wxSocketEvent &event);

	void Debug(wxString Text, int color_code = DBG_OK);

private:
	void SendToAgents(int code, wxString Data = wxT(""), wxString Password = wxT(""));

	wxSemaphore		*sm_debug;

	wxDatagramSocket	*dgram;
	wxIPV4address	*addr_client;
	wxIPV4address	*addr_bcast;
	wxTimer			*timer_udp;

	// Widgets principais
	wxMenuBar		*mbar;
	wxMenu			*mnu_misc;
	wxToolBar		*tbar;
	wxCheckListBox	*ag_list;
	wxCheckListBox	*ag_list_stat;
	wxNotebook		*tabs;
	wxStatusBar		*stbar;

	//wxTreeCtrl	*ag_tree;
	//wxTreeItemId	agt_root;

	// Controles de texto
	wxTextCtrl		*txt_pwd;
	wxTextCtrl		*txt_msg;

	// Labels
	wxStaticText	*lb_pwd;
	wxStaticText	*lb_dynlist;
	wxStaticText	*lb_statlist;

	// Bot�es
	wxButton		*btn_pcall;
	wxButton		*btn_pcnone;
	wxButton		*btn_chpwd;

	// Outros atributos
	vector<Agent *>	agents;
	vector<Agent *>	agents_stat;
	int				allusers;

	// Pain�is
	PanelSystem		*panel_system;
	PanelWindows	*panel_windows;
	PanelNetwork	*panel_network;
	PanelPrograms	*panel_programs;
	PanelLocking	*panel_locking;
	PanelMisc		*panel_misc;
	PanelUsers		*panel_users;

	DECLARE_EVENT_TABLE()
};
#endif
