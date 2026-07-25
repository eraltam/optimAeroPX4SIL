###########################################################################
## Makefile generated for component 'rtwshared'. 
## 
## Makefile     : rtwshared.mk
## Generated on : Thu Jul 23 13:06:52 2026
## Final product: .\rtwshared.lib
## Product type : static library
## 
###########################################################################

###########################################################################
## MACROS
###########################################################################

# Macro Descriptions:
# PRODUCT_NAME            Name of the system to build
# MAKEFILE                Name of this makefile
# COMPILER_COMMAND_FILE   Compiler command listing model reference header paths
# CMD_FILE                Command file
# MODELLIB                Static library target

PRODUCT_NAME              = rtwshared
MAKEFILE                  = rtwshared.mk
MATLAB_ROOT               = C:\PROGRA~1\MATLAB\R2026a
MATLAB_BIN                = C:\PROGRA~1\MATLAB\R2026a\bin
MATLAB_ARCH_BIN           = $(MATLAB_BIN)\win64
START_DIR                 = C:\SITL\AnelloSummer\optimAeroPX4SIL\work
SOLVER                    = 
SOLVER_OBJ                = 
CLASSIC_INTERFACE         = 0
TGT_FCN_LIB               = ISO_C
MODEL_HAS_DYNAMICALLY_LOADED_SFCNS = 
RELATIVE_PATH_TO_ANCHOR   = ..\..\..
COMPILER_COMMAND_FILE     = rtwshared_comp.rsp
CMD_FILE                  = rtwshared.rsp
C_STANDARD_OPTS           = 
CPP_STANDARD_OPTS         = 
NODEBUG                   = 1
MODELLIB                  = rtwshared.lib

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

PRODUCT = .\rtwshared.lib
PRODUCT_TYPE = "static library"
BUILD_TYPE = "Model Reference Library"

###########################################################################
## INCLUDE PATHS
###########################################################################

INCLUDES_BUILDINFO = 

INCLUDES = $(INCLUDES_BUILDINFO)

###########################################################################
## DEFINES
###########################################################################

DEFINES_BUILD_ARGS = -DINTEGER_CODE=0
DEFINES_CUSTOM = 
DEFINES_OPTS = -DNRT -DIS_SIM_TARGET -DENABLE_SLEXEC_SSBRIDGE=1

DEFINES = $(DEFINES_BUILD_ARGS) $(DEFINES_CUSTOM) $(DEFINES_OPTS)

###########################################################################
## SOURCE FILES
###########################################################################

SRCS = $(START_DIR)\slprj\sim\_sharedutils\rt_nonfinite.c $(START_DIR)\slprj\sim\_sharedutils\rtGetInf.c $(START_DIR)\slprj\sim\_sharedutils\binsearch_u32d_prevIdx.c $(START_DIR)\slprj\sim\_sharedutils\intrp2d_la_pw.c $(START_DIR)\slprj\sim\_sharedutils\plook_bincpa.c $(START_DIR)\slprj\sim\_sharedutils\rt_nrand_Upu32_Yd_f_pw_snf.c $(START_DIR)\slprj\sim\_sharedutils\rt_urand_Upu32_Yd_f_pw_snf.c $(START_DIR)\slprj\sim\_sharedutils\rtGetNaN.c $(START_DIR)\slprj\sim\_sharedutils\CalcPAltCOESA.c $(START_DIR)\slprj\sim\_sharedutils\InitCalcAtmosCOESA.c $(START_DIR)\slprj\sim\_sharedutils\eml_rand_shr3cong_idhNEfES.c $(START_DIR)\slprj\sim\_sharedutils\genrand_uint32_vector_K67AoNKW.c $(START_DIR)\slprj\sim\_sharedutils\genrandu_bUoLFLaa.c $(START_DIR)\slprj\sim\_sharedutils\genrandu_mNGNhqv8.c $(START_DIR)\slprj\sim\_sharedutils\look1_binlxpw.c $(START_DIR)\slprj\sim\_sharedutils\rt_TDelayInterpolate.c $(START_DIR)\slprj\sim\_sharedutils\rt_mrdivide_U1d1x3_U2d3x3_Yd1x3_snf.c

