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
 * (c)  Copyright IBM Corporation 2006. All rights reserved.
 *
 *  Title:      sqliapi.h
 *  Description:
 *              header file for all "external" apis for internal users.
 *				These apis are NOT intended for external users.
 *
 ***************************************************************************
 */

#ifndef _SQLIAPI_H
#define _SQLIAPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ifxtypes.h"

#define ESQLINTVERSION		1

mint __cdecl sqli_cl_alloc(int4 version, ifx_collection_t **collection, ifx_typerow_t *typed);
mint __cdecl sqli_desc_alloc(const char *descname, mint occurrence);
mint __cdecl sqli_desc_dealloc(const char *desc_name);
mint __cdecl sqli_desc_get(int4 version, const char *desc_name, mint sqlvar_num, ifx_hostvar_t *hosttab, mint xopen_flg);
mint __cdecl sqli_desc_set(int4 version, const char *desc_name, mint sqlvar_num, ifx_hostvar_t *hosttab, mint xopen_flg);
mint __cdecl sqli_trans_begin(void);
mint __cdecl sqli_trans_begin2(mint replication);
mint __cdecl sqli_trans_commit(void);
mint __cdecl sqli_trans_rollback(void);
mint __cdecl sqli_curs_decl_stat(int4 version, ifx_cursor_t *cursor, const char *curname, const char **cmdtxt, ifx_sqlda_t *idesc, ifx_sqlda_t *odesc, int4 flags, ifx_literal_t *litvalues, ifx_namelist_t *namelist, mint statement_type, mint position, mint col_stmt_flag);
mint __cdecl sqli_curs_decl_dynm(int4 version, ifx_cursor_t *cursor, const char *curname, ifx_cursor_t *stmt, int4 flags, mint col_stmt_flag);
mint __cdecl sqli_curs_fetch(int4 version, ifx_cursor_t *cursor, ifx_sqlda_t *idesc, ifx_sqlda_t *odesc, const char *odesc_name, _FetchSpec *fetchspec);
mint __cdecl sqli_autofree_set(int4 version, ifx_cursor_t *cursor, mint status);
mint __cdecl sqli_defprep_set(mint status);
mint __cdecl sqli_curs_open(int4 version, ifx_cursor_t *cursor, ifx_sqlda_t *idesc, const char *desc_name, struct value *ivalues, mint useflag, mint reoptflag);
ifx_cursor_t * __cdecl sqli_curs_locate(int4 version, const char *name, mint type);
mint __cdecl sqli_curs_flush(int4 version, ifx_cursor_t *cursor);
mint __cdecl sqli_curs_close(int4 version, ifx_cursor_t *cursor);
mint __cdecl sqli_curs_free(int4 version, ifx_cursor_t *cursor);
ifx_cursor_t * __cdecl sqli_prep(int4 version, const char *name, const char *stmt, ifx_literal_t *litvalues, ifx_namelist_t *namelist, mint statement_type, mint position, mint col_stmt_flag);
mint __cdecl sqli_describe_stmt(int4 version, ifx_cursor_t *cursor, ifx_sqlda_t **descp, const char *desc_name);
mint __cdecl sqli_describe_input_stmt(int4 version, ifx_cursor_t *cursor, ifx_sqlda_t **descp, const char *desc_name);
mint __cdecl sqli_describe_output_stmt(int4 version, ifx_cursor_t *cursor, ifx_sqlda_t **descp, const char *desc_name);
mint __cdecl sqli_db_open(const char *db_name, mint exclusive);
mint __cdecl sqli_db_close(void);
mint __cdecl sqli_exec(int4 version, ifx_cursor_t *cursor, ifx_sqlda_t *idesc, const char *idesc_name, struct value *ivalues, ifx_sqlda_t *odesc, const char *odesc_name, struct value *ovalues, mint chkind);
mint __cdecl sqli_exec_immed(const char *stmt);
mint __cdecl sqli_proc_create(char *fname);
mint __cdecl sqli_proc_exec(int4 version, ifx_cursor_t *cursor, const char **cmdtxt, mint icnt, ifx_sqlvar_t *ibind, mint ocnt, ifx_sqlvar_t *obind, mint chkind, mint freecursor);
mint __cdecl sqli_proc_exec_2(int4 version, ifx_cursor_t *cursor, const char **cmdtxt, mint icnt, ifx_sqlvar_t *ibind, mint ocnt, ifx_sqlvar_t *obind, mint sysdesc, mint chkind, mint freecursor);
mint __cdecl sqli_proc_exec_2i(int4 version, ifx_cursor_t *cursor, const char **cmdtxt, mint icnt, ifx_sqlvar_t *ibind, mint ocnt, ifx_sqlvar_t *obind, mint sysdesc, mint chkind, mint freecursor);
mint __cdecl sqli_diag_get(int4 version, ifx_hostvar_t *hosttab, mint exception_num);
mint __cdecl sqli_curs_put(int4 version, ifx_cursor_t *cursor, ifx_sqlda_t *idesc, const char *desc_name);
mint __cdecl sqli_slct(int4 version, ifx_cursor_t *cursor, const char **cmdtxt, mint icnt, ifx_sqlvar_t *ibind, mint ocnt, ifx_sqlvar_t *obind, mint chkind, ifx_literal_t *litvalues, ifx_namelist_t *namelist, mint col_smt_flag);
mint __cdecl sqli_stmt(int4 version, ifx_statement_t *scb, const char **cmdtxt, mint icnt, ifx_sqlvar_t *ibind, struct value *ivalues, ifx_literal_t *litvalues, ifx_namelist_t *namelist, ifx_cursor_t *cur_wco, mint stmttype, mint position, mint col_smt_flag);
void __cdecl sqli_stop_whenever(void);

