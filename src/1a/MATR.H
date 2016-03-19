
#include <math.h>

#define real double

struct matr
{
 int nl, nc;
 real data[8000];
};

typedef struct matr *matr;

#define coef1(m,i,j) ((i<0||i>m->nl||j<0||j>m->nc)?\
 (printf("%s(%d): out of range of %s\n",__FILE__,__LINE__,#m),0):\
 m->data[i*m->nc+j])

#define coef(m,i,j) ((m)->data[(i)*(m)->nc+(j)])

real norme (matr a);
real normeligne (matr a);
real normecolonne (matr a);