ALL_SRCS = $(SRCS)

###########################################################################
## OBJECTS
###########################################################################

OBJS = rt_nonfinite.obj rtGetInf.obj binsearch_u32d_prevIdx.obj intrp2d_la_pw.obj plook_bincpa.obj rt_nrand_Upu32_Yd_f_pw_snf.obj rt_urand_Upu32_Yd_f_pw_snf.obj rtGetNaN.obj CalcPAltCOESA.obj InitCalcAtmosCOESA.obj eml_rand_shr3cong_idhNEfES.obj genrand_uint32_vector_K67AoNKW.obj genrandu_bUoLFLaa.obj genrandu_mNGNhqv8.obj look1_binlxpw.obj rt_TDelayInterpolate.obj rt_mrdivide_U1d1x3_U2d3x3_Yd1x3_snf.obj

ALL_OBJS = $(OBJS)

###########################################################################
## PREBUILT OBJECT FILES
###########################################################################

PREBUILT_OBJS = 

###########################################################################
## LIBRARIES
###########################################################################

LIBS = 

###########################################################################
## SYSTEM LIBRARIES
###########################################################################

SYSTEM_LIBS = 

###########################################################################
## ADDITIONAL TOOLCHAIN FLAGS
###########################################################################

#---------------
# C Compiler
#---------------

CFLAGS_BASIC = $(DEFINES) @$(COMPILER_COMMAND_FILE)

CFLAGS = $(CFLAGS) $(CFLAGS_BASIC)

#-----------------
# C++ Compiler
#-----------------

CPPFLAGS_BASIC = $(DEFINES) @$(COMPILER_COMMAND_FILE)

CPPFLAGS = $(CPPFLAGS) $(CPPFLAGS_BASIC)

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

#---------------------------------
# Create a static library         
#---------------------------------

$(PRODUCT) : $(OBJS) $(PREBUILT_OBJS)
	@cmd /C @echo ### Creating static library "$(PRODUCT)" ...
	$(AR) $(ARFLAGS) -out:$(PRODUCT) @$(CMD_FILE)
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


{$(START_DIR)}.c.obj:
	$(CC) $(CFLAGS) -Fo"$@" "$<"


{$(START_DIR)}.cpp.obj:
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(START_DIR)}.cc.obj:
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(START_DIR)}.cxx.obj:
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


rt_nonfinite.obj : "$(START_DIR)\slprj\sim\_sharedutils\rt_nonfinite.c"
	$(CC) $(CFLAGS) -Fo"$@" "$(START_DIR)\slprj\sim\_sharedutils\rt_nonfinite.c"


rtGetInf.obj : "$(START_DIR)\slprj\sim\_sharedutils\rtGetInf.c"
	$(CC) $(CFLAGS) -Fo"$@" "$(START_DIR)\slprj\sim\_sharedutils\rtGetInf.c"


binsearch_u32d_prevIdx.obj : "$(START_DIR)\slprj\sim\_sharedutils\binsearch_u32d_prevIdx.c"
	$(CC) $(CFLAGS) -Fo"$@" "$(START_DIR)\slprj\sim\_sharedutils\binsearch_u32d_prevIdx.c"


intrp2d_la_pw.obj : "$(START_DIR)\slprj\sim\_sharedutils\intrp2d_la_pw.c"
	$(CC) $(CFLAGS) -Fo"$@" "$(START_DIR)\slprj\sim\_sharedutils\intrp2d_la_pw.c"


plook_bincpa.obj : "$(START_DIR)\slprj\sim\_sharedutils\plook_bincpa.c"
	$(CC) $(CFLAGS) -Fo"$@" "$(START_DIR)\slprj\sim\_sharedutils\plook_bincpa.c"


