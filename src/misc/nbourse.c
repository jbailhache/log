/*

        Analyse des cours de la bourse

        usage :
        bourse cours.txt m[g] a0 b0 a1 b1 c0 d0 c1 d1 [max_inv]
        bourse cours.txt t c0 d0 c1 d1
        bourse cours.txt g[nom] a0 b0 a1 b1 c0 d0 c1 d1

        format de cours.txt
code    nom                     date            ouv     haut    bas     clot    quantite
3112	AIR FRANCE(GROUPE)	21/01/00	19.00	19.03	18.25	18.25	226771

a0, b0 : coef lissage exp cours
a1, b1 : idem derivee
c0, d0 : coef lissage constant / lineaire cours
c1, d1 : idem derivee

*/

#include "stdio.h"

#include "dos.h"

#include <graphics.h>

#define GRAPHMODE HERCMONO

union REGS reg;

char command [200];

int alpha_0, beta_0, alpha_1, beta_1, gamma_0, delta_0, gamma_1, delta_1;

int max_inv;

/* #define TEST */
/* #define LOWLEVEL */

break_debug ()
{
	return;
}

init_graph (int driver)
{
#ifdef LOWLEVEL
	reg.h.ah = 0;
        reg.h.al = driver;
        int86 (0x10, &reg, &reg);
#else
#ifdef TEST
#else
int status;
int mode;
static int done = 0;
	if (done)
		cleardevice();
	/* restorecrtmode(); */
	else
	{
	driver = HERCMONO;
	mode = HERCMONOHI;
	initgraph (&driver, &mode, "");
	status = graphresult ();
	if (status != grOk)
		printf ("Erreur initgraph %d 0x%X\n", status, status);
	}
	done = 1;

#endif
#endif
}

plot (int x, int y, int c)
{
#ifdef LOWLEVEL
	reg.h.ah = 0x0C;
        reg.h.al = c;
        reg.h.bh = 0;
        reg.x.cx = x;
        reg.x.dx = y;
        int86 (0x10, &reg, &reg);

        reg.h.ah = 5;
        reg.h.al = 0;
        int86 (0x10, &reg, &reg);
#else
#ifdef TEST
#else
	if (x < 719 && y < 347)
		putpixel (x, y, c);
#endif
#endif
}

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
        long l, dl, ldl;
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
        /*
        if (feof(f))
        {
                *s = 0;
                return;
        }
        */
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
        /* printf ("."); */
        fflush (stdout);
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

/*
#define alpha 1L
#define beta 2L
*/

#define MAX 40
#define INF 100000000

long mdl;
struct s_record mrecords[MAX];
int nm;

long minv;

long lissage (long x, long l1, long l2, int a, int b, int c, int d)
{
long y;
        y = ( a * x + b * ( c * l1 + d * (2*l1-l2) ) / (c+d) ) / (a+b);
        return y;

}

