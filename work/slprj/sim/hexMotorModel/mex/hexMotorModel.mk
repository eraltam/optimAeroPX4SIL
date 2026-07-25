###########################################################################
## Makefile generated for component 'hexMotorModel'. 
## 
## Makefile     : hexMotorModel.mk
## Generated on : Thu Jul 23 13:05:00 2026
## Final product: $(RELATIVE_PATH_TO_ANCHOR)\hexMotorModel_msf.mexw64
## Product type : shared library
## 
###########################################################################

###########################################################################
## MACROS
###########################################################################

# Macro Descriptions:
# PRODUCT_NAME            Name of the system to build
# MAKEFILE                Name of this makefile
# MODELREF_LINK_RSPFILE   Linker command listing model reference link objects
# COMPILER_COMMAND_FILE   Compiler command listing model reference header paths
# CMD_FILE                Command file
# DEF_FILE                Definition file

PRODUCT_NAME              = hexMotorModel
MAKEFILE                  = hexMotorModel.mk
MATLAB_ROOT               = C:\PROGRA~1\MATLAB\R2026a
MATLAB_BIN                = C:\PROGRA~1\MATLAB\R2026a\bin
MATLAB_ARCH_BIN           = $(MATLAB_BIN)\win64
START_DIR                 = C:\SITL\AnelloSummer\optimAeroPX4SIL\work
SOLVER                    = 
SOLVER_OBJ                = 
CLASSIC_INTERFACE         = 0
TGT_FCN_LIB               = ISO_C
MODEL_HAS_DYNAMICALLY_LOADED_SFCNS = 0
RELATIVE_PATH_TO_ANCHOR   = ..\..\..\..
MODELREF_LINK_RSPFILE     = hexMotorModel_ref.rsp
COMPILER_COMMAND_FILE     = hexMotorModel_comp.rsp
CMD_FILE                  = hexMotorModel.rsp
DEF_FILE                  = $(PRODUCT_NAME).def
C_STANDARD_OPTS           = 
CPP_STANDARD_OPTS         = 
NODEBUG                   = 1

###########################################################################
## TOOLCHAIN SPECIFICATIONS
###########################################################################

# Toolchain Name:          Microsoft Visual C++ 2022 v17.0 | nmake (64-bit Windows)
# Supported Version(s):    17.0
# ToolchainInfo Version:   2026a
# Specification Revision:  1.0
# 
#-------------------------------------------
# Macros assumed to be defined elsewhere
#-------------------------------------------

# C_STANDARD_OPTS
# CPP_STANDARD_OPTS
# NODEBUG
# cvarsdll
# cvarsmt
# conlibsmt
# ldebug
# conflags
# cflags

#-----------
# MACROS
#-----------

MW_EXTERNLIB_DIR    = $(MATLAB_ROOT)\extern\lib\win64\microsoft
MW_LIB_DIR          = $(MATLAB_ROOT)\lib\win64
CPU                 = AMD64
APPVER              = 5.02
CVARSFLAG           = $(cvarsmt)
CFLAGS_ADDITIONAL   = -D_CRT_SECURE_NO_WARNINGS
CPPFLAGS_ADDITIONAL = -EHs -D_CRT_SECURE_NO_WARNINGS /wd4251 /Zc:__cplusplus
LIBS_TOOLCHAIN      = $(conlibs)

TOOLCHAIN_SRCS = 
TOOLCHAIN_INCS = 
TOOLCHAIN_LIBS = 

#------------------------
# BUILD TOOL COMMANDS
#------------------------

# C Compiler: Microsoft Visual C Compiler
CC = cl

# Linker: Microsoft Visual C Linker
LD = link

# C++ Compiler: Microsoft Visual C++ Compiler
CPP = cl

# C++ Linker: Microsoft Visual C++ Linker
CPP_LD = link

# Archiver: Microsoft Visual C/C++ Archiver
AR = lib

# MEX Tool: MEX Tool
MEX_PATH = $(MATLAB_ARCH_BIN)
MEX = "$(MEX_PATH)\mex"

# Download: Download
DOWNLOAD =

# Execute: Execute
EXECUTE = $(PRODUCT)

# Builder: NMAKE Utility
MAKE = nmake


#-------------------------
# Directives/Utilities
#-------------------------

