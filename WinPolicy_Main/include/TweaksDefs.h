#ifndef _TWEAKSDEFS_H
#define _TWEAKSDEFS_H

// Tweaks for the "Windows" section
static const wxChar tweaks_windows[][1024] = {
	wxT("all|win0|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoClose|Desativar os bot?es de desligamento do Windows"),
	wxT("all|win1|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoWinKeys|Desativar as teclas de atalho do Windows"),
	wxT("all|win2|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoWindowsUpdate|Restringir o acesso ao Windows Update"),
	wxT("2kmexp|win3|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|DisablePersonalDirChange|Restringir mudan?a da pasta 'Meus Documentos'"),
	wxT("2kmexp|win4|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|DisableMyPicturesDirChange|Restringir mudan?a da pasta 'Minhas Imagens'"),
	wxT("2kmexp|win5|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|DisableMyMusicDirChange|Restringir mudan?a da pasta 'Minha M?sica'"),
	wxT("2kmexp|win6|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|DisableFavoritesDirChange|Restringir mudan?a da pasta 'Favoritos'"),
	wxT("98me2kxp|win7|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|DisableLocalMachineRun|Desativar a execu??o de programas de inicializa??o"),
	wxT("98me2kxp|win8|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|DisableLocalMachineRunOnce|Desativar a execu??o de programas de inicializa??o (RunOnce)"),
	wxT("98me2kxp|win9|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|DisableCurrentUserRun|Desativar a execu??o de programas de inicializa??o dos usu?rios"),
	wxT("98me2kxp|win10|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|DisableCurrentUserRunOnce|Desativar a execu??o de programas de inicializa??o dos usu?rios (RunOnce)"),
	wxT("me2kxp|win11|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoControlPanel|Desativar o Painel de Controle"),
	wxT("all|win12|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoSetFolders|Desativar o Painel de Controle, pasta das Impressoras e configura??es da rede"),
	wxT("xp|win13|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|ForceClassicControlPanel|For?ar o uso do Painel de Controle cl?ssico"),
	wxT("9598me|win14|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoPrinters|Restringir o acesso ? pasta de Impressoras"),
	wxT("xp|win15|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoThemesTab|Desativar a guia de temas do Windows"),
	wxT("2kxp|win16|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoChangeKeyboardNavigationIndicators|Esconder as configura??es de navega??o do teclado"),
	wxT("2kxp|win17|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoChangeAnimation|Restringir mudan?as das anima??es"),
	wxT("all|win18|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoAddPrinter|Desabilitar a instala??o de impressoras"),
	wxT("all|win19|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoDeletePrinter|Desabilitar a remo??o de impressoras"),
	wxT("9598me|win20|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoPrinterTabs|Desabilitar as op??es gerais e detalhes das impressoras"),
	wxT("all|win21|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoViewContextMenu|Desativar o menu de contexto da ?rea de Trabalho"),
	wxT("all|win22|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|EnforceShellExtensionSecurity|Executar apenas arquivos com extens?es associadas"),
	wxT("2kxp|win23|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoDFSTab|Desabilitar op??es de Sistema de Arquivos Distribu?dos"),
	wxT("xp|win24|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoSecurityTab|Desabilitar a guia de seguran?a"),
	wxT("2kxp|win25|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoHardwareTab|Desabilitar a guia de hardware"),
	wxT("all|win26|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoToolbarCustomize|Desabilitar a personaliza??o da barras de ferramentas"),
	wxT("all|win27|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoBandCustomize|Desabilitar as op??es de esconder e personalizar barras de ferramentas"),
	wxT("all|win28|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoFileMenu|Remover o menu 'Arquivo' do Windows Explorer"),
	wxT("all|win29|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoNetHood|Esconder o ?cone 'Ambiente de Rede'"),
	wxT("all|win30|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoFolderOptions|Desabilitar o menu 'Op??es de Pastas'"),
	wxT("xp|win31|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoPropertiesMyComputer|Desbilitar as propriedades do Meu Computador (Sistema no Painel de Controle)"),
	wxT("2kxp|win32|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoFileAssociate|Desabilitar a mudan?a de tipos de arquivos"),
	wxT("all|win33|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoDesktop|Esconder todos os ?cones da ?rea de Trabalho"),
	wxT("all|win34|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoLogOff|Desabilitar o menu 'Log off'"),
	wxT("2kxp|win35|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoRecentDocsNetHood|Desabilitar compartilhamentos recentes em Meus Locais de Rede"),
	wxT("nt2kxp|win36|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoNetConnectDisconnect|Desabilitar as op??es de mapear e desconectar drives de rede"),
	wxT("2kmexp|win37|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoComputersNearMe|Desabilitar 'Computadores pr?ximos a mim' em Meus Locais de Rede"),
	wxT("2kxp|win38|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoInstrumentation|Desabilitar os relat?rios dos usu?rios"),
	wxT("nt|win39|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoNtSecurity|Remover 'Seguran?a do Windows' no Menu Iniciar"),
	wxT("all|win40|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoSetTaskbar|Desabilitar menu 'Barra de Tarefas' no Menu Iniciar"),
	wxT("all|win41|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoCommonGroups|Remover grupos de programas comuns do Menu Iniciar"),
	wxT("xp|win42|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoStartMenuNetworkPlaces|Desabilitar 'Meus Locais de Rede' no Menu Iniciar"),
	wxT("2kxp|win43|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|ForceStartMenuLogoff|For?ar a exibi??o do 'Log off' no Menu Iniciar"),
	wxT("2kmexp|win44|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|StartMenuLogoff|Remover a op??o 'Log off' do Menu Iniciar"),
	wxT("98me2kxp|win45|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoChangeStartMenu|Desabilitar 'drag-and-drop' no Menu Iniciar"),
	wxT("all|win46|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoRun|Remover a op??o 'Executar' do Menu Iniciar"),
	wxT("all|win47|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoFind|Remover a op??o 'Localizar' do Menu Iniciar"),
	wxT("2kmexp|win48|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoSMHelp|Remover a op??o 'Ajuda' do Menu Iniciar"),
	wxT("xp|win49|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoTrayItemsDisplay|Remover os ?cones da barra de tarefas"),
	wxT("xp|win50|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoToolbarsOnTaskbar|Remover as barras de ferramentas da barra de tarefas"),
	wxT("xp|win51|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|LockTaskbar|Travar a barra de tarefas"),
	wxT("xp|win52|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoSimpleStartMenu|For?ar o uso do Menu Iniciar Cl?ssico"),
	wxT("xp|win53|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|HideClock|Remover o rel?gio da barra de tarefas"),
	wxT("all|win54|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoFavoritesMenu|Remover 'Favoritos' do Menu Iniciar"),
	wxT("all|win55|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoRecentDocsMenu|Remover 'Documentos' do Menu Iniciar"),
	wxT("2kmexp|win56|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoNetworkConnections|Remover 'Conex?es de Rede' do Menu Iniciar"),
	wxT("all|win57|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoRecentDocsHistory|Desabilitar o hist?rico de documentos recentes"),
	wxT("2kmexp|win58|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoSMMyDocs|Remover 'Meus Documentos' do Menu Iniciar"),
	wxT("2kmexp|win59|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoSMMyPictures|Remover 'Minhas Imagens' do Menu Iniciar"),
	wxT("2kmexp|win60|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoStartMenuMyMusic|Remover 'Minha M?sica' do Menu Iniciar"),
	wxT("xp|win61|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoCDBurning|Desabilitar programas de grava??o de CD do Windows"),
	wxT("xp|win74|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoTrayItemsDisplay|Remover os ?cones pr?ximo ao rel?gio da barra de tarefas"),
	wxT("xp|win75|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoToolbarsOnTaskbar|Remover as barras de ferramentas da barra de tarefas"),
	// REPETIDO (win53): wxT("xp|win76|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|HideClock|Remover o rel?gio da barra de tarefas"),
	wxT("all|win77|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoTrayContextMenu|Remover o menu de contexto da barra de tarefas"),
	
	wxT("all|win62|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|ForceActiveDesktopOn|Active Desktop - For?ar o uso do Active Desktop"),
	wxT("all|win63|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoSetActiveDesktop|Active Desktop - Remover 'Active Desktop' do menu 'Configura??es'"),
	wxT("all|win64|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoActiveDesktopChanges|Active Desktop - Restringir mudan?as das configura??es do Active Desktop"),
	wxT("all|win65|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer|NoActiveDesktop|Active Desktop - Desabilitar o Active Desktop"),
	wxT("all|win66|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\ActiveDesktop|NoChangingWallpaper|Active Desktop - Desabilitar as op??es de mudan?a de papel de parede do Active Desktop"),
	wxT("all|win67|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\ActiveDesktop|NoComponents|Active Desktop - Desabilitar os componentes do Active Desktop"),
	wxT("all|win68|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\ActiveDesktop|NoAddingComponents|Active Desktop - Desabilitar as op??es de adicionar componentes do Active Desktop"),
	wxT("all|win69|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\ActiveDesktop|NoDeletingComponents|Active Desktop - Desabilitar as op??es de remover componentes do Active Desktop"),
	wxT("all|win70|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\ActiveDesktop|NoEditingComponents|Active Desktop - Desabilitar as op??es de editar componentes do Active Desktop"),
	wxT("all|win71|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\ActiveDesktop|NoCloseDragDropBands|Active Desktop - Desabilitar as guias 'drag and drop' do Active Desktop"),
	wxT("all|win72|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\ActiveDesktop|NoMovingBands|Active Desktop - Reestringir ajustes nas barras de ferramentas da ?rea de Trabalho do Active Desktop"),
	wxT("all|win73|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\ActiveDesktop|NoHTMLWallPaper|Active Desktop - Permitir apenas bitmaps (BMP) como papel de parede do Active Desktop"),
	wxT("0")};

