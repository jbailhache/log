
A = [ 0, 0, 1, 0;
      0, 0, 0, 1;
     10, 0, 1, 0;
     0, 10, 0, 1;
     20, 0, 1, 0;
     0, 20, 0, 1];

K = [100; 200; 110; 200; 100; 210]

P = diag ([1,1,1,1,1,1])

tAPA = A'*P*A

itAPA = inv (tAPA)

tAPK = A' * P * K

abpq = itAPA * tAPK

a=abpq(1)
b=abpq(2)
p=abpq(3)
q=abpq(4)