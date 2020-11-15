# Microsoft Developer Studio Project File - Name="ide2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ide2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ide2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ide2.mak" CFG="ide2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ide2 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ide2 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ide2 - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\executable\Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\core" /I "..\..\include\ide" /I "..\..\include\lua" /I "..\..\include\scintilla" /I "..\..\include\cnc" /I "..\..\..\adamocnc\include\adamo_canel" /I "..\GridCtrl" /I "..\..\include\lexxer" /I "..\..\..\adamocnc\include\lib" /I "..\..\include\cfg" /I "..\ide2" /I "..\..\..\adamolink\include" /I "..\..\..\AdamoCNC\bblock\CCDK3\INCLUDE" /I "..\..\..\AdamoCNC\bblock\CCDKM\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "ADAMO_IDE" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# ADD BASE RSC /l 0x415 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x410 /i "C:\PROGRAM FILES\CODEJOCK SOFTWARE\MFC\TOOLKITPRO\INCLUDE" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 core.lib standard.lib shlwapi.lib gridctrl.lib mpr.lib lexxer.lib ThiraImageLib.lib AdamoLib.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\executable\Release/Thira.exe" /libpath:"..\..\library\release" /libpath:"..\..\..\AdamoCNC\bin\release"

!ELSEIF  "$(CFG)" == "ide2 - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\executable\Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\core" /I "..\..\include\ide" /I "..\..\include\lua" /I "..\..\include\scintilla" /I "..\..\include\cnc" /I "..\..\..\adamocnc\include\adamo_canel" /I "..\GridCtrl" /I "..\..\include\lexxer" /I "..\..\..\adamocnc\include\lib" /I "..\..\include\cfg" /I "..\ide2" /I "..\..\..\adamolink\include" /I "..\..\..\AdamoCNC\bblock\CCDK3\INCLUDE" /I "..\..\..\AdamoCNC\bblock\CCDKM\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "ADAMO_IDE" /Fr /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /win32
# ADD BASE RSC /l 0x415 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x415 /i "D:\Codejock Software\MFC\Xtreme ToolkitPro v11.2.2\Source" /d "_DEBUG" /d "_AFXDLL"
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 core.lib standard.lib shlwapi.lib gridctrld.lib lexxer.lib adamolib.lib ThiraImageLib.lib mpr.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\executable\Debug/Thira.exe" /pdbtype:sept /libpath:"..\..\library\debug" /libpath:"..\..\..\AdamoCNC\bin\debug"
# SUBTRACT LINK32 /incremental:no

!ENDIF 

# Begin Target

# Name "ide2 - Win32 Release"
# Name "ide2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AdamoAboutBox.cpp
# End Source File
# Begin Source File

SOURCE=.\DataBase\AdamoAnalogIn.cpp
# End Source File
# Begin Source File

SOURCE=.\DataBase\AdamoAnalogOut.cpp
# End Source File
# Begin Source File

SOURCE=.\DataBase\AdamoAxis.cpp
# End Source File
# Begin Source File

SOURCE=.\DataBase\AdamoBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DataBase\AdamoBaseList.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoCC.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoCCFile.cpp
# End Source File
# Begin Source File

SOURCE=..\Cnc\AdamoCfgMachine.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoConfHWView.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoConfigHWView.cpp
# End Source File
# Begin Source File

SOURCE=.\DataBase\AdamoCounter.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoCPDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoDbgIOView.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoDbgVarView.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoDGCards.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoDGComm.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoDGDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoDGDocument.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoDGEthercat.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoDGFilters.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoDGFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoDGHardware.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoDGSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoDGSoftware.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoDGSSCNET.cpp
# End Source File
# Begin Source File

SOURCE=.\DataBase\AdamoDgtIn.cpp
# End Source File
# Begin Source File

SOURCE=.\DataBase\AdamoDgtOut.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoDGView.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoEthercatAssi.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoEthercatGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoEthercatIO.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoEthercatIOAnalogico.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoEthercatNode.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoFileDati.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoFindInFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoFindInFilesView.cpp
# End Source File
# Begin Source File

SOURCE=.\DataBase\AdamoGeneric.cpp
# End Source File
# Begin Source File

SOURCE=.\DataBase\AdamoGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\Cnc\AdamoMachine.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoMDAxButton.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoMDAxesView.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoMDAxSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoMDAxStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoMDCommandBar.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoMDDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoMDFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoMDIOView.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoMDIOViewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoMDSplittedView.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoMDSplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoMDView.cpp
# End Source File
# Begin Source File

SOURCE=.\DataBase\AdamoModule.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoPARDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoPARFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoPARTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoPARView.cpp
# End Source File
# Begin Source File

SOURCE=.\DataBase\AdamoPlant.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoRSCatalogue.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoRSDiagramButton.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoRSDiagramCheckBox.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoRSDiagramCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoRSDiagramEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoRSDiagramGroupBox.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoRSDiagramImage.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoRSDiagramLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoRSDiagramList.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoRSDiagramObjects.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoRSDiagramRadioButton.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoRSDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoRSEditIndexSupergrid.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoRSEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoRSEditTableIndex.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoRSFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoRSObjects.cpp