// Tweaks for the "System" section
static const wxChar tweaks_system[][256] = {
	wxT("all|sys0|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|DisableRegistryTools|Desativar a edi??o do registro do Windows"),
	wxT("all|sys1|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|NoDispCPL|Negar acesso ?s configura??es de v?deo"),
	wxT("xp|sys2|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|NoVisualStyleChoice|Desativar o controle de estilo de janelas e bot?es"),
	wxT("xp|sys3|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|NoColorChoice|Desativar o controle de esquema de cores"),
	wxT("xp|sys4|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|NoSizeChoice|Desativar o controle de tamanho de fonte"),
	wxT("all|sys5|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|NoDispSettingsPage|Ocultar a guia Configura??es nas propriedades de v?deo"),
	wxT("all|sys6|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|NoDispScrSavPage|Ocultar a guia Prote??o de Tela nas propriedades de v?deo"),
	wxT("all|sys7|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|NoDispBackgroundPage|Ocultar a guia Papel de Parede nas propriedades de v?deo"),
	wxT("all|sys8|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|NoDispAppearancePage|Ocultar a guia Apar?ncia nas propriedades de v?deo"),
	wxT("9598me|sys9|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|NoSecCPL|Restringir o acesso ?s configura??es de senhas do Windows"),
	wxT("9598me|sys10|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|NoProfilePage|Ocultar a guia Perfis nas configura??es de senhas"),
	wxT("9598me|sys11|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|NoAdminPage|Ocultar a guia Administra??o Remota nas configura??es de senhas"),
	wxT("9598me|sys12|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|NoPwdPage|Ocultar a guia Alterar Senha nas configura??es de senhas"),
	wxT("9598me|sys13|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|NoVirtMemPage|Ocultar o bot?o de mem?ria virtual nas propriedades do sistema"),
	wxT("9598me|sys14|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|NoFileSysPage|Ocultar o bot?o de sistema de arquivos nas propriedades do sistema"),
	wxT("9598me|sys15|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|NoConfigPage|Ocultar a guia Perfis de Hardware nas propriedades do sistema"),
	wxT("9598me|sys16|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|NoDevMgrPage|Ocultar a guia Gerenciador de Perfis nas propriedades do sistema"),
	wxT("!nt2kxp|sys17|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|ShutdownWithoutLogon|Ativar desligamento do Windows na janela de logon"),
	wxT("nt2kxp|sys18|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|DisableChangePassword|Desativar o bot?o \"Alterar senha\"."),
	wxT("nt2kxp|sys19|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|DisableLockWorkstation|Desativar o bot?o \"Bloquear computador\"."),
	wxT("nt2kxp|sys33|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|DisableTaskMgr|Desativar o bot?o \"Gerenciador de tarefas\"."),
	wxT("2kxp|sys20|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|DontDisplayLastUserName|N?o mostrar o login do ?ltimo usu?rio"),
	wxT("xp|sys21|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|UndockWithoutLogon|Permitir que dispositivos port?teis se desconectem antes do logon"),
	wxT("2kxp|sys22|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System|DisableGPO|Desativar Group Policy Objects"),
	wxT("2kxp|sys23|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Uninstall|NoChooseProgramsPage|Ocultar Padr?es de Acesso a Programas em adicionar e remover programas"),
	wxT("2kxp|sys24|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Uninstall|NoAddRemovePrograms|Desativar o ?cone Adicionar e Remover Programas"),
	wxT("2kxp|sys25|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Uninstall|NoRemovePage|Desativar Alterar e Remover Programas"),
	wxT("2kxp|sys26|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Uninstall|NoAddPage|Desativar Adicionar Programas"),
	wxT("2kxp|sys27|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Uninstall|NoWindowsSetupPage|Desativar o assistente de componentes do Windows"),
	wxT("2kxp|sys28|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Uninstall|NoAddFromCDorFloppy|Ocultar a op??o de adicionar um programa em um disco ou CD-ROM"),
	wxT("2kxp|sys29|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Uninstall|NoAddFromInternet|Ocultar a op??o de adicionar programas da Microsoft"),
	wxT("2kxp|sys30|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Uninstall|NoAddFromNetwork|Ocultar a op??o de adicionar programas da sua rede"),
	wxT("2kxp|sys31|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Uninstall|NoServices|Ir direto para o assistente de componentes do Windows"),
	wxT("2kxp|sys32|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Uninstall|NoSupportInfo|Desabilitar as informa??es de suporte"),
	
	// MSN Messenger (parecem funcionar apenas no MSN 4)
//	wxT("all|sys43|Software\\Policies\\Microsoft\\Messenger\\Client|DisableCollaborationApps|MSN - Desativar compartilhamento de aplicativos"),
//	wxT("all|sys34|Software\\Policies\\Microsoft\\Messenger\\Client|DisableFileTransfer|MSN - Desativar transfer?ncia de arquivos"),
//	wxT("all|sys35|Software\\Policies\\Microsoft\\Messenger\\Client|DisablePC2PCAudio|MSN - Desativar conversa com ?udio"),
//	wxT("all|sys36|Software\\Policies\\Microsoft\\Messenger\\Client|DisablePC2Phone|MSN - Desativar envio de mensagens para celular"),
//	wxT("all|sys37|Software\\Policies\\Microsoft\\Messenger\\Client|DisableVideo|MSN - Desativar uso de webcam"),
//	wxT("all|sys38|Software\\Policies\\Microsoft\\Messenger\\Client|PreventAutoUpdate|MSN - Desativar atualiza??o autom?tica"),
//	wxT("all|sys39|Software\\Policies\\Microsoft\\Messenger\\Client|PreventBackgroundDownload|MSN - Desativar download de plano de fundo"),
//	wxT("all|sys40|Software\\Policies\\Microsoft\\Messenger\\Client|PreventConsumerVersion|MSN - Prevent Consumer Version"),
//	wxT("all|sys41|Software\\Policies\\Microsoft\\Messenger\\Client|PreventAutoRun|MSN - Desativar inicializa??o autom?tica"),
//	wxT("all|sys42|Software\\Policies\\Microsoft\\Messenger\\Client|PreventRun|MSN - Desativar execu??o do MSN"),
	
	wxT("0")};

