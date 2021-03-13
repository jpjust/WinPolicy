/* Main NetModule class
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

#include "NetModule.h"
#include "NetModuleBase.h"
#include "FrameServer.h"
#include "DialogRegister.h"
#include "AppBase.h"
#include "jswinreg.h"

// Main application's method
bool NetModule::OnInit(void)
{
	if (AppBase::CheckPrevInstance(NETMODULE_MUTEX, NULL) == true)
		return false;

	AppBase::SetAppDir(argv[0]);
	wxSocketBase::Initialize();

	// Ativa��o
	bool demo = false;
	if (DialogRegister::GetPCID() == wxEmptyString)
	{
		wxMessageBox(wxT("N�o foi poss�vel encontrar uma placa de rede ativa neste computador. " \
			"Verifique suas interfaces de rede e inicie o servi�o do NetModule novamente."),
			APP_NM_NAME, wxICON_EXCLAMATION);
		return true;
	}

	if (DialogRegister::SerialCompare(
		wxString(jsreg_getvalue(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Activation", "doc", "")),
		DialogRegister::GetPCID(),
		wxString(jsreg_getvalue(HKEY_LOCAL_MACHINE, "Software\\Justsoft\\WinPolicy\\Activation", "serial", ""))) == false)
	{
		DialogRegister *dialogreg = new DialogRegister();

		if (dialogreg->ShowModal() > 0)
		{
			wxMessageBox(wxString(wxT("")) + \
				wxT("O agente NetModule entrar� em modo de demonstra��o. Nesse modo, o agente aceitar� apenas ") + \
				wxT("conex�es locais. Voc� pode executar o WinPolicy Controller neste mesmo computador para ") + \
				wxT("experimentar uma intera��o com o agente que est� tamb�m neste computador, da mesma forma ") + \
				wxT("que pode ser feita pela rede.\n\n") + \
				wxT("Para exibir a tela de ativa��o novamente, reinicie o servi�o do NetModule."), APP_NM_NAME, wxICON_EXCLAMATION);
			demo = true;
		}
		else
		{
			wxMessageBox( \
				wxT("A ativa��o do produto foi efetuada com sucesso."), APP_NM_NAME, wxICON_INFORMATION);
		}
	}

	FrameServer *frame = new FrameServer(demo);

#ifdef _NETMODULE_DEBUG_
	frame->Show(true);
#endif

	SetTopWindow(frame);
	return true;
}
