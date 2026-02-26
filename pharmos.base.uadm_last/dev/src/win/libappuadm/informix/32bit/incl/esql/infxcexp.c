/****************************************************************************
 *
 * Licensed Material - Property Of IBM
 *
 * "Restricted Materials of IBM"
 *
 * IBM Informix Client SDK
 *
 * (c)  Copyright IBM Corporation 2002,2008. All rights reserved.
 *
 *
 *  Title       : infxcexp.c
 *  Description : Informix C run-time export calls
 *
 ***************************************************************************/

#include "io.h"
#include "errno.h"
#include "stdlib.h"
#include "stdio.h"
#include "memory.h"
#include "direct.h"
#include "process.h"
#include "locale.h"
#include "string.h"
#include "ctype.h"
#include "fcntl.h"

#ifndef __BORLANDC__
#if (_MSC_VER >= 1400)                                              /* For VC8 */
_infxcexp(0,  (FILE*) __iob_func(),  1);
#elif (_MSC_VER >= 1200) || ( (_MSC_VER < 1200) && !defined(_DLL) ) /* For VC7 and VC6 with _DLL not defined */
_infxcexp(0,  &_iob,  1);
#else                                                               /* For VC6 with _DLL defined */
_infxcexp(0,  &__p__iob,  0);
#endif
_infxcexp(1,  &_access,   0);
_infxcexp(2,  &atoi,      0);
_infxcexp(3,  &calloc,    0);
_infxcexp(4,  &_chdir,    0);
#if (_MSC_VER <= 1200)
_infxcexp(5,  &_close,    0);
#endif /*(_MSC_VER <= 1200)*/
_infxcexp(6,  &_creat,    0);
#if defined(_DLL) || defined(_MT) || (_MSC_VER >= 1400) 
_infxcexp(9,  &_errno,     0);
#else /* _DLL || _MT || _MSC_VER */
_infxcexp(9,  &errno,     1);
#endif /* _DLL || _MT || _MSC_VER */
_infxcexp(10, &exit,      0);
_infxcexp(12, &fclose,    0);
_infxcexp(13, &feof,      0);
_infxcexp(14, &ferror,    0);
_infxcexp(15, &fflush,    0);
_infxcexp(16, &fgetc,     0);
_infxcexp(17, &fgets,     0);
_infxcexp(18, &fopen,     0);
_infxcexp(19, &vfprintf,  0);
_infxcexp(20, &fread,     0);
_infxcexp(21, &free,      0);
_infxcexp(22, &fseek,     0);
_infxcexp(23, &ftell,     0);
_infxcexp(25, &fwrite,    0);
_infxcexp(26, &getc,      0);
_infxcexp(27, &_getcwd,   0);
_infxcexp(28, &getenv,    0);
_infxcexp(29, &_getpid,   0);
_infxcexp(31, &_lseek,    0);
_infxcexp(32, &malloc,    0);
_infxcexp(33, &memcpy,    0);
_infxcexp(34, &memset,    0);
#if (_MSC_VER <= 1200)
_infxcexp(35, &_open,     0);
#endif /*(_MSC_VER <= 1200)*/
_infxcexp(36, &_pipe,     0);
_infxcexp(37, &vprintf,   0);
_infxcexp(38, &_putenv,   0);
_infxcexp(39, &qsort,     0);
#if (_MSC_VER <= 1200)
_infxcexp(40, &_read,     0);
#endif /*(_MSC_VER <= 1200)*/
_infxcexp(41, &realloc,   0);
_infxcexp(42, &rename,    0);
_infxcexp(43, &setlocale, 0);
_infxcexp(44, &_setmode,  0);
_infxcexp(45, &vsprintf,  0);
_infxcexp(47, &strcat,    0);
#if (_MSC_VER >= 1400)
_infxcexp(48, (const char*(*)(const char*, int))&strchr,    0);
#else
_infxcexp(48, &strchr,    0);
#endif
_infxcexp(49, &strcmp,    0);
_infxcexp(50, &strcoll,   0);
_infxcexp(51, &strcpy,    0);
_infxcexp(52, &_stricmp,  0);
_infxcexp(53, &strlen,    0);
_infxcexp(54, &strncat,   0);
_infxcexp(55, &strncmp,   0);
_infxcexp(56, &strncpy,   0);
#if (_MSC_VER >= 1400)
_infxcexp(57, (const char*(*)(const char*, int))&strrchr,   0);
_infxcexp(58, (const char*(*)(const char*, const char*))&strstr,  0);
#else
_infxcexp(57, &strrchr,   0);
_infxcexp(58, &strstr,    0);
#endif
_infxcexp(59, &strtok,    0);
_infxcexp(61, &tolower,   0);
_infxcexp(62, &toupper,   0);
_infxcexp(64, &ungetc,    0);
_infxcexp(65, &_unlink,   0);
_infxcexp(66, &_write,    0);
_infxcexp(67, &atexit,    0);
#else /* below are the Borland C exports */
#if defined(_RTLDLL)
_infxcexp(0,  &__getStream, 2);
#else /* _RTLDLL */
_infxcexp(0,  &_streams,  3);
#endif /* _RTLDLL */
_infxcexp(1,  &access,    0);
_infxcexp(2,  &atoi,      0);
_infxcexp(3,  &calloc,    0);
_infxcexp(4,  &chdir,     0);
_infxcexp(5,  &close,     0);
_infxcexp(6,  &creat,     0);
#if defined(_MT)
_infxcexp(9,  &__errno,   0);
#else /* _MT */
_infxcexp(8,  &errno,     1);
#endif /* _MT */
_infxcexp(10, &exit,      0);
_infxcexp(12, &fclose,    0);
_infxcexp(13, &feof,      0);
_infxcexp(14, &ferror,    0);
_infxcexp(15, &fflush,    0);
_infxcexp(16, &fgetc,     0);
_infxcexp(17, &fgets,     0);
_infxcexp(18, &fopen,     0);
_infxcexp(19, &vfprintf,  0);
_infxcexp(20, &fread,     0);
_infxcexp(21, &free,      0);
_infxcexp(22, &fseek,     0);
_infxcexp(23, &ftell,     0);
_infxcexp(25, &fwrite,    0);
_infxcexp(26, &getc,      0);
_infxcexp(27, &getcwd,    0);
_infxcexp(28, &getenv,    0);
_infxcexp(29, &getpid,    0);
_infxcexp(31, &lseek,     0);
_infxcexp(32, &malloc,    0);
_infxcexp(33, &memcpy,    0);
_infxcexp(34, &memset,    0);
_infxcexp(35, &open,      0);
_infxcexp(36, &_pipe,     0);
_infxcexp(37, &vprintf,   0);
_infxcexp(38, &putenv,    0);
_infxcexp(39, &qsort,     0);
_infxcexp(40, &read,      0);
_infxcexp(41, &realloc,   0);
_infxcexp(42, &rename,    0);
_infxcexp(43, &setlocale, 0);
_infxcexp(44, &setmode,   0);
_infxcexp(45, &vsprintf,  0);
_infxcexp(47, &strcat,    0);
_infxcexp(48, &strchr,    0);
_infxcexp(49, &strcmp,    0);
_infxcexp(50, &strcoll,   0);
_infxcexp(51, &strcpy,    0);
_infxcexp(52, &stricmp,   0);
_infxcexp(53, &strlen,    0);
_infxcexp(54, &strncat,   0);
_infxcexp(55, &strncmp,   0);
_infxcexp(56, &strncpy,   0);
_infxcexp(57, &strrchr,   0);
_infxcexp(58, &strstr,    0);
_infxcexp(59, &strtok,    0);
_infxcexp(61, &tolower,   0);
_infxcexp(62, &toupper,   0);
_infxcexp(64, &ungetc,    0);
_infxcexp(65, &unlink,    0);
_infxcexp(66, &write,     0);
_infxcexp(67, &atexit,    0);
#endif /* !__BORLANDC__ */
