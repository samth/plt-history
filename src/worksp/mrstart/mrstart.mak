# Microsoft Developer Studio Generated NMAKE File, Based on mrstart.dsp
!IF "$(CFG)" == ""
CFG=MrStart - Win32 Release
!MESSAGE No configuration specified. Defaulting to MrStart - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "MrStart - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mrstart.mak" CFG="MrStart - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MrStart - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
OUTDIR=.\..\..\..\collects\launcher
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\..\..\..\collects\launcher
# End Custom Macros

ALL : "$(OUTDIR)\mrstart.exe"


CLEAN :
	-@erase "$(INTDIR)\start.obj"
	-@erase "$(INTDIR)\start.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\mrstart.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\mrstart.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\mrstart.pdb" /machine:I386 /out:"$(OUTDIR)\mrstart.exe" 
LINK32_OBJS= \
	"$(INTDIR)\start.obj" \
	"$(INTDIR)\start.res"

"$(OUTDIR)\mrstart.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

CPP_PROJ=/nologo /ML /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "MRSTART" /Fp"$(INTDIR)\mrstart.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\start.res" /d "NDEBUG" /d "MRSTART" 

!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("mrstart.dep")
!INCLUDE "mrstart.dep"
!ELSE 
!MESSAGE Warning: cannot find "mrstart.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "MrStart - Win32 Release"
SOURCE=.\..\starters\start.c

"$(INTDIR)\start.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\..\starters\start.rc

"$(INTDIR)\start.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\start.res" /i "..\starters" /d "NDEBUG" /d "MRSTART" $(SOURCE)



!ENDIF 