traiter (struct s_record r[], int n, int p)
{
long l[40], dl[40], ldl[40]; /* lissage */
int i;
long cia, li1, t, ab, dl1, inv;
char buf[30];

        /* traiter1 (r); */
        l[0] = r[0].clot;
        for (i=1; i<n; i++)
        {
                /* r[i].l = l[i] = ( r[i].clot * alpha_0 + l[i-1] * beta_0 ) / ( alpha_0 + beta_0 ); */
                r[i].l = l[i] = lissage ( r[i].clot, l[i-1], (i>1)?l[i-2]:l[i-1],
                        alpha_0, beta_0, gamma_0, delta_0 );

                r[i].dl = dl[i] = l[i] - l[i-1];
                if (i == 1)
                        r[i].ldl = ldl[i] = dl[i];
                else
                        /* r[i].ldl = ldl[i] = ( dl[i] * alpha_1 + ldl[i-1] * beta_1 ) / ( alpha_1 + beta_1 ); */
                        r[i].ldl = ldl[i] = lissage ( dl[i], ldl[i-1], (i>2)?ldl[i-2]:ldl[i-1],
                                alpha_1, beta_1, gamma_1, delta_1 );

                /* printf (" %ld,%ld ", r[i].clot, l[i]); */
                /*
                printf ("\n");
                printf ("ci * alpha = %ld\n", cia = r[i].clot * alpha);
                printf ("li-1 * beta = %ld\n", li1 = l[i-1] * beta);
                printf ("total = %ld\n", t = cia + li1);
                printf ("alpha + beta = %d\n", ab = alpha + beta);
                printf ("rapport : %ld\n", t / ab);
                printf ("\n");
                */
                /* printf ("%ld %ld %ld %ld %ld %ld\n", r[i].clot, l[i-1], r[i-1].l, r[i].l, r[i].dl, r[i].ldl); */
                /* gets (buf); */

        }
                /* gets (buf); */
                /* dl = l[n-1] - l[n-2]; */
                dl1 = ldl[/*n*/ p - 1];
                if (dl1 == 0)
                        inv = INF;
                else
                        inv = l[/*n*/p - 1] / dl1;
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
/*
                        printf ("%s %ld %ld %d %ld %ld\n", r[0].nom, inv, dl1, n, l[p - 1], l[p - 2]);
*/
                        memcpy (&mrecords, r, sizeof(mrecords));
                        nm = n;
                }
                if (max_inv && inv < max_inv /*20*/ && inv > 1 && command[0] == 'm')
                {
                        if (command[1] == 'g')
			{
				init_graph (/*16*/ GRAPHMODE);
                                printf ("%s %ld %ld %d %ld %ld\r", r[0].nom, inv, dl, n, l[n-1], l[n-2]);
                                graph (r);
                                gets (buf);
                        }
                        else
                                printf ("%s %ld %ld %d %ld %ld\n", r[0].nom, inv, dl, n, l[n-1], l[n-2]);

                }
        /* printf ("\n"); */

        if (command[0] == 'g')
        {
            if (command[1] == 0 || !strncmp (command+1, r[0].nom, strlen(command+1)))
            {
		init_graph (/*16*/ GRAPHMODE);
                graph (r);
                gets (buf);
            }
        }

}

#define N 10
#define HAUT 20
#define BAS 150
#define AXE_X 250
#define DIV_D 4
#define XD0 400

graph (struct s_record r[])
{
int i, j;
long max, max_d;
char buf[200];
	if (!strncmp (buf, "CARBONE", 7))
		break_debug();
	/* printf ("%s", r[0].nom); */
	sprintf (buf, "%s", r[0].nom);
#ifdef TEST
#else
#ifdef LOWLEVEL
#else
/*	if (strncmp (buf, "SOPRA", 5)) */
	outtextxy (1, 1, buf);
#endif
#endif
	if (!strcmp (r[0].nom, "CARBONE LORRAINE"))
		restorecrtmode();

	max = 0;
        for (i=0; r[i].status; i++)
        {
                if (r[i].clot > max)
                        max = r[i].clot;
        }
        max_d = 0;
        for (i=1; r[i].status; i++)
        {
                if (r[i].dl > max_d)
                        max_d = r[i].dl;
                if (-r[i].dl > max_d)
                        max_d = -r[i].dl;
        }
        if (max == 0)
                max = 1;
        if (max_d == 0)
                max_d = 1;
        for (i=0; r[i].status; i++)
        {
                plot (i*10, HAUT + BAS - r[i].clot * BAS / max, 15);
                plot (i*10, AXE_X - r[i].dl * AXE_X / max_d / DIV_D, 15);
        }
        for (i=0; r[i+1].status; i++)
        {
                /* plot (i*4, 100-r[i].clot / 100, 1); */
                /* plot (i*10, 800 - r[i].clot, 15); */
                for (j=0; j<=N; j++)
                {
                        plot (i*10+j, HAUT + BAS - (r[i].l * (N-j) + r[i+1].l * j) / N * BAS / max, 7);
                        plot (XD0 + (r[i].dl * (N-j) + r[i+1].dl * j) * AXE_X / N / max_d / DIV_D,
                              HAUT + BAS - (r[i].l * (N-j) + r[i+1].l * j) / N * BAS / max, 7);
                        /*
                        plot (i*10+j, BAS - (r[i].dl * (N-j) + r[i+1].dl * j) / N * BAS / max, 6);
                        plot (i*10+j, BAS - (r[i].ldl * (N-j) + r[i+1].ldl * j) / N * BAS / max, 5);
                        */
                        plot (i*10+j, AXE_X, 6);
                        /* plot (i*10+j, AXE_X - (r[i].dl * (N-j) + r[i+1].dl * j) / N * AXE_X / max_d , 15); */
                        plot (i*10+j, AXE_X - (r[i].ldl * (N-j) + r[i+1].ldl * j) * AXE_X / N / max_d / DIV_D, 7);

                }
                /* printf ("%d\n", r[i].clot); */
                /* printf ("%d %d %d %d\n", r[i].clot, r[i].l, r[i].dl, r[i].ldl);  */
	}
	/*
	if (!strcmp (r[0].nom, "CARBONE LORRAINE"))
		restorecrtmode();
	*/

}