CDEBUG              = -Z7
C_OUTPUT_FLAG       = -Fo
LDDEBUG             = /DEBUG
OUTPUT_FLAG         = -out:
CPPDEBUG            = -Z7
CPP_OUTPUT_FLAG     = -Fo
CPPLDDEBUG          = /DEBUG
OUTPUT_FLAG         = -out:
ARDEBUG             =
STATICLIB_OUTPUT_FLAG = -out:
MEX_DEBUG           = -g
RM                  = @del
ECHO                = @echo
MV                  = @ren
RUN                 = @cmd /C

#----------------------------------------
# "Faster Builds" Build Configuration
#----------------------------------------

MEX_CPPFLAGS         =
MEX_CPPLDFLAGS       =
MEX_CFLAGS           =
MEX_LDFLAGS          =



#---------------------------
# Model-Specific Options
#---------------------------

CFLAGS = $(cflags) $(cvarsdll) $(CFLAGS_ADDITIONAL) $(C_STANDARD_OPTS) /Od /Oy-

LDFLAGS = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN)

SHAREDLIB_LDFLAGS = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN) -dll -def:$(DEF_FILE)

CPPFLAGS = /TP $(cflags) $(cvarsdll) $(CPPFLAGS_ADDITIONAL) $(CPP_STANDARD_OPTS) /Od /Oy-

CPP_LDFLAGS = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN)

CPP_SHAREDLIB_LDFLAGS = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN) -dll -def:$(DEF_FILE)

ARFLAGS = /nologo

DOWNLOAD_FLAGS = 

EXECUTE_FLAGS = 

MAKE_FLAGS = -f $(MAKEFILE)

###########################################################################
## OUTPUT INFO
###########################################################################

PRODUCT = $(RELATIVE_PATH_TO_ANCHOR)\hexMotorModel_msf.mexw64
PRODUCT_TYPE = "shared library"
BUILD_TYPE = "Shared Library Target"

###########################################################################
## INCLUDE PATHS
###########################################################################

INCLUDES_BUILDINFO = 

INCLUDES = $(INCLUDES_BUILDINFO)

###########################################################################
## DEFINES
###########################################################################

DEFINES_ = -DMATLAB_DEFAULT_RELEASE=R2018a
DEFINES_BUILD_ARGS = -DCLASSIC_INTERFACE=0 -DALLOCATIONFCN=0 -DTERMFCN=1 -DONESTEPFCN=0 -DMAT_FILE=0 -DMULTI_INSTANCE_CODE=0 -DINTEGER_CODE=0
DEFINES_CUSTOM = 
DEFINES_OPTS = -DIS_SIM_TARGET -DENABLE_SLEXEC_SSBRIDGE=1 -DMATLAB_MEX_FILE -DTID01EQ=1
DEFINES_STANDARD = -DMODEL=hexMotorModel -DNUMST=2 -DNCSTATES=12 -DHAVESTDIO -DMDL_REF_SIM_TGT=1 -DMODEL_HAS_DYNAMICALLY_LOADED_SFCNS=0

DEFINES = $(DEFINES_) $(DEFINES_BUILD_ARGS) $(DEFINES_CUSTOM) $(DEFINES_OPTS) $(DEFINES_STANDARD)

###########################################################################
## SOURCE FILES
###########################################################################

SRCS = $(START_DIR)\slprj\sim\hexMotorModel\hexMotorModel_msf.c $(MATLAB_ROOT)\extern\version\c_mexapi_version.c

ALL_SRCS = $(SRCS)

###########################################################################
## OBJECTS
###########################################################################

OBJS = hexMotorModel_msf.obj c_mexapi_version.obj

ALL_OBJS = $(OBJS)

###########################################################################
## PREBUILT OBJECT FILES
###########################################################################

PREBUILT_OBJS = 

###########################################################################
## LIBRARIES
###########################################################################

MODELREF_LIBS = C:\SITL\AnelloSummer\optimAeroPX4SIL\work\slprj\sim\hexMotorModel\hexMotorModellib.lib

LIBS = $(START_DIR)\slprj\sim\_sharedutils\rtwshared.lib

###########################################################################
## SYSTEM LIBRARIES
###########################################################################