void __cdecl sqli_connect_open(int4 version, mint conn_kw, const char *dbenv, const char *conn_name, ifx_conn_t *conn_spec, mint concur_tx);
void __cdecl sqli_connect_close(mint conn_kw, const char *conn_name, mint flag, mint from_reassoc);
void __cdecl sqli_connect_set(mint conn_kw, const char *conn_name, mint dormant);
void __cdecl _iqseterr(mint sys_errno);
void __cdecl sqli_mt_ibc(mint cnt);
void __cdecl sqli_mt_ibind(mint type, char *address, mint length, mint itype, char *iaddress, mint ilength, mint ownerlen, char *owner, mint udtlen, char *udtname, mint complexlen, char *complexname, int2 sourcetype, int4 sourceid);
void __cdecl sqli_mt_obc(mint cnt);
void __cdecl sqli_mt_obind(mint type, char *address, mint length, mint itype, char *iaddress, mint ilength, mint ownerlen, char *owner, mint udtlen, char *udtname, mint complexlen, char *complexname, int2 sourcetype, int4 sourceid);
void __cdecl sqli_mt_stmnt(const char *sqlcmdtxt[], mint incnt);
void __cdecl sqli_mt_slct(char *sqlcmdtxt[], mint incnt, mint outcnt, mint chkind);
void __cdecl sqli_mt_copen(int4 version, const char *curname, mint incnt, ifx_sqlda_t *desc, char *dname, mint useflag, mint reoptflag);
void __cdecl sqli_mt_nftch(int4 version, const char *curname, mint outcnt, mint incnt, mint fetch_type, int4 value, ifx_sqlda_t *odesc, ifx_sqlda_t *idesc, mint chk, const char *odname);
void __cdecl sqli_mt_free(const char *name);
void __cdecl sqli_mt_close(const char *name);
void __cdecl sqli_mt_flush(const char *name);
void __cdecl sqli_mt_insput(int4 version, const char *curname, mint incnt, ifx_sqlda_t *desc, char *dname);
void __cdecl sqli_mt_dclcur(int4 version, const char *curname, const char *sqlcmdtxt[], mint incnt, mint outcnt, int4 scrflag, ifx_literal_t *lit, ifx_namelist_t *namelist, mint stmttype, mint position, mint collstmt);
void __cdecl sqli_mt_prepare(const char *name, char *from);
void __cdecl sqli_mt_execute(const char *name, mint incnt, char *indname, mint outcnt, const char *outdname, mint chkind);
void __cdecl sqli_mt_dyndcl(const char *curname, char *sname, int4 flags);
void __cdecl sqli_mt_describe(int4 version, char *sname, ifx_sqlda_t **desc, const char *dname);
void __cdecl sqli_mt_describe_input(int4 version, char *sname, ifx_sqlda_t **desc, const char *dname);
void __cdecl sqli_mt_describe_output(int4 version, char *sname, ifx_sqlda_t **desc, const char *dname);
void __cdecl sqli_mt_database(char *dbname, mint excl);
void __cdecl sqli_mt_exproc(char *stmt, mint num_ibind, mint num_obind, mint chkind);
void __cdecl sqli_mt_connect(int4 version, mint conn_kw, char *dbenv, char *conn_name, void *conn_hdl, mint concur_tx);
void __cdecl sqli_mt_disconnect(mint conn_kw, char *conn_name);
void __cdecl sqli_mt_setconnect(mint conn_kw, char *conn_name);
void __cdecl sqli_mt_rerror (char *msg);
void * __cdecl sqli_mt_alloc_isqlda(int4 version, mint cnt);
ifx_literal_t * __cdecl sqli_mt_alloc_literal(int4 version, mint cnt);
void * __cdecl sqli_mt_alloc_osqlda(int4 version, mint cnt);
void __cdecl sqli_mt_hostbind(int4 version, ifx_hostvar_t *htab, mint vnum, mint type, mint qualifier, mint length, char *addr);
void __cdecl sqli_mt_litbind(int4 version, ifx_literal_t *lit, char *addr, int2 type, int2 len, int2 qual, int2 literal);
mint  __cdecl sqli_setdormant(int2 status);