long bourse (int argc, char *argv[],
        int alpha, int beta, int alpha1, int beta1,
        int c0, int d0, int c1, int d1)
{
FILE *f;
/* char line[1000]; */
struct s_line line;
struct s_record record;
char buf[1000];
int code;
struct s_record records[MAX];
int i, j, p;
long x;

        alpha_0 = alpha;
        beta_0 = beta;
        alpha_1 = alpha1;
        beta_1 = beta1;
        gamma_0 = c0;
        delta_0 = d0;
        gamma_1 = c1;
        delta_1 = d1;

        f = fopen (argv[1], "r");
        if (f == NULL)
        {
                perror ("fopen");
                return;
        }
        code = 0;
        mdl = 0;
        minv = INF;
	p = 10;
	i = 0;
        for (;;)
        {
                /* fgets (&line, sizeof(line), f); */
                /* get_line (f, &line); */
                get_record (f, &record);
                /* printf ("%s\n", record.nom); */
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
                                /* if (i >= 10)
                                        traiter (records, n);
                                else
                                        traiter (records, i); */
                                traiter (records, i, p);
                        }
                        code = record.code;
                        i = 0;
                }
                memcpy (records+i, &record, sizeof(struct s_record));
                i++;

        }
        for (j=0; mrecords[j].status; j++);
        j = nm;
        /* printf ("benefice : %ld%%\n", (long) ((mrecords[j-1].l - mrecords[p-1].l) * 100 / mrecords[p-1].l - 100)); */
        /* printf ("%d %ld  %d %ld\n",
                p, mrecords[p-1].l, j, mrecords[j-1].l); */
        x = ((mrecords[j-1].l - mrecords[p-1].l) * 100 / mrecords[p-1].l );
        /* printf ("%d %d\n", x, x>>16); */
        fclose (f);
        return x;

}

#define AB 100
#define AB1 100

#define NA 10
#define NA1 10

main_t (int argc, char *argv[])
{
int i, j, alpha, beta, alpha1, beta1;
long x;
char buf[20];

        gamma_0 = atoi(argv[3]);
        delta_0 = atoi(argv[4]);
        gamma_1 = atoi(argv[5]);
        delta_1 = atoi(argv[6]);

        for (i=0; i<NA; i++)
        {
                alpha = i*10;
                /* alpha = i * 2; */
                beta = AB - alpha;
                printf ("alpha=%d : ", alpha);
                for (j=1; j<NA1; j++)
                {
                        alpha1 = j*10;
                        /* alpha1 = 20 + j * 4; */
                        beta1 = AB1 - alpha1;
                        if (i==0)
                                printf ("%6d", alpha1);
                        else
                        {
                        x = bourse (argc, argv, alpha, beta, alpha1, beta1,
                                gamma_0, delta_0, gamma_1, delta_1);
                        /* printf ("%6ld", x); */
                        printf ("%3ld %-3.3s", x, mrecords[0].nom);
                        }
                }
                printf ("\n");
        }

}

main_m (int argc, char *argv[])
{
long x;
char buf[20];
        /* x = bourse (argc, argv, 1, 9, 3, 7); */
        /* init_graph (16); */
        x = bourse (argc, argv,
                atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]),
                atoi(argv[7]), atoi(argv[8]), atoi(argv[9]), atoi(argv[10]));

        printf ("%ld\n", x);

        printf ("%s %d\n", mrecords[0].nom, minv);

        printf ("Graph...\n");
        gets (buf);
	init_graph (/*16*/ GRAPHMODE);
        /* for (i=0; i<100; i++)
                plot (i, i, i); */
        graph (mrecords);
}

