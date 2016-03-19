
A =:      _183  53  _12  28
A =: A ,. _392 113  _38  65
A =: A ,. _192  56   _1  25
A =: A ,. _538 157   _7  71

B =:		3 2 1 0
B =: B ,.	4 6 3 1
B =: B ,.	2 3 7 2
B =: B ,. 	1 3 5 7

abs =: 3 : 0
	y. * * y. 
)

step =: 3 : 0
    	i =: i + 1
		w =: u +/ . * A
	    l1 =: l
		l =: v +/ . * w
		u =: w % l
		l ; u
)

init =: 3 : 0
	A =: y.		
    r =: $ A
	n =: 1 { r
	u =: n $ 1
	v =: 1 , (n-1) $ 0
    l =: 1
    l1 =: 0
    i =: 0
)

vppuissance1 =: 3 : 0
    init y.
	while. (i < 10000) * ((i < 5) + ((abs (l - l1)) > 0.000000000000001))
	do.
		step 0
	end.
	l ; u ; i
)
 
vppuissance =: 3 : 0
	A =. y.		
    r =. $ A
	n =. 1 { r
	u =. n $ 1
	v =. 1 , (n-1) $ 0
    l =. 1
    l1 =. 0
    i =. 0
 
	while. (i < 10000) * ((i < 5) + ((abs (l - l1)) > 0.000000000000001))
	do.
    	i =. i + 1
		w =. u +/ . * A
		l1 =. l
		l =. v +/ . * w
		u =. w % l
	end.
		
	l ; u ; i
)

vpdefla1 =: 3 : 0
	A =: y.
	l1x1 =: vppuissance A
	lam1 =: > 0 { l1x1
	x1 =: > 1 { l1x1 
	w =: x1 % (x1 +/ . * x1)
	A1 =: A - lam1 * (w */ x1)
	l2x2 =: vppuissance A1
	l2x2
)

vpdeflabad =: 3 : 0
	A =. y.
	r =. 0 $ 0
    valp =. 0 $ 0
	vecp =. 0 $ 0
    i =. 0
	while. i < (0 { $ A) - 1
	do.
		lx =. vppuissance A
		r =. r ; lx
		lam =. > 0 { lx
		valp =. valp , lam
		x =. > 1 { lx
		if. i = 0 
		do. vecp =. x
		else. vecp =. vecp ,. x
		end.
		w =. x % (x +/ . * x)
		A =. A - lam * (w */ x)
		i =. i + 1
	end.
	lx =. vppuissance A
	r =. r ; < lx
	lam =. > 0 { lx
	valp =. valp , lam
	x =. > 1 { lx
	vecp =. vecp ,. x
	valp ; |: vecp
)
NB. x2 = v2 + l1 / (l2 - l1) (w, v2) x1 

vpdefla =: 3 : 0
	A =. y.
	r =. 0 $ 0
    valp =. 0 $ 0
	vecp =. 0 $ 0
    i =. 0
    l =. 0
	while. i < (0 { $ A) 
	do.
		lv =. vppuissance A
		r =. r ; lv
		l1 =. l
		l =. > 0 { lv
		valp =. valp , l
		v =. > 1 { lv
		w =. x % (x +/ . * x)
		if. i = 0 
		do. 
			x =. v
			vecp =. x
		else. 
			x =. v + (l1 % (l - l1)) * (w +/ . * v) * x1
			vecp =. vecp ,. x
		end.
		x1 =. x
		A =. A - l * (w */ x)
		i =. i + 1
	end.
 	valp ; |: vecp
)
NB. x2 = v2 + l1 / (l2 - l1) (w, v2) x1 

vppuissance_sym =: 3 : 0
	A =. y.		
    r =. $ A
	n =. 1 { r
	x =. n $ 1
    l =. 1
    l1 =. 0
    i =. 0
    e =. 1
 
	while. (i < 10000) * ((i < 5) + ((abs (e)) > 0.000000000000001))
	do.
    	i =. i + 1
		Ax =. x +/ . * A
		x1 =. Ax % ((Ax +/ . * Ax) ^ 0.5)
		d =. x1 - x
		e =. +/ (d +/ . * d)
		x =. x1
 	end.
		
	x1
)

vpdefla_sym =: 3 : 0
	A =. y.
	r =. 0 $ 0
    valp =. 0 $ 0
	vecp =. 0 $ 0
    i =. 0
	while. i < (0 { $ A) 
	do.
		v =. vppuissance A
		l =. (0 { v +/ . * A) % (0 { v)
		valp =. valp , l

 		if. i = 0 
		do. vecp =. v
		else. vecp =. vecp ,. v
		end.

		A =. A - l * (v */ v)
		i =. i + 1
	end.

 	valp ; |: vecp

)