!IF  "$(CFG)" == "ide2 - Win32 Release"

!ELSEIF  "$(CFG)" == "ide2 - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AdamoRSOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoRSProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoRSValues.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoRSView.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoSEDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoSEFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoSEListView.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoSettingsFile.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoSEView.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoShowGlobalsSupergrid.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoSink.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoSplash.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoSSCNETGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoTables.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoTablesSupergrid.cpp
# End Source File
# Begin Source File

SOURCE=.\DataBase\AdamoTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoTraduzione.cpp
# End Source File
# Begin Source File

SOURCE=.\AdamoVersion.cpp
# End Source File
# Begin Source File

SOURCE=..\Cnc\AdamoVMachine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\AdamoLink\MSGSERV\AdaPsw.cpp

!IF  "$(CFG)" == "ide2 - Win32 Release"

!ELSEIF  "$(CFG)" == "ide2 - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ado2.cpp
# End Source File
# Begin Source File

SOURCE=.\AllAxesView.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapSlider.cpp
# End Source File
# Begin Source File

SOURCE=.\Breakpoints.cpp
# End Source File
# Begin Source File

SOURCE=.\BuildResource.cpp
# End Source File
# Begin Source File

SOURCE=.\CallStack.cpp
# End Source File
# Begin Source File

SOURCE=.\CanOpenAnalogIO.cpp
# End Source File
# Begin Source File

SOURCE=.\CanOpenData.cpp
# End Source File
# Begin Source File

SOURCE=.\CanOpenDigitalIO.cpp
# End Source File
# Begin Source File

SOURCE=.\CBrowserContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\CharCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorStaticST.cpp
# End Source File
# Begin Source File

SOURCE=.\ColumnTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ColumnTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfFind.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigHWGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateSlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Curvefit.cpp
# End Source File
# Begin Source File

SOURCE=.\CVariableContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\Debugger.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceVariable.cpp
# End Source File
# Begin Source File

SOURCE=.\DGEcMailBoxView.cpp
# End Source File
# Begin Source File

SOURCE=.\DGSdoView.cpp
# End Source File
# Begin Source File

SOURCE=.\DiagramClipboardHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\DiagramEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\DiagramEntity.cpp
# End Source File
# Begin Source File

SOURCE=.\DiagramEntityContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\DiagramLine.cpp
# End Source File
# Begin Source File

SOURCE=.\DiagramMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\DiagramPropertyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogSlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Digistatic.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEcConfigSingleSlave.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEcConfigSlaves.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEcMailBoxRead.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEcMailBoxWrite.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFindErrors.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGetPath.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHWCanOpenNet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHWCanOpenNodes.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHWEthercatNet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHWSSCNETNet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHWSSCNETStation.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHWYoupsilon.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIDRange.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgImportImage.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOscilloscope.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgParTree.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSamples.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSdoRead.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSdoWrite.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSlotView.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStrings.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTriggers.cpp
# End Source File
# Begin Source File

SOURCE=.\DockConfigHW.cpp
# End Source File
# Begin Source File

SOURCE=.\DockConfigTree.cpp
# End Source File
# Begin Source File

SOURCE=.\DockDbgIOView.cpp
# End Source File
# Begin Source File

SOURCE=.\DockDbgVarView.cpp
# End Source File
# Begin Source File

SOURCE=.\DockDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\DockDevListView.cpp
# End Source File
# Begin Source File

SOURCE=.\DockDevProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DockDGSdo.cpp
# End Source File
# Begin Source File

SOURCE=.\DockEcMailBox.cpp
# End Source File
# Begin Source File

SOURCE=.\DockLangView.cpp
# End Source File
# Begin Source File

SOURCE=.\DockOscilloscope.cpp
# End Source File
# Begin Source File

SOURCE=.\DockPARTree.cpp
# End Source File
# Begin Source File

SOURCE=.\DockRS.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorsDlg.h
# End Source File
# Begin Source File

SOURCE=.\Executor.cpp
# End Source File
# Begin Source File

SOURCE=.\FileFinder.cpp
# End Source File
# Begin Source File

SOURCE=.\FindDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalArr.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalVariable.cpp
# End Source File
# Begin Source File

SOURCE=.\GridView.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\GruppDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\HTMLCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\HTMLDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\HTMLFilter.cpp

!IF  "$(CFG)" == "ide2 - Win32 Release"

!ELSEIF  "$(CFG)" == "ide2 - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\HTMLFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\HTMLView.cpp
# End Source File
# Begin Source File

SOURCE=.\iangulargaugex.cpp
# End Source File
# Begin Source File

SOURCE=.\ide2.cpp
# End Source File
# Begin Source File

SOURCE=.\Ide2.idl
# ADD MTL /h "Ide2_i.h" /iid "Ide2_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\ide2.rc
# End Source File
# Begin Source File

