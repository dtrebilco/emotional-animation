# Microsoft Developer Studio Project File - Name="ModelPreview" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ModelPreview - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ModelPreview.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ModelPreview.mak" CFG="ModelPreview - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ModelPreview - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ModelPreview - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ModelPreview - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
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

!ELSEIF  "$(CFG)" == "ModelPreview - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /Gr /MD /W4 /Gm /GR /GX /ZI /Od /I "c:\c_prog\jetbeta2\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"ModelPreview.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ModelPreview - Win32 Release"
# Name "ModelPreview - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ControlDialog.cpp
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

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelObject.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelPreviewTask.cpp
# End Source File
# Begin Source File

SOURCE=.\RenderData.cpp
# End Source File
# Begin Source File

SOURCE=.\ResourceDatabaseManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SkeletonDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\SkeletonFile.cpp
# End Source File
# Begin Source File

SOURCE=.\SkeletonVertex.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\inc\Camera.h
# End Source File
# Begin Source File

SOURCE=.\ControlDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\Ui\Dialog.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\Ui\DialogFiles.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\DynamicArray.h
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

SOURCE=..\..\inc\File.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\Ui\GControl.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\IndexedMeshInstance.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\Matrix3.h
# End Source File
# Begin Source File

SOURCE=.\ModelObject.h
# End Source File
# Begin Source File

SOURCE=.\ModelPreviewTask.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\Particle.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\Quaternion.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\Rand.h
# End Source File
# Begin Source File

SOURCE=.\RenderData.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\RenderKernel.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\ResourceDatabase.h
# End Source File
# Begin Source File

SOURCE=.\ResourceDatabaseManager.h
# End Source File
# Begin Source File

SOURCE=.\SkeletonDisplay.h
# End Source File
# Begin Source File

SOURCE=.\SkeletonFile.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\SkeletonModifier.h
# End Source File
# Begin Source File

SOURCE=.\SkeletonVertex.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\Ui\Value.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\Vector2.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\Vector3.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\Vector4.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "LinkFiles"

# PROP Default_Filter "*.lib"
# Begin Source File

SOURCE=..\..\lib\SoundCore.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\AIExtension.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\AIFoundation.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\AIFoundation.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\InputExtension.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\InputExtension.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\InputSystem.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\InputSystem.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\InterfaceResources.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\InterfaceResources.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\InterfaceSystem.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\InterfaceSystem.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\JiveLib.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\JiveLib.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\Kernel.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\Kernel.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\KernelResource.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\KernelResource.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\KernelResources.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\KernelResources.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\KernelStandard.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\KernelStandard.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\Lexer.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\Lexer.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\MotionExtensions.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\MotionExtensions.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\MotionSystem.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\MotionSystem.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\NetworkSystem.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\NetworkSystem.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\Render2DCore.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\Render2DCore.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\Render3DCore.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\Render3DCore.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\Render3DExtension.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\Render3DExtension.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\Render3DFoundation.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\Render3DFoundation.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\RenderSystem.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\RenderSystem.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\SoundCore.Dev.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\AIExtension.Dev.lib
# End Source File
# End Group
# End Target
# End Project
