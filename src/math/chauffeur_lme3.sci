
function [r]=plotpoint(x,y)
// xrect(x,y,0,0)
xrect(x,y,(xmax-xmin)/400,(ymax-ymin)/100)
//plot([x,x],[y,y]);
r=0;
endfunction

// function [r]=traj ( )
// function [sl1,sl2]=traj(fp,T,px1,py1,pa1,px2,py2,pa2)

function [sl1,sl2]=traj(fp,T,p1,p2)
px1=p1(1)
py1=p1(2)
pa1=p1(3)

px2=p2(1)
py2=p2(2)
pa2=p2(3)

pi = 3.1415926

dt=0.04
// T=5

v1=5
v2=3
L=2

xmin=-10
xmax=10
ymin=10
ymax=-10



// function [r]=traj()

sl1=0
sl2=0
x1=0
y1=0
x2=-3
y2=1
x=x2-x1
y=y2-y1
a=0
// px1=0
// py1=0
// pa1=0
// px2= 1
// py2=1
// pa2=1

// clf
if (fp > 0)
 xclear()
 plot([xmin,xmax,xmax,xmin],[ymin,ymin,ymax,ymax])
end

// axis([xmin,xmax,ymin,ymax])
// hold on

for t=0:dt:T

 u1=-0.5*pa1*v1/L;
 // u2=atan(py2/px2) ;
u2 = imag(log(px2+sqrt(-1)*py2));

 x1=x1 +dt*v1*cos(a);
 y1=y1 +dt*v1*sin(a);

 x2=x2 +dt*v2*cos(u2);
 y2=y2+dt*v2*sin(u2);

 x=x2-x1;
 y=y2-y1;

if fp
 disp("plot:",x1,y1,x2,y2,sl1,sl2); 
 plotpoint(x1,y1);
 plotpoint(x2,y2);
 plotpoint(xmin+(xmax-xmin)*t/T,sl1/8);
 plotpoint(xmin+(xmax-xmin)*(1-t/T),sl2/8);
end

 a=a+dt *v1/L*u1;

 px1 =px1+dt* (-2*x);
 py1=py1+dt*(-2*y);
 pa1=pa1+dt*(v1*(-px1*sin(a)+py1*cos(a)));

 px2 =px2+dt*(2*x);
 py2=py2+dt*(2*y);
 pa2=pa2+dt*(v1*(-px2*sin(a)+py2*cos(a)));

 sl1=sl1 +dt *(u1^2+x^2 +y^2);
 sl2=sl2+dt*(-x^2-y^2);


end
r=0
endfunction

// traj()

function r=test()
fp=1
T=5
px1=0
py1=0
pa1=0
px2= 1
py2=1
pa2=1
p1=[px1,py1,pa1]
p2=[px2,py2,pa2]
traj(fp,T,p1,p2 )
r=0
endfunction

function [sl1,sl2,p1,p2]=solve(T,eps,m)

px1=0
py1=0
pa1=0
px2=1.01
py2=1.01
pa2=1

p1=[px1,py1,pa1]
p2=[px2,py2,pa2]

dp=0.5

s=0

while 1 

 s=s+1
 if s>m 
  break
 end

 // plotpoint(px1/10,px2/10)
 // plotpoint(py1/10,py2/10)
// plotpoint(pa1/10,pa2/10)
 
 [sl1,sl2] = traj(1,T,p1,p2)
 printf("sl1=%f sl2=%f\n",sl1,sl2)
 disp(p1,p2)

 sl1p = sl1
 sl2p = sl2

af=0

for i=1:3
p1(i)=p1(i)+dp
// clf
 if (af>0) 
  xclear() 
 end
 [sl1t,sl2t] = traj(af,T,p1,p2)
 if sl1t < sl1
  printf ("amelioration pour 1\n") 
  sl1=sl1t
  sl2=sl2t
  if (af==0)
   traj(1,T,p1,p2)
   printf("sl1=%f sl2=%f\n",sl1,sl2)
   disp(p1,p2)
  end
 else
 p1(i)=p1(i)-2*dp
// clf
if (af>0)
xclear()
end
  [sl1t,sl2t] = traj(af,T,p1,p2)
  if sl1t < sl1 
   printf ("amelioration pour 1\n") 
   sl1=sl1t
   sl2=sl2t
   if (af==0)
    traj(1,T,p1,p2)
    printf("sl1=%f sl2=%f\n",sl1,sl2)
    disp(p1,p2)
   end
  else
   p1(i) =p1(i)+dp
  end
 end

p2(i)=p2(i)+dp
// clf
if (af>0)
 xclear()
end
[sl1t,sl2t] = traj(af,T,p1,p2)
 if sl2t < sl2 
  printf ("amelioration pour 2\n") 
  sl1=sl1t
  sl2=sl2t
  if (af==0)
   traj(1,T,p1,p2)
   printf("sl1=%f sl2=%f\n",sl1,sl2)
   disp(p1,p2)
  end
 else
  p2(i)=p2(i)-2*dp
 // clf
 if (af>0)
 xclear()
 end
  [sl1t,sl2t] = traj(af,T,p1,p2)
  if sl2t < sl2 
   printf ("amelioration pour 2\n") 
   sl1=sl1t
   sl2=sl2t
   if (af==0)
    traj(1,T,p1,p2)
    printf("sl1=%f sl2=%f\n",sl1,sl2)
    disp(p1,p2)
   end
  else
   p2(i)=p2(i)+dp
  end
 end

end

 if ((sl1==sl1p) & (sl2==sl2p)) 
  dp=dp/2
 // else
 // dp=dp*2
 end

 if dp<eps 
   break
 end

end

px1=p1(1)
py1=p1(2)
pa1=p1(3)

px2=p2(1)
py2=p2(2)
pa2=p2(3)

traj(1,T,p1,p2)

r=0
endfunction