SOURCE=.\iknobx.cpp
# End Source File
# Begin Source File

SOURCE=.\iledbarx.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageResource.cpp
# End Source File
# Begin Source File

SOURCE=.\IOSuperGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\islidingscalex.cpp
# End Source File
# Begin Source File

SOURCE=.\iswitchleverx.cpp
# End Source File
# Begin Source File

SOURCE=.\iswitchpanelx.cpp
# End Source File
# Begin Source File

SOURCE=.\KSFileDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Label.cpp
# End Source File
# Begin Source File

SOURCE=.\LangView.cpp
# End Source File
# Begin Source File

SOURCE=.\LedButton.cpp
# End Source File
# Begin Source File

SOURCE=.\listeditctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\LuaDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\LuaEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\LuaFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\LuaHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\LuaView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\NewDev.cpp
# End Source File
# Begin Source File

SOURCE=.\NewPlant.cpp
# End Source File
# Begin Source File

SOURCE=.\OleDropSourceEx.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenSource.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OscChannelData.cpp
# End Source File
# Begin Source File

SOURCE=.\OscProp.cpp
# End Source File
# Begin Source File

SOURCE=.\OscView.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PAREDIT.CPP
# End Source File
# Begin Source File

SOURCE=.\Path.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\PlantManage.cpp
# End Source File
# Begin Source File

SOURCE=.\preprocessor.cpp
# End Source File
# Begin Source File

SOURCE=.\PreviewResource.cpp
# End Source File
# Begin Source File

SOURCE=.\Project.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectNew.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyGridItemEx.cpp
# End Source File
# Begin Source File

SOURCE=..\scintilla\PropSet.cxx
# End Source File
# Begin Source File

SOURCE=.\ReplaceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ResourceFile.cpp

!IF  "$(CFG)" == "ide2 - Win32 Release"

!ELSEIF  "$(CFG)" == "ide2 - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SamplesGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\ScintillaBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ScintillaView.cpp
# End Source File
# Begin Source File

SOURCE=.\SdoReadGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\SdoWriteGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\ShowGlobalVars.cpp
# End Source File
# Begin Source File

SOURCE=.\SlotView.cpp
# End Source File
# Begin Source File

SOURCE=.\SSCNETIO.cpp
# End Source File
# Begin Source File

SOURCE=.\SSCNETParameters.cpp
# End Source File
# Begin Source File

SOURCE=.\SSCNetParGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdGrfx.cpp
# End Source File
# Begin Source File

SOURCE=.\supergridctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TabBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\Tokenizer.cpp
# End Source File
# Begin Source File

SOURCE=.\TransparentStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeViewFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\TriggerGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\TypeSource.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItem.cpp
# End Source File
# Begin Source File

SOURCE=.\UserStrings.cpp
# End Source File
# Begin Source File

SOURCE=.\VariablesBar.cpp
# End Source File
# Begin Source File

SOURCE=.\VarListView.cpp
# End Source File
# Begin Source File

SOURCE=.\WatchBar.cpp
# End Source File
# Begin Source File

SOURCE=.\WatchListView.cpp
# End Source File
# Begin Source File

SOURCE=.\Workspace.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkspaceWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\xSkinButton.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AdamoAboutBox.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\AdamoCC.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\adamoconfhwview.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\adamoconfighwview.h
# End Source File
# Begin Source File

SOURCE=.\AdamoConversioneFormato.h
# End Source File
# Begin Source File

SOURCE=.\AdamoCPDlg.h
# End Source File
# Begin Source File

SOURCE=.\AdamoDbgIOView.h
# End Source File
# Begin Source File

SOURCE=.\AdamoDbgVarCtrl.h
# End Source File
# Begin Source File

SOURCE=.\AdamoDbgVarView.h
# End Source File
# Begin Source File

SOURCE=.\AdamoDGComm.h
# End Source File
# Begin Source File

SOURCE=.\AdamoDGDocument.h
# End Source File
# Begin Source File

SOURCE=.\AdamoDGEthercat.h
# End Source File
# Begin Source File

SOURCE=.\AdamoDGFilters.h
# End Source File
# Begin Source File

SOURCE=.\AdamoDGFrame.h
# End Source File
# Begin Source File

SOURCE=.\AdamoDGHardware.h
# End Source File
# Begin Source File

SOURCE=.\AdamoDGSearch.h
# End Source File
# Begin Source File

SOURCE=.\AdamoDGView.h
# End Source File
# Begin Source File

SOURCE=.\AdamoEthercatAssi.h
# End Source File
# Begin Source File

SOURCE=..\..\include\AdamoEthercatGrid.h
# End Source File
# Begin Source File

SOURCE=.\AdamoEthercatGrid.h
# End Source File
# Begin Source File

SOURCE=.\AdamoEthercatIO.h
# End Source File
# Begin Source File

SOURCE=.\AdamoEthercatIOAnalogico.h
# End Source File
# Begin Source File

