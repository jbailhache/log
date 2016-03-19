
#include <stdio.h>
#include <errno.h>

erreur ()
{
	printf ("Error\n");
}

struct message_header
{
	char s1[11];


};

struct message
{
	int nl;
	char auteur[40];
	char date[40];
	char ref_nom[40];
	char ref_date[40];
	char texte[26][120];

};

convert_dec_teletel (char *dst, char *src)
{
int i, j, l;

	l = strlen (src);
	i = 0;
	j = 0;
	for (;;)
	{
		if (i >= l)
			break;
		if (src[i] == 0x19)
		{
#define cvt(tlt,dec) if (src[i+1] == tlt[0] && src[i+2] == tlt[1]) { dst[j++] = dec; i+=3; } else
			cvt ("Aa", 'à')
			cvt ("Ca", 'â')
			cvt ("Ha", 'ä')
			cvt ("Ae", 'è')
			cvt ("Be", 'é')
			cvt ("Ce", 'ê')
			cvt ("He", 'ë')
			cvt ("Ci", 'î')
			cvt ("Hi", 'ï')
			cvt ("Co", 'ô')
			cvt ("Ho", 'ö')
			cvt ("Cu", 'û')
			cvt ("Hu", 'ü')
			cvt ("Kc", 'ç')

			dst[j++] = src[i++];

		}
		else
			dst[j++] = src[i++];
	}
	dst[j] = 0;
}

convert (char *dst, char *src)
{
	/* strcpy (dst, src); */
	convert_dec_teletel (dst, src);
}

char *nom_jour[] = {"Dim", "Lun", "Mar", "Mer", "Jeu", "Ven", "Sam", "___"};

ident_message (char *ident, char *auteur, char *date, char *ref_date)
{
int i;
char heure[7];
char jour[5];
int nj;
	strcpy (ident, "________.htm");
	for (i=0; i<3 && i<strlen(auteur); i++)
	{       if ((auteur[i] >= 'A' && auteur[i] <= 'Z') ||
		    (auteur[i] >= 'a' && auteur[i] <= 'z'))
		{
			if (auteur[i] >= 'a')
				ident[i] = auteur[i];
			else
				ident[i] = auteur[i] - 'A' + 'a';
		}
		else
			ident[i] = '-';
	}
	if (*date)
	{
		strncpy (jour, date, 3);
		strncpy (heure, date+16, 5);
	}
	else if (*ref_date)
	{
		strncpy (heure, ref_date, 5);
		strncpy (jour, ref_date+7, 3);
	}
	else
		erreur();
	for (nj=0; nj<=7; nj++)
	{
		if (!strncmp(nom_jour[nj], jour, 3))
			break;
	}
	ident[3] = '0' + nj;
	ident[4] = heure[0];
	ident[5] = heure[1];
	if (heure[2] != ':')
		erreur();
	ident[6] = heure[3];
	ident[7] = heure[4];

}

print_message (struct message *message)
{
int i;
	printf ("\nMessage de %s le %s en reponse a %s le %s : \n",
		message->auteur,
		message->date,
		message->ref_nom,
		message->ref_date);
/*
	for (i=0; i<message->nl; i++)
		printf ("\t%s\n", message->texte[i]);
*/
}

int lire (FILE *f, char t, int min, char *s)
{
int c, l;
	l = 0;
	for (;;)
	{
		c = fgetc (f);
		if (feof(f) || c == EOF || (c == t && l >= min))
		{
			s[l] = 0;
			return l;
		}
		else
		{
			s[l++] = c;
		}
	}
}

/* char table[300][2][14]; */
struct item
{
	int n_ident;
	char ident[6][14];
	char ident_ref[14];
	char auteur[20];
	char date[23];
	char ref_nom[20];
	char ref_date[14];
	char debut[80];
};

struct item table[200];

int n_messages;

FILE *out;

char auteur_prec[40];

