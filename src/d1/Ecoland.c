
/*

*/
 
#include <stdlib.h>
#include <math.h>

#define NPAYS 3

#define POPMAX 1000

#define NPROD 10

#define PROD_ALIM 0

int t;

typedef float montant;
typedef float quantite;

struct habitant
{
 int naissance;
 int vivant;
 int pays;
 montant fortune;
 quantite stock[NPROD];
 quantite besoin[NPROD];

};

init_habitant (struct habitant *h)
{
int i;
 h->naissance = t;
 h->vivant = 1;
 h->pays = 0;
 h->fortune = 10000;
 for (i=0; i<NPROD; i++)
  h->stock[i] = 0;
}

struct pays
{
 int population;
 /*struct habitant habitant[POPMAX];*/

};

struct pays monde[NPAYS];

int population;
struct habitant habitant[POPMAX];

/* dans un premier temps pour simplifier on achete seulement la quantite
necessaire, par la suite utiliser une fonction d'utilite.
Gerer la repartition entre les differents produits en cas de manque d'argent
 */
quantite quantite_achetee1 (quantite stock, quantite besoin,
	montant prix, montant fortune)
{
quantite qn;
 qn = besoin - stock;
 if (qn < 0) return 0;
 qp = fortune / prix;
 if (qn <= qp) return qn;
 return qp;
}

/* a ameliorer (total, totald)
 tenir compte du cas o— on a plusieurs prix diff‚rents
*/
montant calcul_quantites_achetees (int n, quantite *achat, quantite *stock,
	quantite *besoin, quantite *dispo, montant *prix, montant fortune)
{
montant total, totald, depense;
int i;
float a;
quantite qn;
 total = 0;
 totald = 0;
 for (i=0; i<n; i++)
 {
  qn = besoin[i] - stock[i];
  if (qn < 0) qn = 0;
  total += qn * prix[i];
  if (qn > dispo[i]) qn = dispo[i];
  totald += qn * prix[i];
 }
 if (totald <= fortune) a=1;
 else a = fortune / totald;
 depense = 0;
 for (i=0; i<n; i++)
 {
  qn = besoin[i] - stock[i];
  if (qn < 0) qn = 0;
  achat[i] = a * qn;
  depense += achat[i] * prix[i];
 }
 return depense;
}

achats (struct habitant *h)
{
quantite achat[NPROD];
montant depense;
quantite dispo[NPROD];
quantite prix[NPROD];
 depense = calcul_quantites_achetees (NPROD, achat, h->stock, h->besoin,
	dispo, prix, h->fortune);


}

