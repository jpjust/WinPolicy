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

#include "jswinreg.h"
#include <string.h>
#include <stdlib.h>

/* Set some execution privileges */
BOOL jsreg_setprivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;
	HANDLE hToken;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	if (!LookupPrivilegeValue(NULL, lpszPrivilege, &luid))
		return FALSE;

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;

	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES) NULL, 0);

	return ((GetLastError() != ERROR_SUCCESS) ? FALSE : TRUE);
}

/* Load a file into a registry key */
int jsreg_loadkey(HKEY hKey, const char *SubKey, const char *Filename)
{
	jsreg_setprivilege(SE_RESTORE_NAME, TRUE);
	jsreg_setprivilege(SE_BACKUP_NAME, TRUE);

	if (RegLoadKey(hKey, SubKey, Filename) != ERROR_SUCCESS)
	{
		jsreg_setprivilege(SE_RESTORE_NAME, FALSE);
		jsreg_setprivilege(SE_BACKUP_NAME, FALSE);
		return 2;
	}

	jsreg_setprivilege(SE_RESTORE_NAME, FALSE);
	jsreg_setprivilege(SE_BACKUP_NAME, FALSE);
	return 0;
}

/* Restore a registry key to a file */
int jsreg_unloadkey(HKEY hKey, const char *SubKey)
{
	HKEY hResult;

	jsreg_setprivilege(SE_RESTORE_NAME, TRUE);
	jsreg_setprivilege(SE_BACKUP_NAME, TRUE);

	// Abre a chave
	if (RegOpenKeyEx(hKey, SubKey, 0, KEY_QUERY_VALUE, &hResult) != ERROR_SUCCESS)
	{
		jsreg_setprivilege(SE_RESTORE_NAME, FALSE);
		jsreg_setprivilege(SE_BACKUP_NAME, FALSE);
		return 1;
	}

	// Salva as altera��es restantes e fecha
	if (RegFlushKey(hResult) != ERROR_SUCCESS)
	{
		jsreg_setprivilege(SE_RESTORE_NAME, FALSE);
		jsreg_setprivilege(SE_BACKUP_NAME, FALSE);
		return 2;
	}
	RegCloseKey(hResult);

	// Descarrega chave
	if (RegUnLoadKey(hKey, SubKey) != ERROR_SUCCESS)
	{
		jsreg_setprivilege(SE_RESTORE_NAME, FALSE);
		jsreg_setprivilege(SE_BACKUP_NAME, FALSE);
		return 3;
	}

	jsreg_setprivilege(SE_RESTORE_NAME, FALSE);
	jsreg_setprivilege(SE_BACKUP_NAME, FALSE);

	return 0;
}

/* Get a value in the registry */
char *jsreg_getvalue(HKEY hKey, const char *SubKey, const char *ValueName, char *DefaultValue)
{
	char *result = NULL;
	HKEY hResult;
	DWORD Type = REG_SZ;
	DWORD cData;

	/* Open the key */
	if (RegOpenKeyEx(hKey, SubKey, 0, KEY_READ, &hResult) != ERROR_SUCCESS)
		return DefaultValue;

	/* Querie the open key */
	if (RegQueryValueEx(hResult, ValueName, NULL, &Type, NULL, &cData) != ERROR_SUCCESS)
		return DefaultValue;

	result = (char *)malloc(sizeof(char) * cData);

	/* Now get the value and store in result */
	if (RegQueryValueEx(hResult, ValueName, NULL, &Type, (LPBYTE)result, &cData) != ERROR_SUCCESS)
		return DefaultValue;

	/* Close the open key */
	RegCloseKey(hResult);

	/* Return the value */
	return result;
}

int jsreg_getvalue_int(HKEY hKey, const char *SubKey, const char *ValueName, int DefaultValue)
{
	HKEY hResult;
	DWORD Type = REG_DWORD;
	DWORD cData = sizeof(int);
	int result;

	/* Open the key */
	if (RegOpenKeyEx(hKey, SubKey, 0, KEY_READ, &hResult) != ERROR_SUCCESS)
		return DefaultValue;

	/* Querie the open key */
	if (RegQueryValueEx(hResult, ValueName, NULL, &Type, (LPBYTE)&result, &cData) != ERROR_SUCCESS)
		return DefaultValue;

	/* Close the open key */
	RegCloseKey(hResult);

	return result;
}

