#include "stdio.h"

struct s_line
{
        char code[8];
        char nom[24];
        char date[8];
        char ouv[13];
        char haut[8];
        char bas[8];
        char clot[8];
        char nombre[9];
};

struct s_record
{
        int status;
        long valeur;
        long code;
        char nom[30];
        short jour, mois, annee;
        long ouv, haut, bas, clot;
        long nombre;
        
}

print_line (struct s_line *p)
{
        printf ("Code : %8.8s\n", p->code);
        printf ("Nom  : %24.24s\n", p->nom);
        printf ("Date : %8.8s\n", p->date);
        printf ("Ouverture : %8.8s\n", p->ouv);
        printf ("Haut      : %8.8s\n", p->haut);
        printf ("Bas       : %8.8s\n", p->bas);
        printf ("Cloture   : %8.8s\n", p->clot);
        printf ("Nombre : %9.9s\n", p->nombre);
}

get_item (FILE *f, char *s)
{
char c;
int i;
        i = 0;
        for (;;)
        {
                c = fgetc (f);
                if (feof(f))
                        break;
                if (c == '\t' || c == '\n')
                        break;
                s[i++] = c;
        }
        s[i] = 0;

}

get_line (FILE *f, struct s_line *p)
{
        get_item (f, p->code);
        get_item (f, p->nom);
        get_item (f, p->date);
        get_item (f, p->ouv);
        get_item (f, p->haut);
        get_item (f, p->bas);
        get_item (f, p->clot);
        get_item (f, p->nombre);
}

long scan_val (char *buf)
{
int i;
long ent;
short cent;
        i = strlen(buf) - 3;
        buf[i] = 0;
        sscanf (buf, "%ld", &ent);
        sscanf (buf+i+1, "%d", &cent);
        return ent * 100 + cent;
}

get_record (FILE *f, struct s_record *p)
{
char buf[200];
        p->status = 1;
        p->valeur = 0;

        get_item (f, buf); sscanf (buf, "%ld", &(p->code));
        get_item (f, p->nom);
        get_item (f, buf);
        p->jour = 10*(buf[0]-'0')+buf[1]-'0';
        p->mois = 10*(buf[3]-'0')+buf[4]-'0';
        p->annee = 10*(buf[6]-'0')+buf[7]-'0';
        /*
        get_item (f, buf); sscanf (buf, "%f", &(p->ouv));
        get_item (f, buf); sscanf (buf, "%f", &(p->haut));
        get_item (f, buf); sscanf (buf, "%f", &(p->bas));
        get_item (f, buf); sscanf (buf, "%f", &(p->clot));
        */
        get_item (f, buf); p->ouv = scan_val (buf);
        get_item (f, buf); p->haut = scan_val (buf);
        get_item (f, buf); p->bas = scan_val (buf);
        get_item (f, buf); p->clot = scan_val (buf);
 
        get_item (f,buf); sscanf (buf, "%ld", &(p->nombre));

}

print_record (struct s_record *p)
{
        printf ("Code : %ld\n", p->code);
        printf ("Nom  : %s\n", p->nom);
        printf ("Date : %02d/%02d/%02d\n", p->jour, p->mois, p->annee);
        printf ("Ouverture : %ld.%02d\n", p->ouv/100, p->ouv%100);
        printf ("Haut      : %ld.%02d\n", p->haut/100, p->haut%100);
        printf ("Bas       : %ld.%02d\n", p->bas/100, p->bas%100);
        printf ("Cloture   : %ld.%02d\n", p->clot/100, p->clot%100);
        printf ("Nombre : %ld\n", p->nombre);

}

traiter1 (struct s_record records[])
{
int i;
        printf (" %s : ", records[0].nom);
        i = 0;
        for (;;)
        {
                if (records[i].status == 0)
                        break;
                printf (" %d ", records[i].ouv);
                i++;
        }
        printf ("\n");
}

#define alpha 1L
#define beta 2L

#define MAX 40
#define INF 100000000

long mdl;
struct s_record mrecords[MAX];

long minv;

traiter (struct s_record r[], int n)
{
long l[40], dl[40], ldl[40]; /* lissage */
int i;
long cia, li1, t, ab, dl1, inv;
char buf[30];

        /* traiter1 (r); */
        l[0] = r[0].clot;
        for (i=1; i<n; i++)
        {
                l[i] = ( r[i].clot * alpha + l[i-1] * beta ) / ( alpha + beta );
                dl[i] = l[i] - l[i-1];
                if (i == 1)
                        ldl[i] = dl[i];
                else
                        ldl[i] = ( dl[i] * alpha + dl[i-1] * beta ) / ( alpha + beta );

                /* printf (" %ld,%ld ", r[i].clot, l[i]); */
                /*
                printf ("\n");
                printf ("ci * alpha = %ld\n", cia = r[i].clot * alpha);
                printf ("li-1 * beta = %ld\n", li1 = l[i-1] * beta);
                printf ("total = %ld\n", t = cia + li1);
                printf ("alpha + beta = %d\n", ab = alpha + beta);
                printf ("rapport : %ld\n", t / ab);
                printf ("\n");
                gets (buf);
                */
        }
                /* dl = l[n-1] - l[n-2]; */
                dl1 = ldl[n-1];            
                if (dl1 == 0)
                        inv = INF;
                else
                        inv = l[n-1] / dl1;
                /* printf ("%s %ld %ld %ld %ld\n", r[0].nom, l[n-1], l[n-2], dl, mdl); */
                /*if (dl > mdl && dl < 300000)
                {
                        mdl = dl;
                        printf ("%s %ld %d %ld %ld\n", r[0].nom, dl, n, l[n-1], l[n-2]);
                        memcpy (&mrecords, r, sizeof(mrecords));                        
                }*/
                if ((n > 1) && (inv > 0) && (inv < minv))
                {
                        minv = inv;
                        mdl = dl1;
                        printf ("%s %ld %ld %d %ld %ld\n", r[0].nom, inv, dl1, n, l[n-1], l[n-2]);
                        memcpy (&mrecords, r, sizeof(mrecords));                        

                }
                if (inv < 20 && inv > 1)
                {
                        printf ("%s %ld %ld %d %ld %ld\n", r[0].nom, inv, dl, n, l[n-1], l[n-2]);
                }
        /* printf ("\n"); */
}

main (int argc, char *argv[])
{
FILE *f;
/* char line[1000]; */
struct s_line line;
struct s_record record;
char buf[1000];
int code;
struct s_record records[MAX];
int i;

        f = fopen (argv[1], "r");
        if (f == NULL)
        {
                perror ("fopen");
                return;
        }
        code = 0;
        mdl = 0;
        minv = INF;
        for (;;)
        {
                /* fgets (&line, sizeof(line), f); */
                /* get_line (f, &line); */
                get_record (f, &record);
                if (feof(f))
                        break;
                /* print_line (&line); */
                /* print_record (&record); */
                /* gets (buf); */
                if (record.code != code)
                {
                        if (code)
                        {
                                records[i].status = 0;
                                traiter (records, i);
                        }
                        code = record.code;
                        i = 0;
                }
                memcpy (records+i, &record, sizeof(struct s_record));
                i++;

        }
}


