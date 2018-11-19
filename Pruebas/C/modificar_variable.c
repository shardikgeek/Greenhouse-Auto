/* strtok example */
#include <stdio.h>
#include <stdlib.h>|
#include <string.h>

int main ()
{
  char str[] ="123,42,12,4.";
  char * pch;
  int numero[4];
  int i = 0;
  int j;

	pch = strtok (str," ,.-"); // Divide la cadena hasta el primer terminador.

  while (pch != NULL)
  {
    numero[i] = atoi(pch);
    i++;
    pch = strtok (NULL, " ,.-");
  }

  for(j=0;j<4;j++){
 	 printf("El numero %d es: %d.\n",j,numero[j]);
}
  return 0;
}