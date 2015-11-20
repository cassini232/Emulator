#include <stdio.h>
#include <stdint.h>

int main()
{
  uint16_t uint;
  long int lint;
  int iint;
  char ichar;
  unsigned char uchar;
  unsigned short ushort;

  printf("uint=%d lint=%d int=%d ichar=%d uchar=%d ushort=%d\n",
	sizeof(uint),
	sizeof(lint),
	sizeof(iint),
	sizeof(ichar),
	sizeof(uchar),
	sizeof(ushort)
	);

  return(0);
}