SOURCE=.\AdamoEthercatNode.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\AdamoFileDati.h
# End Source File
# Begin Source File

SOURCE=.\AdamoFindInFiles.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\AdamoFindInFilesView.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\AdamoIdeEthercat.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Cnc\AdamoMachine.h
# End Source File
# Begin Source File

SOURCE=.\AdamoMDAxButton.h
# End Source File
# Begin Source File

SOURCE=.\AdamoMDAxesView.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\AdamoMDAxSettings.h
# End Source File
# Begin Source File

SOURCE=.\AdamoMDDoc.h
# End Source File
# Begin Source File

SOURCE=.\AdamoMDIOView.h
# End Source File
# Begin Source File

SOURCE=.\AdamoMDIOViewDlg.h
# End Source File
# Begin Source File

SOURCE=.\AdamoMDSplittedView.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\AdamoMDSplitter.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\adamomdsscnet.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\AdamoMDView.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\adamopardoc.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\adamoparframe.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\adamopartreeview.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\adamoparview.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\AdamoRS.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\AdamoRSDiagramObjects.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\AdamoRSEditIndexSupergrid.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\adamorsedittableindex.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\AdamoRSObjects.h
# End Source File
# Begin Source File

SOURCE=.\AdamoRSOptions.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\adamorstemplate.h
# End Source File
# Begin Source File

SOURCE=.\AdamoRSValues.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\AdamoSink.h
# End Source File
# Begin Source File

SOURCE=.\AdamoSink.h
# End Source File
# Begin Source File

SOURCE=.\AdamoSplash.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\adamosscnetgrid.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\AdamoSw.h
# End Source File
# Begin Source File

SOURCE=.\AdamoSw.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\adamotables.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\AdamoTablesSupergrid.h
# End Source File
# Begin Source File

SOURCE=.\AdamoTemplates.h
# End Source File
# Begin Source File

SOURCE=.\DataBase\AdamoTimer.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\AdamoTraduzione.h
# End Source File
# Begin Source File

SOURCE=..\..\include\AdamoVersion.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\AdvancedStatusBar.h
# End Source File
# Begin Source File

SOURCE=.\AdvancedStatusBar.h
# End Source File
# Begin Source File

SOURCE=.\AllAxesView.h
# End Source File
# Begin Source File

SOURCE=.\BaseDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\BitmapSlider.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\breakpoints.h
# End Source File
# Begin Source File

SOURCE=.\BuildResource.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\CallStack.h
# End Source File
# Begin Source File

SOURCE=.\CallStack.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\canopenanalogio.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\canopendata.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\canopendigitalio.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\CBrowserContainer.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\CharCheck.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\ColorStaticST.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\conffind.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\configdoc.h
# End Source File
# Begin Source File

SOURCE=.\ConfigDoc.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\ConfigFrame.h
# End Source File
# Begin Source File

SOURCE=.\ConfigFrame.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\ConfigHWGrid.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\ConfTreeView.h
# End Source File
# Begin Source File

SOURCE=.\ConfTreeView.h
# End Source File
# Begin Source File

SOURCE=.\CreateSlot.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\CVariableContainer.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\Data.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\Debugger.h
# End Source File
# Begin Source File

SOURCE=.\Debugger.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\DebuggerMessages.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\DeviceDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\DeviceGridView.h
# End Source File
# Begin Source File

SOURCE=.\DeviceListView.h
# End Source File
# Begin Source File

SOURCE=.\DeviceVariable.h
# End Source File
# Begin Source File

SOURCE=.\DGEcMailBoxView.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\DGSdoView.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\DialogSlot.h
# End Source File
# Begin Source File

SOURCE=.\DialogSlot.h
# End Source File
# Begin Source File

SOURCE=.\DlgEcConfigSingleSlave.h
# End Source File
# Begin Source File

SOURCE=.\DlgEcConfigSlaves.h
# End Source File
# Begin Source File

SOURCE=.\DlgEcMailBoxRead.h
# End Source File
# Begin Source File

SOURCE=.\DlgEcMailBoxWrite.h
# End Source File
# Begin Source File

SOURCE=.\DlgFindErrors.h
# End Source File
# Begin Source File

SOURCE=.\DlgFPGARegisters.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\dlghwcanopennet.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\dlghwcanopennodes.h
# End Source File
# Begin Source File

SOURCE=.\DlgHWEthercatNet.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\dlghwsscnetnet.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\dlghwsscnetstation.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\DlgHWYoupsilon.h
# End Source File
# Begin Source File

SOURCE=.\DlgIDRange.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\dlgimportimage.h
# End Source File
# Begin Source File

SOURCE=.\DlgOscilloscope.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\dlgpartree.h
# End Source File
# Begin Source File

SOURCE=.\DlgSamples.h
# End Source File
# Begin Source File

SOURCE=.\DlgSdoRead.h
# End Source File
# Begin Source File

SOURCE=.\DlgSdoWrite.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\DlgSlotView.h
# End Source File
# Begin Source File

