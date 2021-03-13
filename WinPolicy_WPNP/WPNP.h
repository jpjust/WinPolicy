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

#ifndef _WPNP_H
#define _WPNP_H

// Constantes do protocolo
#define SERVER_PORT 50000
#define DGRAM_PORT 50001
#define DGRAM_PORT_SENDER 50002
#define BUF_SIZE 1024
#define KEY_SIZE 128
#define SOCK_TIMEOUT 10

/* TABELA DE COMANDOS DO PROTOCOLO
 *
 * Essa tabela define cada comando do protocolo.
 * S�o separados por tipo.
 *
 * Formato: 1 01 00
 *          |  |  |- Comando espec�fico da categoria
 *          |  |---- Categoria
 *          |------- Tipo de comando
 */

// Comandos UDP
#define WPNP_WHOISTHERE	00000	// Try to find the agents
#define WPNP_IMHERE		00001	// Tell the controller I'm online
#define WPNP_IMLEAVING	00002	// Tell the controller I'm leaving

// System commands
#define WPNP_SHUTDOWN	10000	// Turn off computer
#define WPNP_LOCK		10001	// Lock station with WPLocker
#define WPNP_SVC_START	10002	// Start AutoLock service
#define WPNP_SVC_STOP	10003	// Stop AutoLock service
#define WPNP_NM_VER		10004	// Vers�o do NetModule
#define WPNP_REBOOT		10005	// Reinicia computador
#define WPNP_LOGOFF		10006	// Efetua logoff de usu�rio
#define WPNP_RUN		10007	// Executa programa
#define WPNP_TIMER		10008	// Ajusta cron�metro

#define MSG_SHUTDOWN	_("Desligando o computador")
#define MSG_LOCK		_("Bloqueando o computador")
#define MSG_SVC_START	_("Iniciando o servi�o do AutoLock")
#define MSG_SVC_STOP	_("Parando o servi�o do AutoLock")
#define MSG_NM_VER		_("Obtendo vers�o do NetModule")
#define MSG_REBOOT		_("Reiniciando o computador")
#define MSG_LOGOFF		_("Efetuando logoff de usu�rio")
#define MSG_RUN			_("Executando programa")
#define MSG_TIMER		_("Ajustando o cron�metro")

// Authentication commands
#define WPNP_AUTH_KEY	10100	// Authentication: WPNP_AUTH_KEY RSA_N RSA_E
#define WPNP_AUTH		10101	// Authentication: WPNP_AUTH PWD
#define WPNP_AUTH_FAIL	10102	// Authentication failed
#define WPNP_AUTH_OK	10103	// Authentication suceeded

#define MSG_AUTH_KEY	_("Enviando chave p�blica RSA")
#define MSG_AUTH		_("Enviando senha criptografada")
#define MSG_AUTH_FAIL	_("Autentica��o falhou")
#define MSG_AUTH_OK		_("Autentica��o v�lida")

// Password commands
#define WPNP_DELPWD		10200	// Erase password
#define WPNP_SETPWD		10201	// Set password: WPNP_SETPWD PWD

#define MSG_DELPWD		_("Apagando senha")
#define MSG_SETPWD		_("Alterando senha")

// Users commands
#define WPNP_USER_ADD	10300	// Add a new user: WPNP_USER_ADD USERNAME
#define WPNP_USER_DEL	10301	// Delete an existing user: WPNP_USER_DEL USERNAME
#define WPNP_USER_DELA	10305	// Delete all users
#define WPNP_USER_PWD	10302	// Set user's password: WPNP_USER_PWD USERNAME,PWD
#define WPNP_USER_TIMES	10303	// Set user's allowed times: WPNP_USER_TIMES USERNAME,TIME1,TIME2,...
#define WPNP_USER_SDOWN	10304	// Set user's shutdown permission: WPNP_USER_SDOWN USERNAME,VALUE
#define WPNP_USER_GLIST	10306	// Envia lista de usu�rios: WPNP_USER_GLIST USER1,USER2,...
#define WPNP_USER_GPWD	10307	// Envia a senha do usu�rio: WPNP_USER_GPWD USER,PASSWORD
#define WPNP_USER_GTIME	10308	// Envia hor�rios do usu�rio: WPNP_USER_GTIME USER,TIME1,TIME2,...
#define WPNP_USER_GPERM	10309	// Envia permiss�es do usu�rio: WPNP_USER_GPERM USER,SDOWN

