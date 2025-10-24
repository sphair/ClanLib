# Microsoft Developer Studio Project File - Name="Configure" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Configure - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Configure.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Configure.mak" CFG="Configure - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Configure - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Configure - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Configure - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\Configure"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_UNICODE" /Yu"precomp.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x406 /d "NDEBUG"
# ADD RSC /l 0x406 /d "NDEBUG" /d "UNICODE" /d "_UNICODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib Rpcrt4.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Configure - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug\Configure"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_UNICODE" /Yu"precomp.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x406 /d "_DEBUG"
# ADD RSC /l 0x406 /d "_DEBUG" /d "UNICODE" /d "_UNICODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib Rpcrt4.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Configure - Win32 Release"
# Name "Configure - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Setup\Win32\main.cpp
# End Source File
# Begin Source File

SOURCE=.\Setup\Win32\page_finished.cpp
# End Source File
# Begin Source File

SOURCE=.\Setup\Win32\page_system.cpp
# End Source File
# Begin Source File

SOURCE=.\Setup\Win32\page_system2.cpp
# End Source File
# Begin Source File

SOURCE=.\Setup\Win32\page_target.cpp
# End Source File
# Begin Source File

SOURCE=.\Setup\Win32\page_welcome.cpp
# End Source File
# Begin Source File

SOURCE=.\Setup\Win32\precomp.cpp
# ADD CPP /Yc"precomp.h"
# End Source File
# Begin Source File

SOURCE=.\Setup\Win32\wizard.cpp
# End Source File
# Begin Source File

SOURCE=.\Setup\Win32\workspace_generator_msvc.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Setup\Win32\page_finished.h
# End Source File
# Begin Source File

SOURCE=.\Setup\Win32\page_system.h
# End Source File
# Begin Source File

SOURCE=.\Setup\Win32\page_system2.h
# End Source File
# Begin Source File

SOURCE=.\Setup\Win32\page_target.h
# End Source File
# Begin Source File

SOURCE=.\Setup\Win32\page_welcome.h
# End Source File
# Begin Source File

SOURCE=.\Setup\Win32\precomp.h
# End Source File
# Begin Source File

SOURCE=.\Setup\Win32\resource.h
# End Source File
# Begin Source File

SOURCE=.\Setup\Win32\wizard.h
# End Source File
# Begin Source File

SOURCE=.\Setup\Win32\workspace_generator_msvc.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Setup\Win32\res\clanlib_banner.bmp
# End Source File
# Begin Source File

SOURCE=.\Setup\Win32\res\clanlib_logo_ascii.bmp
# End Source File
# Begin Source File

SOURCE=.\Setup\Win32\res\configure.ico
# End Source File
# Begin Source File

SOURCE=.\Setup\Win32\configure.rc
# End Source File
# Begin Source File

SOURCE=.\Setup\Win32\res\configure.rc2
# End Source File
# End Group
# Begin Group "Generic Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Setup\Generic\project.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\Setup\Generic\project.h
# End Source File
# Begin Source File

SOURCE=.\Setup\Generic\workspace.h
# End Source File
# End Group
# End Target
# End Project
