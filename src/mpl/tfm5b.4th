40 3 + EMIT
49 5 - EMIT
45 SP@ @ EMIT
: g 41 EMIT ; IMMEDIATE
: f 6 g + ;
VARIABLE a
40 a !
a @ f EMIT
48 SP@ @ EMIT
4A 1 - EMIT
40 5 + EMIT
BYE