#define MSG_USER_ADD	_("Adicionando usu�rio")
#define MSG_USER_DEL	_("Removendo usu�rio")
#define MSG_USER_DELA	_("Removendo todos os usu�rios")
#define MSG_USER_PWD	_("Alterando senha do usu�rio")
#define MSG_USER_TIMES	_("Enviando hor�rio(s) do usu�rio")
#define MSG_USER_SDOWN	_("Enviando permiss�o do usu�rio para desligar o computador")
#define MSG_USER_GLIST	_("Enviando a lista de usu�rios")
#define MSG_USER_GPWD	_("Enviando a senha do usu�rio")
#define MSG_USER_GTIME	_("Enviando os hor�rios do usu�rio")
#define MSG_USER_GPERM	_("Enviando as permiss�es do usu�rio")

// System tweaks
#define WPNP_SYS_TON	10400	// Enable tweak: WPNP_SYS_TON ALLUSERS,TWEAK1,TWEAK2,...
#define WPNP_SYS_TOFF	10401	// Disable tweak: WPNP_SYS_TOFF ALLUSERS,TWEAK1,TWEAK2,...
#define WPNP_SYS_GTON	10402	// Envia restri��es ativadas: WPNP_SYS_GTON TWEAK1,TWEAK2,...
#define WPNP_SYS_GTOFF	10403	// Envia restri��es desativadas: WPNP_SYS_GTOFF TWEAK1,TWEAK2,...

#define MSG_SYS_TON		_("Enviando restri��es ativadas do sistema")
#define MSG_SYS_TOFF	_("Enviando restri��es desativadas do sistema")
#define MSG_SYS_GTON	_("Obtendo restri��es ativadas do sistema")
#define MSG_SYS_GTOFF	_("Obtendo restri��es desativadas do sistema")

// Windows tweaks
#define WPNP_WIN_TON	10500	// Enable tweak: WPNP_WIN_TON ALLUSERS,TWEAK1,TWEAK2,...
#define WPNP_WIN_TOFF	10501	// Disable tweak: WPNP_WIN_TON ALLUSERS,TWEAK1,TWEAK2,...
#define WPNP_WIN_GTON	10502	// Envia restri��es ativadas: WPNP_WIN_GTON TWEAK1,TWEAK2,...
#define WPNP_WIN_GTOFF	10503	// Envia restri��es desativadas: WPNP_WIN_GTOFF TWEAK1,TWEAK2,...

#define MSG_WIN_TON		_("Enviando restri��es ativadas do Windows Explorer")
#define MSG_WIN_TOFF	_("Enviando restri��es desativadas do Windows Explorer")
#define MSG_WIN_GTON	_("Obtendo restri��es ativadas do Windows Explorer")
#define MSG_WIN_GTOFF	_("Obtendo restri��es desativadas do Windows Explorer")

// Network tweaks
#define WPNP_NET_TON	10600	// Enable tweak: WPNP_NET_TON ALLUSERS,TWEAK1,TWEAK2,...
#define WPNP_NET_TOFF	10601	// Enable tweak: WPNP_NET_TON ALLUSERS,TWEAK1,TWEAK2,...
#define WPNP_NET_PWDLEN	10602	// Set password minimum lenght: WPNP_NET_PWSIZE SIZE
#define WPNP_NET_GTON	10603	// Envia restri��es ativadas: WPNP_NET_GTON TWEAK1,TWEAK2,...
#define WPNP_NET_GTOFF	10604	// Envia restri��es desativadas: WPNP_NET_GTOFF TWEAK1,TWEAK2,...
#define WPNP_NET_GPWD	10605	// Envia o comprimento m�nimo da senha: WPNP_NET_GPWD SIZE

#define MSG_NET_TON		_("Enviando restri��es ativadas da rede")
#define MSG_NET_TOFF	_("Enviando restri��es desativadas da rede")
#define MSG_NET_PWDLEN	_("Enviando o comprimento m�nimo da senha de rede")
#define MSG_NET_GTON	_("Obtendo restri��es ativadas da rede")
#define MSG_NET_GTOFF	_("Obtendo restri��es desativadas da rede")
#define MSG_NET_GPWD	_("Obtendo comprimento m�nimo da senha de rede")

// Programs tweaks
#define WPNP_PROG_SET	10700	// Enable/disable tweak: WPNP_PROG_SET ALLUSERS,VALUE
#define WPNP_PROG_LIST	10701	// Set programs list: WPNP_PROG_LIST ALLUSERS,PROG1,PROG2,... (an empty list erases everything)
#define WPNP_PROG_GSET	10702	// Envia a restri��o de programas: WPNP_PROG_GSET VALUE
#define WPNP_PROG_GLIST	10703	// Envia a lista de programas permitidos: WPNP_PROG_GLIST PROG1,PROG2,...

