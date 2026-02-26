#================================================================================
#   common libs and headers
#================================================================================
$(info Including main makevars.inc)


#-------------------------------------------------------------
# externals 
#-------------------------------------------------------------

COMMON_RULES_DIR   = $(DEVLIB_PATH)/marathon/make
INCLUDE_FILE       = $(COMMON_RULES_DIR)/makefile.inc

BASE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

EXT_DIR            = $(BASE_DIR)../../ext

##############################################
####### AIX ##################################
##############################################
LIBS_SYSTEM        = -lz
##############################################

SERVCLNT_DIR       = $(EXT_DIR)/servclnt
LIBS_SERVCLNT_DIR  = $(SERVCLNT_DIR)/lib/$(CONFIG)
LIBS_INFX_ESQL_DIR = $(INFORMIXDIR)/lib/esql
LIBS_INFX_DIR      = $(INFORMIXDIR)/lib

# modeline - do not remove
# vim: ff=unix ts=4 sw=4 noexpandtab syntax=make
