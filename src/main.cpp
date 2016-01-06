#include <stdio.h>
#include "SolaireConfig.h"

int main (int argc, char *argv[])
{
  printf("%s\n %s Version %d.%d\n",
	"PRAISE THE SUN!",
	argv[0],
	Solaire_VERSION_MAJOR,
	Solaire_VERSION_MINOR);

  return 0;
}