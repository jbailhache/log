 
// D'après G. Saporta, Probabilités, analyse de données et statistiques, ed. Technip
 
variables = ["PAO", "PAA", "VIO", "VIA", "POT", "LEC", "RAI", "PLP"]
individus = ["AGRI", "SAAG", "PRIN", "CSUP", "CMOY", "EMPL", "OUVR", "INAC", "XXXX"]
  
X = [ 	167	1	163	23	41	8	6	6	;
	162	2	141	12	40	12	4	15	;
	119	6	69	56	39	5	13	41	;
	87	11	63	111	27	3	18	39	;
	103	5	68	77	32	4	11	30	;
	111	4	72	66	34	6	10	28	;
	130	3	76	52	43	7	7	16	;
	138	7	117	74	53	8	12	20	;
	120	8	110	40	45	9	10	26	]
 
[n,p] = size(X)

D = 1/n * eye(n,n)
g = X' * D * ones(n,1)
Y = X - ones(n,1) * g'
V = Y' * D * Y
 
moyenne = sum (X, 1) / n
variance = sum (Y .* Y, 1) / n
ecart = sqrt (variance)
Dinvs = diag (ones(ecart)./ecart)
Z = Y * Dinvs
R = Dinvs * V * Dinvs
R1 = Z' * D * Z
R-R1
Dinvs2 = diag (ones(variance)./variance)
M = Dinvs2
MV = M * V
 
[valpr, vecpr] = bdiag (MV)
valprv = diag(valpr)
[valprvo, ordre] = sort(valprv)
u = vecpr (:,ordre)
lambda = valpr (:, ordre)
c = Y * u
   
[valpr, vecpr] = bdiag (R)
valprv = diag (valpr)
[valprvo, ordre] = sort (valprv)
u = vecpr (:, ordre)
lambda = valpr (:, ordre)
c = Z * u

// r = (ones(p,1) * sqrt(valprvo)') .* u
r = (sqrt(valprvo) * ones(1,p)) .* u
// semble incorrect
// r = (sqrt(valprvo) * ones(1,p)) .* (X' * c)
// r = u' * Z' * D * Z
// r = (Z' * D * Z * u) ./ (ones(1,p) * sqrt(valprvo))   
// r = (c' * D * Z) ./ (sqrt(valprvo) * ones(1,p))

// mcarac = eye (p, p)
// cmcarac = mcarac - ones (p, 1) * g'
// r = cmcarac * u

r1 = [ 0.975, -0.860, 0.866, -0.927, 0.616, 0.914, 0.930, -0.905;
       0.135,  0.440, 0.214,  0.25,  0.689, 0.117, 0.293, -0.038]
	 
xset ("window", 3)
 
plot2d (c(:,1), c(:,2), 0)

for i=1 : n
	xstring (c(i,1), c(i,2), individus(i))
end

xset ("window", 4)

plot2d (r(1,:), r(2,:), 0)

for i=1 : p
	xstring (r(1,i), r(2,i), variables(i))
end 