#define SQLI_cl_alloc(coll, typed)      sqli_cl_alloc(ESQLINTVERSION, coll, typed)
#define	SQLI_alloc(descname, occ)	sqli_desc_alloc(descname, occ)
#define SQLI_dealloc(descname)		sqli_desc_dealloc(descname)
#define SQLI_get_desc(descname, sqlvar_num, hosttab, xopen_flg)	\
	sqli_desc_get(ESQLINTVERSION, descname, sqlvar_num, hosttab, xopen_flg)
#define SQLI_set_desc(descname, sqlvar_num, hosttab, xopen_flg)	\
	sqli_desc_set(ESQLINTVERSION, descname, sqlvar_num, hosttab, xopen_flg)
#define SQLI_beginwork()		sqli_trans_begin()
#define SQLI_beginwork2(replication)		sqli_trans_begin2(replication)
#define	SQLI_commitwork()		sqli_trans_commit()
#define	SQLI_rollback()			sqli_trans_rollback()
#define	SQLI_declare_curs(curs, cname, cmd, in, out, flg, lit, nm, type, pos, is_coll) \
	sqli_curs_decl_stat(ESQLINTVERSION, curs, cname, cmd, in, out, flg, lit, nm, type, pos, is_coll)
#define SQLI_declare_dyn_curs(curs, cname, stmt, flg, is_coll) \
	sqli_curs_decl_dynm(ESQLINTVERSION, curs, cname, stmt, flg, is_coll)
#define SQLI_fetch(cursor, idesc, odesc, odesc_name, fetchspec)	\
	sqli_curs_fetch(ESQLINTVERSION, cursor, idesc, odesc, odesc_name, fetchspec)
#define	SQLI_set_autofree(cursor, status)			\
	sqli_autofree_set(ESQLINTVERSION, cursor, status)
#define	SQLI_set_defprep(status)	sqli_defprep_set(status)
#define	SQLI_open_curs(curs, idesc, dname, ivalues, flg, reopt) \
	sqli_curs_open(ESQLINTVERSION, curs, idesc, dname, ivalues, flg, reopt)
#define SQLI_locate_curs(name, type)				\
	sqli_curs_locate(ESQLINTVERSION, name, type)
