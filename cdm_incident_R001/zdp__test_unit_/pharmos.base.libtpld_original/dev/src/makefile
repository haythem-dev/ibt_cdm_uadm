#================================================================================
#   libtpld makefile (Solaris)
#   05.04.2012
#   v 1.0.0
#================================================================================

#--------------------------------------------------------------------------------
# build artifact 
#--------------------------------------------------------------------------------

TARGET         = libtpld.a

#--------------------------------------------------------------------------------
# include common library versions and library depends 
#--------------------------------------------------------------------------------

VARS_FILE      = makevars.inc

include $(VARS_FILE)

DEPLOYMENT_DIR = $(MARATHON_DIR)/orderentry/libtpld

#---------------------------------------------------------------------------------
# source directories
#---------------------------------------------------------------------------------

# sources (seperated with blank if more than one directory)
DIR_SRCS       = libtpld

# headers (seperated with blank if more than one directory)
DIR_INCS       = libtpld

#--------------------------------------------------------------------------------
# libraries 
#--------------------------------------------------------------------------------

LIBS_STATIC    =

LIBS_SHARED    =

# additional library dirs to add with -L like -L$(LIB_DIR)
ADD_LIB_DIRS   =

# additional compiler options like -pthread
ADD_COMP_OPTS  = -DIFX_DISABLE_LOC_T

# additional linker options like -lpthread
ADD_LINK_OPTS  =

#--------------------------------------------------------------------------------
# defines
#--------------------------------------------------------------------------------

COMMON_DEFINES =

C_DEFINES      =
ESQL_DEFINES   = -DESQL_C -DZENTRAL
CPP_DEFINES    =

#--------------------------------------------------------------------------------
# header
#--------------------------------------------------------------------------------

# include paths
INCS           =

#--------------------------------------------------------------------------------
# build rules includes
#--------------------------------------------------------------------------------

BUILD_RULES_FILE = $(MARATHON_DIR)/make/makefile.inc
include $(BUILD_RULES_FILE)