sommaire (FILE *out, char *capname)
{
char titre[40];
int i;
	sprintf (titre, "Messages de %s", capname);
	fprintf (out, "<HTML><HEAD><TITLE>%s</TITLE></HEAD><BODY>\n<H2>%s</H2>\n<ul>\n",
		titre, titre);
	/* for (i=0; i<n_messages; i++) */
	for (i=n_messages-1; i>=0; i--)
	{
		fprintf (out, "<li><a href=\"%s\">%s le %s</a> <a href=\"%s\">Ref: %s %s</a>\n",
			table[i].ident[0], table[i].auteur, table[i].date,
			table[i].ident_ref, table[i].ref_nom, table[i].ref_date);
		fprintf (out, "<br>%s\n", table[i].debut);
	}
	fprintf (out, "\n</ul>\n</BODY></HTML>\n");
}

fin_fichier (FILE *out)
{
int i, j;
    if (out != NULL)
    {
	fprintf (out, "\n<p>Reponses : <ul>\n");

	/* for (i=0; i<n_messages; i++) */
	for (i=n_messages-1; i>=0; i--)
	for (j=0; j<table[n_messages].n_ident; j++)
	{
		if (!strcmp (table[i].ident_ref, table[n_messages].ident[j]))
		{
			fprintf (out, "<li><a href=\"%s\">%s %s</a><br>%s\n",
				table[i].ident[0], table[i].auteur, table[i].date, table[i].debut);
		}
	}

	fprintf (out, "</ul>");

	/*if (out != NULL)
	{ */
		fprintf (out, "\n</BODY></HTML>\n");
		fclose (out);
	/*}*/
	n_messages++;
    }

}

traiter_message (struct message *m)
{
char titre[120];
int i, j;
	if (m->ref_nom[0] || strcmp(m->auteur, auteur_prec))
	{
		fin_fichier (out);

		table[n_messages].n_ident = 1;
		ident_message (table[n_messages].ident[0],
			m->auteur, m->date, "");
		ident_message (table[n_messages].ident_ref,
			m->ref_nom, "", m->ref_date);

		strcpy (table[n_messages].auteur, m->auteur);
		strcpy (table[n_messages].date, m->date);
		strcpy (table[n_messages].ref_nom, m->ref_nom);
		strcpy (table[n_messages].ref_date, m->ref_date);

		table[n_messages].debut[0] = 0;
		j = 0;
		while (strlen(table[n_messages].debut) < 32)
		{
			strcat (table[n_messages].debut, m->texte[j++]);
			strcat (table[n_messages].debut, " ");

		}

		/* fin_fichier (out); */

		out = fopen (table[n_messages].ident[0], "w");
		if (out == NULL)
		{
			perror (table[n_messages].ident[0]);
		}
		else
		{
			sprintf (titre, "Message de %s le %s, Ref: %s %s.",
				m->auteur, m->date, m->ref_nom, m->ref_date);
			/*
			fprintf (out, "<HTML><HEAD><TITLE>%s</TITLE></HEAD><BODY>\n<H2>%s</H2>\n",
				titre, titre);
			*/
			fprintf (out, "<HTML><HEAD><TITLE>%s</TITLE></HEAD><BODY>\n", titre);
			/*
			for (j=0; j<n_messages; j++)
			{
				if (!strcmp (table[j].ident[0], table[n_messages].ident_ref))
					break;
			}
			*/
			fprintf (out, "<H2>Message de %s le %s</H2><br><a href=\"%s\">Ref: %s %s</a><p>\n",
				m->auteur, m->date, table[n_messages].ident_ref, m->ref_nom, m->ref_date);
				/*table[j].debut); */


			for (i=0; i<m->nl; i++)
			{
				fprintf (out, "%s<br>\n", m->texte[i]);
			}
		}
	}
	else
	{
		if (table[n_messages].n_ident < sizeof(table[n_messages].ident)/sizeof(table[n_messages].ident[0]))
			ident_message (table[n_messages].ident[table[n_messages].n_ident++],
				m->auteur, m->date, "");
		for (i=0; i<m->nl; i++)
		{
			fprintf (out, "%s<br>\n", m->texte[i]);
		}

	}
	strcpy (auteur_prec, m->auteur);

}

