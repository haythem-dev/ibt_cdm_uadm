/****************************************************************************
 *
 * Licensed Material - Property Of IBM
 *
 * "Restricted Materials of IBM"
 *
 * IBM Informix Client SDK
 * Copyright IBM Corporation 2007, 2012
 *
 *  Title:       ifxgls.h
 *  Description: UDS interface to GLS API
 *
 ***************************************************************************/

#ifndef IFXGLS_INCLUDED
#define IFXGLS_INCLUDED

#include "ifxtypes.h"
#include "gls.h"
#include "datetime.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The following defines determine whether gl_locale implies a session
 * locale or a db_locale/client_locale combination depending on server/client
 * initialisation.
 */

#ifdef NT_MI_SAPI /* NT Server/Blade */
#ifdef MI_SERVBUILD
#ifdef NT_SERVER  /* NT Server */
#else             /* NT Blade  */
__declspec(dllimport) gl_lc_t ** mi_get_locale();
#define gl_locale ** mi_get_locale()
#endif /* NT_SERVER */
#endif /* MI_SERVBUILD */

#else /*  UNIX/Win32 Client - Default */

#ifdef MI_SERVBUILD
extern gl_lc_t *scb;
#define gl_locale (*scb)
#else
#ifdef _WIN32
extern __declspec(dllimport) gl_lc_t gls_env;
#else
extern gl_lc_t gls_env;
#endif /* _WIN32 */
#define gl_locale (gls_env)

#endif /* MI_SERVBUILD */
#endif /* NT_MI_SAPI */

/*
 * Locale initialisation function.
 */
mint __cdecl ifx_gl_init (void);

/*
 * Codeset conversion functions.
 */
mint __cdecl ifx_gl_cv_mconv (gl_cv_state_t *state,
                            gl_mchar_t **dst,
                            mint *dstbytes,
                            char *dst_codeset,
                            gl_mchar_t **src,
                            mint *srcbytes,
                            char *src_codeset);

mint __cdecl ifx_gl_cv_outbuflen (char *dst_codeset,
                                char *src_codeset,
                                mint  srcbytes);

mint __cdecl ifx_gl_conv_needed (char *dst_codeset,
                               char *src_codeset);

mint __cdecl ifx_gl_cv_sb2sb_table(char *dstcs,
                                 char *srccs,
                                 unsigned char **array);

/*
 * Built-in Data Type Conversion.
 */
mint __cdecl ifx_gl_convert_date(int4 *date,
                                char *datestr,
                                char *format);

mint __cdecl ifx_gl_format_date(char *datestr,
                               int len,
                               int4 *date,
                               char *format);

mint __cdecl ifx_gl_convert_datetime(dtime_t *dt,
                                    char *datetimestr,
                                    char *format);

mint __cdecl ifx_gl_format_datetime(char *datetimestr,
                                   mint len,
                                   dtime_t *dt,
                                   char *format);

mint __cdecl ifx_gl_convert_double(double *d,
                                 char *dstr,
                                char *format);

mint __cdecl ifx_gl_format_double(char *dstr,
                               mint len,
                                double d,
                                char *format);

mint __cdecl ifx_gl_convert_money(dec_t *mon,
                                 char *monstr,
                                 char *format);

mint __cdecl ifx_gl_format_money(char *monstr,
                                mint len,
                                dec_t *mon,
                                char *format);

mint __cdecl ifx_gl_convert_number(dec_t *dec,
                                  char *decstr,
                                  char *format);

mint __cdecl ifx_gl_format_number(char *decstr,
                                 int len,
                                 dec_t *dec,
                                 char *format);

#ifdef __cplusplus
}
#endif

#endif  /* IFXGLS_INCLUDED : To handle multiple includes */
