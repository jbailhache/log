#include "stdio.h"
#include "dos.h"
 
union REGS reg;
struct SREGS sreg;

int cylsect (int cyl, int sect)
{
int sect1;
        sect1 = ((cyl & 0xFF) << 8) | ((cyl & 0x300) >> 2) | sect;
        return sect1;
}

int readdisk (char *s, int drive, int head, int sect, int ns)
{
    reg.h.ah = 2;
    reg.h.al = ns;
    reg.x.cx = sect;
    reg.h.dh = head;
    reg.h.dl = drive;
    sreg.es = _DS;
    reg.x.bx = s;
    int86x (0x13, &reg, &reg, &sreg);
/*
    printf ("Flags = 0x%04X, status = 0x%02X, %d sectors read\n",
        reg.x.flags, reg.h.ah, reg.h.al);
*/
    return reg.x.ax;
}

#define NL 16
#define NC 16

dump (unsigned char *s, int n)
{
int p, l, c;
unsigned char ch;
char b[10];
    for (p=0; ; p++)
    {
        for (l=0; l<NL; l++)
        {
                if ((p*NL+l)*NC+NC-1 >= n)
                    return;
                printf (" %04X: ", (p*NL+l)*NC);
                for (c=0; c<NC; c++)
                    printf ("%02X ", s[(p*NL+l)*NC+c]);
                for (c=0; c<NC; c++)
                {
                    ch = s[(p*NL+l)*NC+c];
                    if (ch >= 32 && ch <= 126)
                        printf ("%c", ch);
                    else
                        printf (".");
                }
                printf ("\n");
        }
        gets (b);
    }
}

main_readdisk (int argc, char *argv[])
{
int drive, head, sect;
char buf [0x200];
int i;
    /* dump ("This is a test of memory dump. xkjv sdh fgsrkjfserkh shfguh ", 48); */
    sscanf (argv[1], "%x", &drive);
    sscanf (argv[2], "%x", &head);
    sscanf (argv[3], "%x", &sect);
    for (i=0; i<0x200; i++)
        buf[i] = 0;
    readdisk (buf, drive, head, sect, 1);
    dump (buf, 0x200);
}

#define NS 17

main (int argc, char *argv[])
{
int drive, nhead, ncyl, nsect, nsects, head, cyl, sect, sects, icyl, isect,
        sect1, ncyls;
char buf[0x200 * NS], buf1[60];
char filename[200];
FILE *out;
int status;

     printf ("Disk characteristics : \n");
/*
     printf ("\tNumber of heads ? ");
     gets (buf1);
     sscanf (buf1, "%d", &nhead);
*/
/*
     printf ("\tNumber of cylinders ? ");
     gets (buf1);
     sscanf (buf1, "%d", &ncyl);

     printf ("\tNumber of sectors ? ");
     gets (buf1);
     sscanf (buf1, "%d", &nsect);
*/
/*
     ncyl = 782;
     nsect = 63;
*/
     ncyl = 977;
     nsect = 17;

     printf ("Hexa %X %X\n", /*nhead,*/ ncyl, nsect);

     /*
     printf ("Drive ? 0x");
     gets (buf1);
     sscanf (buf1, "%x", &drive);
     */
     drive = 0x80;

     printf ("Head ? 0x");
     gets (buf1);
     sscanf (buf1, "%x", &head);

     printf ("Initial cylinder ? 0x");
     gets (buf1);
     sscanf (buf1, "%x", &icyl);
/*
     printf ("Initial sector ? 0x");
     gets (buf1);
     sscanf (buf1, "%x", &isect);

     printf ("Number of sectors to save ? 0x");
     gets (buf1);
     sscanf (buf1, "%x", &nsects);
*/
     printf ("Number of cylinders to save ? 0x");
     gets (buf1);
     sscanf (buf1, "%x", &ncyls);
     nsects = ncyls * nsect;
     isect = 1;

     printf ("Filename for saving ? ");
     gets (filename);
     out = fopen (filename, "wb");
     if (out == NULL)
     {
         perror (filename);
         return;
     }

     cyl = icyl;
     sect = isect;
     sects = 0;
     for (;;)
     {
        /* printf ("Saving cyl 0x%X sect 0x%X\n", cyl, sect);  */
        /* sect1 = ((cyl & 0xFF) << 8) Ý ((cyl & 0x300) >> 2) Ý sect; */
        sect1 = cylsect (cyl, sect);
        status = readdisk (buf, drive, head, sect1, NS);
        if (status & 0xFF00)
        {
            printf ("Error 0x%04X cyl 0x%X sect 0x%X\n",
                status, cyl, sect);
        }
        fwrite (buf, sizeof(buf), 1, out);
        sects += NS;
        if (sects >= nsects)
            break;
        if (sect+NS <= nsect)                             
            sect += NS;
        else
        {
            sect = 1;
            cyl++;
            if (cyl >= ncyl)
                break;
        }
     }
     printf ("%d sectors saved, last: cyl 0x%X sect 0x%X\n",
        sects, cyl, sect);
}





