
#define VAX_C

#include <stdio.h>
#include <time.h>

#define N 5000

int puberte;
int menopause;
int temps;

struct individu
{
    int naissance;
    int genome[2];
    int parent[2];
    int partenaire;
};

struct individu individus[2][N];
/*males [N], femelles[N];*/
int population[2];

int random (int n)
{
long r;
    r = rand();
    r = r & 0x7FFF;
    r = (r * (long)n) / 0x7FFF;
    return (int) r;
}

int ii;

int meme_parent (int j[2], int sexe)
{
int sex;
    if (ii == 0)
	return 0;
    for (sex=0; sex<2; sex++)
    {
	if (individus[sex][j[sex]].parent[sexe] == -1)
	    return 0;
    }
    return individus[0][j[0]].parent[sexe] ==
	   individus[1][j[1]].parent[sexe];
}

int freres (int j[2])
{
int sexe;
    for (sexe=0; sexe<2; sexe++)
    {
	if (meme_parent (j, sexe))
	    return 1; 
    }
    return 0;
}

main ()
{
int duree;
int sexe, i, g;
int j[2];
int k;
int age;
int fidelite;
int n;
int sex;
#ifdef VAX_C
char seed[8];
#else
time_t seed;
#endif
int n_exp;
int total[2][2];
int n_essais;
int pop_init[2];

 ii = 1;

 total[0][0] = total[0][1] = total[1][0] = total[1][1] = 0;
 for (n_exp = 0; n_exp < 50; n_exp++)
 {
/*
    duree = 1000;
    puberte = 10;
    menopause = 200;
*/
#ifdef VAX_C
  SYS$GETTIM (seed);
  srand (* (int *) seed + 4);
#else
  time (&seed);
  srand ((int)seed);
#endif

  duree = /*150 + random(50);*/ 500 + random (1500);
  puberte = 20 + random (30);
  menopause = 60 + random (300);
  for (sexe=0; sexe<2; sexe++)
    pop_init[sexe] = 50 + random(40);
  
  printf ("\nExperience %d: duree %d, puberte %d, menopause %d, population %d %d\n",
    n_exp, duree, puberte, menopause, pop_init[0], pop_init[1]);

  for (ii=0; ii<2; ii++)
  for (fidelite=0; fidelite<2; fidelite++)
  {
    printf ("fidelite %d\n", fidelite);

    temps = 0;

    for (sexe=0; sexe<2; sexe++)
    {
	population[sexe] = pop_init[sexe];  
	    /*50 + random(40); *//*2000 + random(1500);  60 */
	/*printf ("Population %d: %d\n", sexe, population[sexe]);*/
#ifdef VAX_C
  SYS$GETTIM (seed);
  srand (* (int *) seed + 4);
#else
	time (&seed);
	srand ((int)seed);
#endif
	for (i=0; i<population[sexe]; i++)
	{
	    individus[sexe][i].naissance = temps - puberte - 1;
	    for (g=0; g<2; g++)
		individus[sexe][i].genome[g] = (random (4) == 0 ? 1 : 0);
		    /*(rand() & 3) == 3 ? 1 : 0;*/
            individus[sexe][i].parent[0] = individus[sexe][i].parent[1] = -1;
	    individus[sexe][i].partenaire = -1;
	}
    }

    for (temps=0; temps<duree; temps++)
    {
	/*
	if ((temps % 10) == 0)
		printf ("\r temps %d ", temps);
	*/
	fflush (stdout);
        if (!fidelite)
         do
	  for (sexe=0; sexe<2; sexe++)
	    do
	    {	
		j[sexe] = random (population[sexe]);
		age = temps - individus[sexe][j[sexe]].naissance;
	    }
	    while (age < puberte || age > menopause);
         while (freres (j));
	else
	{
	other:
	    do
	    {
		j[0] = random (population[0]);
		age = temps - individus[0][j[0]].naissance;
	    }
	    while (age < puberte || age > menopause);
	    if (individus[0][j[0]].partenaire == -1)
	    {
		do
		{
		    j[1] = random (population[1]);
		    age = temps - individus[1][j[1]].naissance;
		}
		while ((age < puberte || age > menopause)/* ||
			individus[1][j[1]].partenaire != -1*/);
		if (individus[1][j[1]].partenaire != -1 || freres (j))
		    goto other;
		individus[0][j[0]].partenaire = j[1];
		individus[1][j[1]].partenaire = j[0];
	    }
	    else
	    {
		j[1] = individus[0][j[0]].partenaire;
		age = temps - individus[0][j[0]].naissance;
		if (age < puberte || age > menopause)
		   goto other;
            } 
	}

	sexe = random (2);
	individus[sexe][population[sexe]].naissance = temps;
	for (sex=0; sex<2; sex++)
	{
	    individus[sexe][population[sexe]].genome[sex] = 
		individus[sex][j[sex]].genome[random(2)];
	    individus[sexe][population[sexe]].parent[sex] = j[sex];
        }
	individus[sexe][population[sexe]].partenaire = -1;
	population[sexe]++;
    }

    n = 0;
    for (sexe=0; sexe<2; sexe++)
	for (i=0; i<population[sexe]; i++)
	    if (individus[sexe][i].genome[0] & individus[sexe][i].genome[1])
		n++;
    printf ("\nfidelite %d: n = %d\n", fidelite, n);
    total[ii][fidelite] += n;
  }
  printf ("Total: %d %d %d %d\n", 
		total[0][0], total[0][1], total[1][0], total[1][1]);
 }
}

