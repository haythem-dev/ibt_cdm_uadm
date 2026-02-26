/*****************************************************************************/
/*                                                                           */
/*   header     : DadeDb.cxx                                                 */
/*   Author     : A. Schmidt-Rehschuh (644)                                  */
/*   made       : 01.09.97                                                   */
/*   language   : C/Esql                                                     */
/*                                                                           */
/*   declaration: standard-functions for database calls. made for dade       */
/*                                                                           */
/*****************************************************************************/

/*** INCLUDE *****************************************************************/

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdarg.h>
 #include "dadedb.h" 
 exec sql include sqlca;

 #ifdef __S390IBM__
 #pragma linkage(dsnali, OS)    
 #pragma linkage(dsntiar, OS)   
 #endif

/*** DECLARATION *************************************************************/

 static void DecimalPoint( char *spBuf );
 static long Exp10( int iExp );
 
/*** OPEN-DB *****************************************************************/

 #if !defined (__S390IBM__) && !defined (__ORACLE__)
 retClient OpenDb( const char *spDatabase, tError *stError )
 {
   exec sql begin declare section;
   char *sqlDb = (char*)spDatabase;
   exec sql end declare section;
   char *fun = "OpenDb"; 
   exec sql database :sqlDb;
   return CheckSql( fun, stError, "open %s", spDatabase );
 }	 
 #endif
 #ifdef __S390IBM__
 retClient OpenDb( char *spDatabase, char *spSsid, tError *stError )
 {
   char    plan[9];
   char    ssid[5];
   char    open[13];
   long    return_code = 0;
   long    reason_code = 0;
   long    ret;
   char *fun = "OpenDb"; 

   sprintf( open, "%-12.12s", "OPEN" );
   sprintf( plan, "%-8.8s", spDatabase );
   sprintf( ssid, "%-4.4s", spSsid );

   if ( ( ret = dsnali( open, ssid, plan, &return_code, &reason_code ) ) != 0 )
   {
      dsnali( "TRANSLATE  ", &sqlca, &return_code, &reason_code );
	  sqlca.sqlcode = sqlca.sqlcode == 0 ? -1 : sqlca.sqlcode;
   }
   
   return CheckSql( fun, stError, "open %s (ret=%ld)", spDatabase, ret );
 }	 
 #endif 

 /*** CONNECT-DB **************************************************************/
 
 #if !defined (__S390IBM__) && !defined (__ORACLE__)
 retClient ConnectDb( const char *spDatabase, tError *stError )
 {
   exec sql begin declare section;
   char *sqlDb = (char*)spDatabase;;
   exec sql end declare section;
   char *fun = "ConnectDb";
   exec sql connect to :sqlDb;
   return CheckSql( fun, stError, "connect %s", spDatabase );
 }	 
 #endif
 #ifdef __S390IBM__
 retClient ConnectDb( char *spDatabase, char *spSsid, tError *stError )
 {
   char    plan[9];
   char    ssid[5];
   char	   srdura[11] = "SRDURA(CD)";
   long    return_code = 0;
   long    reason_code = 0;
   long    tecb        = 0;
   long    secb        = 0;
   long    ribptr      = 0;
   long    eibptr      = 0;
   char *fun = "ConnectDb";

   sprintf( plan, "%-8.8s", spDatabase );
   sprintf( ssid, "%-4.4s", spSsid );

   dsnali( "CONNECT    ", ssid, tecb, &secb, &ribptr,
	       &return_code, &reason_code, srdura, &eibptr);                                                        
   if ( return_code )
   {
	   dsnali( "TRANSLATE   ", &sqlca, &return_code, &reason_code );
   }
   
   return CheckSql( fun, stError, "connect %s", spDatabase );
 }	 
 #endif
 #ifdef __ORACLE__
 retClient ConnectDb( char *spDatabase, char *spUser,
	                  char *spPassword, tError *stError )
 {
   exec sql begin declare section;
   char *sqlDb;
   char *sqlUser;
   char *sqlPassword;   
   exec sql end declare section;
   char *fun = "ConnectDb";

   sqlDb       = spDatabase;
   sqlUser     = spUser;
   sqlPassword = spPassword;
   
   exec sql connect :sqlUser identified by :sqlPassword using :sqlDb;
   return CheckSql( fun, stError, "connect %s", spDatabase );
 }	 
 #endif

