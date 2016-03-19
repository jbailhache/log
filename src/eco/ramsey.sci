
A=8.0
alpha=0.3
rho=0
mu=0.1
psi=0.05

function r=cobbdouglas(x)
 r=A*x^alpha;
endfunction

function r=ces(x)
 r=A*(alpha*x^(-rho)+1-alpha)^(-1/rho);
endfunction

eps=0.00001

function r=dcobbdouglas(x)
 r=(cobbdouglas(x+eps)-cobbdouglas(x))/eps;
endfunction

function r=dces(x)
 r=(ces(x+eps)-ces(x))/eps;
endfunction

function eqst 
// (A,alpha,rho,mu,sigma,psi)
 kstar = fzero ('euler', [1 100], 0.0001)
 if rho == 0
  ystar = cobbdouglas (kstar)
  rstar = dcobbdouglas (kstar)
 else
  ystar = ces (kstar) 
  rstar = dces (kstar) 
 end
 cstar = ystar - mu*kstar
 wstar = ystar - rstar*kstar
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

function evolcoe(k0,c0,tf)
// k0 c0 tf
 tspan = [0 tf];
 z0 = [k0;c0];
 [t,z] = ode45 ('edcoe', tspan, z0);
 x = z(:,1);
 y = z(:,2);
 plot(t,x,t,y)
 xlabel('temps'), ylabel('k et c')
 // title('Evolution temporelle')
 // pause
 // figure(2)
 plot(x,y)
 if rho==0
  y2 = cobbdouglas(x)-mu*x
 else
  y2 = ces(x)-mu*x
 end
 line(x,y2)
 xlabel('capital')
 ylabel('consommation')
 // title('Evolution c(k)')
endfunction

function test
 evolcoe(100,10,20)
endfunction