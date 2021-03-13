/* NetModule base class
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

#include <vector>
#include "NetModuleBase.h"
#include "WPNP.h"

// These headers are needed to configure WinPolicy
#include "AppBase.h"
#include "PwdMgr.h"
#include "UserMgr.h"
#include "Tweaks.h"
#include "TweaksAutoLock.h"
#include "TweaksMisc.h"
#include "TweaksNetwork.h"
#include "TweaksPrograms.h"
#include "TweaksSystem.h"
#include "TweaksWindows.h"

using namespace std;

/////////////////////////////////////////////////
// SockData class
// Constructor
SockData::SockData(void)
{
	key = rsa_genkey(KEY_SIZE);
	auth_ok = false;
}

// Set methods
void SockData::SetAuth(bool auth)
{
	auth_ok = auth;
}

// Get methods
struct rsa_key_st SockData::GetKey(void)
{
	return key;
}

bool SockData::GetAuth(void)
{
	return auth_ok;
}
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// NetModuleBase class

// Execute the given command
void NetModuleBase::ExecuteCommand(wxString Command, wxSocketBase *client)
{
	// Command header
	SockData *sockdata = (SockData *)client->GetClientData();
	wxString Code = Command.BeforeFirst(' ');
	Command.Remove(0, Code.Len() + 1);
	int code = atoi((char *)Code.c_str());
	bool success = true;
	wxString ErrMsg;

	vector<wxString> parm = Split(Command, ',', -1);

	// Need authentication first
	if ((code != WPNP_AUTH) && (sockdata->GetAuth() == false))
	{
		SendData(client, WPNP_AUTH_FAIL, wxT("A autentica��o n�o foi conclu�da ainda. Conclua a autentica��o antes de prosseguir."));
		code = 0;
	}

	switch (code)
	{
	///////////////////////// System commands ///////////////////////
	case WPNP_SHUTDOWN:
		ErrMsg = MSG_SHUTDOWN;
		success = AppBase::ShutdownWindows(WP_SHUTDOWN);
		break;

	case WPNP_REBOOT:
		ErrMsg = MSG_REBOOT;
		success = AppBase::ShutdownWindows(WP_REBOOT);
		break;

	case WPNP_LOGOFF:
		ErrMsg = MSG_LOGOFF;
		success = AppBase::ShutdownWindows(WP_LOGOFF);
		break;

	case WPNP_LOCK:
		ErrMsg = MSG_LOCK;
		success = AppBase::LockNow(true);
		break;

	case WPNP_SVC_START:
		ErrMsg = MSG_SVC_START;
		success = AppBase::RunAutoLockService();
		break;

	case WPNP_SVC_STOP:
		ErrMsg = MSG_SVC_STOP;
		success = AppBase::StopAutoLockService();
		break;

	case WPNP_NM_VER:
		ErrMsg = wxString(wxT("")) + MSG_NM_VER + wxT(": ") + NETMODULE_VERSION;
		success = true;
		break;

	case WPNP_RUN:
		ErrMsg = wxString(wxT("")) + MSG_RUN + wxT(": ") + Command;
		if (parm.size() != 1) goto fail;
		success = BOOL2INT(wxExecute(Command, wxEXEC_ASYNC));
		break;

	///////////////////////// Authentication ////////////////////////
	case WPNP_AUTH:
	{
		struct rsa_key_st key = sockdata->GetKey();
		wxString Pwd(rsa_decrypt((char *)Command.c_str(), key));
		ErrMsg = wxT("Autentica��o");

		if (PwdMgr::CheckPasswd(Pwd) == true)
		{
			sockdata->SetAuth(true);
			SendData(client, WPNP_AUTH_OK, wxT("Autentica��o OK!"));
		}
		else
		{
			SendData(client, WPNP_AUTH_FAIL, wxT("Autentica��o falhou!"));
			client->Destroy();
			success = false;
		}

		break;
	}

	///////////////////////// Password commands /////////////////////
	case WPNP_DELPWD:
		ErrMsg = MSG_DELPWD;
		success = PwdMgr::ErasePasswd();
		break;

	case WPNP_SETPWD:
	{
		ErrMsg = MSG_SETPWD;
		if (parm.size() != 1)
		{
			success = false;
			break;
		}

		success = PwdMgr::SetPasswdCode(parm[0]);
		break;
	}

	///////////////////////// Users manager /////////////////////////
	case WPNP_USER_ADD:
	{
		ErrMsg = MSG_USER_ADD;
		if (parm.size() != 1) goto fail;
		success = UserMgr::AddUser(parm[0]);
		break;
	}

	case WPNP_USER_DEL:
	{
		ErrMsg = MSG_USER_DEL;
		if (parm.size() != 1) goto fail;
		success = UserMgr::DelUser(parm[0]);
		break;
	}

	case WPNP_USER_DELA:
	{
		ErrMsg = MSG_USER_DELA;
		success = UserMgr::DelAllUsers();
		break;
	}

	case WPNP_USER_PWD:
	{
		ErrMsg = MSG_USER_PWD;
		if (parm.size() != 2) goto fail;
		success = UserMgr::SetPasswordCode(parm[0], parm[1]);
		break;
	}

	case WPNP_USER_TIMES:
	{
		ErrMsg = MSG_USER_TIMES;
		if (parm.size() < 1) goto fail;
		else if (parm.size() == 1)
		{
			success = UserMgr::ClearTimes(parm[0]);
			break;
		}

		for (unsigned int i = 1; i < parm.size(); i++)
			success = (success && UserMgr::AddTime(parm[0], UserMgrTime((char *)parm[i].c_str())));

		break;
	}

	case WPNP_USER_SDOWN:
	{
		ErrMsg = MSG_USER_SDOWN;
		if (parm.size() != 2) goto fail;
		success = UserMgr::SetUserCanShutdown(parm[0], INT2BOOL(atoi((char *)parm[1].c_str())));
		break;
	}

	case WPNP_USER_GLIST:
	{
		ErrMsg = MSG_USER_GLIST;
		vector<wxString> users = UserMgr::GetUsersList();
		wxString Data;

		// Lista de usu�rios
		for (unsigned int i = 0; i < users.size(); i ++)
			Data.Append(users[i] + wxT(":") + UserMgr::GetPassword(users[i]) + wxT(","));

		SendData(client, code, Data);

		// Configura��o de cada usu�rio
		for (unsigned int i = 0; i < users.size(); i ++)
		{
			// Hor�rios
			Data.Clear();
			Data.Append(users[i] + wxT(","));
			vector<UserMgrTime> utimes = UserMgr::GetTimes(users[i]);
			for (unsigned int j = 0; j < utimes.size(); j++)
				Data.Append(wxString(utimes[j].GetRawTime()) + wxT(","));

			SendData(client, WPNP_USER_GTIME, Data);

			// Permiss�o de desligamento do computador
			Data.Clear();
			if (UserMgr::GetUserCanShutdown(users[i]) == true)
				Data.Append(users[i] + wxT(",1"));
			else
				Data.Append(users[i] + wxT(",0"));

			SendData(client, WPNP_USER_GPERM, Data);
		}
		break;
	}

	case WPNP_USER_GPWD:
	{
		ErrMsg = MSG_USER_GPWD;
		if (parm.size() != 1) goto fail;

		wxString Data = parm[0];
		Data.Append(wxString::Format(wxT(",%s"), (char *)UserMgr::GetPassword(parm[0]).mb_str()));
		SendData(client, code, Data);
		break;
	}

	case WPNP_USER_GTIME:
	{
		ErrMsg = MSG_USER_GTIME;
		if (parm.size() != 1) goto fail;

		wxString Data = parm[0];
		vector<UserMgrTime> times = UserMgr::GetTimes(parm[0]);

		for (unsigned int i = 0; i < times.size(); i++)
			Data.Append(wxT(",") + wxString(times[i].GetRawTime()));

		SendData(client, code, Data);
		break;
	}

	case WPNP_USER_GPERM:
	{
		ErrMsg = MSG_USER_GPERM;
		if (parm.size() != 1) goto fail;

		wxString Data = parm[0] + wxString::Format(wxT(",%d"), BOOL2INT(UserMgr::GetUserCanShutdown(parm[0])));
		SendData(client, code, Data);
		break;
	}

	///////////////////////// System tweaks /////////////////////////
	case WPNP_SYS_TON:
	case WPNP_SYS_TOFF:
	{
		if (code == WPNP_SYS_TON)
			ErrMsg = MSG_SYS_TON;
		else
			ErrMsg = MSG_SYS_TOFF;

		if (parm.size() < 2)
		{
			success = false;
			break;
		}

		TweaksSystem Tweaks(true);

		for (unsigned int i = 1; i < parm.size(); i++)
		{
			for (int j = 0; j < Tweaks.GetTweaksCount(); j++)
			{
				if (Tweaks.GetTweak(j).GetTag() == parm[i])
				{
					Tweak NewTweak = Tweaks.GetTweak(j);

					if (code == WPNP_SYS_TON)
						NewTweak.SetData(1);
					else
						NewTweak.SetData(0);

					Tweaks.SetTweak(j, NewTweak);
					break;
				}
			}
		}

		success = Tweaks.SaveTweaks(INT2BOOL(atoi((char *)parm[0].c_str())));
		break;
	}

	case WPNP_SYS_GTON:
	case WPNP_SYS_GTOFF:
	{
		if (code == WPNP_SYS_GTON)
			ErrMsg = MSG_SYS_GTON;
		else
			ErrMsg = MSG_SYS_GTOFF;

		TweaksSystem Tweaks(true, true);
		wxString Data;

		for (int i = 0; i < Tweaks.GetTweaksCount(); i++)
		{
			if (((code == WPNP_SYS_GTON) && (Tweaks.GetTweak(i).GetData() == 1)) ||
				((code == WPNP_SYS_GTOFF) && (Tweaks.GetTweak(i).GetData() == 0)))
				Data.Append(Tweaks.GetTweak(i).GetTag() + wxT(","));
		}

		SendData(client, code, Data);
		break;
	}

	///////////////////////// Windows tweaks ////////////////////////
	case WPNP_WIN_TON:
	case WPNP_WIN_TOFF:
	{
		if (code == WPNP_WIN_TON)
			ErrMsg = MSG_WIN_TON;
		else
			ErrMsg = MSG_WIN_TOFF;

		if (parm.size() < 2)
		{
			success = false;
			break;
		}

		TweaksWindows Tweaks(true);

		for (unsigned int i = 1; i < parm.size(); i++)
		{
			for (int j = 0; j < Tweaks.GetTweaksCount(); j++)
			{
				if (Tweaks.GetTweak(j).GetTag() == parm[i])
				{
					Tweak NewTweak = Tweaks.GetTweak(j);

					if (code == WPNP_WIN_TON)
						NewTweak.SetData(1);
					else
						NewTweak.SetData(0);

					Tweaks.SetTweak(j, NewTweak);
					break;
				}
			}
		}

		success = Tweaks.SaveTweaks(INT2BOOL(atoi((char *)parm[0].c_str())));
		break;
	}

	case WPNP_WIN_GTON:
	case WPNP_WIN_GTOFF:
	{
		if (code == WPNP_WIN_GTON)
			ErrMsg = MSG_WIN_GTON;
		else
			ErrMsg = MSG_WIN_GTOFF;

		TweaksWindows Tweaks(true, true);
		wxString Data;

		for (int i = 0; i < Tweaks.GetTweaksCount(); i++)
		{
			if (((code == WPNP_WIN_GTON) && (Tweaks.GetTweak(i).GetData() == 1)) ||
				((code == WPNP_WIN_GTOFF) && (Tweaks.GetTweak(i).GetData() == 0)))
				Data.Append(Tweaks.GetTweak(i).GetTag() + wxT(","));
		}

		SendData(client, code, Data);
		break;
	}

	///////////////////////// Network tweaks ////////////////////////
	case WPNP_NET_TON:
	case WPNP_NET_TOFF:
	{
		if (code == WPNP_NET_TON)
			ErrMsg = MSG_NET_TON;
		else
			ErrMsg = MSG_NET_TOFF;

		if (parm.size() < 2)
		{
			success = false;
			break;
		}

		TweaksNetwork Tweaks(true);

		for (unsigned int i = 1; i < parm.size(); i++)
		{
			for (int j = 0; j < Tweaks.GetTweaksCount(); j++)
			{
				if (Tweaks.GetTweak(j).GetTag() == parm[i])
				{
					Tweak NewTweak = Tweaks.GetTweak(j);

					if (code == WPNP_NET_TON)
						NewTweak.SetData(1);
					else
						NewTweak.SetData(0);

					Tweaks.SetTweak(j, NewTweak);
					break;
				}
			}
		}

		success = Tweaks.SaveTweaks(INT2BOOL(atoi((char *)parm[0].c_str())));
		break;
	}

	case WPNP_NET_PWDLEN:
	{
		ErrMsg = MSG_NET_PWDLEN;
		if (parm.size() < 1)
		{
			success = false;
			break;
		}

		TweaksNetwork Tweaks;
		Tweaks.SetPwdLen(atoi((char *)Command.c_str()));
		success = Tweaks.SaveTweaks(false);
		break;
	}

	case WPNP_NET_GTON:
	case WPNP_NET_GTOFF:
	{
		if (code == WPNP_NET_GTON)
			ErrMsg = MSG_NET_GTON;
		else
			ErrMsg = MSG_NET_GTOFF;

		TweaksNetwork Tweaks(true, true);
		wxString Data;

		for (int i = 0; i < Tweaks.GetTweaksCount(); i++)
		{
			if (((code == WPNP_NET_GTON) && (Tweaks.GetTweak(i).GetData() == 1)) ||
				((code == WPNP_NET_GTOFF) && (Tweaks.GetTweak(i).GetData() == 0)))
				Data.Append(Tweaks.GetTweak(i).GetTag() + wxT(","));
		}

		SendData(client, code, Data);
		break;
	}

	case WPNP_NET_GPWD:
	{
		ErrMsg = MSG_NET_GPWD;
		TweaksNetwork Tweaks(true, true);
		SendData(client, code, wxString::Format(wxT("%d"), Tweaks.GetPwdLen()));
		break;
	}

	///////////////////////// Programs tweaks ///////////////////////
	case WPNP_PROG_SET:
	{
		ErrMsg = MSG_PROG_SET;
		if (parm.size() < 2)
		{
			success = false;
			break;
		}

		TweaksPrograms Tweaks;
		Tweaks.SetTEnabled(INT2BOOL(atoi((char *)parm[1].c_str())));

		success = Tweaks.SaveTweaks(INT2BOOL(atoi((char *)parm[0].c_str())));
		break;
	}

	case WPNP_PROG_LIST:
	{
		ErrMsg = MSG_PROG_LIST;
		if (parm.size() < 1)
		{
			success = false;
			break;
		}

		TweaksPrograms Tweaks;
		Tweaks.ClearPrograms();

		if (parm.size() > 1)
			for (unsigned int i = 1; i < parm.size(); i++)
				Tweaks.AddFilename(parm[i]);

		success = Tweaks.SaveTweaks(BOOL2INT(atoi((char *)parm[0].c_str())));
		break;
	}

	case WPNP_PROG_GSET:
	{
		ErrMsg = MSG_PROG_GSET;
		TweaksPrograms Tweaks(true);
		SendData(client, code, wxString::Format(wxT("%d"), BOOL2INT(Tweaks.GetTEnabled())));
		break;
	}

	case WPNP_PROG_GLIST:
	{
		ErrMsg = MSG_PROG_GLIST;
		TweaksPrograms Tweaks(true);
		wxString Data;

		for (int i = 0; i < Tweaks.GetProgramsCount(); i++)
			Data.Append(Tweaks.GetTPrograms()[i] + wxT(","));

		SendData(client, code, Data);
		break;
	}

	///////////////////////// Locking tweaks ////////////////////////
	case WPNP_LOCK_TON:
	case WPNP_LOCK_TOFF:
	{
		if (code == WPNP_LOCK_TON)
			ErrMsg = MSG_LOCK_TON;
		else
			ErrMsg = MSG_LOCK_TOFF;

		if (parm.size() < 1)
		{
			success = false;
			break;
		}

		TweaksAutoLock Tweaks;
		bool value = false;
		if (code == WPNP_LOCK_TON)
			value = true;

		for (unsigned int i = 0; i < parm.size(); i++)
		{
			switch (atoi((char *)parm[i].c_str()))
			{
			case 0: Tweaks.SetTALockEnabled(value); break;
			case 1: Tweaks.SetTPwdToLock(value); break;
			case 2: Tweaks.SetTPwdToShutdown(value); break;
			case 3: Tweaks.SetTLockKeys(value); break;
			case 4: Tweaks.SetTLockOnStartup(value); break;
			case 5: Tweaks.SetTLockOnLogon(value); break;
			case 6: Tweaks.SetTLockOnTime(value); break;
			default: break;
			}
		}

		success = Tweaks.SaveTweaks();
		break;
	}

	case WPNP_LOCK_TIMES:
	{
		ErrMsg = MSG_LOCK_TIMES;

		TweaksAutoLock Tweaks;
		Tweaks.ClearTimes();

		if (parm.size() < 1)
		{
			success = Tweaks.SaveTweaks();
			break;
		}

		for (unsigned int i = 0; i < parm.size(); i++)
		{
			unsigned int h, m;
			h = atoi((char *)parm[i].c_str()) / 60;
			m = atoi((char *)parm[i].c_str()) % 60;
			wxDateTime NewTime(h, m);
			Tweaks.AddTime(NewTime);
		}

		success = Tweaks.SaveTweaks();
		break;
	}

	case WPNP_LOCK_GTON:
	case WPNP_LOCK_GTOFF:
	{
		bool enable;

		if (code == WPNP_LOCK_GTON)
		{
			ErrMsg = MSG_LOCK_GTON;
			enable = true;
		}
		else
		{
			ErrMsg = MSG_LOCK_GTOFF;
			enable = false;
		}

		TweaksAutoLock Tweaks(true);
		wxString Data;

		if (Tweaks.GetTPwdToLock() == enable)		Data.Append(wxT("1,"));
		if (Tweaks.GetTPwdToShutdown() == enable)	Data.Append(wxT("2,"));
		if (Tweaks.GetTLockKeys() == enable)		Data.Append(wxT("3,"));
		if (Tweaks.GetTLockOnStartup() == enable)	Data.Append(wxT("4,"));
		if (Tweaks.GetTLockOnLogon() == enable)		Data.Append(wxT("5,"));
		if (Tweaks.GetTLockOnTime() == enable)		Data.Append(wxT("6"));

		SendData(client, code, Data);
		break;
	}

	case WPNP_LOCK_GTIMES:
	{
		ErrMsg = MSG_LOCK_GTIMES;
		TweaksAutoLock Tweaks(true);
		vector<wxDateTime> times = Tweaks.GetTTimes();
		wxString Data;

		for (unsigned int i = 0; i < times.size(); i++)
			Data.Append(wxString::Format(wxT("%d,"), times[i].GetHour() * 60 + times[i].GetMinute()));

		SendData(client, code, Data);
		break;
	}

	///////////////////////// Misc tweaks ///////////////////////////
	case WPNP_MISC_TON:
	case WPNP_MISC_TOFF:
	{
		if (code == WPNP_MISC_TON)
			ErrMsg = MSG_MISC_TON;
		else
			ErrMsg = MSG_MISC_TOFF;

		if (parm.size() < 2)
		{
			success = false;
			break;
		}

		TweaksMisc Tweaks;
		bool value = false;
		if (code == WPNP_MISC_TON)
			value = true;

		for (unsigned int i = 1; i < parm.size(); i++)
		{
			switch (atoi((char *)parm[i].c_str()))
			{
			case 0: Tweaks.SetTNoIEStartPage(value); break;
			case 1: Tweaks.SetTNoDOSRun(value); break;
			case 2: Tweaks.SetTNoDOSMode(value); break;
			case 3: Tweaks.SetTNoF8(value); break;
			case 4: Tweaks.SetTForceLogon(value); break;
			case 5: Tweaks.SetTRequirePassword(value); break;
			default: break;
			}
		}

		success = Tweaks.SaveTweaks(INT2BOOL(atoi((char *)parm[0].c_str())));
		break;
	}

	case WPNP_MISC_HIDE:
	case WPNP_MISC_DENY:
	{
		if (code == WPNP_MISC_HIDE)
			ErrMsg = MSG_MISC_HIDE;
		else
			ErrMsg = MSG_MISC_DENY;

		if (parm.size() != 2)
		{
			success = false;
			break;
		}

		TweaksMisc Tweaks;

		if (code == WPNP_MISC_HIDE)
			Tweaks.SetTHiddenDrives(TweaksMisc::GetDrivesNum(&parm[1]));
		else
			Tweaks.SetTDeniedDrives(TweaksMisc::GetDrivesNum(&parm[1]));

		success = Tweaks.SaveTweaks(INT2BOOL(atoi((char *)parm[0].c_str())));
		break;
	}

	case WPNP_MISC_GTON:
	case WPNP_MISC_GTOFF:
	{
		bool enable;

		if (code == WPNP_MISC_GTON)
		{
			ErrMsg = MSG_MISC_GTON;
			enable = true;
		}
		else
		{
			ErrMsg = MSG_MISC_GTOFF;
			enable = false;
		}

		TweaksMisc Tweaks(true);
		wxString Data;

		if (Tweaks.GetTNoIEStartPage() == enable)		Data.Append(wxT("0,"));
		if (Tweaks.GetTNoDOSRun() == enable)			Data.Append(wxT("1,"));
		if (Tweaks.GetTNoDOSMode() == enable)			Data.Append(wxT("2,"));
		if (Tweaks.GetTNoF8() == enable)				Data.Append(wxT("3,"));
		if (Tweaks.GetTForceLogon() == enable)			Data.Append(wxT("4,"));
		if (Tweaks.GetTRequirePassword() == enable)		Data.Append(wxT("5"));

		SendData(client, code, Data);
		break;
	}

	case WPNP_MISC_GHIDE:
	case WPNP_MISC_GDENY:
	{
		TweaksMisc Tweaks;
		wxString Data;

		if (code == WPNP_MISC_GHIDE)
		{
			ErrMsg = MSG_MISC_GHIDE;
			Data = Tweaks.GetDrivesStr(Tweaks.GetTHiddenDrives());
		}
		else
		{
			ErrMsg = MSG_MISC_GDENY;
			Data = Tweaks.GetDrivesStr(Tweaks.GetTDeniedDrives());
		}

		SendData(client, code, Data);
		break;
	}

	///////////////////////// Unknown command ///////////////////////
	default:
		ErrMsg = wxT("Comando desconhecido");
		success = false;
		break;
	}

	goto end;

	fail:
	success = false;

	end:
	if (success)
		SendData(client, WPNP_OK, ErrMsg + wxT("... OK."));
	else
		SendData(client, WPNP_FAIL, ErrMsg + wxT("... falhou!"));
}
/////////////////////////////////////////////////