SYSTEM_LIBS = /LIBPATH:"$(MATLAB_ROOT)\extern\lib\win64\microsoft" "$(MW_LIB_DIR)\libmwipp.lib" "$(MW_EXTERNLIB_DIR)\libmwipp.lib" "$(MW_EXTERNLIB_DIR)\libfixedpoint.lib" "$(MW_EXTERNLIB_DIR)\libmwmathutil.lib" "$(MW_EXTERNLIB_DIR)\libmwsl_AsyncioQueue.lib" "$(MW_EXTERNLIB_DIR)\sf_runtime.lib" "$(MW_EXTERNLIB_DIR)\libmwslexec_simlog.lib" "$(MW_EXTERNLIB_DIR)\libmwcoder_ToAsyncQueueTgtAppSvc.lib" "$(MW_EXTERNLIB_DIR)\libmwsl_simtarget_instrumentation.lib" "$(MW_EXTERNLIB_DIR)\libmwsl_simtarget_core.lib" "$(MW_EXTERNLIB_DIR)\libmwstringutil.lib" "$(MW_EXTERNLIB_DIR)\libemlrt.lib" "$(MW_EXTERNLIB_DIR)\libmx.lib" "$(MW_EXTERNLIB_DIR)\libmex.lib"

###########################################################################
## ADDITIONAL TOOLCHAIN FLAGS
###########################################################################

#---------------
# C Compiler
#---------------

CFLAGS_ = /wd4701 /wd4100 /wd4189 /wd4090 /wd4505
CFLAGS_BASIC = $(DEFINES) @$(COMPILER_COMMAND_FILE)

CFLAGS = $(CFLAGS) $(CFLAGS_) $(CFLAGS_BASIC)

#-----------
# Linker
#-----------

LDFLAGS_ = /IMPLIB:hexMotorModel_msf.lib /EXPORT:mexFunction /EXPORT:mexfilerequiredapiversion

LDFLAGS = $(LDFLAGS) $(LDFLAGS_)

#--------------------------
# Shared Library Linker
#--------------------------

SHAREDLIB_LDFLAGS_ = /IMPLIB:hexMotorModel_msf.lib /EXPORT:mexFunction /EXPORT:mexfilerequiredapiversion

SHAREDLIB_LDFLAGS = $(SHAREDLIB_LDFLAGS) $(SHAREDLIB_LDFLAGS_)

#-----------------
# C++ Compiler
#-----------------

CPPFLAGS_ = /wd4701 /wd4100 /wd4189 /wd4090 /wd4505
CPPFLAGS_BASIC = $(DEFINES) @$(COMPILER_COMMAND_FILE)

CPPFLAGS = $(CPPFLAGS) $(CPPFLAGS_) $(CPPFLAGS_BASIC)

#---------------
# C++ Linker
#---------------

CPP_LDFLAGS_ = /IMPLIB:hexMotorModel_msf.lib /EXPORT:mexFunction /EXPORT:mexfilerequiredapiversion

CPP_LDFLAGS = $(CPP_LDFLAGS) $(CPP_LDFLAGS_)

#------------------------------
# C++ Shared Library Linker
#------------------------------

CPP_SHAREDLIB_LDFLAGS_ = /IMPLIB:hexMotorModel_msf.lib /EXPORT:mexFunction /EXPORT:mexfilerequiredapiversion

CPP_SHAREDLIB_LDFLAGS = $(CPP_SHAREDLIB_LDFLAGS) $(CPP_SHAREDLIB_LDFLAGS_)

###########################################################################
## INLINED COMMANDS
###########################################################################


!include $(MATLAB_ROOT)\rtw\c\tools\vcdefs.mak


###########################################################################
## PHONY TARGETS
###########################################################################

.PHONY : all build clean info prebuild download execute set_environment_variables


all : build
	@cmd /C @echo ### Successfully generated all binary outputs.


build : set_environment_variables prebuild $(PRODUCT)


prebuild : 


download : $(PRODUCT)


execute : download


set_environment_variables : 
	@set INCLUDE=$(INCLUDES);$(INCLUDE)
	@set LIB=$(LIB)


###########################################################################
## FINAL TARGET
###########################################################################

#----------------------------------------
# Create a shared library
#----------------------------------------

$(PRODUCT) : $(OBJS) $(PREBUILT_OBJS) $(MODELREF_LIBS) $(LIBS)
	@cmd /C @echo ### Creating shared library "$(PRODUCT)" ...
	$(LD) $(SHAREDLIB_LDFLAGS) -out:$(PRODUCT) @$(CMD_FILE) @$(MODELREF_LINK_RSPFILE) $(LIBS) $(SYSTEM_LIBS) $(TOOLCHAIN_LIBS)
	@cmd /C @echo ### Created: "$(PRODUCT)"


