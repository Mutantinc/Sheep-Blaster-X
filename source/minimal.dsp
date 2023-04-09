# Microsoft Developer Studio Project File - Name="minimal" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=minimal - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "minimal.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "minimal.mak" CFG="minimal - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "minimal - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "minimal - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "minimal - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "minimal_Release"
# PROP BASE Intermediate_Dir "minimal_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "output"
# PROP Intermediate_Dir "output"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G6 /MT /W3 /Ox /Og /Oi /Os /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x417 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib glaux.lib winmm.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib /out:"minimal.exe" /pdbtype:sept /opt:nowin98 /opt:ref
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "minimal - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "output"
# PROP Intermediate_Dir "output"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MTd /W3 /Gm /Zi /Oi /Op /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x417 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib glaux.lib winmm.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /out:"minimal.exe" /pdbtype:sept /opt:nowin98 /opt:ref
# SUBTRACT LINK32 /profile /pdb:none /nodefaultlib

!ENDIF 

# Begin Target

# Name "minimal - Win32 Release"
# Name "minimal - Win32 Debug"
# Begin Group "General"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\mg_add.h
# End Source File
# Begin Source File

SOURCE=.\mg_crt.h
# End Source File
# Begin Source File

SOURCE=.\mg_defines.h
# End Source File
# Begin Source File

SOURCE=.\mg_global.h
# End Source File
# Begin Source File

SOURCE=.\mg_keyb.h
# End Source File
# Begin Source File

SOURCE=.\mg_mem.h
# End Source File
# Begin Source File

SOURCE=.\mg_types.h
# End Source File
# Begin Source File

SOURCE=.\rtlib2.h
# End Source File
# End Group
# Begin Group "OpenGL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\mg_ogl.h
# End Source File
# Begin Source File

SOURCE=.\mg_ogl2D.h
# End Source File
# Begin Source File

SOURCE=.\mg_ogltk.h
# End Source File
# End Group
# Begin Group "3D"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\mg_material.h
# End Source File
# Begin Source File

SOURCE=.\mg_texture.h
# End Source File
# Begin Source File

SOURCE=.\mg_vector.h
# End Source File
# End Group
# Begin Group "Music & Sfx"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\mg_fmod.h
# End Source File
# End Group
# Begin Group "TimeLine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\data.h
# End Source File
# Begin Source File

SOURCE=.\mg_effects.h
# End Source File
# Begin Source File

SOURCE=.\mg_time.h
# End Source File
# Begin Source File

SOURCE=.\mg_timeline.h
# End Source File
# End Group
# Begin Group "Dialogs"

# PROP Default_Filter ""
# End Group
# Begin Group "Resources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\res.rc
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "External"

# PROP Default_Filter ""
# End Group
# Begin Group "Assembler Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\data.asm

!IF  "$(CFG)" == "minimal - Win32 Release"

# Begin Custom Build
OutDir=.\output
InputPath=.\data.asm
InputName=data

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm -f win32 -o $(OutDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "minimal - Win32 Debug"

# Begin Custom Build
OutDir=.\output
InputPath=.\data.asm
InputName=data

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm -f win32 -o $(OutDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rtlib2.asm

!IF  "$(CFG)" == "minimal - Win32 Release"

# Begin Custom Build
OutDir=.\output
InputPath=.\rtlib2.asm
InputName=rtlib2

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm -f win32 -o $(OutDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "minimal - Win32 Debug"

# Begin Custom Build
OutDir=.\output
InputPath=.\rtlib2.asm
InputName=rtlib2

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasm -f win32 -o $(OutDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Notes"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\minimal.cpp
# End Source File
# Begin Source File

SOURCE=.\MP.ico
# End Source File
# End Target
# End Project
