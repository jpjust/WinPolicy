/* WinPolicy Setup
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

#include "WPSetup.h"
#include "AppBase.h"
#include "PwdMgr.h"
#include "TweaksSystem.h"
#include "TweaksWindows.h"
#include "TweaksNetwork.h"
#include "TweaksPrograms.h"
#include "TweaksAutoLock.h"
#include "TweaksMisc.h"
#include "AutoLockBase.h"
#include "NMServiceBase.h"

bool WPSetup::OnInit(void)
{
	// Verifica a senha do admin
	wxString Pwd = wxGetPasswordFromUser(wxT("Para desinstalar o WinPolicy, digite a senha do usu�rio \"admin\":"), wxT("WinPolicy Setup"));
	if (PwdMgr::CheckPasswd(Pwd) == false)
	{
		wxMessageBox(wxT("Senha incorreta! Imposs�vel continuar com a desinstala��o."), APP_NAME, wxICON_ERROR);
		exit(ERR_FAIL);
	}

	// Desativa o AutoLock
	AppBase::ActivateService(false);
	AppBase::StopAutoLockService();

	// Desfaz as restri��es
	if (wxMessageBox(wxT("Deseja desfazer todas as restri��es antes de desinstalar o WinPolicy?"), wxT("WinPolicy Setup"), wxYES_NO | wxICON_QUESTION) == wxYES)
	{
		TweaksSystem tsys(true, false);
		TweaksWindows twin(true, false);
		TweaksNetwork tnet(true, false);
		TweaksPrograms tprog(false);
		TweaksAutoLock talock(false);
		TweaksMisc tmisc(false);

		tsys.SaveTweaks(true);
		twin.SaveTweaks(true);
		tnet.SaveTweaks(true);
		tprog.SaveTweaks(true);
		talock.SaveTweaks();
		tmisc.SaveTweaks(true);
	}

	if (wxMessageBox(wxT("Deseja tamb�m remover a senha do usu�rio \"admin\" do WinPolicy?"), wxT("WinPolicy Setup"), wxYES_NO | wxICON_QUESTION) == wxYES)
		PwdMgr::ErasePasswd();

	// Verifica o AutoLock
	while (AppBase::CheckPrevInstance(ALOCK_MUTEX, NULL) == true)
	{
		wxMessageBox(wxT("Foi detectado que o AutoLock (�cone da bandeija) ainda est� em execu��o. Desative o AutoLock antes de continuar."), APP_NAME, wxICON_EXCLAMATION);
	}

	// Verifica o NetModule
	while (AppBase::CheckPrevInstance(NMSERVICE_MUTEX, NULL) == true)
	{
		wxMessageBox(wxT("Foi detectado que o agente NetModule ainda est� em execu��o. Desative o NetModule antes de continuar."), APP_NAME, wxICON_EXCLAMATION);
	}

	// Tudo OK agora!
	exit(ERR_OK);
	return false;
}