/*** CLOSE-DB ****************************************************************/
	 
 #if !defined (__S390IBM__) && !defined (__ORACLE__)
 retClient CloseDb( tError *stError )
 {
   char *fun = "CloseDb"; 
   exec sql close database;	 
   return CheckSql( fun, stError, "close Database" );
 }	 
 #endif
 #ifdef __S390IBM__
 retClient CloseDb( char *spDatabase, char *spSsid, tError *stError )
 {
   char    plan[9];
   char    ssid[5];
   long    return_code = 0;
   long    reason_code = 0;
   char *fun = "CloseDb"; 

   sprintf( plan, "%-8.8s", spDatabase );
   sprintf( ssid, "%-4.4s", spSsid );

   dsnali( "CLOSE        ", ssid, plan, &return_code, &reason_code );
   dsnali( "TRANSLATE   ", &sqlca, &return_code, &reason_code );
   
   return CheckSql( fun, stError, "close Database" );
 }	 
 #endif	 
 
/*** SET-ISOLATION ***********************************************************/
	 
 #ifndef __S390IBM__
 retClient SetIsolation( sqlread iSqlRead, tError *stError )
 {
   char *fun = "SetIsolation"; 
   char msg[20];

   *msg = '\0';
   switch( iSqlRead )
   {
   case DIRTY_READ		: exec sql set isolation to dirty read; break;
   case COMMITTED_READ	: exec sql set isolation to committed read; break;
   case CURSOR_STABILITY: exec sql set isolation to cursor stability; break;
   case REPEATABLE_READ : exec sql set isolation to repeatable read; break;                      
   default              : sqlca.sqlcode = -1; strcpy( msg, "wrong parameter" );
   }

   return CheckSql( fun, stError, "Set Isolation %s", msg );
 }   
 #else
 retClient SetIsolation( sqlread iSqlRead, tError *stError )
 {
   char *fun = "SetIsolation"; 
   sqlca.sqlcode = IS_OK;
   return CheckSql( fun, stError, "Set Isolation %s", " " );
 }   
 #endif

/*** SET-LOCK MODE ***********************************************************/

 #ifndef __S390IBM__
 retClient SetLockMode( int iSec, tError *stError )
 {
   exec sql begin declare section;
   char szPrepare[40]; 
   exec sql end declare section;
   char *fun = "SetLockMode"; 
   
   if ( iSec <= 0 ) 
      strcpy( szPrepare, "set lock mode to not wait" );   
      else
      sprintf( szPrepare, "set lock mode to wait %d", iSec > 60 ? 60 : iSec );   
   
   exec sql prepare PlockMode from :szPrepare;             
   if ( sqlca.sqlcode == 0 )
      exec sql execute PlockMode; 
 
   return CheckSql( fun, stError, "set lock mode to wait %d", iSec );
 }   
 #endif

/*** SET-EXPLAIN-ON **********************************************************/

 #if !defined (__S390IBM__) && !defined (__ORACLE__)
 retClient SetExplainOn( tError *stError )
 {
   char *fun = "SetExplainOn"; 
   exec sql set explain on;
   return CheckSql( fun, stError, "explain on" );
 }   
 #endif

/*** SET-EXPLAIN-OFF *********************************************************/

 #if !defined  (__S390IBM__) && !defined (__ORACLE__)
 retClient SetExplainOff( tError *stError )
 {
   char *fun = "SetExplainOff"; 
   exec sql set explain off;
   return CheckSql( fun, stError, "explain off" );
 }   
 #endif

/*** BEGIN *******************************************************************/

 #if !defined (__S390IBM__) && !defined (__ORACLE__)
 retClient Begin( char *spFun, tError *stError )
 {
   exec sql begin work;
   return CheckSql( spFun, stError, "begin" );
 }
 #else
 retClient Begin( char *spFun, tError *stError )
 {
   sqlca.sqlcode = IS_OK;
   return CheckSql( spFun, stError, "begin" );
 }
 #endif

