
#pragma hdrstop
#include <condefs.h>

#include <stdio.h>

//---------------------------------------------------------------------------
#pragma argsused

class Ex
{
public:
        Ex (int x);
        int val;
};

Ex::Ex (int x)
{
        val = x;
}

int f (int x)
{
        printf ("f (%d) called\n", x);
        if (x < 0)
        {
                printf ("Throwing %d\n", -x);
                throw (Ex(-x));
                printf ("Thrown\n");
        }
        printf ("Returning %d\n", x+1);
        return x+1;
}

int main(int argc, char* argv[])
{
int y;
char buf[10];
        try
        {
                printf ("Calling f(3)\n");
                y = f (3);
                printf ("Returned %d\n", y);
                printf ("Calling f(-5)\n");
                y = f(-5);
                printf ("Returned %d", y);
         }
         catch (Ex& e)
         {
                printf ("Exception %d\n", e.val);
         }
         gets (buf);
         return 0;
}
 