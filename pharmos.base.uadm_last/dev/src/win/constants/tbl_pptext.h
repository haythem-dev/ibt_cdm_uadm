#ifndef POS1G_PPTEXT_INCLUDED
#define POS1G_PPTEXT_INCLUDED

/* ----- Feldlaengen ------ */

#define L_PPTEXT_REFTEXT       254
#define L_PPTEXT_LANGUAGEID    2
#define L_PPTEXT_TEXT          240

/* ----- Feldpositionen ------ */

#define P_PPTEXT_REFTEXT       0
#define P_PPTEXT_APPLICATIONID 1
#define P_PPTEXT_MODULEID      2
#define P_PPTEXT_LANGUAGEID    3
#define P_PPTEXT_TEXT          4

#define POSANZ_PPTEXT          5

/* ----- Struktur ------ */

struct PPTEXT  {
       char    REFTEXT       [255];
       short   APPLICATIONID ;
       short   MODULEID      ;
       char    LANGUAGEID    [3];
       char    TEXT          [241];
};

/* ----- Structur Beschreibung ------ */

#define STRUCDESC_PPTEXT \
       "c254ssc2c240"

/* ----- Laenge des Uebertragungspuffers ------ */

#define BUFLEN_PPTEXT  513

/* ----- Projektion ------ */

#define PROJECTION_PPTEXT \
       "REFTEXT," \
       "APPLICATIONID," \
       "MODULEID," \
       "LANGUAGEID," \
       "TEXT"

/* ----- From ------ */

#define FROM_PPTEXT \
       "PPTEXT"

#endif   /* POS1G_PPTEXT_INCLUDED */

