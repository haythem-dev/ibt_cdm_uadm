#ifndef VTCODUM_H_INCLUDED
#define VTCODUM_H_INCLUDED

/* isExtend 0: CheckAbdruckbar() allows only 0x20 < *cp < 0x7F
            1: CheckAbdruckbar() allows      0x20 < *cp
               (e.g cyrillic characters are possible)
   default: 0
*/
void setExtendedCharSet(int isExtended);
                              
void CheckAbdruckbar( unsigned char *cp );
void CheckAbdruckbarZahl( unsigned char *cp );

void StoEbcd( char *str, int lng );
unsigned char ToAsci( unsigned char ze );

#endif /* VTCODUM_H_INCLUDED   */
