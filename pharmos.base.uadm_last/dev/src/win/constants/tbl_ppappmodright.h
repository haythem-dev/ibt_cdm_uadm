#ifndef PCUA1_PPAPPMODRIGHT_INCLUDED
#define PCUA1_PPAPPMODRIGHT_INCLUDED

/* ----- Feldlaengen ------ */

#define L_PPAPPMODRIGHT_RIGHTNAME      20
#define L_PPAPPMODRIGHT_DESCRIPTION    254
#define L_PPAPPMODRIGHT_DESCRIPTION_EN 254
#define L_PPAPPMODRIGHT_DESCRIPTION_FR 254
#define L_PPAPPMODRIGHT_DESCRIPTION_HR 254

/* ----- Feldpositionen ------ */

#define P_PPAPPMODRIGHT_APPLICATIONID  0
#define P_PPAPPMODRIGHT_MODULEID       1
#define P_PPAPPMODRIGHT_RIGHTNAME      2
#define P_PPAPPMODRIGHT_DESCRIPTION    3
#define P_PPAPPMODRIGHT_DESCRIPTION_EN 4
#define P_PPAPPMODRIGHT_DESCRIPTION_FR 5
#define P_PPAPPMODRIGHT_DESCRIPTION_HR 6
#define P_PPAPPMODRIGHT_CATEGORY       7

#define POSANZ_PPAPPMODRIGHT           8

/* ----- Struktur ------ */

struct PPAPPMODRIGHT  {
       short   APPLICATIONID  ;
       short   MODULEID       ;
       char    RIGHTNAME      [21];
       char    DESCRIPTION    [255];
       char    DESCRIPTION_EN [255];
       char    DESCRIPTION_FR [255];
       char    DESCRIPTION_HR [255];
       short   CATEGORY       ;
};

/* ----- Structur Beschreibung ------ */

#define STRUCDESC_PPAPPMODRIGHT \
       "ssc20c254c254c254c254s"

/* ----- Laenge des Uebertragungspuffers ------ */

#define BUFLEN_PPAPPMODRIGHT  1062

/* ----- Projektion ------ */

#define PROJECTION_PPAPPMODRIGHT \
       "APPLICATIONID," \
       "MODULEID," \
       "RIGHTNAME," \
       "DESCRIPTION," \
       "DESCRIPTION_EN," \
       "DESCRIPTION_FR," \
       "DESCRIPTION_HR," \
       "CATEGORY"

/* ----- Projektion fuer Joins ------ */

#define JOIN_PROJECTION_PPAPPMODRIGHT \
       "PPAPPMODRIGHT.APPLICATIONID," \
       "PPAPPMODRIGHT.MODULEID," \
       "PPAPPMODRIGHT.RIGHTNAME," \
       "PPAPPMODRIGHT.DESCRIPTION," \
       "PPAPPMODRIGHT.DESCRIPTION_EN," \
       "PPAPPMODRIGHT.DESCRIPTION_FR," \
       "PPAPPMODRIGHT.DESCRIPTION_HR," \
       "PPAPPMODRIGHT.CATEGORY"

/* ----- From ------ */

#define FROM_PPAPPMODRIGHT \
       "PPAPPMODRIGHT"

#endif   /* PCUA1_PPAPPMODRIGHT_INCLUDED */

