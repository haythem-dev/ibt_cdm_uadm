#ifndef POS1G_PPAPPRIGHT_INCLUDED
#define POS1G_PPAPPRIGHT_INCLUDED

/* ----- Feldlaengen ------ */

#define L_PPAPPRIGHT_RIGHTNAME      20

/* ----- Feldpositionen ------ */

#define P_PPAPPRIGHT_APPLICATIONID  0
#define P_PPAPPRIGHT_MODULEID       1
#define P_PPAPPRIGHT_USERID         2
#define P_PPAPPRIGHT_GROUPID        3
#define P_PPAPPRIGHT_RIGHTNAME      4
#define P_PPAPPRIGHT_BRANCHREGIONID 5

#define POSANZ_PPAPPRIGHT           6

/* ----- Struktur ------ */

struct PPAPPRIGHT  {
       short   APPLICATIONID  ;
       short   MODULEID       ;
       short   USERID         ;
       short   GROUPID        ;
       char    RIGHTNAME      [21];
       long    BRANCHREGIONID ;
};

/* ----- Structur Beschreibung ------ */

#define STRUCDESC_PPAPPRIGHT \
       "ssssc20l"

/* ----- Laenge des Uebertragungspuffers ------ */

#define BUFLEN_PPAPPRIGHT  61

/* ----- Projektion ------ */

#define PROJECTION_PPAPPRIGHT \
       "APPLICATIONID," \
       "MODULEID," \
       "USERID," \
       "GROUPID," \
       "RIGHTNAME," \
       "BRANCHREGIONID"

/* ----- From ------ */

#define FROM_PPAPPRIGHT \
       "PPAPPRIGHT"

#endif   /* POS1G_PPAPPRIGHT_INCLUDED */