main1 (int argc, char *argv[])
{
        strcpy (command, argv[2]);
        if (argc > 11)
                max_inv = atoi (argv[11]);
        else
                max_inv = 0;
        switch (command[0])
        {
                case 'm':
                        main_m (argc, argv);
                        break;
                case 't':
                        main_t (argc, argv);
                        break;
                case 'g':
                        main_m (argc, argv);
                        break;

        }
        return 0;
}

typedef long (*traitement) (/* struct s_record r[], int n, int p, void *param */);

int ntraiter1 (struct s_record r[], int n, int p, void *param)
{
int i;
char buf[10];
	printf ("----------------\n");
	for (i=0; i<n; i++)
	{
		print_record (r+i);
		gets (buf);
	}
}

struct param_ntraiter
{
	long capital;
	long divi;
	long frais;
};

int op (long a, long b, long c, struct param_ntraiter *param)
{
	if (/* ((a - b) > (b / 1000)) && */
	    ((c - b) > (b / param->divi /*1000*/)))
		return 1;
	if (/* ((b - a) > (b / 1000)) && */
	    ((b - c) > (b / param->divi)))
		return -1;

	return 0;
}

long ntraiter (struct s_record r[], int n, int p, struct param_ntraiter *param)
{
int i;
long val;
int nt;
int d;
int o;
long frais;
long solde;
	solde = param->capital; /*100000;*/
	nt = 0;
	val = 0;
	for (i=2; i<n-1; i++)
	{
		o = op (r[i-2].clot, r[i-1].clot, r[i].clot, param);
		switch (o)
		{
			case 0 :
				d = 0;
				break;
			case 1 :
				/* if (nt == 0)
					d = 100000 / r[i].clot;
				else
					d = 0; */
				d = solde / r[i].clot;
				break;
			case -1 :
				d = -nt;
				break;
		}
		nt += d;
		frais = param->frais; /*400;*/
		solde -= d * r[i+1].ouv + frais;
	}
	solde += nt * r[n-1].clot;
	/* printf ("%ld\n", solde); */
	return solde;
}


long traiter_fichier (char *filename, traitement ptraiter, void *param)
{
struct s_record record;
int code;
struct s_record records[MAX];
int p;
FILE *f;
int i;
long solde, total, nombre, moyenne;

	f = fopen (filename, "r");
	if (f == NULL)
	{
		perror (filename);
		return;
	}

	code = 0;
	p = 10;
	i = 0;
	total = 0;
	nombre = 0;
	for (;;)
	{
		get_record (f, &record);
		if (feof(f))
			break;
		if (record.code != code)
		{
			if (code)
                        {
                                records[i].status = 0;
				solde = (*ptraiter) (records, i, p, param);
				total += solde;
				nombre++;
				printf ("%d\r", nombre);
                        }
                        code = record.code;
                        i = 0;
                }
                memcpy (records+i, &record, sizeof(struct s_record));
                i++;
	}
	moyenne = total / nombre;
	/* printf ("Moyenne : %ld\n", moyenne); */
	return moyenne;
	fclose (f);

}

main (int argc, char *argv[])
{
long capital;
struct param_ntraiter param[1];
long moyenne;
float taux;

	param->frais = 0;

	if (argc > 2)
	{
		param->capital = atol (argv[2]);
		moyenne = traiter_fichier (argv[1], ntraiter, param);
		taux = ((float)moyenne) / ((float)capital);
		printf (" %8ld -> %8ld\t%f \n", capital, moyenne, taux);

	}
	/*
	for (capital=600000; capital<1000000; capital+=100000)
	{
		param->capital = capital;
		moyenne = traiter_fichier (argv[1], ntraiter, param);
		taux = ((float)moyenne) / ((float)capital);
		printf (" %8ld -> %8ld\t%f \n", capital, moyenne, taux);
	}
	*/
	param->capital = 100000;
	for (param->divi=100; param->divi<10000; param->divi*=2)
	{
		moyenne = traiter_fichier (argv[1], ntraiter, param);
		taux = ((float)moyenne) / ((float)(param->capital));
		printf (" %6ld : %8ld -> %8ld\t%f \n",
			param->divi, param->capital, moyenne, taux);
	}


}