/*** COMMIT ******************************************************************/

 #ifndef __S390IBM__
 retClient Commit( char *spFun, tError *stError )
 {
   exec sql commit work;
   return CheckSql( spFun, stError, "commit" );
 }
 #else
 retClient Commit( char *spFun, tError *stError )
 {
   exec sql commit;
   return CheckSql( spFun, stError, "commit" );
 }
 #endif
 
/*** ROLLBACK ****************************************************************/

 #ifndef __S390IBM__
 retClient Rollback( char *spFun, tError *stError )
 {
   exec sql rollback work;
   return CheckSql( spFun, stError, "rollback" );
 }
 #else
 retClient Rollback( char *spFun, tError *stError )
 {
   exec sql rollback;
   return CheckSql( spFun, stError, "rollback" );
 }
 #endif


/*** GET-NUMBER-OF-BRANCH ****************************************************/

 retClient GetNumberOfBranch( void *vBranchNo, dataTyp d, tError *stError )
 {
    exec sql begin declare section;
	short sBranchNo;
	exec sql end declare section;
	char *fun = "GetNumberOfBranch";

    #ifdef _PHOSIX_
	exec sql select filialnr into :sBranchNo from filiale;
    #else
	sqlca.sqlcode = 0;
	if ( getenv( "BRANCH_NO" ) == NULL )
		sqlca.sqlcode = -1; 
	else if ( ( sBranchNo = (short)atoi( (char *)getenv( "BRANCH_NO" ) ) ) == 0 )
	   sqlca.sqlcode = -1;
    #endif	 

	if ( BOOL( sqlca.sqlcode ) ) 
	{
	   switch( (int)d )
	   {
		 case D_STRING: sprintf( (char *)vBranchNo, "%02d", sBranchNo ); break;
		 case D_SHORT : *(short *)vBranchNo = (short)sBranchNo; break;
         case D_INT   : *(int *)vBranchNo = (int)sBranchNo; break;
		 case D_LONG  : *(long *)vBranchNo = (long)sBranchNo; break;
	   	 default: vBranchNo = (void *)0;
	   }
	}
   
    return CheckSql( fun, stError, "select from filiale" );
 }

/*** GetValueOfStream ********************************************************/

 int GetValueOfStream( char *spBuf, void *vValue, int iLen, dataTyp dTyp )    
 {                                                             
    char *pPtr;                                                 
    char *pCop;                                                 
    char szConv[256];                                            
    int iCount;                                                     
                                                              
    pPtr = spBuf + iLen;                                          
    pCop = pPtr;                                                 
                                                              
    for( iCount=0; *pPtr != '|' && *pPtr != '\0'; pPtr++, iCount++  );      
    
	if ( iCount )
	{
	   memcpy( szConv, pCop, iCount );                                    
       szConv[iCount] = '\0';                                            

       switch( (int)dTyp )                                                           
	   {                                                                          
         case D_STRING: strcpy( (char *)vValue, szConv ); break;                     
         case D_INT   : *(int *)vValue = (int)atoi( szConv ); break;                 
         case D_LONG  : *(long *)vValue = (long)atol( szConv ); break;               
         case D_SHORT : *(short *)vValue = (short)atoi( szConv ); break;             
         case D_CHAR  : *(char *)vValue = (char)szConv[0]; break;                    
         case D_DOUBLE: DecimalPoint( szConv ); *(double *)vValue = atof( szConv ); break;
         case D_FLOAT : DecimalPoint( szConv ); *(float *)vValue = (float)atof( szConv ); break;
         default      : vValue = (void *)0;                            
	   }
	}
	else
	{
		/* while null-value! */
       switch( (int)dTyp )                                                           
	   {                                                                          
         case D_STRING: strcpy( (char *)vValue, " " ); break;                     
         case D_INT   : *(int *)vValue = (int)0; break;                 
         case D_LONG  : *(long *)vValue = (long)0; break;               
         case D_SHORT : *(short *)vValue = (short)0; break;             
         case D_CHAR  : *(char *)vValue = ' '; break;                    
         case D_DOUBLE: *(double *)vValue = (double)0; break;
         case D_FLOAT : *(float *)vValue = (float)0; break;
         default      : vValue = (void *)0;                            
	   }
	}
    
	if ( *pPtr == '\0' || *(pPtr+1) == '\0' || *(pPtr+1) == '\n' )              
       return 0;                                                             
    return iCount + iLen + 1;                                                     
 }