rt_nrand_Upu32_Yd_f_pw_snf.obj : "$(START_DIR)\slprj\sim\_sharedutils\rt_nrand_Upu32_Yd_f_pw_snf.c"
	$(CC) $(CFLAGS) -Fo"$@" "$(START_DIR)\slprj\sim\_sharedutils\rt_nrand_Upu32_Yd_f_pw_snf.c"


rt_urand_Upu32_Yd_f_pw_snf.obj : "$(START_DIR)\slprj\sim\_sharedutils\rt_urand_Upu32_Yd_f_pw_snf.c"
	$(CC) $(CFLAGS) -Fo"$@" "$(START_DIR)\slprj\sim\_sharedutils\rt_urand_Upu32_Yd_f_pw_snf.c"


rtGetNaN.obj : "$(START_DIR)\slprj\sim\_sharedutils\rtGetNaN.c"
	$(CC) $(CFLAGS) -Fo"$@" "$(START_DIR)\slprj\sim\_sharedutils\rtGetNaN.c"


CalcPAltCOESA.obj : "$(START_DIR)\slprj\sim\_sharedutils\CalcPAltCOESA.c"
	$(CC) $(CFLAGS) -Fo"$@" "$(START_DIR)\slprj\sim\_sharedutils\CalcPAltCOESA.c"


InitCalcAtmosCOESA.obj : "$(START_DIR)\slprj\sim\_sharedutils\InitCalcAtmosCOESA.c"
	$(CC) $(CFLAGS) -Fo"$@" "$(START_DIR)\slprj\sim\_sharedutils\InitCalcAtmosCOESA.c"


eml_rand_shr3cong_idhNEfES.obj : "$(START_DIR)\slprj\sim\_sharedutils\eml_rand_shr3cong_idhNEfES.c"
	$(CC) $(CFLAGS) -Fo"$@" "$(START_DIR)\slprj\sim\_sharedutils\eml_rand_shr3cong_idhNEfES.c"


genrand_uint32_vector_K67AoNKW.obj : "$(START_DIR)\slprj\sim\_sharedutils\genrand_uint32_vector_K67AoNKW.c"
	$(CC) $(CFLAGS) -Fo"$@" "$(START_DIR)\slprj\sim\_sharedutils\genrand_uint32_vector_K67AoNKW.c"


genrandu_bUoLFLaa.obj : "$(START_DIR)\slprj\sim\_sharedutils\genrandu_bUoLFLaa.c"
	$(CC) $(CFLAGS) -Fo"$@" "$(START_DIR)\slprj\sim\_sharedutils\genrandu_bUoLFLaa.c"


genrandu_mNGNhqv8.obj : "$(START_DIR)\slprj\sim\_sharedutils\genrandu_mNGNhqv8.c"
	$(CC) $(CFLAGS) -Fo"$@" "$(START_DIR)\slprj\sim\_sharedutils\genrandu_mNGNhqv8.c"


look1_binlxpw.obj : "$(START_DIR)\slprj\sim\_sharedutils\look1_binlxpw.c"
	$(CC) $(CFLAGS) -Fo"$@" "$(START_DIR)\slprj\sim\_sharedutils\look1_binlxpw.c"


rt_TDelayInterpolate.obj : "$(START_DIR)\slprj\sim\_sharedutils\rt_TDelayInterpolate.c"
	$(CC) $(CFLAGS) -Fo"$@" "$(START_DIR)\slprj\sim\_sharedutils\rt_TDelayInterpolate.c"


rt_mrdivide_U1d1x3_U2d3x3_Yd1x3_snf.obj : "$(START_DIR)\slprj\sim\_sharedutils\rt_mrdivide_U1d1x3_U2d3x3_Yd1x3_snf.c"
	$(CC) $(CFLAGS) -Fo"$@" "$(START_DIR)\slprj\sim\_sharedutils\rt_mrdivide_U1d1x3_U2d3x3_Yd1x3_snf.c"


###########################################################################
## DEPENDENCIES
###########################################################################

$(ALL_OBJS) : rtw_proj.tmw


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


