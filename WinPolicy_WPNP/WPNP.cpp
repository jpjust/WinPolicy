/* WinPolicy Network Protocol
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

#include "WPNP.h"

// Divide o texto atrav�s de caracteres separadores
vector<wxString> Split(wxString Text, char token, int parts)
{
	vector<wxString> result;

	if (parts == 0)
		return result;

	while (parts != 0)
	{
		wxString Piece;

		if (Text.Len() == 0)
			break;
		else
		{
			Piece = Text.BeforeFirst(token);
			Text.Remove(0, Piece.Len() + 1);
		}

		result.push_back(Piece);
		parts--;
	}

	result.resize(result.size());
	return result;
}

// Envia dados atrav�s do socket em *socket
bool SendData(wxSocketBase *socket, int code, wxString data)
{
	if ((socket == NULL) || (code <= 0) || (socket->IsConnected() == false))
		return false;

	wxString Msg;
	int len;

	Msg.Printf(wxT("%d %s"), code, (char *)data.c_str());
	const char *msg = Msg.mb_str();
	len = Msg.Len();

	if (socket->WaitForWrite() == false)
		return false;
	socket->WriteMsg(msg, sizeof(char) * (len + 1));

	return !socket->Error();
}