#define SQLI_flush_curs(curs)		sqli_curs_flush(ESQLINTVERSION, curs)
#define	SQLI_close_curs(curs)		sqli_curs_close(ESQLINTVERSION, curs)
#define	SQLI_free_curs(curs)		sqli_curs_free(ESQLINTVERSION, curs)
#define SQLI_prepare(name, stmt, litv, namelist, type, pos, iscoll) \
	sqli_prep(ESQLINTVERSION, name, stmt, litv, namelist, type, pos, iscoll)
#define SQLI_describe(curs, desc, name)				\
	sqli_describe_stmt(ESQLINTVERSION, curs, desc, name)
#define SQLI_describe_input(curs, desc, name)				\
	sqli_describe_input_stmt(ESQLINTVERSION, curs, desc, name)
#define SQLI_describe_output(curs, desc, name)				\
	sqli_describe_output_stmt(ESQLINTVERSION, curs, desc, name)
#define	SQLI_dbase(name, excl)		sqli_db_open(name, excl)
#define	SQLI_dbclose()			sqli_db_close()
#define	SQLI_execute(curs, idesc, iname, ivalues, odesc, oname, ovalues, chk) \
	sqli_exec(ESQLINTVERSION, curs, idesc, iname, ivalues, odesc, oname, ovalues, chk)
#define	SQLI_exim(stmt)			sqli_exec_immed(stmt)
#define	SQLI_create_proc(name)		sqli_proc_create(name)
#define SQLI_exec_proc(curs, cmd, icnt, ibind, ocnt, obind, chkind, freecurs) \
	sqli_proc_exec(ESQLINTVERSION, curs, cmd, icnt, ibind, ocnt, obind, chkind, freecurs)
#define SQLI_exec_proc_2(curs, cmd, icnt, ibind, ocnt, obind, sysdesc, chkind, freecurs) \
	sqli_proc_exec_2i(ESQLINTVERSION, curs, cmd, icnt, ibind, ocnt, obind, sysdesc, chkind, freecurs)
#define SQLI_get_diag(hosttab, enum)				\
	sqli_diag_get(ESQLINTVERSION, hosttab, enum)
#define SQLI_put(curs, idesc, dname)				\
	sqli_curs_put(ESQLINTVERSION, curs, idesc, dname)
#define SQLI_select(curs, cmd, icnt, ibind, ocnt, obind, chk, lit, nl, iscoll) \
    	sqli_slct(ESQLINTVERSION, curs, cmd, icnt, ibind, ocnt, obind, chk, lit, nl, iscoll)
#define	SQLI_statement(scb, cmd, icnt, ibind, ival, lit, nl, cur_wco, stmt, pos, iscoll) \
	sqli_stmt(ESQLINTVERSION, scb, cmd, icnt, ibind, ival, lit, nl, cur_wco, stmt, pos, iscoll)
#define SQLI_stop()			sqli_stop_whenever()
#define SQLI_connect(kw, dbenv, name, spec, tx)			\
	sqli_connect_open(ESQLINTVERSION, kw, dbenv, name, spec, tx)
#define	SQLI_disconnect(kw, name, flag, reassoc)		\
	sqli_connect_close(kw, name, flag, reassoc)
#define	SQLI_setconnect(kw, name, dormant)			\
	sqli_connect_set(kw, name, dormant)

#define	SQLI_iec_ibc(cnt)		sqli_mt_ibc(cnt)
#define	SQLI_iec_ibind(type, address, length, itype, iaddress, ilength, ownerlen, owner, udtlen, udtname, complexlen, complexname, sourcetype, sourceid) \
	sqli_mt_ibind(type, address, length, itype, iaddress, ilength, ownerlen, owner, udtlen, udtname, complexlen, complexname, sourcetype, sourceid)
#define SQLI_iec_obc(cnt)		sqli_mt_obc(cnt)
#define	SQLI_iec_obind(type, address, length, itype, iaddress, ilength, ownerlen, owner, udtlen, udtname, complexlen, complexname, sourcetype, sourceid) \
	sqli_mt_obind(type, address, length, itype, iaddress, ilength, ownerlen, owner, udtlen, udtname, complexlen, complexname, sourcetype, sourceid)
