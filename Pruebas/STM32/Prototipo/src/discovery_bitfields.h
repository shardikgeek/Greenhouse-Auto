/*  Libreria bitfields
Manejador de bitfields: es una forma de utiizar cada bit de un char.
*/

typedef struct{
    unsigned char menu : 1;
    unsigned char lectura_dht : 1;
    unsigned char lectura_humedad : 1;
    unsigned char lectura_temp : 1;
    unsigned char lectura_temp2 : 1;
    unsigned char motor : 1;
    unsigned char tanque : 1;
    unsigned char tanque2 : 1;
}flags;

/* Modelo de bitfield
Se va creando por bloques de  bytes.
Por ejemplo: si se ponen 4 unsigend char, la estructura pesara 1 byte.
Si se ponen 7 chars tambien pesara 1 byte. En cambio si se agregan 14 chars
la estructura pesara 2 bytes.

struct nombre_bitfield{
  unsigned char bit0 : 1;
  unsigned char bit1 : 1;
  unsigned char bit2 : 1;
  unsigned char bit3 : 1;
  unsigned char bit4 : 1;
  unsigned char bit5 : 1;
  unsigned char bit6 : 1;
  unsigned char bit7 : 1;
};
*/