SOURCE=.\DlgSlotView.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\dlgstrings.h
# End Source File
# Begin Source File

SOURCE=.\DlgStrings.h
# End Source File
# Begin Source File

SOURCE=.\DlgTriggers.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\DockConfigHW.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\DockConfigTree.h
# End Source File
# Begin Source File

SOURCE=.\DockConfigTree.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\DockDbgIOView.h
# End Source File
# Begin Source File

SOURCE=.\DockDbgVarView.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\DockDevice.h
# End Source File
# Begin Source File

SOURCE=.\DockDevice.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\DockDevListView.h
# End Source File
# Begin Source File

SOURCE=.\DockDevListView.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\DockDevProperty.h
# End Source File
# Begin Source File

SOURCE=.\DockDevProperty.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\DockDGSdo.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\DockLangView.h
# End Source File
# Begin Source File

SOURCE=.\DockOscilloscope.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\DockPARTree.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\ErrorsComboBox.h
# End Source File
# Begin Source File

SOURCE=.\ErrorsComboBox.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\ErrorsDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\Executor.h
# End Source File
# Begin Source File

SOURCE=.\Executor.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\FindDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\FlatTabCtrl.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\GenDC.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\GlobalArr.h
# End Source File
# Begin Source File

SOURCE=.\GlobalArr.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\GlobalMatrix.h
# End Source File
# Begin Source File

SOURCE=.\GlobalMatrix.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\GlobalVariable.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\GridCtrlDev.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\GridCtrlDevCan.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\GridDeviceBase.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\GridView.h
# End Source File
# Begin Source File

SOURCE=.\GridView.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\HTMLFilter.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\ide2.h
# End Source File
# Begin Source File

SOURCE=.\ide2.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\Ide2_i.h
# End Source File
# Begin Source File

SOURCE=.\iledbarx.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\imageresource.h
# End Source File
# Begin Source File

SOURCE=.\InplaceComboBox.h
# End Source File
# Begin Source File

SOURCE=.\IOSuperGrid.h
# End Source File
# Begin Source File

SOURCE=.\iplotannotationx.h
# End Source File
# Begin Source File

SOURCE=.\iplotaxisx.h
# End Source File
# Begin Source File

SOURCE=.\iplotchannelx.h
# End Source File
# Begin Source File

SOURCE=.\iplotdatacursorx.h
# End Source File
# Begin Source File

SOURCE=.\iplotdataviewx.h
# End Source File
# Begin Source File

SOURCE=.\iplotlabelx.h
# End Source File
# Begin Source File

SOURCE=.\iplotlegendx.h
# End Source File
# Begin Source File

SOURCE=.\iplotlimitx.h
# End Source File
# Begin Source File

SOURCE=.\iplottablecolumnx.h
# End Source File
# Begin Source File

SOURCE=.\iplottablex.h
# End Source File
# Begin Source File

SOURCE=.\iplottoolbarx.h
# End Source File
# Begin Source File

SOURCE=.\iplotx.h
# End Source File
# Begin Source File

SOURCE=.\islidingscalex.h
# End Source File
# Begin Source File

SOURCE=.\iswitchleverx.h
# End Source File
# Begin Source File

SOURCE=.\iswitchpanelx.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\KSFileDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\LangView.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\LedButton.h
# End Source File
# Begin Source File

SOURCE=.\listeditctrl.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\LuaDoc.h
# End Source File
# Begin Source File

SOURCE=.\LuaDoc.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\LuaEditor.h
# End Source File
# Begin Source File

SOURCE=.\LuaEditor.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\LuaFrame.h
# End Source File
# Begin Source File

SOURCE=.\LuaFrame.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\LuaHelper.h
# End Source File
# Begin Source File

SOURCE=.\LuaHelper.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\LuaView.h
# End Source File
# Begin Source File

SOURCE=.\LuaView.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\mainframe.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\NewDev.h
# End Source File
# Begin Source File

SOURCE=.\NewDev.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\NewPlant.h
# End Source File
# Begin Source File

SOURCE=.\NewPlant.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\OpenSource.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\OptionDlg.h
# End Source File
# Begin Source File

SOURCE=.\OscChannelData.h
# End Source File
# Begin Source File

SOURCE=.\OscProp.h
# End Source File
# Begin Source File

SOURCE=.\OscView.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\OutputView.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\OutputWnd.h
# End Source File
# Begin Source File

SOURCE=.\OutputWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\PAREDIT.H
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\PlantManage.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\preprocessor.h
# End Source File
# Begin Source File

SOURCE=.\PreviewResource.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\Project.h
# End Source File
# Begin Source File

SOURCE=.\Project.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\ProjectFile.h
# End Source File
# Begin Source File

SOURCE=.\ProjectFile.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\ProjectNew.h
# End Source File
# Begin Source File

SOURCE=.\ProjectNew.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\ProjectProperties.h
# End Source File
# Begin Source File

