
A=8.0
alpha=0.3
rho=0
mu=0.1
psi=0.05
sigma=1

function r=cobbdouglas(x)
 r=A*x^alpha;
endfunction

function r=ces(x)
 r=A*(alpha*x^(-rho)+1-alpha)^(-1/rho);
endfunction

function y=f(k)
 if rho==0
  y=cobbdouglas(k)
 else
  y=ces(k)
 end
endfunction

eps=0.00001

function r=dcobbdouglas(x)
 r=(cobbdouglas(x+eps)-cobbdouglas(x))/eps;
endfunction

function r=dces(x)
 r=(ces(x+eps)-ces(x))/eps;
endfunction

function y=df(k)
 y=(f(k+eps)-f(k-eps))/(2*eps);
endfunction

function y=euler(x)
 if rho == 0
  y = dcobbdouglas(x) - mu - psi
 else
  y = dces(x) - mu - psi
 end
endfunction

function [tr,yr]=ode45 (f, t, y)
 yr=ode (y, t(1), t(2), f);
 tr=0
endfunction

kstar=fsolve(1,euler)
cstar=f(kstar)-mu*kstar
f_sec=500*(df(kstar+0.001)-df(kstar-0.001));

jacobien=[psi,-1;cstar*f_sec,0]
pent=spec(jacobien)
pente=pent(1)

t=linspace(0,1.2*kstar,100);

y=f(t)-mu*t;
// plot(t,y);
x_k=kstar*ones(1,20);
y_k=linspace(0,cstar,20);
// line(x_k,y_k);


k0=1
c0=1
tf=1
z0=[k0,c0]
tspan=[0,tf]

if 0==1
[t,z]=ode45('edcoe',tspan,z0);
x=z(:,1)
y=z(:,2)
figure(2);
x1=0:kstar;
y_t=pente*(x1-kstar)+cstar;
y2=f(x1)-mu*x1;
x_k=kstar*ones(1,20);
y_k=linspace(0,cstar,20);
plot(x,y);
line(x1,y2);
line(x_k,y_k);
line(x1,y_t);
end