main (int argc, char *argv[])
{
FILE *cap;
char buf[1000], buf1[120];
int c;
struct message message;
int l, l0;
int status;

	n_messages = 0;
	out = NULL;
	auteur_prec[0] = 0;

	cap = fopen (argv[1], "rb");
	if (cap == NULL)
	{
		perror (argv[1]);
		return errno;
	}

	/* fread (&header, sizeof(header), 1, cap); */
loop:
	for (;;)
	{
		c = fgetc (cap);

		status = n_messages >= sizeof(table)/sizeof(table[0]);

		if (feof(cap) || c == EOF || status)
		{
			if (status)
				printf ("Overflow\n");
			fin_fichier (out);
			out = fopen ("somness.htm", "w");
			if (out == NULL)
			{
				perror ("somness.htm");
			}
			else
			{
				sommaire (out, argv[1]);
			}
			return;
		}
		if (c == 0x14)
			break;
	}

	fread (buf, 10, 1, cap);
	buf[10] = 0;
	if (strcmp (buf, "\x1F\x40\x62   \x0A\x0C\x0A "))
		goto loop;

	lire (cap, 0x12, 1, message.auteur);
	message.auteur[strlen(message.auteur)-1] = 0;

	/*
	fread (buf, 2, 1, cap);
	if (buf[0] != 0x12)
		printf ("Error\n");
	*/
	c = fgetc (cap);

	lire (cap, ' ', 21, message.date);

/*
	fread (buf, 15, 1, cap);
	buf[15] = 0;
	if (strcmp (buf, "\x1F\x43\x41\x0E\x2C\x12\x67\x1F\x45\x41Ref: "))
		printf ("Error\n");
*/
	fread (buf, 8, 1, cap);
	buf[8] = 0;
	if (strcmp (buf, "\x1F\x43\x41\x0E\x2C\x12\x67\x1F"))
		erreur ();

	c = fgetc (cap);
	if (c == 0x45)
	{
		fread (buf, 6, 1, cap);
		buf[6] = 0;
		if (!strcmp (buf, "ARef: "))
		{
			lire (cap, 0x0F, 1, message.ref_nom);

			c = fgetc (cap);
			if (c != ' ')
				/* printf ("Error\n");*/
				erreur();

			lire (cap, 0x1F, 1, message.ref_date);
			l0 = 0x46;
			l = 0;
		}
		else
		{
			message.ref_nom[0] = 0;
			message.ref_date[0] = 0;

			message.texte[0][0] = 0;
			/*strcpy*/ convert (message.texte[1], buf+1);
			lire (cap, 0x1F, 0, buf);
			convert (buf1, buf);
			strcat (message.texte[1], buf1);

			l0 = 0x44;
			l = 2;

		}
	}
	else
	{
		message.ref_nom[0] = 0;
		message.ref_date[0] = 0;
		l0 = c;
		l = 0;
		goto pabo;
	}

	for (;;)
	{
		c = fgetc (cap);
	pabo:
		if (c < /*0x47*/l0 || c > 0x56)
			break;
		while (c - l0 != l)
		{
			message.texte[l][0] = 0;
			l++;
			if (l >= 16)
				goto suite;
		}
		c = fgetc (cap);
		if (c != 0x41)
			erreur();
		lire (cap, 0x1F, 1, /*message.texte[l++]*/buf);
		convert (message.texte[l++], buf);

	}
suite:
	message.nl = l;
	print_message (&message);
	/* gets (buf); */
	traiter_message (&message);
	goto loop;

	/* lire (cap, 0x14, buf); */

}
