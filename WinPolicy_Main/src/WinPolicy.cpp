/* Main WinPolicy method
 *
 * Copyright (c) 2004 - 2006 João Paulo Just Peixoto <just1982@gmail.com>.
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

#include "WinPolicy.h"
#include "FrameWinPolicy.h"
#include "AppBase.h"
#include "PwdMgr.h"
#include <windows.h>
#include "jswinreg.h"
#include <gmp.h>
#include <wx/socket.h>
#include <wx/sysopt.h>

// OnInit method: the main method
bool WinPolicy::OnInit(void)
{
	wxString str_wpp, str_pwd;
	bool loadusers = false, silent = false, allusers = false, savetweaks = false;

	AppBase::SetAppDir(argv[0]);
	wxInitAllImageHandlers();
	wxSystemOptions::SetOption(wxT("msw.remap"), 0);

#ifdef USE_NETREG
	wxSocketBase::Initialize();
#endif

	if (AppBase::CheckPrevInstance("WinPolicy", NULL) == true)
		return false;

	// Read the command-line arguments
	for (int i = 1; i < argc; i++)
	{
		if (wxString(argv[i]) == "/load")
		{
			str_wpp = argv[i + 1];
			continue;
		}

		if (wxString(argv[i]) == "/pwd")
		{
			str_pwd = argv[i + 1];
			continue;
		}

		if (wxString(argv[i]) == "/loadusers")
		{
			loadusers = true;
			continue;
		}

		if (wxString(argv[i]) == "/allusers")
		{
			allusers = true;
			continue;
		}

		if (wxString(argv[i]) == "/silent")
		{
			silent = true;
			savetweaks = true;
			continue;
		}

		if ((wxString(argv[i]) == "/setpwd") || (wxString(argv[i]) == "/setpwdf"))
		{
			wxString PwdNow, NewPwd[2];

			if (PwdMgr::PasswdExists())
			{
				if (wxString(argv[i]) == "/setpwdf")
					return false;

				do
				{
					PwdNow = wxGetPasswordFromUser(wxT("Digite a senha atual do WinPolicy para fazer a altera��o ou clique\nem \"Cancelar\" para manter a mesma senha do programa:"), APP_NAME);
					if (PwdNow.Len() == 0)
					{
						wxMessageBox(wxT("Opera��o cancelada!\n\nPara executar a configura��o de senha de administrador novamente, execute o WinPolicy e use o formul�rio na se��o \"Seguran�a\"."), APP_NAME, wxICON_EXCLAMATION);
						return false;
					}
					if (PwdMgr::CheckPasswd(PwdNow) == false)
						wxMessageBox(wxT("A senha digitada est� incorreta!"), APP_NAME, wxICON_EXCLAMATION);
				} while (PwdMgr::CheckPasswd(PwdNow) == false);
			}

			do
			{
				NewPwd[0] = wxGetPasswordFromUser(wxT("Digite uma senha de administrador para o WinPolicy:"), APP_NAME);
				if (NewPwd[0].Len() == 0)
				{
					wxMessageBox(wxT("Opera��o cancelada!\n\nPara executar a configura��o de senha de administrador novamente, execute o WinPolicy e use o formul�rio na se��o \"Seguran�a\"."), APP_NAME, wxICON_EXCLAMATION);
					return false;
				}

				NewPwd[1] = wxGetPasswordFromUser(wxT("Confirme a senha digitada:"), APP_NAME);
				if (NewPwd[0] != NewPwd[1])
					wxMessageBox(wxT("As senhas digitadas n�o coincidem!"), APP_NAME, wxICON_EXCLAMATION);
			} while (NewPwd[0] != NewPwd[1]);

			if (PwdMgr::SetPasswd(NewPwd[0]) == true)
				wxMessageBox(wxT("A nova senha foi configurada com sucesso."), APP_NAME, wxICON_INFORMATION);
			else
				wxMessageBox(wxT("Houve um erro ao tentar salvar a senha. Execute o WinPolicy para tentar novamente."), APP_NAME, wxICON_EXCLAMATION);

			return false;
		}
	}

	// Try to detect Windows version
	int reg_winver = AppBase::GetWinVer(true);
	if ((reg_winver < WINVER_95) || (reg_winver > WINVER_XP))
	{
		bool bres = false;

		switch (AppBase::GetRealWinVer())
		{
		case 0:
			wxMessageBox("N�o foi poss�vel detectar a vers�o do Windows. Voc� deve especificar a vers�o manualmente na se��o \"Perfis\".", \
				APP_NAME, wxICON_EXCLAMATION);
			break;

		case RWINVER_95:	// Windows 95 or NT
			bres = AppBase::SetWinVer(WINVER_NT);
			break;

		case RWINVER_98:	// Windows 98
			bres = AppBase::SetWinVer(WINVER_98);
			break;

		case RWINVER_ME:	// Windows Me
			bres = AppBase::SetWinVer(WINVER_ME);
			break;

		case RWINVER_2K:	// Windows 2000
			bres = AppBase::SetWinVer(WINVER_2K);
			break;

		case RWINVER_XP:	// Windows XP
			bres = AppBase::SetWinVer(WINVER_XP);
			break;

		case RWINVER_2K3:	// Windows 2003
			bres = AppBase::SetWinVer(WINVER_2K3);
			break;

		case RWINVER_VIS:	// Windows Vista
			wxMessageBox("O WinPolicy detectou que voc� est� usando o Windows Vista. " \
				"Esta vers�o do Windows ainda n�o � oficialmente suportada pelo WinPolicy. " \
				"Por�m, o WinPolicy ir� mostrar as op��es do Windows XP, por ser o mais " \
				"pr�ximo do Vista.", APP_NAME, wxICON_EXCLAMATION);
			bres = AppBase::SetWinVer(WINVER_XP);
			break;

		default:
			bres = AppBase::SetWinVer(WINVER_95);
		}

		if (!bres)
			wxMessageBox("Houve um erro ao tentar salvar as configura��es de vers�o do Windows no registro. " \
				"certifique-se de que est� executando o WinPolicy com privil�gios de administrador e tente "\
				"novamente.", APP_NAME, wxICON_ERROR);
	}

	// Main window initialization
	FrameWinPolicy *frame = new FrameWinPolicy();

	// Check the parameters and use them
	if (str_wpp.Len() > 0)
	{
		if ((str_pwd.Len() == 0) && (PwdMgr::PasswdExists() == true))
			str_pwd = wxGetPasswordFromUser("Digite a senha do WinPolicy para carregar o perfil:", APP_NAME);

		if (PwdMgr::CheckPasswd(str_pwd) == false)
		{
			wxMessageBox("Senha incorreta!", APP_NAME, wxICON_EXCLAMATION);
			return false;
		}
		else
			frame->LoadProfile(str_wpp, loadusers, allusers, silent, savetweaks);
	}

	// Show window
	if (silent == false)
	{
		frame->Show();
		SetTopWindow(frame);
	}

	return true;
}
