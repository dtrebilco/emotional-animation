# Microsoft Developer Studio Project File - Name="ExMovement1" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ExMovement1 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Ve.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Ve.mak" CFG="ExMovement1 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ExMovement1 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ExMovement1 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /Gr /MD /W3 /GR /GX /O2 /I "..\..\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /Gr /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\..\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "ExMovement1 - Win32 Release"
# Name "ExMovement1 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ECanvas.cpp
# End Source File
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

SOURCE=.\EmotionModifier.cpp
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
# Begin Source File

SOURCE=.\ve.cpp
# End Source File
# Begin Source File

SOURCE=.\VEInferenceEngine.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\inc\BoundingVolume.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\Display.h
# End Source File
# Begin Source File

SOURCE=.\ECanvas.h
# End Source File
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

SOURCE=.\EmotionModifier.h
# End Source File
# Begin Source File

SOURCE=.\EmotionSkeletonModifier.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\FastDynamicArray.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\Quaternion.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\Rand.h
# End Source File
# Begin Source File

SOURCE=.\SkeletonFile.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ve.h
# End Source File
# Begin Source File

SOURCE=.\VEInferenceEngine.h
# End Source File
# End Group
# Begin Group "Library Files"

# PROP Default_Filter ""
# Begin Group "Debug"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lib\RenderSystem.Dev.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\AIFoundation.Dev.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\InputExtension.Dev.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\InputSystem.Dev.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\InterfaceResources.Dev.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\InterfaceSystem.Dev.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\Kernel.Dev.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\KernelResource.Dev.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\KernelResources.Dev.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\KernelStandard.Dev.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\Lexer.Dev.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\MotionExtensions.Dev.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\MotionSystem.Dev.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\NetworkSystem.Dev.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\Render2DCore.Dev.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\Render3DCore.Dev.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\Render3DExtension.Dev.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\Render3DFoundation.Dev.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\AIExtension.Dev.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Release"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lib\AIExtension.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\AIFoundation.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\InputExtension.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\InputSystem.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\InterfaceResources.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\InterfaceSystem.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\Kernel.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\KernelResource.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\KernelResources.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\KernelStandard.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\Lexer.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\MotionExtensions.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\MotionSystem.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\NetworkSystem.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\Render2DCore.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\Render3DCore.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\Render3DExtension.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\Render3DFoundation.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\lib\RenderSystem.lib

!IF  "$(CFG)" == "ExMovement1 - Win32 Release"

!ELSEIF  "$(CFG)" == "ExMovement1 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# End Group
# End Target
# End Project
