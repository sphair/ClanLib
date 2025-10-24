# Microsoft Developer Studio Project File - Name="ExampleGUIStyle" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ExampleGUIStyle - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ExampleGUIStyle.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ExampleGUIStyle.mak" CFG="ExampleGUIStyle - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ExampleGUIStyle - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ExampleGUIStyle - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ExampleGUIStyle - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x406 /d "NDEBUG"
# ADD RSC /l 0x406 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /profile /map /debug

!ELSEIF  "$(CFG)" == "ExampleGUIStyle - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GR /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GR /Gm /GX /Zi /Od /D "_DEBUG" /D "_MBCS" /D "__STL_DEBUG" /D "WIN32" /D "_WINDOWS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x406 /d "_DEBUG"
# ADD RSC /l 0x406 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ExampleGUIStyle - Win32 Release"
# Name "ExampleGUIStyle - Win32 Debug"
# Begin Group "ZZ Theme"

# Begin Source File

SOURCE=.\ZZ_Theme\button_zz.cpp
# End Source File
# Begin Source File

SOURCE=.\ZZ_Theme\button_zz.h
# End Source File
# Begin Source File

SOURCE=.\ZZ_Theme\checkbox_zz.cpp
# End Source File
# Begin Source File

SOURCE=.\ZZ_Theme\checkbox_zz.h
# End Source File
# Begin Source File

SOURCE=.\ZZ_Theme\frame_zz.cpp
# End Source File
# Begin Source File

SOURCE=.\ZZ_Theme\frame_zz.h
# End Source File
# Begin Source File

SOURCE=.\ZZ_Theme\inputbox_zz.cpp
# End Source File
# Begin Source File

SOURCE=.\ZZ_Theme\inputbox_zz.h
# End Source File
# Begin Source File

SOURCE=.\ZZ_Theme\label_zz.cpp
# End Source File
# Begin Source File

SOURCE=.\ZZ_Theme\label_zz.h
# End Source File
# Begin Source File

SOURCE=.\ZZ_Theme\listbox_zz.cpp
# End Source File
# Begin Source File

SOURCE=.\ZZ_Theme\listbox_zz.h
# End Source File
# Begin Source File

SOURCE=.\ZZ_Theme\scrollbar_zz.cpp
# End Source File
# Begin Source File

SOURCE=.\ZZ_Theme\scrollbar_zz.h
# End Source File
# Begin Source File

SOURCE=.\ZZ_Theme\stylemanager_zz.cpp
# End Source File
# Begin Source File

SOURCE=.\ZZ_Theme\stylemanager_zz.h
# End Source File
# Begin Source File

SOURCE=.\ZZ_Theme\window_zz.cpp
# End Source File
# Begin Source File

SOURCE=.\ZZ_Theme\window_zz.h
# End Source File
# End Group
# Begin Group "MenuSystem"

# Begin Source File

SOURCE=.\MenuSystem\NS_Menu_Credits.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuSystem\NS_Menu_Credits.h
# End Source File
# Begin Source File

SOURCE=.\MenuSystem\NS_Menu_Game.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuSystem\NS_Menu_Game.h
# End Source File
# Begin Source File

SOURCE=.\MenuSystem\NS_Menu_Main.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuSystem\NS_Menu_Main.h
# End Source File
# Begin Source File

SOURCE=.\MenuSystem\NS_Menu_Play.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuSystem\NS_Menu_Play.h
# End Source File
# Begin Source File

SOURCE=.\MenuSystem\NS_MenuGeneric.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuSystem\NS_MenuGeneric.h
# End Source File
# Begin Source File

SOURCE=.\MenuSystem\NS_MenuManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuSystem\NS_MenuManager.h
# End Source File
# Begin Source File

SOURCE=.\MenuSystem\NS_MessageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuSystem\NS_MessageBox.h
# End Source File
# Begin Source File

SOURCE=.\MenuSystem\NS_Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuSystem\NS_Profile.h
# End Source File

# End Group
# Begin Group "GUIDefinitions"

# Begin Source File

SOURCE=.\Graphics\menu_credits.xml
# End Source File
# Begin Source File

SOURCE=.\Graphics\menu_game.xml
# End Source File
# Begin Source File

SOURCE=.\Graphics\menu_main.xml
# End Source File
# Begin Source File

SOURCE=.\Graphics\menu_play.xml
# End Source File

# End Group
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
