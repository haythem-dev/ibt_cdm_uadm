#ifndef POS1G_PPLOGPARA_INCLUDED
#define POS1G_PPLOGPARA_INCLUDED

/* ----- Feldlaengen ------ */

#define L_PPLOGPARA_PARA1         30
#define L_PPLOGPARA_PARA2         30
#define L_PPLOGPARA_PARABEZ       30
#define L_PPLOGPARA_PARAVALUE     254

/* ----- Feldpositionen ------ */

#define P_PPLOGPARA_APPLICATIONID 0
#define P_PPLOGPARA_PARA1         1
#define P_PPLOGPARA_PARA2         2
#define P_PPLOGPARA_PARABEZ       3
#define P_PPLOGPARA_PARAVALUE     4

#define POSANZ_PPLOGPARA          5

/* ----- Struktur ------ */

struct PPLOGPARA  {
       short   APPLICATIONID ;
       char    PARA1         [31];
       char    PARA2         [31];
       char    PARABEZ       [31];
       char    PARAVALUE     [255];
};

/* ----- Structur Beschreibung ------ */

#define STRUCDESC_PPLOGPARA \
       "sc30c30c30c254"

/* ----- Laenge des Uebertragungspuffers ------ */

#define BUFLEN_PPLOGPARA  355

/* ----- Projektion ------ */

#define PROJECTION_PPLOGPARA \
       "APPLICATIONID," \
       "PARA1," \
       "PARA2," \
       "PARABEZ," \
       "PARAVALUE"

/* ----- From ------ */

#define FROM_PPLOGPARA \
       "PPLOGPARA"

#endif   /* POS1G_PPLOGPARA_INCLUDED */

