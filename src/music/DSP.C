
#include <dos.h>

#define BASE 0x220
#define RESET 		(BASE+6)
#define READ_DATA 	(BASE+0xA)
#define WRITE_COMMAND	(BASE+0xC)
#define WRITE_DATA	(BASE+0xC)
#define BUFFER_STATUS	(BASE+0xC)
#define DATA_AVAILABLE 	(BASE+0xE)

attendre()
{
int i;
 for (i=0; i<10000; i++);
}

int wait_data ()
{
 int x, i;
 for (i=0; ; i++)
 {
  attendre();
  x = inport (DATA_AVAILABLE);
  if (x & (1 << 7))
   return 1;
  if (i > 10000)
   return 0;
 }
}

int wait_write ()
{
 int x, i;
 for (i=0; ; i++)
 {
  attendre();
  x = inport (BUFFER_STATUS);
  if (x & (1<<7))
   return 1;
  if (i > 10000)
   return 0;
 }
}

int read_data (int *r)
{
int x;
 x = wait_data();
 if (x == 0)
  return 0;
 x = inport (READ_DATA);
 *r = x;
 return 1;
}

int write_data (char x)
{
int y;
 y = wait_write();
 if (y == 0)
  return 0;
 outportb (WRITE_DATA, x);
}


CheckBaseAddress ()
{
int x, i;
 outportb (RESET, 1);
 attendre();
 outportb (RESET, 0);
 for (i=0; ; i++)
 {
  x = wait_data ();
  if (x == 0)
  {
   printf ("Data not available\n");
   break;
  }
  x = inport (READ_DATA);
  /*printf ("%02X\n", x);*/
  if ((x & 0xFF) == 0xAA)
   break;
  if (i > 10000)
  {
   printf ("AA not received\n");
   break;
  }
 }
}

main ()
{
 CheckBaseAddress ();
}