SOURCE=.\ProjectProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\PropertyGridItemEx.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\ReplaceDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\ResourceFile.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\ScintillaBar.h
# End Source File
# Begin Source File

SOURCE=.\ScintillaBar.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\ScintillaView.h
# End Source File
# Begin Source File

SOURCE=.\ScintillaView.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\SdoReadGrid.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\SdoWriteGrid.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\SlotView.h
# End Source File
# Begin Source File

SOURCE=.\SSCNETIO.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Adamo.PrimaConfigurazioneHardwareNuova\AdamoIDE\include\Ide\sscnetparameters.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\SSCNetParGrid.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TransparentCheckBox.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\TreeViewFiles.h
# End Source File
# Begin Source File

SOURCE=.\TreeViewFiles.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\TypeSource.h
# End Source File
# Begin Source File

SOURCE=.\UserStrings.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\VariablesBar.h
# End Source File
# Begin Source File

SOURCE=.\VariablesBar.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\VarListView.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\VisValEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\WatchBar.h
# End Source File
# Begin Source File

SOURCE=.\WatchBar.h
# End Source File
# Begin Source File

SOURCE=.\WatchList.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\WatchListView.h
# End Source File
# Begin Source File

SOURCE=.\WatchListView.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\Workspace.h
# End Source File
# Begin Source File

SOURCE=.\Workspace.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Ide\WorkspaceWnd.h
# End Source File
# Begin Source File

SOURCE=.\WorkspaceWnd.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Accelerazione.bmp
# End Source File
# Begin Source File

SOURCE=.\res\AdaDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\AllErrors.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ArrowDownP.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ArrowDownU.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ArrowMask.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ArrowPageDownP.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ArrowPageDownU.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ArrowPageUpP.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ArrowPageUpU.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ArrowUpP.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ArrowUpU.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b1down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b1gray.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b1mask.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b1norm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b1norm1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b2down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b2gray.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b2mask.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b2norm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b2over.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b3down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b3gray.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b3mask.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b3norm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b3over.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b4down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b4gray.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b4mask.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b4norm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b4over.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b5down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b5gray.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b5mask.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b5norm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b5over.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00011.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00012.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00013.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00014.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00015.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00016.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00017.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00018.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00019.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00020.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00021.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00022.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00023.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00024.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00025.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00026.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00027.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00028.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00029.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00030.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00031.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00032.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_empt.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmpDate.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BmpSfondoPagine.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BreakPoints.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ButtonStartTaraturaMask.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ButtonStartTaraturaOff.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ButtonStartTaraturaOn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ButtonStopTaraturaMask.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ButtonStopTaraturaOff.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ButtonStopTaraturaOn.bmp
# End Source File
# Begin Source File

SOURCE=.\CAdamoSink.rgs
# End Source File
# Begin Source File

SOURCE=.\res\CapAlarm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CapAnteprima.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CapComment.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CapCycErrors.bmp
# End Source File
# Begin Source File

SOURCE=.\res\capdgcom.bmp
# End Source File
# Begin Source File

SOURCE=.\res\capdialogs.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CapFunction.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CapGCode.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CapHWErrors.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CapImmagine.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CapIntErrors.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CapLine.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CapMessaggi.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CapMessaggiIndice.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CapMessaggiIndiceStringa.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CapName.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CapPriority.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CapSlots.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CapString.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CapTask.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CapTaskNew.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CapTime.bmp
# End Source File
# Begin Source File

SOURCE=.\res\captracecomm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CapTraceHW.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ClearOsc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ClearOscPressed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CommLed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\conf_tre.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\CustomParameters.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Decelerazione.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DeviceGroup.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Disco.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Dispositivo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\eyeblue.bmp
# End Source File
# Begin Source File

SOURCE=.\res\eyecl2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\eyeclosed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\eyegray.bmp
# End Source File
# Begin Source File

SOURCE=.\res\eyegreen.bmp
# End Source File
# Begin Source File

SOURCE=.\res\eyemask.bmp
# End Source File
# Begin Source File

SOURCE=.\res\eyered.bmp
# End Source File
# Begin Source File

SOURCE=.\res\filetypes.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Finestra.bmp
# End Source File
# Begin Source File

SOURCE=.\res\gomma.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HWTabView.bmp
# End Source File
# Begin Source File

SOURCE=.\res\i.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon4.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon5.ico
# End Source File
# Begin Source File

SOURCE=.\res\ID.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ide2.ico
# End Source File
# Begin Source File

SOURCE=.\res\ide2.rc2
# End Source File
# Begin Source File

SOURCE=.\Ide2.rgs
# End Source File
# Begin Source File

SOURCE=.\res\il_file.bmp
# End Source File
# Begin Source File

SOURCE=.\res\il_tab.bmp
# End Source File
# Begin Source File

SOURCE=.\res\InputOff.bmp
# End Source File
# Begin Source File

SOURCE=.\res\InputOn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\JogCloseLoop.bmp
# End Source File
# Begin Source File