#define MSG_PROG_SET	_("Enviando restri��o de bloqueio de programas")
#define MSG_PROG_LIST	_("Enviando lista de programas permitidos")
#define MSG_PROG_GSET	_("Obtendo restri��o de bloqueio de programas")
#define MSG_PROG_GLIST	_("Obtendo lista de programas permitidos")

// Locking tweaks
#define WPNP_LOCK_TON	10800	// Enable tweak: WPNP_LOCK_TON TWEAK1,TWEAK2,...
#define WPNP_LOCK_TOFF	10801	// Disable tweak: WPNP_LOCK_TON TWEAK1,TWEAK2,...
#define WPNP_LOCK_TIMES	10802	// Set times list: WPNP_LOCK_TIMES TIME1,TIME2,... (empty list clears everything)
#define WPNP_LOCK_GTON	10803	// Envia restri��es ativadas: WPNP_LOCK_GTON TWEAK1,TWEAK2,...
#define WPNP_LOCK_GTOFF	10804	// Envia restri��es desativadas: WPNP_LOCK_GTOFF TWEAK1,TWEAK2,...
#define WPNP_LOCK_GTIMES 10805	// Envia hor�rios de bloqueio: WPNP_LOCK_GTIMES TIME1,TIME2,...

#define MSG_LOCK_TON	_("Enviando restri��es ativadas do AutoLock")
#define MSG_LOCK_TOFF	_("Enviando restri��es desativadas do AutoLock")
#define MSG_LOCK_TIMES	_("Enviando hor�rio(s) de bloqueio")
#define MSG_LOCK_GTON	_("Obtendo restri��es ativadas do AutoLock")
#define MSG_LOCK_GTOFF	_("Obtendo restri��es desativadas do AutoLock")
#define MSG_LOCK_GTIMES	_("Obtendo hor�rio(s) de bloqueio")

// Misc tweaks (format: WPNP_MISC_TWEAK ALLUSERS,TWEAK,VALUE)
#define WPNP_MISC_TON	10900	// Enable tweak: WPNP_MISC_TON ALLUSERS,TWEAK1,TWEAK2,...
#define WPNP_MISC_TOFF	10901	// Disable tweak: WPNP_MISC_TON ALLUSERS,TWEAK1,TWEAK2,...
#define WPNP_MISC_HIDE	10902	// Hidden drives: WPNP_MISC_HIDE ALLUSERS,VALUE
#define WPNP_MISC_DENY	10903	// Denied drives: WPNP_MISC_HIDE ALLUSERS,VALUE
#define WPNP_MISC_GTON	10904	// Envia restri��es ativadas: WPNP_MISC_GTON TWEAK1,TWEAK2,...
#define WPNP_MISC_GTOFF	10905	// Envia restri��es desativadas: WPNP_MISC_GTOFF TWEAK1,TWEAK2,...
#define WPNP_MISC_GHIDE	10906	// Envia configura��o de drives ocultos: WPNP_MISC_GHIDE VALUE
#define WPNP_MISC_GDENY	10907	// Envia configura��o de drives restritos: WPNP_MISC_GDENY VALUE

#define MSG_MISC_TON	_("Enviando restri��es ativadas da se��o \"Outros\"")
#define MSG_MISC_TOFF	_("Enviando restri��es desativadas da se��o \"Outros\"")
#define MSG_MISC_HIDE	_("Enviando configura��o de drives ocultos")
#define MSG_MISC_DENY	_("Enviando configura��o de drives restritos")
#define MSG_MISC_GTON	_("Obtendo restri��es ativadas da se��o \"Outros\"")
#define MSG_MISC_GTOFF	_("Obtendo restri��es desativadas da se��o \"Outros\"")
#define MSG_MISC_GHIDE	_("Obtendo configura��o de drives ocultos")
#define MSG_MISC_GDENY	_("Obtendo configura��o de drives restritos")

///////////////////////////// Answers ///////////////////////////////
// Standard answers
#define WPNP_OK			20000	// Command OK
#define WPNP_FAIL		20001	// Command failed
#define WPNP_NOPWD		20002	// No password defined

#define MSG_NOPWD		_("Sem senha definida no agente")
/////////////////////////////////////////////////////////////////////

// Some useful functions
#include <wx/string.h>
#include <wx/socket.h>
#include <vector>

using namespace std;

// Splits Text using token as separator
vector<wxString> Split(wxString Text, char token, int parts);

// Send data using *socket
bool SendData(wxSocketBase *socket, int code, wxString data);

#endif
