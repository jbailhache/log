
typedef int object;

typedef int ((**function) ());

#define ap(f,x) (**((function)f))((f),(x))

#define app(f,x,y) (**((function)f))((f),(x),(y))

#define appp(f,x,y,z) (**((function)f))((f),(x),(y),(z))

struct s1 
{
    int (*f) ();
    int p;
};

int plus (struct s1 *fp, int x)
{
    return fp->p + x;
}

test ()
{
struct s1 plus3[1];
int r;
    plus3->f = plus;
    plus3->p = 3;
    r = ap (plus3, 5);
    printf ("%d\n", r);
};

one (function h, function s, object *z, object *r)
{
    app (s, z, r);
}

omega (function h, function s, object *z, object *r)
{
    appp (h, s, z, r);
}

omega_plus_5 (function h, function s, object *z, object *r)
{
object r1[1];
int i;
    omega (h, s, z, r);
    for (i=0; i<5; i++)
	one (h, s, r, r);
}

struct f1
{ 
    int (*f)(); 
    function h, s; 
};

int ff1 (struct f1 *f1, int *z, int *r)
{
    omega (f1->h, f1->s, z, r);    
}

omega_square (function h, function s, object *z, object *r)
{
struct f1 f1[1];
    f1->f = ff1;
    f1->h = h;
    f1->s = s;
    appp (h, f1, z, r);
}

struct suc
{
    int (*f) ();    
};

int f_suc (struct suc *fp, int *x, int *r)
{
    *r = *x + 1;
}

struct h
{
    int (*f) ();
};

int f_h (struct h *fp, function s, object *x, object *r)
{
int i;
object t[1];
    memcpy (t, x, sizeof(t));
    for (i=0; i<0x10; i++)
    {
	app (s, t, t);	
    }
    memcpy (r, t, sizeof(*r));
}

ord_x (function h, function s, object *z, object *r)
{
object t[1];
int i;
    *t = *z;
    for (i=0; i<3; i++)
	omega_square (h, s, t, t);
    omega_plus_5 (h, s, t, t);
    *r = *t;    
}

test_ord ()
{
int z[1], r[1];
struct suc s_suc[1];
struct h s_h[1];
    *z = 0;
    s_suc->f = f_suc;
    s_h->f = f_h; 
    /* omega_plus_5 (s_h, s_suc, z, r); */
    ord_x (s_h, s_suc, z, r);
    printf ("%X\n", *r);    
}

main ()
{
    test_ord ();
}

