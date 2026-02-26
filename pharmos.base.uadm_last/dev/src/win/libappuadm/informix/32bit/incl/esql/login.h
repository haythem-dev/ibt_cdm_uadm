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
 * (c)  Copyright IBM Corporation 2002. All rights reserved.
 *
 *  Title:	login.h
 *  Description:
 *
 *		login structure definition
 *
 ***************************************************************************
 */


#ifndef LOGIN_INCL
#define LOGIN_INCL

#ifdef __BORLANDC__
#pragma option -a8
#else
#pragma pack (8)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*This type is for saving LOGIN variables for communication between FE and BE*/

typedef struct LoginInfoStructTag
	{
	/* Env Info */
	char InfxServer[19];
	char DbPath[129];
	char DbDate[6];	  
	char DbMoney[19];
	char DbTime[81];
	char DbTemp[81];
	char DbLang[19];
	char DbAnsiWarn[1];
	char InformixDir[255];
	char *Client_Loc;
	char *DB_Loc;
	char DbNls[3];
	char CollChar[3];
	char Lang[81];
	char Lc_Collate[81];
	char Lc_CType[81];
	char Lc_Monetary[81];
	char Lc_Numeric[81];
	char Lc_Time[81];
	char ConRetry[4];
	char ConTime[4]; 
	char DelimIdent[4];
	/* Server Info */
	char Host[19];
	char User[19];
	char Pass[19];
	char AskPassAtConnect[2];
	char Service[19];
	char Protocol[19];
	char Options [20];
	/* SqlHosts Info */
	char InformixSqlHosts[255];
	/* more Env. Info */
	char FetBuffSize[6];
	char CC8BitLevel[2];
	char EsqlMF[2];
	/* GL variables */
	char GlDate[129];
	char GlDateTime[129];
	/* Additional Environment variables */	
	char DbAlsBc[2]; 		 
	char DbApiCode[24]; 
	char DbAsciiBc[2]; 	
	char DbCentury[2];		
	char DbCodeset[24];	  
	char DbConnect[2];	   			
	char DbCsConv[9]; 		
	char DbCsOverride[2];  
	char DbCsWidth[12];
	char DbFltMsk[4];
	char DbMoneyScale[6];
	char DbSS2[5];
	char DbSS3[5];	   
	char OptoFC[2];
	char OptMSG[2];
    char AutoFree[2];
    char DefPrepare[2];
	char NoDefDac[4];
	char OptionsEx[256];
	char DbDateEx[256];	  
	} LoginInfoStruct;

typedef struct HostInfoStructTag
{
	char InfxServer[19];
	char Host[19];
	char User[19];
	char Pass[19];
	char AskPassAtConnect[2];
	char Service[19];
	char Protocol[19];
	char Options [20];
} HostInfoStruct; 

typedef struct HostInfoStructExTag
{
	char InfxServer[19];
	char Host[19];
	char User[19];
	char Pass[19];
	char AskPassAtConnect[2];
	char Service[19];
	char Protocol[19];
	char Options[256];
} HostInfoStructEx; 

typedef struct HostInfoStructEx2Tag
{
    int  StructLen;
    char InfxServer[129];
    char Host[257];
    char User[33];
    char Pass[19];
    char AskPassAtConnect[2];
    char Service[19];
    char Protocol[19];
    char Options [256];
} HostInfoStructEx2; 

#ifdef __cplusplus
}
#endif

#ifdef __BORLANDC__
#pragma option -a-
#else
#pragma pack ()
#endif

#endif /* LOGIN_INCL */
