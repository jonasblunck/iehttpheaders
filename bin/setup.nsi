;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General

  ;Name and file
  Name "ieHTTPHeaders"
  OutFile "ieHTTPHeadersSetup.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\ieHTTPHeaders"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\ieHTTPHeaders" ""

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "License.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Required" Required

  SetOutPath "$INSTDIR"
  
  File "ieHTTPTrace.dll"
  File "ieHTTPHeaders.html"
  File "iehttpheaders_enable.jpg"
  File "iehttpheaders_enable_ie7.jpg"
  File "iehttpheaders_enable_ie9.png"
  File "styles.css"
  
  RegDLL "$INSTDIR\ieHTTPTrace.dll"
 
  ;Store installation folder
  WriteRegStr HKCU "Software\ieHTTPHeaders" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

; optional section
Section "Start Menu items" StartMenuItems
  CreateDirectory "$SMPROGRAMS\ieHTTPHeaders"
  CreateShortCut "$SMPROGRAMS\ieHTTPHeaders\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\ieHTTPHeaders\ieHTTPHeaders Documentation.lnk" "$INSTDIR\ieHTTPHeaders.html" "" "$INSTDIR\ieHTTPHeaders.html" 0
SectionEnd


;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...
  UnRegDLL "$INSTDIR\ieHTTPTrace.dll"

  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\ieHTTPHeaders"

SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_Required ${LANG_ENGLISH} "Required binaries will be copied and registered"
  LangString DESC_StartMenuItems ${LANG_ENGLISH} "Start menu items for documentation and uninstall"

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${Required} $(DESC_Required)
    !insertmacro MUI_DESCRIPTION_TEXT ${StartMenuItems} $(DESC_StartMenuItems)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

