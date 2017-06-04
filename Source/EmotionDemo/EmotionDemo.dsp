# Microsoft Developer Studio Project File - Name="ExMesh4" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ExMesh4 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EmotionDemo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EmotionDemo.mak" CFG="ExMesh4 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ExMesh4 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ExMesh4 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ExMesh4 - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Gr /W3 /GR /GX /Z7 /O2 /I "..\..\..\..\Inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386

!ELSEIF  "$(CFG)" == "ExMesh4 - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Gr /W3 /Gm /GR /GX /ZI /Od /I "..\..\Inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ExMesh4 - Win32 Release"
# Name "ExMesh4 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\EModifier.cpp
# End Source File
# Begin Source File

SOURCE=.\EModifier_AF.cpp
# End Source File
# Begin Source File

SOURCE=.\EModifier_Grav.cpp
# End Source File
# Begin Source File

SOURCE=.\EModifier_Pose.cpp
# End Source File
# Begin Source File

SOURCE=.\EModifier_Rand.cpp
# End Source File
# Begin Source File

SOURCE=.\EmotionDemo.cpp
# End Source File
# Begin Source File

SOURCE=.\EmotionModifier.cpp
# End Source File
# Begin Source File

SOURCE=.\EmotionObject.cpp
# End Source File
# Begin Source File

SOURCE=.\EmotionSkeletonModifier.cpp
# End Source File
# Begin Source File

SOURCE=.\SkeletonFile.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\EModifier.h
# End Source File
# Begin Source File

SOURCE=.\EModifier_AF.h
# End Source File
# Begin Source File

SOURCE=.\EModifier_Grav.h
# End Source File
# Begin Source File

SOURCE=.\EModifier_Pose.h
# End Source File
# Begin Source File

SOURCE=.\EModifier_Rand.h
# End Source File
# Begin Source File

SOURCE=.\EmotionDemo.h
# End Source File
# Begin Source File

SOURCE=.\EmotionModifier.h
# End Source File
# Begin Source File

SOURCE=.\EmotionObject.h
# End Source File
# Begin Source File

SOURCE=.\EmotionSkeletonModifier.h
# End Source File
# Begin Source File

SOURCE=.\SkeletonFile.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Library Files"

# PROP Default_Filter ""
# Begin Group "Release"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\Lib\RenderSystem.lib

!IF  "$(CFG)" == "ExMesh4 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMesh4 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\Lib\Render3DFoundation.lib

!IF  "$(CFG)" == "ExMesh4 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMesh4 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\Lib\Render3DCore.lib

!IF  "$(CFG)" == "ExMesh4 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMesh4 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\Lib\KernelStandard.lib

!IF  "$(CFG)" == "ExMesh4 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMesh4 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\Lib\Kernel.lib

!IF  "$(CFG)" == "ExMesh4 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMesh4 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\Lib\KernelResources.lib

!IF  "$(CFG)" == "ExMesh4 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMesh4 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\Lib\KernelResource.lib

!IF  "$(CFG)" == "ExMesh4 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMesh4 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\Lib\Render2DCore.lib

!IF  "$(CFG)" == "ExMesh4 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMesh4 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\Lib\Render3DExtension.lib

!IF  "$(CFG)" == "ExMesh4 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMesh4 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\Lib\InputSystem.lib

!IF  "$(CFG)" == "ExMesh4 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMesh4 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\Lib\InputExtension.lib

!IF  "$(CFG)" == "ExMesh4 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMesh4 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\Lib\InterfaceSystem.lib

!IF  "$(CFG)" == "ExMesh4 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMesh4 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\Lib\InterfaceResources.lib

!IF  "$(CFG)" == "ExMesh4 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMesh4 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Debug"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lib\RenderSystem.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\AIFoundation.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\InputExtension.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\InputSystem.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\InterfaceResources.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\InterfaceSystem.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\Kernel.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\KernelResource.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\KernelResources.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\KernelStandard.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\Lexer.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\MotionExtensions.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\MotionSystem.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\NetworkSystem.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\Render2DCore.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\Render3DCore.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\Render3DExtension.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\Render3DFoundation.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\AIExtension.Dev.lib
# End Source File
# End Group
# End Group
# End Target
# End Project
