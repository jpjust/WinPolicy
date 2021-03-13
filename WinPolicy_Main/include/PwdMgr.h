/* Password management class
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

#ifndef _PWDMGR_H
#define _PWDMGR_H

#include <wx/string.h>
#include "AppBase.h"

#define RSA_KEY_SIZE 1024
#define RSA_KEY_N "u0vxd8dsq1ncsqaklw47q4lubaxspnbzdtjrw2zec79uq1prkgcybmcwhqrzejh2y9kzylw5fyuzfr74sehaq7j17b2ofm4r2ovni6k1jhwqqxz5uamnm5amcpxvk7xu1ppfnvtdt0hqa6acgr2wml7asb2kzzidf1t242drxkysws66hocpybcu8slag1df7n8s97"
#define RSA_KEY_E "u0vxd8dsq1ncsqaklw47q4lubaxspnbzdtjrw2zec79uq1prkgcybmcwhqrzejh2y9kzylw5fyuzfr74sehaq7j17b2ofm4r2otti6bekl68mvbvraeo9hs4xost3zwcuys8du3zeh0aiw8mls35fwrx1v2lglmcs46r2otj5c03wf5fl8a0bwckor8jie5s6fi7wd"
/*#define RSA_KEY_D "a0an4eslkojs9krivapekpja3rn9kjrzslulaozss2fa8okl6tgbfvgatwxbsuhozf6zzjaptbmbt92dlgtrkqicefow57dl0w9xu23suv22vmfyl3gw35xdn89lpbysablesm1bsto3iaqvj9d1taxb0mcv5j7g9de90w9uds01at1t72rc3ys6w92ui4pxe562mt"*/

class PwdMgr
{
public:
	static wxString GetPasswd(void);
	static bool SetPasswd(wxString NewPasswd);
	static bool SetPasswdCode(wxString NewCode);
	static bool CheckPasswd(wxString Passwd);
	static bool PasswdExists(void);
	static bool ErasePasswd(void);
	static wxString Text2Code(wxString Text);

private:
};

#endif