/*** GetValueOfStreamLen *****************************************************/

 int GetValueOfStreamLen( char *spBuf, void *vValue, int iLen, int iByte,
	                      dataTyp dTyp, int iDec )    
 {                                                             
    char szConv[256];                                            
    
	if ( iByte > 255 )
	   return 0;
	
	memcpy( szConv, spBuf + iLen, iByte );                                    
    szConv[iByte] = '\0';                                            

    switch( (int)dTyp )                                                           
    {                                                                        
      case D_STRING: strcpy( (char *)vValue, szConv ); break;                     
      case D_INT   : *(int *)vValue = (int)atoi( szConv ); break;                 
      case D_LONG  : *(long *)vValue = (long)atol( szConv ); break;               
      case D_SHORT : *(short *)vValue = (short)atoi( szConv ); break;             
      case D_CHAR  : *(char *)vValue = (char)szConv[0]; break;                    
      case D_DOUBLE: DecimalPoint( szConv );
		             if ( iDec )
		                *(double *)vValue = (double)((double)atol( szConv ) /
						                    Exp10( iDec ) );
					 else
					    *(double *)vValue = atof( szConv );
					 break;
      case D_FLOAT : DecimalPoint( szConv );
		             if ( iDec )
		                *(float *)vValue = (float)((float)atol( szConv ) /
						                    Exp10( iDec ) );
					 else
					    *(float *)vValue = (float)atof( szConv );
					 break;
	  default      : vValue = (void *)0;                            
    }                                                                        
    return iByte + iLen;                                                     
 }

/*** CHECK-SQL ***************************************************************/

 retClient CheckSql( char *spFun, tError *stError, char *spForm, ... )
 {
   va_list ptr_vaList;

   stError->lSqlcode  = sqlca.sqlcode;
   stError->lSqlcisam = sqlca.sqlerrd[1];
   stError->lUpdateNo = sqlca.sqlerrd[2];
   sprintf(stError->spErrm,"%47.47s",sqlca.sqlerrm);

   if ( sqlca.sqlcode == 0 )
      return IS_OK;      
   else if ( sqlca.sqlcode == SQLNOTFOUND )
   {
       sprintf( stError->spError, "%s: SqlNotFound", spFun );
	   return IS_NOTFOUND;
   }
   else if ( sqlca.sqlcode == SQLEXISTS )
   {
       sprintf( stError->spError, "%s: Record already exists", spFun );
	   return IS_EXISTS;
   }
   else
   {    
      va_start( ptr_vaList, spForm );
      sprintf( stError->spError, "%s %d/%d %47.47s", 
	           spFun,
               sqlca.sqlcode,
	           sqlca.sqlerrd[1],
			   sqlca.sqlerrm );
      vsprintf( stError->spError + strlen( stError->spError ), spForm, ptr_vaList );
      va_end( ptr_vaList );
      return IS_ERROR;
   }      
 }

/*** private *****************************************************************/
/*** DecimalPoint ************************************************************/

 static void DecimalPoint( char *spBuf )
 {
   int i;
   int iLen;
  
   for( i = 0, iLen = strlen( spBuf ); i < iLen; i++ )
   {
	   if ( spBuf[i] == ',' )
	   {
		  spBuf[i] = '.';
		  break;
   	   }
   }
   return;
 }
 
/*** DecimalPoint ************************************************************/

 static long Exp10( int iExp )
 {
   long lValue = 1;                  
   int i;                         
                                
   for ( i = 0; i < iExp; i++ )   
       lValue = lValue * 10;            

   return lValue;                    
 }

/*** END *********************************************************************/
