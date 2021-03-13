; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "Justsoft WinPolicy"
!define PRODUCT_VERSION "3.1.5 Freeware"
!define PRODUCT_PUBLISHER "Justsoft Inform�tica Ltda."
!define PRODUCT_WEB_SITE "http://www.justsoft.com.br/"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\WinPolicy.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"

SetCompressor bzip2

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "justsoft.ico"
!define MUI_UNICON "justsoft.ico"
!define MUI_CUSTOMFUNCTION_UNGUIINIT "un.CustomGUIInit"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "licenca.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Start menu page
var ICONS_GROUP
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "Justsoft WinPolicy"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\WinPolicy.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "PortugueseBR"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "winpolicy-3.1.5.exe"
InstallDir "$PROGRAMFILES\Justsoft WinPolicy"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "WinPolicy" SEC01
  ; Program files
  SetOutPath "$INSTDIR"
  SetOverwrite on
  File "..\WinPolicy\Release\WinPolicy.exe"
  File "..\AutoLock\Release\AutoLock.exe"
  File "..\WPService\Release\WPService.exe"
  File "..\WPSetup\Release\WPSetup.exe"
  File "..\WinPolicy\resources\img\wpp.ico"
  
  ; Help files
  SetOutPath "$INSTDIR\help"
  SetOverwrite on
  File "..\help\*.html"
  File "..\help\*.css"
  File "..\help\*.gif"
  
  ; Profiles
  SetOutPath "$INSTDIR\Perfis do WinPolicy"
  SetOverwrite on
  File "..\Perfis\*.wpp"

  ; WinPolicy system files
  SetOutPath "$SYSDIR"
  SetOverwrite on
  File "..\WPHooks\Release\WPHooks.dll"
  File "..\WPSSaver\Release\WinPolicy.scr"
  File "..\WPLocker\Release\WPLocker.exe"
  
  ; System libraries
  SetOutPath "$SYSDIR"
  SetOverwrite ifnewer
  File "C:\WINDOWS\system32\librsatool.dll"
  File "C:\WINDOWS\system32\libgmp-3.dll"
  File "C:\MinGW\bin\mingwm10.dll"
  
  ; wxWidgets files
  SetOutPath "$SYSDIR"
  SetOverwrite ifnewer
  File "C:\wxMSW-2.6\lib\wxbase26_gcc_justsoft.dll"
  File "C:\wxMSW-2.6\lib\wxmsw26_core_gcc_justsoft.dll"
  File "C:\wxMSW-2.6\lib\wxmsw26_adv_gcc_justsoft.dll"

  ; Shortcuts
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  SetOutPath "$INSTDIR"     ; Sets working directory
  SetShellVarContext all    ; Context of variables (creates shortcuts for all users)
  CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Justsoft WinPolicy.lnk" "$INSTDIR\WinPolicy.exe"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Ajuda do WinPolicy.lnk" "$INSTDIR\help\index.html"
  CreateShortCut "$DESKTOP\Justsoft WinPolicy.lnk" "$INSTDIR\WinPolicy.exe"
  !insertmacro MUI_STARTMENU_WRITE_END

  ; AutoLock Service
  Version::IsWindowsPlatformNT
  Pop $0
  StrCmp $0 "0" ItIsNotNT
  
  Exec '"$INSTDIR\WPService.exe" /install'
  
  ItIsNotNT:
  ExecWait '"$INSTDIR\WinPolicy.exe" /setpwdf'
  ExecShell "open" "http://www.justsoft.com.br/winpolicy/register/"
SectionEnd

Section -AdditionalIcons
  SetOutPath $INSTDIR
  SetShellVarContext all
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\WinPolicy.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\WinPolicy.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  
  ;.WPP files association
  WriteRegStr HKCR ".wpp" "" "WinPolicyProfile"
  WriteRegStr HKCR "WinPolicyProfile" "" "Perfil do WinPolicy"
  WriteRegStr HKCR "WinPolicyProfile" "DefaultIcon" '"$INSTDIR\wpp.ico"'
  WriteRegStr HKCR "WinPolicyProfile\shell" "" "open"
  WriteRegStr HKCR "WinPolicyProfile\shell\open" "" "&Carregar Perfil"
  WriteRegStr HKCR "WinPolicyProfile\shell\open\command" "" '"$INSTDIR\WinPolicy.exe" /load "%1"'
SectionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) foi removido com sucesso do seu computador."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Tem certeza que quer remover completamente $(^Name) e todos os seus componentes?" IDYES +2
  Abort
FunctionEnd

; Executa janela de prepara��o para desinstala��o
Function un.CustomGUIInit
  ExecWait '"$INSTDIR\wpsetup.exe"' $0
  IntCmp $0 1 is1
  Goto done
  is1:
    MessageBox MB_OK|MB_ICONEXCLAMATION "Ocorreu um erro no aplicativo de configura��o do WinPolicy. Por favor, tente novamente."
    Abort
  done:
FunctionEnd

Section Uninstall
  SetShellVarContext all
  !insertmacro MUI_STARTMENU_GETFOLDER "Application" $ICONS_GROUP
  Delete "$SYSDIR\WPHooks.dll"
  Delete "$SYSDIR\WinPolicy.scr"
  Delete "$SYSDIR\WPLocker.exe"

  Delete "$DESKTOP\Justsoft WinPolicy.lnk"
  
  RMDir /r "$SMPROGRAMS\$ICONS_GROUP"
  RMDir /r "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
  
  ExecShell "open" "http://www.justsoft.com.br/winpolicy/feedback/"
SectionEnd