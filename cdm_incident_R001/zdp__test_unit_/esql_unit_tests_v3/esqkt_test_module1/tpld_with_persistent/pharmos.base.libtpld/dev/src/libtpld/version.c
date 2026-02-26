#include "version.h"

const char versioninfo[] = "@(#)libtpld v " LIBTPLD_VERSION " "
#ifdef NDEBUG
        "rel";
#else
        "dbg";
#endif
