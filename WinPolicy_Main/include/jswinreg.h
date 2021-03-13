/* Windows Registry manipulation functions
 *
 * Copyright (c) 2005 João Paulo Just Peixoto <just1982@gmail.com>.
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

#ifndef _JSWINREG_H
#define _JSWINREG_H

#define JSREG_LOGGED_USER ((HKEY)0x800000ff) // Implemente isso aqui
#define MAX_KEY_LEN 512

#include <windows.h>

#ifdef _cplusplus
extern "C"
{
#endif
/* Set some execution privileges */
int jsreg_setprivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);

/* Load a file into a registry key */
int jsreg_loadkey(HKEY hKey, const char *SubKey, const char *Filename);

/* Flush a registry key and unload it */
int jsreg_unloadkey(HKEY hKey, const char *SubKey);

/* Get a value in the registry */
char *jsreg_getvalue(HKEY hKey, const char *SubKey, const char *ValueName, char *DefaultValue);
int jsreg_getvalue_int(HKEY hKey, const char *SubKey, const char *ValueName, int DefaultValue);

/* Save a value in the registry */
int jsreg_savevalue(HKEY hKey, const char *SubKey, const char *ValueName, const char *Value);
int jsreg_savevalue_int(HKEY hKey, const char *SubKey, const char *ValueName, int Value);

/* Delete a key or value in the registry */
int jsreg_delkey(HKEY hKey, const char *SubKey);
int jsreg_delvalue(HKEY hKey, const char *SubKey, const char *ValueName);

/* Get the subkeys inside a key */
char **jsreg_getsubkeys(HKEY hKey, const char *SubKey, int *SubKeysCount);

/* Get the values inside a key or subkey */
char **jsreg_getkeyvalues(HKEY hKey, const char *SubKey, int *ValuesCount);

/* Delete all values inside a key or subkey */
int jsreg_delallvalues(HKEY hKey, const char *SubKey);
#ifdef _cplusplus
}
#endif

#endif