###########################################################################
## INTERMEDIATE TARGETS
###########################################################################

#---------------------
# SOURCE-TO-OBJECT
#---------------------

.c.obj:
	$(CC) $(CFLAGS) -Fo"$@" "$<"


.cpp.obj:
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


.cc.obj:
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


.cxx.obj:
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(RELATIVE_PATH_TO_ANCHOR)}.c.obj:
	$(CC) $(CFLAGS) -Fo"$@" "$<"


{$(RELATIVE_PATH_TO_ANCHOR)}.cpp.obj:
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(RELATIVE_PATH_TO_ANCHOR)}.cc.obj:
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(RELATIVE_PATH_TO_ANCHOR)}.cxx.obj:
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


hexMotorModel_msf.obj : "$(START_DIR)\slprj\sim\hexMotorModel\hexMotorModel_msf.c"
	$(CC) $(CFLAGS) -Fo"$@" "$(START_DIR)\slprj\sim\hexMotorModel\hexMotorModel_msf.c"


c_mexapi_version.obj : "$(MATLAB_ROOT)\extern\version\c_mexapi_version.c"
	$(CC) $(CFLAGS) -Fo"$@" "$(MATLAB_ROOT)\extern\version\c_mexapi_version.c"


###########################################################################
## DEPENDENCIES
###########################################################################

$(ALL_OBJS) : $(COMPILER_COMMAND_FILE) $(MAKEFILE)


###########################################################################
## MISCELLANEOUS TARGETS
###########################################################################

info : 
	@cmd /C @echo ### PRODUCT = $(PRODUCT)
	@cmd /C @echo ### PRODUCT_TYPE = $(PRODUCT_TYPE)
	@cmd /C @echo ### BUILD_TYPE = $(BUILD_TYPE)
	@cmd /C @echo ### INCLUDES = $(INCLUDES)
	@cmd /C @echo ### DEFINES = $(DEFINES)
	@cmd /C @echo ### ALL_SRCS = $(ALL_SRCS)
	@cmd /C @echo ### ALL_OBJS = $(ALL_OBJS)
	@cmd /C @echo ### LIBS = $(LIBS)
	@cmd /C @echo ### MODELREF_LIBS = $(MODELREF_LIBS)
	@cmd /C @echo ### SYSTEM_LIBS = $(SYSTEM_LIBS)
	@cmd /C @echo ### TOOLCHAIN_LIBS = $(TOOLCHAIN_LIBS)
	@cmd /C @echo ### CFLAGS = $(CFLAGS)
	@cmd /C @echo ### LDFLAGS = $(LDFLAGS)
	@cmd /C @echo ### SHAREDLIB_LDFLAGS = $(SHAREDLIB_LDFLAGS)
	@cmd /C @echo ### CPPFLAGS = $(CPPFLAGS)
	@cmd /C @echo ### CPP_LDFLAGS = $(CPP_LDFLAGS)
	@cmd /C @echo ### CPP_SHAREDLIB_LDFLAGS = $(CPP_SHAREDLIB_LDFLAGS)
	@cmd /C @echo ### ARFLAGS = $(ARFLAGS)
	@cmd /C @echo ### MEX_CFLAGS = $(MEX_CFLAGS)
	@cmd /C @echo ### MEX_CPPFLAGS = $(MEX_CPPFLAGS)
	@cmd /C @echo ### MEX_LDFLAGS = $(MEX_LDFLAGS)
	@cmd /C @echo ### MEX_CPPLDFLAGS = $(MEX_CPPLDFLAGS)
	@cmd /C @echo ### DOWNLOAD_FLAGS = $(DOWNLOAD_FLAGS)
	@cmd /C @echo ### EXECUTE_FLAGS = $(EXECUTE_FLAGS)
	@cmd /C @echo ### MAKE_FLAGS = $(MAKE_FLAGS)


clean : 
	$(ECHO) "### Deleting all derived files ..."
	@if exist $(PRODUCT) $(RM) $(PRODUCT)
	$(RM) $(ALL_OBJS)
	$(ECHO) "### Deleted all derived files."


