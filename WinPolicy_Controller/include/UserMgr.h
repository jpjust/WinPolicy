/* Users management class
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

#ifndef _USERMGR_H
#define _USERMGR_H

#define USERS_RSA_KEY_SIZE 1024
#define USERS_RSA_KEY_N "nuqa4qam1bvbdrltnke1puq6z0az40da9vw4zy3db6224iod772l7rkz6t4t3px02thvsorw6bpfrdrxduib9rtjwcapx9g3ly73bg3htz32uwlz8std4h8jt2j02ywkuwsbi7s6t41nhxna02txxh0ehq1z6ljjlval5y2lp2wugtw5hhk36i2ikk202rtrp6nxep"
#define USERS_RSA_KEY_E "nuqa4qam1bvbdrltnke1puq6z0az40da9vw4zy3db6224iod772l7rkz6t4t3px02thvsorw6bpfrdrxduib9rtjwcapx9g3ly5dxr44sup9qtpqmnohe9oktmm5809rjpi2x3f1fhzp6rl3dowf6m6wmgg1bsjhuxkbuau4si0y7l5bqti4b8jrt010v15j4dfg8v"
/*#define USERS_RSA_KEY_D "eb8kh8krmebl89d3e5188b8il06l2emdd4qhed97zi18h407x48r4nqzihha9fjsuhp4oev4wlf9gfgr8b3sd2aj506fjyguyrotyngvoif5uhtudleou5t5akroqek9btp8y990uwe7wuy987qnidbcdh2f72q3ik4zwkwh9weyxcosnai2l54nvstex0ox2mggy7"*/

#include <wx/string.h>
#include <wx/datetime.h>
#include <vector>

using namespace std;

// User times management
class UserMgrTime
{
public:
	UserMgrTime(void);
	UserMgrTime(wxDateTime time_from, wxDateTime time_until, bool daysofweek[7]);
	UserMgrTime(char rawtime[16]);

	char *GetRawTime(void);
	bool *GetDaysOfWeek(void);
	wxDateTime GetTimeFrom(void);
	wxDateTime GetTimeUntil(void);

	void SetDaysOfWeek(bool daysofweek[7]);
	void SetTimeFrom(unsigned int new_time);
	void SetTimeUntil(unsigned int new_time);

	bool operator ==(const UserMgrTime& x);

private:
	bool t_daysofweek[7];
	wxDateTime t_from;
	wxDateTime t_until;
};

// User management
class UserMgr
{
public:
	UserMgr(void);
	UserMgr(wxString Username);

	void SetUsername(wxString Username);
	void SetPassword(wxString Password);
	void SetPasswordCode(wxString Code);
	void AddTime(UserMgrTime NewTime);
	void DelTime(UserMgrTime Time);
	void SetCanShutdown(bool shutdown);

	wxString GetUsername(void);
	wxString GetPassword(void);
	vector<UserMgrTime> GetTimes(void);
	bool GetCanShutdown(void);

private:
	wxString m_username;
	wxString m_password;
	vector<UserMgrTime> m_times;
	bool m_shutdown;
};

#endif
