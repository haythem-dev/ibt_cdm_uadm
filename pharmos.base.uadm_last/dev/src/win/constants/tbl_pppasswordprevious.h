#ifndef ZPCUA1_PPPASSWORDPREVIOUS_INCLUDED
#define ZPCUA1_PPPASSWORDPREVIOUS_INCLUDED

/* ----- Feldlaengen ------ */

#define L_PPPASSWORDPREVIOUS_PASSWORD      8
#define L_PPPASSWORDPREVIOUS_PWD_ENC       254

/* ----- Feldpositionen ------ */

#define P_PPPASSWORDPREVIOUS_USERID        0
#define P_PPPASSWORDPREVIOUS_PASSWORD      1
#define P_PPPASSWORDPREVIOUS_DATEVALIDFROM 2
#define P_PPPASSWORDPREVIOUS_PWD_ENC       3

#define POSANZ_PPPASSWORDPREVIOUS          4

/* ----- Struktur ------ */

struct PPPASSWORDPREVIOUS  {
       long    USERID        ;
       char    PASSWORD      [9];
       long    DATEVALIDFROM ;
       char    PWD_ENC       [255];
};

/* ----- Structur Beschreibung ------ */

#define STRUCDESC_PPPASSWORDPREVIOUS \
       "lc8lc254"

/* ----- Laenge des Uebertragungspuffers ------ */

#define BUFLEN_PPPASSWORDPREVIOUS  288

/* ----- Projektion ------ */

#define PROJECTION_PPPASSWORDPREVIOUS \
       "USERID," \
       "PASSWORD," \
       "DATEVALIDFROM," \
       "PWD_ENC"

/* ----- Projektion fuer Joins ------ */

#define JOIN_PROJECTION_PPPASSWORDPREVIOUS \
       "PPPASSWORDPREVIOUS.USERID," \
       "PPPASSWORDPREVIOUS.PASSWORD," \
       "PPPASSWORDPREVIOUS.DATEVALIDFROM," \
       "PPPASSWORDPREVIOUS.PWD_ENC"

/* ----- From ------ */

#define FROM_PPPASSWORDPREVIOUS \
       "PPPASSWORDPREVIOUS"

#endif   /* ZPCUA1_PPPASSWORDPREVIOUS_INCLUDED */

