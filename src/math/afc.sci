 
/// Ref. Statistique exploratoire multidimensionnelle, ed. Dunod,
/// Initiation à l'analyse de données

colonnes1 = ["ChBrn", "ChCha", "ChRou", "ChBln"];
lignes1 = ["YeMar", "YeNoi", "YeVer", "YeBle", "YeXxx"];

K1 = [	68	119	26	7;
       	15	54	14	10;
	5	29	14	16;
	20	84	17	94];

colonnes2 = ["Chan", "Rock", "Mcla"];
lignes2 = ["Jeune", "Femme", "Homme", "Vieux"];

K2 = [	69 	41 	18;
	172 	84 	127;
	133 	118 	157;
	27 	11 	43];

colonnes = colonnes1;
lignes = lignes1;
K = K1;
 
[n,p] = size (K);

k = sum(K);

F = K / k;

M = F * ones(p,1);
P = ones(1,n) * F;

Dn = diag (M);
Dp = diag (P);

S = F' * inv(Dn) * F * inv(Dp);
T = F * inv(Dp) * F' * inv(Dn);

MP = M * P;
X = (F - MP) ./ sqrt(MP);
C = (F - MP) ./ (M * sqrt(P));

[valpr,vecpr] = bdiag (X'*X);
valprv = diag(valpr);
[valprvo,ordre] = sort (valprv);
Cp = vecpr (:,ordre);
lambdap = diag(valprvo);
Cp1 = Cp * 1/sqrt(lambdap);

Cn = C * Cp;

[valpr,vecpr] = bdiag (X*X');
valprv = diag(valpr);
[valprvo,ordre] = sort (valprv);
Cn1 = vecpr (:,ordre);
lambdan = diag(valprvo);
Cn2 = Cn1 * 1/sqrt(lambdan); 

xset ("window", 1);
plot2d (Cp(:,1), Cp(:,2), 0);
for i=1 : p
 xstring (Cp(i,1), Cp(i,2), colonnes(i));
end

plot2d (Cn(:,1), Cn(:,2), 0);
for i=1 : n
 xstring (Cn(i,1), Cn(i,2), lignes(i));
end

[valpr,vecpr] = bdiag (S);
valprv = diag(valpr);
[valprvo,ordre] = sort (valprv);
u = vecpr (:,ordre);
/// lambda = valpr(:,ordre);
lambda = diag(valprvo);

[valpr,vecpr] = bdiag (T);
valprv = diag(valpr);
[valprvo,ordre] = sort (valprv);
v = vecpr (:,ordre);
 
psi = inv(Dn) * F * inv(Dp) * u;
phi = inv(Dp) * F' * inv(Dn) * v;

Gp = u * lambda;
/// Gn = v * lambda;



/// xset ("window", 1);
/// plot2d (psi(:,1), psi(:,2), 0);
/// plot2d (Gn(:,1), Gn(:,2), 0);
for i=1 : n
/// xstring (psi(i,1), psi(i,2), lignes(i));
/// xstring (Gn(i,1), Gn(i,2), lignes(i));
end
/// plot2d (phi(:,1), phi(:,2), 0);
/// plot2d (Gp(:,1), Gp(:,2), 0);
for i=1 : p
/// xstring (phi(i,1), phi(i,2), colonnes(i));
///  xstring (Gp(i,1), Gp(i,2), colonnes(i));
end
 



