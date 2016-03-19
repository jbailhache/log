

A = [ 4, -2, 1, 0;
      2, -4, 0, 1;
     11, -3, 1, 0;
      3,-11, 0, 1;
      6, -6, 1, 0;
      6, -6, 0, 1]

K = [104; 201; 111; 203; 101; 204]

P = diag([0.5, 0.5, 0.4, 0.4, 0.6, 0.6])

tAPA = A'*P*A

itAPA = inv (tAPA)

tAPK = A' * P * K

abpq = itAPA * tAPK

a=abpq(1)
b=abpq(2)
p=abpq(3)
q=abpq(4)

xy = [4,2; 11,3; 2,4; 5,9]'

R = [a,-b;b,a]
T = [p;q] * [1,1,1,1]

XY = T + R * xy