SOURCE=.\res\JogCloseLoopPressed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\JogMinus.bmp
# End Source File
# Begin Source File

SOURCE=.\res\JogMinusPressed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\JogOpenLoop.bmp
# End Source File
# Begin Source File

SOURCE=.\res\JogOpenLoopPressed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\JogPlus.bmp
# End Source File
# Begin Source File

SOURCE=.\res\JogPlusPressed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\JogStop.bmp
# End Source File
# Begin Source File

SOURCE=.\res\JogStopPressed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LedRosso.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LedVerde.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LuaDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Minus.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Monitor.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Monitor1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TentativiDiMotta\Monitor1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Monitor2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Monitor3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Monitor4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mp_channel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mp_channel_active.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mp_thumb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mp_thumb_active.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OptionOsc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OptionOscPressed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\options.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OscCursor.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OscCursorPressed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OscLoad.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OscLoadPressed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OscPrint.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OscPrintPressed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OutputOff.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OutputOn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ParParametriche.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ParTestine.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ParUtensili.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Plus.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Quota.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Regime.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SampleSym.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SaveOsc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SaveOscPressed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\scheda1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sesta322.ico
# End Source File
# Begin Source File

SOURCE=.\res\SettingsOsc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SettingsOscPressed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\short_mp_channel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\short_mp_channel_active.bmp
# End Source File
# Begin Source File

SOURCE=.\res\short_mp_thumb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\short_mp_thumb_active.bmp
# End Source File
# Begin Source File

SOURCE=.\res\showglob.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SlideBar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\slotstat.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SpecialLedMask.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SpecialLedOff.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SpecialLedOn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SpiaLoopClose.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SpiaLoopOpen.bmp
# End Source File
# Begin Source File

SOURCE=.\res\StartOsc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\StartOscPressed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\StartOscPT.bmp
# End Source File
# Begin Source File

SOURCE=.\res\StartOscPTPressed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\StartOscTriggered.bmp
# End Source File
# Begin Source File

SOURCE=.\res\StartOscTriggeredPressed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Stop.bmp
# End Source File
# Begin Source File

SOURCE=.\res\StopOsc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\StopOscPressed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TaskBreaked.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TaskDeath.bmp
# End Source File
# Begin Source File

SOURCE=.\res\taskrun.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Telecomando.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Temple 01.ico"
# End Source File
# Begin Source File

SOURCE=.\res\test.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TestIcon.ico
# End Source File
# Begin Source File

SOURCE=.\res\ThyraAbout.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ThyraSplash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TipoDispositivo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_view.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBarBuild.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBarCanOpen.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBarConf.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBarConfDevices.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBarConfHW.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBarDebug.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBarDiagnostic.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarEdit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBarEthercat.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBarMessages.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBarOsc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBarPlc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarResources.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarResourcesControls.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarResourcesFormat.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarResourcesTools.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbarSize.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBarSSCNET.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBarViewErrors.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TriggerSym.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Debug\CncInterface.tli
# End Source File
# Begin Source File

