/* AutoLock Tasbar Icon class
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

#include "TaskIconAutoLock.h"
#include "TweaksAutoLock.h"
#include "AppBase.h"
#include "PwdMgr.h"
#include "UserMgr.h"
#include "jswinreg.h"
#include "DialogWarning.h"
#include "WPServiceBase.h"
#include <vector>

bool warn = false;

BEGIN_EVENT_TABLE(TaskIconAutoLock, wxTaskBarIcon)
	EVT_MENU(ID_TASKBAR_MENU_RUNWP, TaskIconAutoLock::RunWinPolicy)
	EVT_MENU(ID_TASKBAR_MENU_EXIT, TaskIconAutoLock::QuitProgram)
	EVT_MENU(ID_TASKBAR_MENU_SETCLOCK, TaskIconAutoLock::SetClock)
	EVT_MENU(ID_TASKBAR_MENU_LOCKNOW, TaskIconAutoLock::LockNow)
	EVT_MENU(ID_TASKBAR_MENU_SHUTDOWN, TaskIconAutoLock::Shutdown)
	EVT_MENU(ID_TASKBAR_MENU_LOGOFF, TaskIconAutoLock::Shutdown)
	EVT_TIMER(ID_TASKBAR_TIMER_ALOCK, TaskIconAutoLock::OnTimer)
	EVT_TIMER(ID_TASKBAR_TIMER_CLOCK, TaskIconAutoLock::OnTimerClock)
END_EVENT_TABLE()

// Class constructor
TaskIconAutoLock::TaskIconAutoLock(wxIcon icon, wxString tooltip)
	:wxTaskBarIcon()
{
	SetIcon(icon, tooltip);

	timer_alock = new wxTimer(this, ID_TASKBAR_TIMER_ALOCK);
	timer_alock->Start(1000);

	timer_clock = new wxTimer(this, ID_TASKBAR_TIMER_CLOCK);
}

// CreatePopupMenu method
wxMenu *TaskIconAutoLock::CreatePopupMenu()
{
	wxMenu *taskbar_menu = new wxMenu(_("WinPolicy AutoLock"));
	wxString str_clock;

	if (clock_time > 0)
		str_clock = _("Parar cron�metro");
	else
		str_clock = _("Ajustar cron�metro");

	wxString str_item[] = {
		wxString(_("Abrir o WinPolicy")),
		str_clock,
		wxString(_("Bloquear agora")),
		wxString(_("Desligar o computador")),
		wxString(_("Efetuar logoff")),
		wxString(_("Sair"))
	};

	taskbar_menu->Append(ID_TASKBAR_MENU_RUNWP, str_item[0]);
	taskbar_menu->Append(ID_TASKBAR_MENU_SETCLOCK, str_item[1]);
	taskbar_menu->Append(ID_TASKBAR_MENU_LOCKNOW, str_item[2]);
	taskbar_menu->Append(ID_TASKBAR_MENU_SHUTDOWN, str_item[3]);
	taskbar_menu->Append(ID_TASKBAR_MENU_LOGOFF, str_item[4]);
	taskbar_menu->AppendSeparator();
	taskbar_menu->Append(ID_TASKBAR_MENU_EXIT, str_item[5]);

	return taskbar_menu;
}

// RunWinPolicy method
void TaskIconAutoLock::RunWinPolicy(wxCommandEvent &event)
{
	AppBase::RunWinPolicy();
}

// SetClock method
void TaskIconAutoLock::SetClock(wxCommandEvent &event)
{
	wxString str_pwd = wxGetPasswordFromUser("Digite a senha do WinPolicy para configurar o cron�metro:", APP_NAME);

	if (PwdMgr::CheckPasswd(str_pwd) == false)
	{
		wxMessageBox("Senha incorreta!", APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	if (clock_time <= 0)
	{
		clock_time = wxGetNumberFromUser("O cron�metro do AutoLock ir� bloquear o computador\n" \
			"automaticamente quando seu tempo esgotar.\n" \
			"Insira o tempo para o bloqueio do cron�metro\n" \
			"abaixo e clique em \"OK\" para iniciar a contagem.", \
			"Digite o tempo em minutos:", APP_NAME, 0, 0, 1440);
	}
	else
		clock_time = 0;

	if (clock_time > 0)
		timer_clock->Start(60000);
	else
		timer_clock->Stop();

	RefreshTooltip();
}

// LockNow method
void TaskIconAutoLock::LockNow(wxCommandEvent &event)
{
	wxString str_pwd;
	TweaksAutoLock Tweaks;

	if (Tweaks.GetTPwdToLock() == true)
	{
		str_pwd = wxGetPasswordFromUser("Digite a senha do WinPolicy para bloquear o computador:", APP_NAME);
		if (PwdMgr::CheckPasswd(str_pwd) == false)
		{
			wxMessageBox("Senha incorreta!", APP_NAME, wxICON_EXCLAMATION);
			return;
		}
	}

	AppBase::LockNow(true);
}

// Shutdown method
void TaskIconAutoLock::Shutdown(wxCommandEvent &event)
{
	TweaksAutoLock Tweak;
	ShutdownType stype = WP_SHUTDOWN;
	wxString Action;

	switch (event.GetId())
	{
	case ID_TASKBAR_MENU_SHUTDOWN:
		stype = WP_SHUTDOWN;
		Action = _("desligar");
		break;

	case ID_TASKBAR_MENU_LOGOFF:
		stype = WP_LOGOFF;
		Action = _("efetuar logoff");
		break;
	}

	// Verifica se precisa de senha
	if (Tweak.GetTPwdToShutdown() == false)
	{
		AppBase::ShutdownWindows(stype);
		return;
	}

	wxString str_username = wxGetTextFromUser(_("Digite seu nome de usu�rio do WinPolicy:"), APP_NAME, wxT("admin"));

	if (str_username == wxEmptyString)
		return;

	wxString str_pwd = wxGetPasswordFromUser(_("Usu�rio: ") + str_username + _("\n\nDigite sua senha do WinPolicy para ") + Action + wxT(":"), APP_NAME);

	if (str_username == wxT("admin"))
	{
		// Admin user
		if (PwdMgr::CheckPasswd(str_pwd) == false)
		{
			wxMessageBox(_("Senha incorreta!"), APP_NAME, wxICON_EXCLAMATION);
			return;
		}
	}
	else
	{
		// Ordinary user
		if (UserMgr::CheckPasswd(str_username, str_pwd) == false)
		{
			wxMessageBox(_("Senha incorreta!"), APP_NAME, wxICON_EXCLAMATION);
			return;
		}

		if (UserMgr::GetUserCanShutdown(str_username) == false)
		{
			wxMessageBox(_("Voc� n�o tem permiss�o para ") + Action + wxT("!"), APP_NAME, wxICON_EXCLAMATION);
			return;
		}
	}

	AppBase::ShutdownWindows(stype);
}

// QuitProgram method
void TaskIconAutoLock::QuitProgram(wxCommandEvent &event)
{
	wxString str_pwd = wxGetPasswordFromUser("Digite a senha do WinPolicy para encerrar o programa:", APP_NAME);
	if (PwdMgr::CheckPasswd(str_pwd) == false)
	{
		wxMessageBox("Senha incorreta!", APP_NAME, wxICON_EXCLAMATION);
		return;
	}

	AppBase::StopAutoLockService();

	RemoveIcon();
	wxExit();
}

// OnTimer method
void TaskIconAutoLock::OnTimer(wxTimerEvent &event)
{
	// If we are running on a NT environment, we need to check
	// if WPService.exe is running. If not, AutoLock.exe must shutdown.
	if (AppBase::GetRealWinVer() >= 500)
	{
		if (AppBase::CheckPrevInstance(WPSERVICE_MUTEX, NULL) == false)
		{
			RemoveIcon();
			wxExit();
		}
	}

	TweaksAutoLock Tweak;

	// Save WPLocker.exe path
	jsreg_savevalue(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy", "Path", (char *)AppBase::GetAppDir().c_str());

	// Key locking
	AppBase::DisableKeyboard(Tweak.GetTLockKeys());

	// Time locking
	if (Tweak.GetTLockOnTime() == false)
		return;

	wxDateTime time_now = wxDateTime::Now();
	vector<wxDateTime> times = Tweak.GetTTimes();

	time_now.SetSecond(0);

	if (time_now == prev_time)
		return;

	for (unsigned int i = 0; i < times.size(); i++)
	{
		times[i].SetSecond(0);
		if (times[i].IsSameTime(time_now))
		{
			prev_time = time_now;
			AppBase::LockNow(true);
		}
	}
}

// OnTimerClock event
void TaskIconAutoLock::OnTimerClock(wxTimerEvent &event)
{
	if (--clock_time == 0)
	{
		AppBase::LockNow(true);
		timer_clock->Stop();
		warn = false;
	}
	else if ((clock_time == 1) && (warn == false))
	{
		warn = true;
		DialogWarning *dialog_warning = new DialogWarning(NULL);
		dialog_warning->Show();
	}

	RefreshTooltip();
}

// RefreshTooltip method
void TaskIconAutoLock::RefreshTooltip(void)
{
	wxString str_tooltip;

	if (clock_time > 0)
		str_tooltip.Printf("WinPolicy Autolock - Tempo para o bloqueio: %ld minuto(s)", clock_time);
	else
		str_tooltip.Append("WinPolicy Autolock");

	SetIcon(wxIcon("wxICON", wxBITMAP_TYPE_ICO_RESOURCE), str_tooltip);
}
