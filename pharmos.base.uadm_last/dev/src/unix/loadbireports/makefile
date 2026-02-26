#=============================================================
#   makefile
#=============================================================

#-------------------------------------------------------------
# build artifact 
#-------------------------------------------------------------

TARGET = loadbireport.bin

#--------------------------------------------------------------------------------
# include common library versions and library depends 
#--------------------------------------------------------------------------------+
BASEDIR        = ..
VARS_FILE      = $(BASEDIR)/makevars.inc
include $(VARS_FILE)

#-------------------------------------------------------------
# header
#-------------------------------------------------------------

# include paths
INCS         = -I$(SERVCLNT_DIR)/include \
               -I$(SERVCLNT_DIR)/include/libscsvoodoo

#-------------------------------------------------------------
# libraries 
#-------------------------------------------------------------

LIBS_DIRS    = -L$(LIBS_INFX_ESQL_DIR) \
               -L$(LIBS_INFX_DIR) \
	             -L$(LIBS_SERVCLNT_DIR)

LIBS_INFX_ESQL = -lifgen -lifos -lifsql -lifgls
LIBS_INFX      = -lifasf

LIBS_SHARED    = $(LIBS_INFX_ESQL) $(LIBS_INFX) $(LIBS_SYSTEM)

LIBS_STATIC    = $(LIBS_SERVCLNT_DIR)/libscsvoodoo.a \
                 $(LIBS_SERVCLNT_DIR)/libscczlib.a \
                 $(LIBS_SERVCLNT_DIR)/libsccstring.a \
                 $(LIBS_SERVCLNT_DIR)/libsccutilclass.a \
                 $(LIBS_SERVCLNT_DIR)/libsccdate.a

#-------------------------------------------------------------
# build options
#-------------------------------------------------------------

# additional compiler options like -pthread
ADD_COMP_OPTS  = -pedantic -Wno-long-long -std=c++0x -w -D_SC_VOODOO_LOCAL_  
# additional linker options like -lpthread
ADD_LINK_OPTS   =

#-------------------------------------------------------------
# Sources
#-------------------------------------------------------------

DIR_SRCS = .

#-------------------------------------------------------------
# Headers
#-------------------------------------------------------------

DIR_INCS = .

#-------------------------------------------------------------
# Library Directories
#-------------------------------------------------------------

ADD_LIB_DIRS = $(LIBS_DIRS)

#-------------------------------------------------------------
# Import all common rules
#-------------------------------------------------------------

include $(INCLUDE_FILE)