#define	SQLI_iec_stmnt(txt, cnt)	sqli_mt_stmnt(txt, cnt)
#define	SQLI_iec_slct(txt, icnt, ocnt, chk)			\
	sqli_mt_slct(txt, icnt, ocnt, chk)
#define SQLI_iec_copen(cname, incnt, desc, dname, flag, reopt)	\
	sqli_mt_copen(ESQLINTVERSION, cname, incnt, desc, dname, flag, reopt)
#define	SQLI_iec_nftch(cname, ocnt, icnt, type, val, odesc, idesc, chk, oname) \
	sqli_mt_nftch(ESQLINTVERSION, cname, ocnt, icnt, type, val, odesc, idesc, chk, oname)
#define	SQLI_iec_free(name)		sqli_mt_free(name)
#define	SQLI_iec_close(name)		sqli_mt_close(name)
#define	SQLI_iec_flush(name)		sqli_mt_flush(name)
#define	SQLI_iec_insput(curname, incnt, desc, dname)		\
	sqli_mt_insput(ESQLINTVERSION, curname, incnt, desc, dname)
#define	SQLI_iec_dclcur(curname, sqlcmdtxt, incnt, outcnt, scrflag, lit, namelist, stmttype, position, collstmt)				\
	sqli_mt_dclcur(ESQLINTVERSION, curname, sqlcmdtxt, incnt, outcnt, scrflag, lit, namelist, stmttype, position, collstmt)
#define SQLI_iec_prepare(name, from)	sqli_mt_prepare(name, from)
#define	SQLI_iec_execute(name, icnt, indname, outcnt, outdname, chkind)	\
	sqli_mt_execute(name, icnt, indname, outcnt, outdname, chkind)
#define	SQLI_iec_dyndcl(cname, sname, flg)			\
	sqli_mt_dyndcl(cname, sname, flg)
#define	SQLI_iec_describe(sname, desc, dname)			\
	sqli_mt_describe(ESQLINTVERSION, sname, desc, dname)
#define	SQLI_iec_describe_input(sname, desc, dname)			\
	sqli_mt_describe_input(ESQLINTVERSION, sname, desc, dname)
#define	SQLI_iec_describe_output(sname, desc, dname)			\
	sqli_mt_describe_output(ESQLINTVERSION, sname, desc, dname)
#define SQLI_iec_database(name, excl)	sqli_mt_database(name, excl)
#define	SQLI_iec_exproc(stmt, icnt, ocnt, chk)			\
	sqli_mt_exproc(stmt, icnt, ocnt, chk)
#define	SQLI_iec_connect(kw, dbenv, name, spec, tx)		\
	sqli_mt_connect(ESQLINTVERSION, kw, dbenv, name, spec, tx)
#define	SQLI_iec_disconnect(kw, name)	sqli_mt_disconnect(kw, name)
#define	SQLI_iec_setconnect(kw, name)	sqli_mt_setconnect(kw, name)
#define	SQLI_iec_rerror(msg)		sqli_mt_rerror(msg)
#define	SQLI_iec_alloc_isqlda(cnt)	sqli_mt_alloc_isqlda(ESQLINTVERSION, cnt)
#define	SQLI_iec_alloc_literal(cnt)	sqli_mt_alloc_literal(ESQLINTVERSION, cnt)
#define	SQLI_iec_alloc_osqlda(cnt)	sqli_mt_alloc_osqlda(ESQLINTVERSION, cnt)
#define	SQLI_iec_hostbind(htab, vnum, type, qualifier, length, addr)	\
	sqli_mt_hostbind(ESQLINTVERSION, htab, vnum, type, qualifier, length, addr)
#define	SQLI_iec_litbind(lit, addr, type, len, qual, literal)		\
	sqli_mt_litbind(ESQLINTVERSION, lit, addr, type, len, qual, literal)

#ifdef __cplusplus
}
#endif

#endif /* _SQLIAPI_H */