/* Save a value in the registry */
int jsreg_savevalue(HKEY hKey, const char *SubKey, const char *ValueName, const char *Value)
{
	HKEY hResult;
	char *CKey, KeyCopy[MAX_KEY_LEN] = {0}, FinalKey[MAX_KEY_LEN] = {0};

	/* Create the key path */
	strncpy(KeyCopy, SubKey, MAX_KEY_LEN - 1);
	CKey = strtok(KeyCopy, "\\");

	do
	{
		if (FinalKey[0] != 0)
			strcat(FinalKey, "\\");
		strcat(FinalKey, CKey);

		/* It was commented because we think this is causing some erros in Windows 2000 and 2003 (it's not
		 * possible to create keys directly under HKEY_USERS and HKEY_LOCAL_MACHINE) */
		/*if (RegCreateKeyEx(hKey, FinalKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hResult, NULL) != ERROR_SUCCESS)
			return 1;*/
		RegCreateKeyEx(hKey, FinalKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hResult, NULL);

		RegCloseKey(hResult);
	} while ((CKey = strtok(NULL, "\\")) != NULL);

	/* Open the key for writting */
	if (RegOpenKeyEx(hKey, FinalKey, 0, KEY_WRITE, &hResult) != ERROR_SUCCESS)
		return 1;

	/* Save the data */
	if (RegSetValueEx(hResult, ValueName, 0, REG_SZ, (const unsigned char *)Value, strlen((char *)Value) + 1) != ERROR_SUCCESS)
		return 1;

	/* Close the open key */
	RegCloseKey(hResult);

	if (CKey != NULL)
		free(CKey);

	return 0;
}

int jsreg_savevalue_int(HKEY hKey, const char *SubKey, const char *ValueName, int Value)
{
	HKEY hResult;
	BYTE savedata[4] = {0};
	int i;
	char *CKey, KeyCopy[MAX_KEY_LEN] = {0}, FinalKey[MAX_KEY_LEN] = {0};

	for (i = 0; (i < 4) && (Value >= 1); i++)
	{
		savedata[i] = Value % 256;
		Value /= 256;
	}

	/* Create the key path */
	strncpy(KeyCopy, SubKey, MAX_KEY_LEN - 1);
	CKey = strtok(KeyCopy, "\\");

	do
	{
		if (FinalKey[0] != 0)
			strcat(FinalKey, "\\");
		strcat(FinalKey, CKey);

		/* It was commented because we think this is causing some erros in Windows 2000 and 2003 (it's not
		 * possible to create keys directly under HKEY_USERS and HKEY_LOCAL_MACHINE) */
		/*if (RegCreateKeyEx(hKey, FinalKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hResult, NULL) != ERROR_SUCCESS)
			return 1;*/
		RegCreateKeyEx(hKey, FinalKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hResult, NULL);

		RegCloseKey(hResult);
	} while ((CKey = strtok(NULL, "\\")) != NULL);

	/* Open the key for writting */
	if (RegOpenKeyEx(hKey, FinalKey, 0, KEY_WRITE, &hResult) != ERROR_SUCCESS)
		return 1;

	/* Save the data */
	if (RegSetValueEx(hResult, ValueName, 0, REG_DWORD, savedata, 4) != ERROR_SUCCESS)
		return 1;

	/* Close the open key */
	RegCloseKey(hResult);

	return 0;
}

/* Deletes a key or value in the registry */
int jsreg_delkey(HKEY hKey, const char *SubKey)
{
	/* There is a RegDeleteKeyEx function which works with
	 * 64-bits Windows. Take a look on it some day. */

	/* List the keys */
	int cKeys = 0, i;
	char **SubKeysList = jsreg_getsubkeys(hKey, SubKey, &cKeys);
	char SubSubKey[MAX_KEY_LEN] = {0};

	/* Delete the key if there are no subkeys */
	if (cKeys == 0)
	{
		if (RegDeleteKey(hKey, SubKey) != ERROR_SUCCESS)
			return 1;
	}
	else	/* Go deeper to delete the subkeys */
	{
		for (i = 0; i < cKeys; i++)
		{
			if ((strlen(SubKey) + strlen("\\") + strlen(SubKeysList[i])) >= MAX_KEY_LEN)
				return 1;

			strcpy(SubSubKey, SubKey);
			strcat(SubSubKey, "\\");
			strcat(SubSubKey, SubKeysList[i]);

			if (jsreg_delkey(hKey, SubSubKey) != 0)
				return 1;
		}

		/* Finally, delete the key  */
		if (jsreg_delkey(hKey, SubKey) != 0)
			return 1;
	}

	return 0;
}

