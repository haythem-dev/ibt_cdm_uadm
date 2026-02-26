/****************************************************************************
 *
 *                               IBM INC.
 *
 *                           PROPRIETARY DATA
 *
 * Licensed Material - Property Of IBM
 *
 * "Restricted Materials of IBM"
 *
 * IBM Informix Client SDK
 *
 * (c)  Copyright IBM Corporation 2007, 2008. All rights reserved.
 *
 *  Title:	sqlproto.h
 *  Description:
 *		Prototypes of user functions exported by ISQLIxxx.DLL 
 *
 ***************************************************************************
 */

#ifndef _SQLPROTO
#define _SQLPROTO
#ifdef __cplusplus
extern "C" {
#endif

mint __cdecl bycmpr(char *s1,char *s2,mint cnt);
void __cdecl bycopy(char *s1,char *s2,mint n);
void __cdecl byfill(char *s,mint n,char c);
mint __cdecl byleng(char *beg,mint cnt);
mint __cdecl decadd(struct decimal *n1,struct decimal *n2,struct decimal *n3);
mint __cdecl deccmp(struct decimal *n1,struct decimal *n2);
void __cdecl deccopy(struct decimal *n1,struct decimal *n2);
mint __cdecl deccvasc(char *cp,mint len,struct decimal *np);
mint __cdecl deccvdbl(double dbl,struct decimal *np);
mint __cdecl deccvint(mint in,struct decimal *np);
mint __cdecl deccvlong(int4 lng,struct decimal *np);
mint __cdecl decdiv(struct decimal *n1,struct decimal *n2,struct decimal *n3);
char * __cdecl dececvt(struct decimal *np,mint ndigit,mint *decpt,mint *sign);
char * __cdecl decfcvt(struct decimal *np,mint ndigit,mint *decpt,mint *sign);
mint __cdecl decmul(struct decimal *n1,struct decimal *n2,struct decimal *n3);
void __cdecl decround(struct decimal *np,mint dec_round);
mint __cdecl decsub(struct decimal *n1,struct decimal *n2,struct decimal *n3);
mint __cdecl dectoasc(struct decimal *np,char *cp,mint len,mint right);
mint __cdecl dectodbl(struct decimal *np,double *dblp);
mint __cdecl dectoint(struct decimal *np,mint *ip);
mint __cdecl dectolong(struct decimal *np,int4 *lngp);
void __cdecl dectrunc(struct decimal *np,mint trunc);
void __cdecl dtcurrent(struct dtime *d);
mint __cdecl dtcvasc(char *str,struct dtime *d);
mint __cdecl ifx_dtcvasc(char *str,struct dtime *d, char db_century);
mint __cdecl dtcvfmtasc(char *input,char *fmt,struct dtime *d);
mint __cdecl ifx_dtcvfmtasc(char *input,char *fmt,struct dtime *d, char db_century);
mint __cdecl dtextend(struct dtime *id,struct dtime *od);
mint __cdecl dttoasc(struct dtime *d,char *str);
mint __cdecl dttofmtasc(struct dtime *d,char *output,mint str_len,char *fmtstr);
mint __cdecl ifx_dttofmtasc(struct dtime *d,char *output,mint str_len,char *fmtstr, char db_century);
mint __cdecl incvasc(char *str,struct intrvl *i);
mint __cdecl incvfmtasc(char *input,char *fmt,struct intrvl *intvl);
mint __cdecl intoasc(struct intrvl *i,char *str);
mint __cdecl intofmtasc(struct intrvl *i,char *output,mint str_len,char *fmtstr);
void __cdecl ldchar(char *from,mint count,char *to);
mint __cdecl rdatestr(int4 jdate,char *str);
mint __cdecl rdayofweek(int4 date);
mint __cdecl rdefmtdate(int4 *pdate,char *fmtstring,char *input);
mint __cdecl ifx_defmtdate(int4 *pdate,char *fmtstring,char *input, char db_century);
void __cdecl rdownshift(char *s);
mint __cdecl rfmtdate(int4 date,char *fmtstring,char *result);
mint __cdecl rfmtdec(struct decimal *dec,char *format,char *outbuf);
mint __cdecl rfmtdouble(double dvalue,char *format,char *outbuf);
mint __cdecl rfmtlong(int4 lvalue,char *format,char *outbuf);
mint __cdecl rget_lmsg(int4 msgnum,char *s,mint maxsize,mint *msg_length);
mint __cdecl rgetlmsg(int4 msgnum,char *s,mint maxsize,mint *msg_length);
mint __cdecl rgetmsg(mint msgnum,char *s,mint maxsize);
mint __cdecl risnull(mint vtype,char *pcvar);
mint __cdecl rjulmdy(int4 jdate,short *mdy);
mint __cdecl rleapyear(mint year);
mint __cdecl rmdyjul(short *mdy,int4 *jdate);
mint __cdecl rsetnull(mint vtype,char *pcvar);
mint __cdecl rstrdate(char *str,int4 *jdate);
mint __cdecl ifx_strdate(char *str,int4 *jdate, char db_century);
void __cdecl rtoday(int4 *today);
#if MI_PTR_SIZE == 64  /* 64-bit pointer */
mintptr __cdecl rtypalign(mintptr offset,mint type);
#else
mint __cdecl rtypalign(mint offset,mint type);
#endif
mint __cdecl rtypmsize(mint type,mint len);
char * __cdecl rtypname(mint type);
mint __cdecl rtypwidth(mint type,mint len);
void __cdecl rupshift(char *s);
mint __cdecl sqlexit(void);
void __cdecl stchar(char *from,char *to,mint count);
mint __cdecl stcmpr(char *s1,char *s2);
mint __cdecl rstod(char *str,double *val);
mint __cdecl rstoi(char *s,mint *val);
mint __cdecl rstol(char *s,mlong *val);
mint __cdecl sqgetdbs(mint *ret_fcnt,char **fnames ,mint fnsize,char *farea,mint fasize);
mint __cdecl sqlbreak(void);
mint __cdecl sqlbreakcallback(int4 timeout,void (*callback)(mint));
mint __cdecl ifx_pam_callback(mint (*callbackfunc_ptr)(char *challenge, char *response, mint msg_style));
mint __cdecl sqldetach(void);
mint __cdecl sqldone(void);
mint __cdecl sqlstart(void);
mint __cdecl dtaddinv(struct dtime *d,struct intrvl *i,struct dtime *r);
mint __cdecl dtsub(struct dtime *d1,struct dtime *d2,struct intrvl *i);
mint __cdecl dtsubinv(struct dtime *d,struct intrvl *i,struct dtime *r);
mint __cdecl invdivdbl(struct intrvl *iv,double dbl,struct intrvl *ov);
mint __cdecl invdivinv(struct intrvl *i1,struct intrvl *i2,double *res);
mint __cdecl invextend(struct intrvl *i,struct intrvl *o);
mint __cdecl invmuldbl(struct intrvl *iv,double dbl,struct intrvl *ov);
mint __cdecl ifx_to_gl_datetime(char *source_str, char *target_str, mint maxlen);
void * __cdecl GetConnect ( void );
void * __cdecl SetConnect ( void *NewConInfo );
void * __cdecl ReleaseConnect ( void *NewConInfo );
mint __cdecl deccvflt(double source, struct decimal *destination);
mint __cdecl dectoflt(struct decimal *source, float *destination);
char * __cdecl ifx_getenv(char *env_name);
mint __cdecl ifx_putenv(const char *env_name);
mint __cdecl _infxcexp(mint RtnNum, void * RtnAddr, mint OptParm);
void __cdecl SqlFreeMem( void * MemAddr, mint FreeType );
char * __cdecl ifx_getcur_conn_name(void);

mint __cdecl ifx_int8cmp(struct ifx_int8 *op1, struct ifx_int8 *op2);
mint __cdecl ifx_int8cvlong(int4 lng, struct ifx_int8 *int8p);
mint __cdecl ifx_int8tolong(struct ifx_int8 *int8p, int4 *lngp);
mint __cdecl ifx_int8cvint(mint in, struct ifx_int8 *int8p);
mint __cdecl ifx_int8toint(struct ifx_int8 *int8p, mint *intp);
mint __cdecl ifx_int8toint2(struct ifx_int8 *int8p,short *int2p);
mint __cdecl ifx_int8toint4(struct ifx_int8 *int8p,int4 *int4p);
mint __cdecl ifx_int8cvasc(char *cp, mint len, struct ifx_int8 *int8p);
mint __cdecl ifx_int8toasc(struct ifx_int8 *int8p, char *cp, mint len);
mint __cdecl ifx_int8cvdec(struct decimal *decp, struct ifx_int8 *int8p);
mint __cdecl ifx_int8todec(struct ifx_int8 *int8p, struct decimal *decp);
mint __cdecl ifx_int8cvdbl(double dbl, struct ifx_int8 *int8p);
mint __cdecl ifx_int8todbl(struct ifx_int8 *int8p, double *dblp);
mint __cdecl ifx_int8cvflt(double flt, struct ifx_int8 *int8p);
mint __cdecl ifx_int8toflt(struct ifx_int8 *int8p, float *fltp);
mint __cdecl deccvint8(struct ifx_int8 *int8p, struct decimal *decp);
mint __cdecl ifx_int8sub(struct ifx_int8 *int8op1, struct ifx_int8 *int8op2, struct ifx_int8 *int8result);
mint __cdecl ifx_int8add(struct ifx_int8 *int8op1, struct ifx_int8 *int8op2, struct ifx_int8 *int8result);
mint __cdecl ifx_int8mul(struct ifx_int8 *int8op1, struct ifx_int8 *int8op2, struct ifx_int8 *int8result);
mint __cdecl ifx_int8div(struct ifx_int8 *int8op1, struct ifx_int8 *int8op2, struct ifx_int8 *int8result);
void __cdecl ifx_int8copy(struct ifx_int8 *sint8p, struct ifx_int8 *dint8p);
void __cdecl ifx_getserial8(struct ifx_int8 *int8p);
void __cdecl ifx_getbigserial(bigint *bigintp);

mint __cdecl ifx_cl_dealloc(struct _ifx_collection_struct **collection);
mlong __cdecl ifx_get_row_xid(struct _ifx_collection_struct *collp, mint *colnum);
char * __cdecl ifx_get_row_extended_name(struct _ifx_collection_struct *collp);
char * __cdecl ifx_get_msg_param(void);

mint __cdecl ifx_var_flag(void **variable, short alloc_flag);
mint __cdecl ifx_var_alloc(void **variable, int4 size);
mint __cdecl ifx_var_dealloc(void **variable);
mint __cdecl ifx_var_freevar(void **variable);
mint __cdecl ifx_var_setdata(void **variable, char *data, int4 size);
mint __cdecl ifx_var_isnull(void **variable);
mint __cdecl ifx_var_setnull(void **variable, mint flag);
mint __cdecl ifx_var_setlen(void **variable, int4 size);
void * __cdecl ifx_var_getdata(void **variable);
mint __cdecl ifx_var_init(void **variable);
mint __cdecl ifx_var_getlen(void **variable);
mint __cdecl ifx_lvar_alloc(mint alloc);

/*
 * Set of ifx_lo_* accessors for opaque data types:
 *	- ifx_lo_create_spec_t
 *	- ifx_lo_stat_t
 *
 * See also: locator.h for description of possible values for arguments 
 */

/* smartblob create spec SET accessors: */
mint __cdecl ifx_lo_specset_flags(ifx_lo_create_spec_t *cspec, mint flags);
mint __cdecl ifx_lo_specset_def_open_flags(ifx_lo_create_spec_t *cspec, mint def_open_flags);
mint __cdecl ifx_lo_specset_estbytes(ifx_lo_create_spec_t *cspec, ifx_int8_t *size);
mint __cdecl ifx_lo_specset_maxbytes(ifx_lo_create_spec_t *cspec, ifx_int8_t *size);
mint __cdecl ifx_lo_specset_extsz(ifx_lo_create_spec_t *cspec, mint n);
mint __cdecl ifx_lo_specset_sbspace(ifx_lo_create_spec_t *cspec, const char *str);
  
/* smartblob create spec GET accessors: */
mint __cdecl ifx_lo_specget_flags(ifx_lo_create_spec_t *cspec);
mint __cdecl ifx_lo_specget_def_open_flags(ifx_lo_create_spec_t *cspec);
mint __cdecl ifx_lo_specget_estbytes(ifx_lo_create_spec_t *cspec, ifx_int8_t *size);
mint __cdecl ifx_lo_specget_maxbytes(ifx_lo_create_spec_t *cspec, ifx_int8_t *size);
mint __cdecl ifx_lo_specget_extsz(ifx_lo_create_spec_t *cspec);
mint __cdecl ifx_lo_specget_sbspace(ifx_lo_create_spec_t *cspec, char *str, mint len);

/* smartblob stat GET accessors: */
mint __cdecl ifx_lo_stat_size(ifx_lo_stat_t *lostat, ifx_int8_t *size);
mint __cdecl ifx_lo_stat_uid(ifx_lo_stat_t *lostat);
mint __cdecl ifx_lo_stat_atime(ifx_lo_stat_t *lostat);
mint __cdecl ifx_lo_stat_mtime_sec(ifx_lo_stat_t *lostat);
mint __cdecl ifx_lo_stat_mtime_usec(ifx_lo_stat_t *lostat);
mint __cdecl ifx_lo_stat_ctime(ifx_lo_stat_t *lostat);
mint __cdecl ifx_lo_stat_refcnt(ifx_lo_stat_t *lostat);
ifx_lo_create_spec_t * __cdecl ifx_lo_stat_cspec(ifx_lo_stat_t *lostat);

/* smartblob spec and stat destructors: */
mint __cdecl ifx_lo_spec_free(ifx_lo_create_spec_t *cspec);
mint __cdecl ifx_lo_stat_free(ifx_lo_stat_t *lostat);
/*
 * Set of ifx_lo_ functions for support of Large Objects
 */
mint __cdecl ifx_lo_col_info(char *column_name,ifx_lo_create_spec_t *create_spec);
mint __cdecl ifx_lo_def_create_spec(ifx_lo_create_spec_t **cspec);
mint __cdecl ifx_lo_create(ifx_lo_create_spec_t *create_spec, mint flags, ifx_lo_t *loptr, mint *error);
mint __cdecl ifx_lo_open(ifx_lo_t *loptr, mint flags, mint *error);
mint __cdecl ifx_lo_close(mint lofd);
mint __cdecl ifx_lo_seek(mint lofd, ifx_int8_t *off, mint whence, ifx_int8_t *seek_pos);
mint __cdecl ifx_lo_tell(mint lofd, ifx_int8_t *seek_pos);
mint __cdecl ifx_lo_truncate(mint lofd, ifx_int8_t *off);
mint __cdecl ifx_lo_filename(ifx_lo_t *loptr, char *fname, char *result, mint result_buffer_nbytes);
mint __cdecl ifx_lo_alter(ifx_lo_t *loptr, ifx_lo_create_spec_t *create_spec);
mint __cdecl ifx_lo_stat(mint lofd, ifx_lo_stat_t **lostat);
mint __cdecl ifx_lo_read(mint lofd, char *buf, mint nbytes, mint *error);
mint __cdecl ifx_lo_readwithseek(mint lofd, char *buf, mint nbytes, ifx_int8_t *off, mint whence, mint *error);
mint __cdecl ifx_lo_write(mint lofd, char *buf, mint nbytes, mint *error);
mint __cdecl ifx_lo_writewithseek(mint lofd, char *buf, mint nbytes, ifx_int8_t *off, mint whence, mint *error);
mint __cdecl ifx_lo_copy_to_lo(mint lofd, char *fname, mint flags);
mint __cdecl ifx_lo_copy_to_file(ifx_lo_t *loptr, char *fname, mint flags, char *result);
mint __cdecl ifx_lo_release(ifx_lo_t *loptr);
mint __cdecl ifx_lo_to_buffer(struct ifx_lo_ts *lo_handle,mint size_limit,char **buffer ,mint *error);
mint __cdecl ifx_lo_from_buffer(struct ifx_lo_ts *lo_handle,mint size,char *buffer,mint *error);
mint __cdecl ifx_lo_lock(mint lofd, ifx_int8_t *off, mint whence, ifx_int8_t *range, mint lockmode);
mint __cdecl ifx_lo_unlock(mint lofd, ifx_int8_t *off, mint whence, ifx_int8_t *range);

mint __cdecl ifx_xactevent(void *);
mint __cdecl ifx_getserowner(char *serowner);

void *__cdecl ifx_alloc_conn_user(char *username,char *passwd);    
void __cdecl ifx_free_conn_user(struct ifx_connect_struct **conn);
void *__cdecl ifx_alloc_conn_cred(void *cred);                     
void __cdecl ifx_free_conn_cred(struct ifx_connect_struct **conn);
mint __cdecl ifx_isius(void);
mint __cdecl ifx_cl_card(struct _ifx_collection_struct *colt, mint *isnull);
char * __cdecl	sqli_server_version(void);
mint    __cdecl	ldcollection(char *cp, ifx_collection_t *collp, mint readsize,
								mint longidver);
void   __cdecl	stcollection(ifx_collection_t *collp, char *start, mint longidver);

#ifdef __cplusplus
}
#endif

#endif /* !_SQLPROTO */