// Tweaks for the "Network" section
static const wxChar tweaks_network[][256] = {
	wxT("!all|net0|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Network|AlphanumPwds|Permitir apenas senhas alfanum?ricas no Windows"),
	wxT("!all|net1|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Network|DisablePwdCaching|Desativar o cache de senhas"),
	wxT("!all|net2|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Network|HideSharePwds|Ocultar as senhas digitadas com asteriscos"),
	wxT("!all|net3|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Network|NoDialIn|Desativar atendimento de conex?es Dial-Up"),
	wxT("!9598me|net4|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Network|NoFileSharing|Desativar compartilhamento de arquivos"),
	wxT("!9598me|net5|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Network|NoPrintSharing|Desativar o compartilhamento de impressoras"),
	// The first six tweaks must be applied in HKEY_LOCAL_MACHINE
	wxT("9598me|net6|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Network|NoNetSetup|Restringir o acesso ?s Propriedades de Rede"),
	wxT("9598me|net7|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Network|NoNetSetupSecurityPage|Remover a guia 'Seguran?a' das Propriedades de Rede"),
	wxT("9598me|net8|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Network|NoNetSetupIDPage|Remover a guia 'Identifica??o' das Propriedades de Rede"),
	wxT("9598me|net9|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Network|NoFileSharingControl|Remover os controles de compartilhamento de arquivos e impressoras"),
	wxT("all|net10|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Network|NoEntireNetwork|Ocultar 'Toda a rede' no Ambiente de Rede"),
	wxT("all|net11|Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Network|NoWorkgroupContents|Ocultar o conte?do do grupo de trabalho no Ambiente de Rede"),
	wxT("2kxp|net12|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_AddRemoveComponents|Restringir a instala??o e remo??o de componentes de rede"),
	wxT("2kxp|net13|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_AdvancedSettings|Restringir mudan?as nas configura??es avan?adas"),
	wxT("2kxp|net14|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_AllowAdvancedTCPIPConfig|Restringir mudan?as nas configura??es avan?adas de TCP/IP"),
	wxT("2kxp|net15|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_ChangeBindState|Restringir mudan?as nas configura??es de associa??o de servi?os e protocolos"),
	wxT("2kxp|net16|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_DeleteAllUserConnection|Restringir a remo??o de conex?es RAS p?blicas"),
	wxT("2kxp|net17|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_DeleteConnection|Restringir a remo??o de conex?es RAS"),
	wxT("2kxp|net18|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_DialupPrefs|Restringir mudan?as nas prefer?ncias de Dial-Up"),
	wxT("2kxp|net19|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_EnableAdminProhibits|Ativar as conex?es de rede do Windows 2000 para administradores"),
	wxT("2kxp|net20|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_LanChangeProperties|Restringir o acesso ?s propriedades dos componentes das conex?es LAN"),
	wxT("2kxp|net21|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_LanConnect|N?o permitir conectar/desconectar uma conex?o LAN"),
	wxT("2kxp|net22|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_LanProperties|Restringir o acesso ?s propriedades de uma conex?o LAN"),
	wxT("2kxp|net23|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_NewConnectionWizard|Desativar o assistente de conex?o de rede"),
	wxT("2kxp|net24|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_RasAllUserProperties|Restringir o acesso ?s propriedades de conex?es RAS p?blicas"),
	wxT("2kxp|net25|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_RasChangeProperties|Restringir o acesso ?s propriedades de conex?es RAS"),
	wxT("2kxp|net26|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_RasConnect|N?o permitir conectar/desconectar uma conex?o RAS"),
	wxT("2kxp|net27|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_RasMyProperties|Restringir o acesso ?s propriedades de conex?es RAS privadas"),
	wxT("2kxp|net28|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_RenameAllUserRasConnection|N?o permitir o renomeamento de conex?es RAS p?blicas"),
	wxT("2kxp|net29|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_RenameConnection|N?o permitir o renomeamento de conex?es"),
	wxT("2kxp|net30|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_RenameLanConnection|N?o permitir o renomeamento de conex?es LAN"),
	wxT("2kxp|net31|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_RenameMyRasConnection|N?o permitir o renomeamento de conex?es RAS privadas"),
	wxT("2kxp|net32|Software\\Policies\\Microsoft\\Windows\\Network Connections|NC_Statistics|Desativar as estat?sticas das conex?es ativas"),
	wxT("0")};
#endif
