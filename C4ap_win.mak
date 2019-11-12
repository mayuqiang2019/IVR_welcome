# Microsoft Developer Studio Generated NMAKE File, Based on C4ap_win.dsp
!IF "$(CFG)" == ""
CFG=C4AP_WIN - Win32 Release
!MESSAGE No configuration specified. Defaulting to C4AP_WIN - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "C4AP_WIN - Win32 Release" && "$(CFG)" !=\
 "C4AP_WIN - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "C4ap_win.mak" CFG="C4AP_WIN - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "C4AP_WIN - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "C4AP_WIN - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "C4AP_WIN - Win32 Release"

OUTDIR=.\debug
INTDIR=.\debug
# Begin Custom Macros
OutDir=.\.\debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\C4ap_win.exe"

!ELSE 

ALL : "$(OUTDIR)\C4ap_win.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\example.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\C4ap_win.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "S4DLL" /Fp"$(INTDIR)\C4ap_win.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=.\debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\C4ap_win.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=c4lib.lib sctools.lib libdtimt.lib libdxxmt.lib libsrlmt.lib\
 libfaxmt.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib c4dll.lib /nologo\
 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\C4ap_win.pdb" /machine:I386\
 /def:".\C4ap_win.def" /out:"$(OUTDIR)\C4ap_win.exe" 
DEF_FILE= \
	".\C4ap_win.def"
LINK32_OBJS= \
	"$(INTDIR)\example.obj"

"$(OUTDIR)\C4ap_win.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "C4AP_WIN - Win32 Debug"

OUTDIR=.\.
INTDIR=.\.
# Begin Custom Macros
OutDir=.\.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\C4ap_win.exe"

!ELSE 

ALL : "$(OUTDIR)\C4ap_win.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\example.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\C4ap_win.exe"
	-@erase "$(OUTDIR)\C4ap_win.ilk"
	-@erase "$(OUTDIR)\C4ap_win.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MT /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "S4DLL" /Fp"$(INTDIR)\C4ap_win.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=./
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\C4ap_win.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=sctools.lib libdtimt.lib libdxxmt.lib libsrlmt.lib libfaxmt.lib\
 c4lib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib c4dll.lib /nologo\
 /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\C4ap_win.pdb" /debug\
 /machine:I386 /def:".\C4ap_win.def" /out:"$(OUTDIR)\C4ap_win.exe" 
DEF_FILE= \
	".\C4ap_win.def"
LINK32_OBJS= \
	"$(INTDIR)\example.obj"

"$(OUTDIR)\C4ap_win.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "C4AP_WIN - Win32 Release" || "$(CFG)" ==\
 "C4AP_WIN - Win32 Debug"
SOURCE=.\example.c

!IF  "$(CFG)" == "C4AP_WIN - Win32 Release"

