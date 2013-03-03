# Microsoft Developer Studio Project File - Name="FSRad" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FSRad - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FSRad.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FSRad.mak" CFG="FSRad - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FSRad - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FSRad - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FSRad - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"../../Run/FSRad.exe"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "FSRad - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"../../Run/FSRadd.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "FSRad - Win32 Release"
# Name "FSRad - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BeamTree.cpp
# End Source File
# Begin Source File

SOURCE=.\BSP.cpp
# End Source File
# Begin Source File

SOURCE=.\CmdHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\DatabasePage.cpp
# End Source File
# Begin Source File

SOURCE=.\Ent.cpp
# End Source File
# Begin Source File

SOURCE=.\FSRad.cpp
# End Source File
# Begin Source File

SOURCE=.\FSRad.rc
# End Source File
# Begin Source File

SOURCE=.\GeneralPage.cpp
# End Source File
# Begin Source File

SOURCE=.\GeomDB.cpp
# End Source File
# Begin Source File

SOURCE=.\LightmapPage.cpp
# End Source File
# Begin Source File

SOURCE=.\LMapGen.cpp
# End Source File
# Begin Source File

SOURCE=.\PolarQuadtree.cpp
# End Source File
# Begin Source File

SOURCE=.\PostPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RadGen.cpp
# End Source File
# Begin Source File

SOURCE=.\RadLMap.cpp
# End Source File
# Begin Source File

SOURCE=.\RadPrim.cpp
# End Source File
# Begin Source File

SOURCE=.\RadSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\SOctree.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\webbrowser.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\Include\fstl\array
# End Source File
# Begin Source File

SOURCE=.\BeamTree.h
# End Source File
# Begin Source File

SOURCE=.\BSP.h
# End Source File
# Begin Source File

SOURCE=.\CmdHelp.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\fstl\common
# End Source File
# Begin Source File

SOURCE=.\DatabasePage.h
# End Source File
# Begin Source File

SOURCE=.\Ent.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\fstl\error
# End Source File
# Begin Source File

SOURCE=.\FSRad.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\fstl\fstl
# End Source File
# Begin Source File

SOURCE=.\GeneralPage.h
# End Source File
# Begin Source File

SOURCE=.\GeomDB.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\fstl\hash
# End Source File
# Begin Source File

SOURCE=.\LightmapPage.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\fstl\list
# End Source File
# Begin Source File

SOURCE=.\LMapGen.h
# End Source File
# Begin Source File

SOURCE=.\PolarQuadtree.h
# End Source File
# Begin Source File

SOURCE=.\PostPage.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\RadGen.h
# End Source File
# Begin Source File

SOURCE=.\RadLMap.h
# End Source File
# Begin Source File

SOURCE=.\RadPatch.h
# End Source File
# Begin Source File

SOURCE=.\RadPrim.h
# End Source File
# Begin Source File

SOURCE=.\RadSheet.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\fstl\reservoir
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SOctree.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\fstl\string
# End Source File
# Begin Source File

SOURCE=..\..\Include\fstl\util
# End Source File
# Begin Source File

SOURCE=.\webbrowser.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\FSRad.ico
# End Source File
# Begin Source File

SOURCE=.\res\FSRad.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\Todo.txt
# End Source File
# End Target
# End Project
# Section FSRad : {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}
# 	2:5:Class:WebBrowser
# 	2:10:HeaderFile:webbrowser.h
# 	2:8:ImplFile:webbrowser.cpp
# End Section
# Section FSRad : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowser.h
# 	2:16:DefaultSinkClass:WebBrowser
# End Section
