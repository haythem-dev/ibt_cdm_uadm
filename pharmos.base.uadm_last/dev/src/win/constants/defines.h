/**                                                                           */
/**   Author : T.Hörath PRZ GmbH		                                     */
/**   Date   : 05.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __DEFINES_H_
#define __DEFINES_H_

/******************************************************************************/
// Module
/******************************************************************************/
#define MODULE_RIGHTS	1 // Modul Benutzer-Verwaltung 
						  //-->Rechte vergeben, User anlegen / bearbeiten, 
						  //   Benutzergruppen

#define MODULE_DLL		2 // Modul Login-DLL
						  //-->Einstellungen für die Applikationen hinterlegen, 
						  //   die libscmlogin einbinden
/******************************************************************************/
// globale Rechte für UADM 
//	(es existieren weiterhin Rechte für die Bearbeitung von Einstellungen 
//	 in MODULE_DLL für die einzelnen Applikationen - diese sind nach 
//	 dem Applikationskürzel benannt!)
/******************************************************************************/
#define RIGHT_READ_RI	"READ" //Lesen in MODULE_RIGHTS
#define RIGHT_WORK_RI	"WORK" //Arbeiten in MODULE_RIGHTS
#define RIGHT_USRDEL_RI "USRDEL" // User löschen in MODULE_RIGHTS
#define RIGHT_READ_DLL	"READ" // Lesen in MODULE_DLL
#define RIGHT_APPGRP_DLL "APPGRP" //neue Appl + Gruppen in MODULE_DLL anlegen
#define RIGHT_WORK_DLL	"WORK" // Arbeiten in MODULE_DLL -> für alle Applikationen, 
							   //aber nichts globales löschen!
#define RIGHT_DEL_DLL	"DELFORALLAPP" // Daten löschen, die applikations-übergreifende
										//Gültigkeit besitzen -> nur für ADMIN
#define RIGHT_UA25_RI   "UA25" // für UA25 mask permissions to users
/******************************************************************************/
// PW für UADM
/******************************************************************************/
#define PW_UADM			"itsuadm04"

/******************************************************************************/
// ID für Appl. "UADM" + ID für Region "Global"
/******************************************************************************/
#define UADM_ID			3 // NASE in sql-script !? ( analog LOGINDLL )
#define GLOBAL_REG_ID	1000

/******************************************************************************/
// Standard-Sprache
/******************************************************************************/
#define STANDARD_LANGUAGE "DE"

/******************************************************************************/
// SQL-Returncodes
/******************************************************************************/
#define SQL_LOCKED						-243
#define SQL_UNIQUE_EXISTS				-239
#define SQL_UNIQUE_CONSTRAINT_EXISTS	-268

/******************************************************************************/
// Parameter-Bezeichnung in UADM für ODBC
/******************************************************************************/
#define PARA_CONNECT		"ODBC_CONNECT"
#define PARA_UID	        "ODBC_UID"
#define PARA_PWD	        "ODBC_PWD"

#define PARA_CONNECT_TEST	"ODBC_CONNECT_TEST"
#define PARA_UID_TEST       "ODBC_UID_TEST"
#define PARA_PWD_TEST       "ODBC_PWD_TEST"

#define PARA_BI_COUNTRIES	"BI_COUNTRIES"

#define	PARA_HOST				"HOST"
#define	PARA_HOST_BACKUP		"HOST_BACKUP"
#define	PARA_TEST_HOST			"TEST_HOST"
#define	PARA_TEST_HOST_BACKUP	"TEST_HOST_BACKUP"
#define	PARA_DB					"DB"
#define	PARA_DB_BACKUP			"DB_BACKUP"
#define PARA_TEXT_TABLE			"TEXT_TABLE"
#define PARA_UISERV				"UISERV"
#define PARA_ONL_SERV			"ONL_SERV"
#define PARA_VOODOO_PORT		"VOODOO_PORT"

/******************************************************************************/
// Standardbelegung in IBI_USER
/******************************************************************************/
#define IBIUSER_STD_COUNTRY "D"
#define IBIUSER_STD_FORMAT  "HTML"
#define IBIUSER_STD_STYLE   "PHOENIX.sty"

/******************************************************************************/
// Express - Rechte - Vergabe für alle VZ´s ohne HAGEDA
/******************************************************************************/
#define VZ_HAGEDA			21

/******************************************************************************/
// Sonderbehandlung für BI
/******************************************************************************/
#define BI_APPL				"BI-"
#define RIGHT_CATEGORY_NONE	0
#define RIGHT_CATEGORY_MAIN	1
#define RIGHT_CATEGORY_VAR	2

#endif
