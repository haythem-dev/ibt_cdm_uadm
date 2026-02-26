#ifndef POS1G_PPCORAPPLANG_INCLUDED
#define POS1G_PPCORAPPLANG_INCLUDED

/* ----- Feldlaengen ------ */

#define L_PPCORAPPLANG_LANGUAGEID    2

/* ----- Feldpositionen ------ */

#define P_PPCORAPPLANG_LANGUAGEID    0
#define P_PPCORAPPLANG_APPLICATIONID 1

#define POSANZ_PPCORAPPLANG          2

/* ----- Struktur ------ */

struct PPCORAPPLANG  {
       char    LANGUAGEID    [3];
       short   APPLICATIONID ;
};

/* ----- Structur Beschreibung ------ */

#define STRUCDESC_PPCORAPPLANG \
       "c2s"

/* ----- Laenge des Uebertragungspuffers ------ */

#define BUFLEN_PPCORAPPLANG  10

/* ----- Projektion ------ */

#define PROJECTION_PPCORAPPLANG \
       "LANGUAGEID," \
       "APPLICATIONID"

/* ----- From ------ */

#define FROM_PPCORAPPLANG \
       "PPCORAPPLANG"

#endif   /* POS1G_PPCORAPPLANG_INCLUDED */

