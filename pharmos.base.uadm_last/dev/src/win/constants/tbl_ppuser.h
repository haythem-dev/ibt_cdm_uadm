#ifndef ZPCUA1_PPUSER_INCLUDED
#define ZPCUA1_PPUSER_INCLUDED

/* ----- Feldlaengen ------ */

#define L_PPUSER_USERNAME     20
#define L_PPUSER_PASSWORD     8
#define L_PPUSER_SURNAME      30
#define L_PPUSER_FORENAME     20
#define L_PPUSER_DESCRIPTION  254
#define L_PPUSER_MAILUSER     30
#define L_PPUSER_EMAILUSER    50
#define L_PPUSER_NTUSER       50
#define L_PPUSER_REFERENCE    40
#define L_PPUSER_PWD_ENC      254
#define L_PPUSER_COUNTRY      2

/* ----- Feldpositionen ------ */

#define P_PPUSER_USERID       0
#define P_PPUSER_USERNAME     1
#define P_PPUSER_PASSWORD     2
#define P_PPUSER_SURNAME      3
#define P_PPUSER_FORENAME     4
#define P_PPUSER_DESCRIPTION  5
#define P_PPUSER_MAILUSER     6
#define P_PPUSER_EMAILUSER    7
#define P_PPUSER_CHANGEPW     8
#define P_PPUSER_ACTIV        9
#define P_PPUSER_LASTLOGIN    10
#define P_PPUSER_NTUSER       11
#define P_PPUSER_LASTPWCHANGE 12
#define P_PPUSER_REFERENCE    13
#define P_PPUSER_EXTERNAL     14
#define P_PPUSER_PWD_ENC      15
#define P_PPUSER_COUNTRY      16
#define P_PPUSER_USERTYPEID   17

#define POSANZ_PPUSER         18

/* ----- Struktur ------ */

struct PPUSER  {
       long    USERID       ;
       char    USERNAME     [21];
       char    PASSWORD     [9];
       char    SURNAME      [31];
       char    FORENAME     [21];
       char    DESCRIPTION  [255];
       char    MAILUSER     [31];
       char    EMAILUSER    [51];
       short   CHANGEPW     ;
       short   ACTIV        ;
       long    LASTLOGIN    ;
       char    NTUSER       [51];
       long    LASTPWCHANGE ;
       char    REFERENCE    [41];
       short   EXTERNAL     ;
       char    PWD_ENC      [255];
	   char    COUNTRY      [3];
	   short   USERTYPEID   ;
};

/* ----- Structur Beschreibung ------ */

#define STRUCDESC_PPUSER \
       "ic20c8c30c20c254c30c50sslc50lc40sc254c2s"

/* ----- Laenge des Uebertragungspuffers ------ */

#define BUFLEN_PPUSER  833

/* ----- Projektion ------ */

#define PROJECTION_PPUSER \
       "USERID," \
       "USERNAME," \
       "PASSWORD," \
       "SURNAME," \
       "FORENAME," \
       "DESCRIPTION," \
       "MAILUSER," \
       "EMAILUSER," \
       "CHANGEPW," \
       "ACTIV," \
       "LASTLOGIN," \
       "NTUSER," \
       "LASTPWCHANGE," \
       "REFERENCE," \
       "EXTERNAL," \
       "PWD_ENC," \
       "COUNTRY," \
       "USERTYPEID"

/* ----- Projektion fuer Joins ------ */

#define JOIN_PROJECTION_PPUSER \
       "PPUSER.USERID," \
       "PPUSER.USERNAME," \
       "PPUSER.PASSWORD," \
       "PPUSER.SURNAME," \
       "PPUSER.FORENAME," \
       "PPUSER.DESCRIPTION," \
       "PPUSER.MAILUSER," \
       "PPUSER.EMAILUSER," \
       "PPUSER.CHANGEPW," \
       "PPUSER.ACTIV," \
       "PPUSER.LASTLOGIN," \
       "PPUSER.NTUSER," \
       "PPUSER.LASTPWCHANGE," \
       "PPUSER.REFERENCE," \
       "PPUSER.EXTERNAL," \
       "PPUSER.PWD_ENC," \
       "PPUSER.COUNTRY," \
       "PPUSER.USERTYPEID"

/* ----- From ------ */

#define FROM_PPUSER \
       "PPUSER"

#endif   /* ZPCUA1_PPUSER_INCLUDED */

