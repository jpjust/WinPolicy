/* Background frame
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

#include "FrameBackground.h"
#include "DialogWarning.h"
#include "AppBase.h"
#include "PwdMgr.h"
#include "UserMgr.h"
#include "RDiskMgr.h"
#include "TweaksAutoLock.h"
#include "jswinreg.h"
#include <windows.h>
#include <process.h>
#include <vector>

short recdisk_access = 0;
char weekdays_str[][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
bool warn = false;

BEGIN_EVENT_TABLE(FrameBackground, wxFrame)
	EVT_BUTTON(ID_FRAMEBACKGROUND_BTN_OK, FrameBackground::CheckPwd)
	EVT_BUTTON(ID_FRAMEBACKGROUND_BTN_RECOVERY, FrameBackground::RecoveryDisk)
	EVT_BUTTON(ID_FRAMEBACKGROUND_BTN_SHUTDOWN, FrameBackground::Shutdown)
	EVT_TIMER(ID_FRAMEBACKGROUND_TIMER, FrameBackground::OnTimer)
	EVT_TIMER(ID_FRAMEBACKGROUND_TIMER_ENDTIME, FrameBackground::OnTimerEndTime)
	EVT_TEXT_ENTER(ID_FRAMEBACKGROUND_PWDPROMPT, FrameBackground::CheckPwd)
	EVT_TEXT_ENTER(ID_FRAMEBACKGROUND_LOGINPROMPT, FrameBackground::ChangeFocusPwd)
    EVT_PAINT(FrameBackground::OnPaint)
    EVT_CLOSE(FrameBackground::OnTryClose)
END_EVENT_TABLE()

// Class constructor
FrameBackground::FrameBackground(HANDLE mutex)
	:wxFrame(NULL, -1, "WinPolicy Locker", wxDefaultPosition, wxDefaultSize, wxMAXIMIZE | wxSTAY_ON_TOP | wxFRAME_NO_TASKBAR | wxNO_FULL_REPAINT_ON_RESIZE)
{
	if (PwdMgr::PasswdExists() == false)
	{
		wxMessageBox("Nenhuma senha foi definida para o WinPolicy. Defina uma senha antes de ativar o bloqueio.", \
			APP_NAME, wxICON_ERROR);
		//Close(true);
		wxExit();
	}

	proc_mutex = mutex;

	statbox = new wxStaticBox(this, -1, "");
	wxStaticBoxSizer *sizer_box = new wxStaticBoxSizer(statbox, wxVERTICAL);
	wxBoxSizer *sizer_v1 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *sizer_v2 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *sizer_h1 = new wxBoxSizer(wxHORIZONTAL);
	wxGridSizer *sizer_grid = new wxGridSizer(2, 2, 0, 0);

	timer_winpos = new wxTimer(this, ID_FRAMEBACKGROUND_TIMER);
	timer_endtime = new wxTimer(this, ID_FRAMEBACKGROUND_TIMER_ENDTIME);
	wxBitmap bitmap("IMG_TITLE", wxBITMAP_TYPE_BMP_RESOURCE);

	label_prompt = new wxStaticText(this, -1, "Digite sua senha de acesso do Justsoft WinPolicy", \
		wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	label_prompt->SetOwnForegroundColour(wxColour(255, 0, 0));
	label_login = new wxStaticText(this, -1, "Usu�rio:");
	label_pwd = new wxStaticText(this, -1, "Senha:");
	label_recovery = new wxStaticText(this, -1, "Caso tenha esquecido a senha, insira o Disco de Recupera��o\n" \
												"na unidade A: e clique em \"Desbloquear com o disco\".", \
												wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);

	SetBackgroundColour(wxColour(255, 255, 255));
	Refresh();

	// Widgets that can get focus are created in the correct order
	txt_pwd = new wxTextCtrl(this, ID_FRAMEBACKGROUND_PWDPROMPT, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	btn_ok = new wxButton(this, ID_FRAMEBACKGROUND_BTN_OK, "OK");
	btn_recovery = new wxButton(this, ID_FRAMEBACKGROUND_BTN_RECOVERY, "Desbloquear com o disco");
	btn_shutdown = new wxButton(this, ID_FRAMEBACKGROUND_BTN_SHUTDOWN, "Desligar o computador");
	txt_login = new wxTextCtrl(this, ID_FRAMEBACKGROUND_LOGINPROMPT, "admin");

	// Layout development
	sizer_grid->Add(label_login, 0, wxALL, 5);
	sizer_grid->Add(txt_login, 0, wxALL, 5);
	sizer_grid->Add(label_pwd, 0, wxALL, 5);
	sizer_grid->Add(txt_pwd, 0, wxALL, 5);

	sizer_h1->Add(sizer_grid, 0, wxALL, 0);
	sizer_h1->Add(btn_ok, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	sizer_v2->Add(label_recovery, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	sizer_v2->Add(btn_recovery, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	sizer_v2->Add(btn_shutdown, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

	sizer_box->Add(bitmap.GetWidth(), bitmap.GetHeight(), 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	//sizer_box->Add(panel_blank, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	sizer_box->Add(label_prompt, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	sizer_box->Add(sizer_h1, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	sizer_box->Add(sizer_v2, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

	sizer_v1->Add(0, 0, 1, wxALL | wxEXPAND, 5);
	sizer_v1->Add(sizer_box, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	sizer_v1->Add(0, 0, 1, wxALL | wxEXPAND, 5);

	SetSizer(sizer_v1);
	Layout();

	// Lock keys
	timer_winpos->Start(1);
	AppBase::DisableKeyboard(true);
	txt_pwd->SetFocus();
	ShowFullScreen(true);
}

// OnPaint method
void FrameBackground::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dcp(this);
	wxScreenDC dc;
	PrepareDC(dc);

	wxBitmap bitmap("IMG_TITLE", wxBITMAP_TYPE_BMP_RESOURCE);

	dc.Clear();
	dc.DrawBitmap(bitmap, statbox->GetPosition().x + 10, statbox->GetPosition().y + 15, false);
}

// OnTryClose method
void FrameBackground::OnTryClose(wxCloseEvent &event)
{
	if (event.CanVeto())
		event.Veto(true);
}

// CheckPwd method
void FrameBackground::CheckPwd(wxCommandEvent &event)
{
	bool access = false;

	if (txt_login->GetValue() == "admin")
		access = PwdMgr::CheckPasswd(txt_pwd->GetValue());
	else
	{
		// First, let's check the user's password
		if (UserMgr::CheckPasswd(txt_login->GetValue(), txt_pwd->GetValue()) == true)
		{
			// Now, let's check if the user is allowed to use the computer
			access = UserMgr::IsTimeAllowed(txt_login->GetValue(), wxDateTime::Now(), &end_time);
		}
	}

	if (access)
	{
		// Re-enable keys
		AppBase::DisableKeyboard(false);

		// If "admin" closed the WPLocker, let's go away...
		if (txt_login->GetValue() == "admin")
		{
			//Close(true);
			wxExit();
		}

		// Otherwise, we must keep running to lock again when the time is over
		Show(false);
		ShowFullScreen(false);
		timer_winpos->Stop();
		timer_endtime->Start(1000);

		// We must now close the mutex, so when another process of WPLocker starts
		// running it won't be blocked
		CloseHandle(proc_mutex);
	}
	else
		label_prompt->SetLabel("ACESSO NEGADO!");

	txt_pwd->Clear();
	recdisk_access = 0;
}

// RecoveryDisc method
void FrameBackground::RecoveryDisk(wxCommandEvent &event)
{
	_beginthread(recdisk_thread, 0, NULL);
}

// Shutdown method
void FrameBackground::Shutdown(wxCommandEvent &event)
{
	TweaksAutoLock Tweaks;

	// Check if we need to ask for a password before shutdown
	if (Tweaks.GetTPwdToShutdown() == false)
	{
		AppBase::ShutdownWindows();
		return;
	}

	if (txt_login->GetValue() == "admin")
	{
		// Admin user
		if (PwdMgr::CheckPasswd(txt_pwd->GetValue()))
		{
			AppBase::ShutdownWindows();
			return;
		}
	}
	else
	{
		// Ordinary user
		if ((UserMgr::CheckPasswd(txt_login->GetValue(), txt_pwd->GetValue())) &&
			(UserMgr::GetUserCanShutdown(txt_login->GetValue())))
		{
			AppBase::ShutdownWindows();
			return;
		}
	}

	label_prompt->SetLabel("ACESSO NEGADO!");
	txt_pwd->Clear();
}

// OnTimer method
void FrameBackground::OnTimer(wxTimerEvent &event)
{
	// Keep the window above others
	SetWindowPos((HWND)GetHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_ASYNCWINDOWPOS | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

	// Check rescovery disk access
	switch (recdisk_access)
	{
	case 1:
		// Re-enable keys
		AppBase::DisableKeyboard(false);

		//Close(true);
		wxExit();
		break;

	case 2:
		label_prompt->SetLabel("DISCO DE RECUPERA��O INV�LIDO!");
	}
}

// OnTimerEndTime method
void FrameBackground::OnTimerEndTime(wxTimerEvent &event)
{
	wxDateTime time_now = wxDateTime::Now();

	// Warn when there is only one minute left
	if ((end_time.Subtract(time_now).GetSeconds() <= 60) && (warn == false))
	{
		warn = true;
		DialogWarning *dialog_warning = new DialogWarning(this);
		dialog_warning->Show();
	}

	// Lock it again!
	if (time_now.IsLaterThan(end_time) == true)
	{
		if (AppBase::CheckPrevInstance("WPLocker", &proc_mutex) == true)
		{
			//Close(true);
			wxExit();
		}

		wxPaintEvent event(wxID_ANY);
		ShowFullScreen(true);
		OnPaint(event);
		Refresh();
		AppBase::DisableKeyboard(true);
		timer_winpos->Start(1);
		timer_endtime->Stop();
		warn = false;
		label_prompt->SetLabel(wxT("Digite sua senha de acesso do Justsoft WinPolicy"));
	}
}

// ChangeFocusPwd method
void FrameBackground::ChangeFocusPwd(wxCommandEvent &event)
{
	txt_pwd->SetFocus();
}

// redisc_thread function
__cdecl void recdisk_thread(void *param)
{
	if (RDiskMgr::CheckRescuePwd(false) == true)
		recdisk_access = 1;
	else
		recdisk_access = 2;
}