int jsreg_delvalue(HKEY hKey, const char *SubKey, const char *ValueName)
{
	HKEY hResult;

	/* Open the key */
	if (RegOpenKeyEx(hKey, SubKey, 0, KEY_WRITE, &hResult) != ERROR_SUCCESS)
		return 1;

	/* Delete the value */
	if (RegDeleteValue(hResult, ValueName) != ERROR_SUCCESS)
		return 1;

	RegCloseKey(hResult);

	return 0;
}

/* Get the subkeys inside a key */
char **jsreg_getsubkeys(HKEY hKey, const char *SubKey, int *SubKeysCount)
{
	HKEY hResult;
	DWORD cSubKeys, cMaxSubKeyLen, cSubKeyLen;
	char **result = NULL;
	int i;

	*SubKeysCount = 0;

	/* Open the key */
	if (RegOpenKeyEx(hKey, SubKey, 0, KEY_READ, &hResult) != ERROR_SUCCESS)
		return NULL;

	/* Get the class name and the value count */
    if (RegQueryInfoKey(hResult, NULL, NULL, NULL, &cSubKeys, &cMaxSubKeyLen, NULL,
		NULL, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
		return NULL;

	/* Enumerate the subkeys */
	if (cSubKeys > 0)
	{
		result = (char **)malloc(cSubKeys * sizeof(char *));

		for (i = 0; i < (int)cSubKeys; i++)
		{
			cSubKeyLen = cMaxSubKeyLen + 1;
			result[i] = (char *)malloc(cSubKeyLen * sizeof(char));
			if (RegEnumKeyEx(hResult, i, result[i], &cSubKeyLen, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
				return NULL;
		}
	}
	else
		cSubKeys = 0;

	RegCloseKey(hResult);

	*SubKeysCount = (int)cSubKeys;
	return result;
}

/* Get the values inside a key or subkey */
char **jsreg_getkeyvalues(HKEY hKey, const char *SubKey, int *ValuesCount)
{
	HKEY hResult;
	DWORD cValues, cMaxValueLen, cValueLen;
	char **result = NULL;
	int i;

	*ValuesCount = -1;

	/* Open the key */
	if (RegOpenKeyEx(hKey, SubKey, 0, KEY_READ, &hResult) != ERROR_SUCCESS)
		return NULL;

	/* Get the class name and the value count */
    if (RegQueryInfoKey(hResult, NULL, NULL, NULL, NULL, NULL, NULL,
		&cValues, &cMaxValueLen, NULL, NULL, NULL) != ERROR_SUCCESS)
		return NULL;

	/* Enumerate the values */
	if (cValues > 0)
	{
		result = (char **)malloc(cValues * sizeof(char *));

		for (i = 0; i < (int)cValues; i++)
		{
			cValueLen = cMaxValueLen + 1;
			result[i] = (char *)malloc(cValueLen * sizeof(char));
			if (RegEnumValue(hResult, i, result[i], &cValueLen, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
				return NULL;
		}
	}

	RegCloseKey(hResult);

	*ValuesCount = (int)cValues;
	return result;
}

/* Delete all values inside a key or subkey */
int jsreg_delallvalues(HKEY hKey, const char *SubKey)
{
	int cValues, i;
	char **values = jsreg_getkeyvalues(hKey, SubKey, &cValues);

	if (cValues == -1)
		return 1;

	if (values == NULL)
		return 0;

	for (i = 0; i < cValues; i++)
		if (jsreg_delvalue(hKey, SubKey, values[i]) > 0)
			return 1;

	return 0;
}
