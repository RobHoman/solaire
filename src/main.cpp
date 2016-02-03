#include <stdio.h>
#include "SolaireConfig.h"

int main (int argc, char *argv[])
{
  printf("SOLAIRE Version %d.%d\n %s\n Args: %s\n",
         Solaire_VERSION_MAJOR,
         Solaire_VERSION_MINOR,
         "PRAISE THE SUN!",
         argv[0]);

  return 0;
}