/*
 actions :
  achats
  ventes
  travail
  embauche

  chaque habitant met en vente une certaine quantit‚ de chaque produit
  pour un certain prix si son stock > besoin : pour quel prix ?

  si stock < besoin il cherche … acheter

  qui fixe le prix ? marchandage ?
  fonctions d'utilit‚
  banque, placement, emprunt, taux
  si manque d'argent, emprunter
  si trop d'argent, placer

  chaque habitant produit une certaine quantit‚ de chaque produit
  en un certain temps

  contrat de travail : diff‚rents contrats
  temps de travail et loisir
  salaire
  production

  services ?

  fondement : fonction d'utilit‚ des produits
  produits interchangeables, compl‚mentaires
  usure, dur‚e

  moyens d'acquisition
   production directe, sans ou avec outils
   achat
   emploi d'un salari‚

  fonction d'utilit‚ de l'argent
  offre, demande … plusieurs personnes
  co–t du transport, taxes douaniŠres
  anticipation

  ‚pargne, investissement
   outils : produits aidant … la fabrication d'autre produits
  emprunt
  taux


 un produit
  - dure un certain temps
  - apporte une certaine quantit‚ de satisfaction, pour une personne donn‚e

 pour plusieurs produits si ind‚pendants somme des satisfactions
 cas g‚n‚ral : interchangeabilit‚, compl‚mentarit‚ :
  satisfaction associ‚e … un vecteur de quantit‚s.

 une quantit‚ d'un produit peut ˆtre obtenue
  en un certain temps
  avec tels produits (outils -> usure, produits de base qui disparaissent)
  avec une certaine main d'oeuvre pay‚e tel salaire

  cas particulier : achat : le produit de base est le produit voulu
  achat des outils et produits de base : tenir compte des frais de port
   et taxes douaniŠres


 fonction d'utilit‚ g‚n‚rale :
  fonction qui … chaque possession … l'instant t
  associe un ensemble de couples
   nouvelle possession … t+dt
   gain de satisfaction
  choisir un de ces couples

 possession :
  vecteur de distribution : 1 distribution par produit, fonction de l'‚tat d'usure
  1 produit : num‚ro de produit, ‚tat d'usure
  ou bien
  1 produit repr‚sent‚ par (x1, ... xn) vecteur de parametres reels
  usure = 1 de ces paramŠtres
  ou usure implicite : satisfaction d‚pendant de l'ensemble des paramŠtres
  type d'objet : valeur d'un des paramŠtres ou 1 paramŠtre par type
  possession = distribution (x1,...,xn) -> quantit‚
  denr‚es et objets
  denr‚es repr‚sent‚es par objets : quantit‚s ‚l‚mentaires
   mol‚cule, mg...
  possession = ensemble d'objets { (x11,...,x1n), (x21,...,x2n), ... }
  ou ensemble d'objets avec quantit‚s : { (n1, (x11,...x1n)), ...}

 choix :
  fonction d'‚volution (possession, choix) -> (nouvelle possession, satisfaction)
  choix repr‚sent‚ par vecteur de paramŠtres (c1, ... cp)

  rend compte de
   utilit‚ directe
   utilit‚ comme outil
   utilit‚ comme matiŠre premiŠre
   utilit‚ pour conserver (exemple: r‚frig‚rateur)
   usure naturelle
   usure par usage

  argent consid‚r‚ comme produit particulier
   (avec ou sans usure ?)

  reste :
   ‚changes, offre, demande
   frais de port, taxes importation, TVA
   embauche
   ‚pargne, investissement
   emprunt
   taux
   prix stockage, loyer
   vol

  ‚changes :
   fusion
   argent : compensation diff‚rence satisfaction

  possession partag‚e
  matrice de possession
  ou
  pas de possession
  concept ‚mergeant des effets des choix
  automate : ‚tat du monde, choix des individus -> nouvel ‚tat, satisfactions
  ‚tat du monde = liste d'objets
  objet = vecteur
  1 choix par individu = vecteur

  th‚orie des jeux et des automates
   ‚tat du monde
   n joueurs
   action
   ‚tat, actions a1 ... an -> nouvel ‚tat, satisfactions s1 ... sn
                              ou nouvel ‚tat seulement
                              et fonction ‚tat -> satisfactions s1, ... sn
   d‚composition de l'‚tat en objets o1 ... op
   1 objet oi d‚crit par paramŠtres (x1, ... xq)
   propri‚t‚ : objets titres de propri‚t‚ ?

   paramŠtres des objets :
    - usure
    - caract‚ristiques physiques : longueur, volume, masse...
    - x-it‚ (type d'objet)
    - appartenances
    - prˆt

   prˆts imbriqu‚s
   notion d'engagement, contrat

   actions
    exemples
     manger un fruit
     regarder la t‚l‚vision
     fabriquer un objet avec des outils et des matiŠres premiŠres
     mettre un aliment dans un r‚frig‚rateur
     louer un appartement
     donner quelque chose … qualqu'un

   contrats
    dans 1 mois je te rendrai cet objet
    si tu me fabrique tel objet je te donnerai tel autre objet
 */

action_habitant (struct habitant *h)
{

}


main ()
{

}