SOURCE=.\res\html_non.htm
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section ide2 : {072C4747-A112-488E-8BB1-9695F632A101}
# 	2:5:Class:CiSwitchPanelX
# 	2:10:HeaderFile:iswitchpanelx.h
# 	2:8:ImplFile:iswitchpanelx.cpp
# End Section
# Section ide2 : {57408B52-305D-4BAF-8A08-2D0F50E9A992}
# 	2:5:Class:CiSlidingScaleX1
# 	2:10:HeaderFile:islidingscalex2.h
# 	2:8:ImplFile:islidingscalex2.cpp
# End Section
# Section ide2 : {E29C6B91-3542-4F37-82CE-2BFB7B8933D3}
# 	2:21:DefaultSinkHeaderFile:islidingscalex2.h
# 	2:16:DefaultSinkClass:CiSlidingScaleX1
# End Section
# Section ide2 : {249CF20E-8331-4C41-8C13-7C8D2A72FADA}
# 	2:5:Class:CiPlotLabelX
# 	2:10:HeaderFile:iplotlabelx.h
# 	2:8:ImplFile:iplotlabelx.cpp
# End Section
# Section ide2 : {1791C036-8981-492A-BD28-F2331BC9B7C7}
# 	2:21:DefaultSinkHeaderFile:iplotx.h
# 	2:16:DefaultSinkClass:CiPlotX
# End Section
# Section ide2 : {C5412DD1-2E2F-11D3-85BF-00105AC8B715}
# 	2:5:Class:CiAngularGaugeX
# 	2:10:HeaderFile:iangulargaugex.h
# 	2:8:ImplFile:iangulargaugex.cpp
# End Section
# Section ide2 : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section ide2 : {E5F63B80-D2A6-11D3-85C2-00A0CC3A58C9}
# 	2:5:Class:CiSwitchLeverX
# 	2:10:HeaderFile:iswitchleverx.h
# 	2:8:ImplFile:iswitchleverx.cpp
# End Section
# Section ide2 : {217995A5-7C8E-4307-8750-34F4BEC14BB5}
# 	2:5:Class:CiPlotAnnotationX
# 	2:10:HeaderFile:iplotannotationx.h
# 	2:8:ImplFile:iplotannotationx.cpp
# End Section
# Section ide2 : {C5412DD5-2E2F-11D3-85BF-00105AC8B715}
# 	2:21:DefaultSinkHeaderFile:iangulargaugex.h
# 	2:16:DefaultSinkClass:CiAngularGaugeX
# End Section
# Section ide2 : {24DA2975-3806-46E5-BA56-DAC1F3C234DE}
# 	2:5:Class:CiPlotDataViewX
# 	2:10:HeaderFile:iplotdataviewx.h
# 	2:8:ImplFile:iplotdataviewx.cpp
# End Section
# Section ide2 : {D017E3C4-9CA0-4D94-9AA9-CF421E1AF6DD}
# 	2:5:Class:CiPlotX
# 	2:10:HeaderFile:iplotx.h
# 	2:8:ImplFile:iplotx.cpp
# End Section
# Section ide2 : {C0FC6A66-D740-4962-97D5-E0ADD4658924}
# 	2:5:Class:CiPlotAxisX
# 	2:10:HeaderFile:iplotaxisx.h
# 	2:8:ImplFile:iplotaxisx.cpp
# End Section
# Section ide2 : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section ide2 : {E5F63B84-D2A6-11D3-85C2-00A0CC3A58C9}
# 	2:21:DefaultSinkHeaderFile:iswitchleverx.h
# 	2:16:DefaultSinkClass:CiSwitchLeverX
# End Section
# Section ide2 : {27FDC973-2ED3-4769-9ED7-01D81C45A32E}
# 	2:5:Class:CiPlotLimitX
# 	2:10:HeaderFile:iplotlimitx.h
# 	2:8:ImplFile:iplotlimitx.cpp
# End Section
# Section ide2 : {C5412E10-2E2F-11D3-85BF-00105AC8B715}
# 	2:5:Class:CiLedBarX
# 	2:10:HeaderFile:iledbarx.h
# 	2:8:ImplFile:iledbarx.cpp
# End Section
# Section ide2 : {8FB1DF7E-747F-44CA-A5AE-6B0082F299B2}
# 	2:21:DefaultSinkHeaderFile:iswitchpanelx.h
# 	2:16:DefaultSinkClass:CiSwitchPanelX
# End Section
# Section ide2 : {00430002-0002-004D-0200-4C0002005100}
# 	1:8:IDR_IDE2:118
# End Section
# Section ide2 : {0BFA465F-B214-41F6-B5D6-986D4991F257}
# 	2:5:Class:CiPlotToolBarX
# 	2:10:HeaderFile:iplottoolbarx.h
# 	2:8:ImplFile:iplottoolbarx.cpp
# End Section
# Section ide2 : {02537798-A7C9-42D3-A8EE-70088290EAA4}
# 	2:5:Class:CiPlotChannelX
# 	2:10:HeaderFile:iplotchannelx.h
# 	2:8:ImplFile:iplotchannelx.cpp
# End Section
# Section ide2 : {854A06F7-C75E-4A42-B757-B4C352FC8488}
# 	2:5:Class:CiPlotLegendX
# 	2:10:HeaderFile:iplotlegendx.h
# 	2:8:ImplFile:iplotlegendx.cpp
# End Section
# Section ide2 : {C5412DC3-2E2F-11D3-85BF-00105AC8B715}
# 	2:5:Class:CiKnobX
# 	2:10:HeaderFile:iknobx.h
# 	2:8:ImplFile:iknobx.cpp
# End Section
# Section ide2 : {C5412E14-2E2F-11D3-85BF-00105AC8B715}
# 	2:21:DefaultSinkHeaderFile:iledbarx.h
# 	2:16:DefaultSinkClass:CiLedBarX
# End Section
# Section ide2 : {C5412DC7-2E2F-11D3-85BF-00105AC8B715}
# 	2:21:DefaultSinkHeaderFile:iknobx.h
# 	2:16:DefaultSinkClass:CiKnobX
# End Section
# Section ide2 : {E3F19885-4F77-438B-96FC-B3D16F3CC31F}
# 	2:5:Class:CiPlotTableColumnX
# 	2:10:HeaderFile:iplottablecolumnx.h
# 	2:8:ImplFile:iplottablecolumnx.cpp
# End Section
# Section ide2 : {57EE716C-BC86-4DFC-8548-5FD12F6D855F}
# 	2:5:Class:CiPlotDataCursorX
# 	2:10:HeaderFile:iplotdatacursorx.h
# 	2:8:ImplFile:iplotdatacursorx.cpp
# End Section
# Section ide2 : {48ED4A15-F4EE-410F-B52E-AC3875D1608E}
# 	2:5:Class:CiPlotTableX
# 	2:10:HeaderFile:iplottablex.h
# 	2:8:ImplFile:iplottablex.cpp
# End Section
