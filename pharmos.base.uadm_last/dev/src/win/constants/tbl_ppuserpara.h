#ifndef PCUA1_PPUSERPARA_INCLUDED
#define PCUA1_PPUSERPARA_INCLUDED

/* ----- Feldlaengen ------ */

#define L_PPUSERPARA_NAME    100
#define L_PPUSERPARA_DESC    254

/* ----- Feldpositionen ------ */

#define P_PPUSERPARA_ID      0
#define P_PPUSERPARA_NAME    1
#define P_PPUSERPARA_DESC    2

#define POSANZ_PPUSERPARA    3

/* ----- Struktur ------ */

struct PPUSERPARA
{
    short    USERPARAID;
    char     NAME[101];
    char     DESC[255];
};

/* ----- Structur Beschreibung ------ */

#define STRUCDESC_PPUSERPARA \
       "sc100c254"

/* ----- Laenge des Uebertragungspuffers ------ */

#define BUFLEN_PPUSERPARA  362

/* ----- Projektion ------ */

#define PROJECTION_PPUSERPARA \
       "USERPARAID," \
       "NAME," \
       "DESCRIPTION"

/* ----- From ------ */

#define FROM_PPUSERPARA \
       "PPUSERPARA"

#endif   /* PCUA1_PPUSERPARA_INCLUDED */