DEP_CPP_EXAMP=\
	"..\..\..\..\codebase\cpp\source\c4com.h"\
	"..\..\..\..\codebase\cpp\source\c4comws.h"\
	"..\..\..\..\codebase\cpp\source\c4trans.h"\
	"..\..\..\..\codebase\cpp\source\d4data.h"\
	"..\..\..\..\codebase\cpp\source\d4declar.h"\
	"..\..\..\..\codebase\cpp\source\d4defs.h"\
	"..\..\..\..\codebase\cpp\source\d4inline.h"\
	"..\..\..\..\codebase\cpp\source\e4error.h"\
	"..\..\..\..\codebase\cpp\source\e4expr.h"\
	"..\..\..\..\codebase\cpp\source\e4string.h"\
	"..\..\..\..\codebase\cpp\source\f4flag.h"\
	"..\..\..\..\codebase\cpp\source\o4opt.h"\
	"..\..\..\..\codebase\cpp\source\r4relate.h"\
	"..\..\..\..\codebase\cpp\source\r4report.h"\
	"..\..\..\..\codebase\cpp\source\s4sort.h"\
	"..\..\..\..\program files\dialogic\inc\d40low.h"\
	"..\..\..\..\program files\dialogic\inc\dtilib.h"\
	"..\..\..\..\program files\dialogic\inc\dxcallp.h"\
	"..\..\..\..\program files\dialogic\inc\dxdevdef.h"\
	"..\..\..\..\program files\dialogic\inc\dxdigit.h"\
	"..\..\..\..\program files\dialogic\inc\dxr2mf.h"\
	"..\..\..\..\program files\dialogic\inc\dxsync.h"\
	"..\..\..\..\program files\dialogic\inc\dxtables.h"\
	"..\..\..\..\program files\dialogic\inc\dxuio.h"\
	"..\..\..\..\program files\dialogic\inc\dxxxlib.h"\
	"..\..\..\..\program files\dialogic\inc\dxxxlow.h"\
	"..\..\..\..\program files\dialogic\inc\msilib.h"\
	"..\..\..\..\program files\dialogic\inc\sctools.h"\
	"..\..\..\..\program files\dialogic\inc\srl_eblk.h"\
	"..\..\..\..\program files\dialogic\inc\srllib.h"\
	"..\..\..\..\program files\dialogic\inc\srltpt.h"\
	".\d4all.h"\
	

"$(INTDIR)\example.obj" : $(SOURCE) $(DEP_CPP_EXAMP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "C4AP_WIN - Win32 Debug"

DEP_CPP_EXAMP=\
	"..\..\..\..\codebase\cpp\source\c4com.h"\
	"..\..\..\..\codebase\cpp\source\c4comws.h"\
	"..\..\..\..\codebase\cpp\source\c4trans.h"\
	"..\..\..\..\codebase\cpp\source\d4data.h"\
	"..\..\..\..\codebase\cpp\source\d4declar.h"\
	"..\..\..\..\codebase\cpp\source\d4defs.h"\
	"..\..\..\..\codebase\cpp\source\d4inline.h"\
	"..\..\..\..\codebase\cpp\source\e4error.h"\
	"..\..\..\..\codebase\cpp\source\e4expr.h"\
	"..\..\..\..\codebase\cpp\source\e4string.h"\
	"..\..\..\..\codebase\cpp\source\f4flag.h"\
	"..\..\..\..\codebase\cpp\source\o4opt.h"\
	"..\..\..\..\codebase\cpp\source\r4relate.h"\
	"..\..\..\..\codebase\cpp\source\r4report.h"\
	"..\..\..\..\codebase\cpp\source\s4sort.h"\
	"..\..\..\..\program files\dialogic\inc\d40low.h"\
	"..\..\..\..\program files\dialogic\inc\dtilib.h"\
	"..\..\..\..\program files\dialogic\inc\dxcallp.h"\
	"..\..\..\..\program files\dialogic\inc\dxdevdef.h"\
	"..\..\..\..\program files\dialogic\inc\dxdigit.h"\
	"..\..\..\..\program files\dialogic\inc\dxr2mf.h"\
	"..\..\..\..\program files\dialogic\inc\dxsync.h"\
	"..\..\..\..\program files\dialogic\inc\dxtables.h"\
	"..\..\..\..\program files\dialogic\inc\dxuio.h"\
	"..\..\..\..\program files\dialogic\inc\dxxxlib.h"\
	"..\..\..\..\program files\dialogic\inc\dxxxlow.h"\
	"..\..\..\..\program files\dialogic\inc\msilib.h"\
	"..\..\..\..\program files\dialogic\inc\sctools.h"\
	"..\..\..\..\program files\dialogic\inc\srl_eblk.h"\
	"..\..\..\..\program files\dialogic\inc\srllib.h"\
	"..\..\..\..\program files\dialogic\inc\srltpt.h"\
	".\d4all.h"\
	

"$(INTDIR)\example.obj" : $(SOURCE) $(DEP_CPP_EXAMP) "$(INTDIR)"


!ENDIF 


!ENDIF 

