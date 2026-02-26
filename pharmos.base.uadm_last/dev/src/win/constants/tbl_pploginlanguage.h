#ifndef POS1G_PPLOGINLANGUAGE_INCLUDED
#define POS1G_PPLOGINLANGUAGE_INCLUDED

/* ----- Feldlaengen ------ */

#define L_PPLOGINLANGUAGE_LANGUAGEID 2
#define L_PPLOGINLANGUAGE_NAME       30

/* ----- Feldpositionen ------ */

#define P_PPLOGINLANGUAGE_LANGUAGEID 0
#define P_PPLOGINLANGUAGE_NAME       1

#define POSANZ_PPLOGINLANGUAGE       2

/* ----- Struktur ------ */

struct PPLOGINLANGUAGE  {
       char    LANGUAGEID [3];
       char    NAME       [31];
};

/* ----- Structur Beschreibung ------ */

#define STRUCDESC_PPLOGINLANGUAGE \
       "c2c30"

/* ----- Laenge des Uebertragungspuffers ------ */

#define BUFLEN_PPLOGINLANGUAGE  34

/* ----- Projektion ------ */

#define PROJECTION_PPLOGINLANGUAGE \
       "LANGUAGEID," \
       "NAME"

/* ----- From ------ */

#define FROM_PPLOGINLANGUAGE \
       "PPLOGINLANGUAGE"

#endif   /* POS1G_PPLOGINLANGUAGE_INCLUDED */

