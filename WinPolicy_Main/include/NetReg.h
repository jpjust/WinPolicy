/* Class:       NetReg
 * Description: Performs WinPolicy user's sign up.
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

#ifndef NETREG_H_
#define NETREG_H_

// Defini��es do registro
#define NETREG_URL wxT("http://garoupa.cepedi.org.br/wpreg/wpreg.cgi")
#define NETREG_MANUFACTURER wxT("cepedi")

// Chave
#define NETREG_KEYSIZE 128
#define NETREG_KEY_N "bmmlm6xrau2c0r471a5d34jcj"
#define NETREG_KEY_E "bmmlm6xrau2btvl5tv97tkiz1"
//#define NETREG_KEY_D "3vjj7eb93m0rxyj1xyf2luubp"

#include <wx/wx.h>

class NetReg: public wxThread
{
public:
	virtual void *Entry(void);
};

#endif /*NETREG_H_*/
