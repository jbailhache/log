
pi = 3.1415926

dt=0.01

v1=5
v2=3
L=2

xmin=-10
xmax=10
ymin=10
ymax=-10

function [r]=plotpoint(x,y)
 xrect(x,y,0,0)
 r=0
endfunction

function [sl1,sl2]=traj(fp,T,px1,py1,pa1,px2,py2,pa2)

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

plot2d([xmin,xmax,xmax,xmin],[ymin,ymin,ymax,ymax])

for t=0:dt:T

 u1=-0.5*pa1*v1/L
 // u2=atan(py2/px2)
 u2 = imag(log(px2+sqrt(-1)*py2))

 x1=x1 +dt*v1*cos(a)
 y1=y1 +dt*v1*sin(a)

 x2=x2 +dt*v2*cos(u2)
 y2=y2+dt*v2*sin(u2)

 x=x2-x1
 y=y2-y1

 if fp then
  plotpoint(x1,y1)
  plotpoint(x2,y2)
 end

 a=a+dt *v1/L*u1

 px1 =px1+dt* (-2*x)
 py1=py1+dt*(-2*y)
 pa1=pa1+dt*(v1*(-px1*sin(a/pi*180)+py1*cos(a)))

 px2 =px2+dt*(2*x)
 py2=py2+dt*(2*y)
 pa2=pa2+dt*(v1*(-px2*sin(a/pi*180)+py2*cos(a)))

 sl1=sl1+dt*(u1^2+x^2+y^2)
 sl2=sl2+dt*(-x^2-y^2)

end

endfunction


function [px1,py1,pa1,px2,py2,pa2]=solve(T,eps,m)

px1=0
py1=0
pa1=0
px2=1.01
py2=1.01
pa2=1

dp=1

s=0

while 1 do

 s=s+1
 if s>m then break; end

 plotpoint(px1/10,px2/10)
 plotpoint(py1/10,py2/10)
 plotpoint(pa1/10,pa2/10)
 
 xclear()
 [sl1,sl2] = traj(1,T,px1,py1,pa1,px2,py2,pa2)

 sl1p = sl1
 sl2p = sl2

 [sl1t,sl2t] = traj(0,T,px1+dp,py1,pa1,px2,py2,pa2)
 if sl1t < sl1 then 
  px1=px1+dp
  sl1=sl1t
  sl2=sl2t
 else
  [sl1t,sl2t] = traj(0,T,px1-dp,py1,pa1,px2,py2,pa2)
  if sl1t < sl1 then 
   px1=px1-dp
   sl1=sl1t
   sl2=sl2t
  end
 end

 [sl1t,sl2t] = traj(0,T,px1,py1+dp,pa1,px2,py2,pa2)
 if sl1t < sl1 then 
  py1=py1+dp
  sl1=sl1t
  sl2=sl2t
 else
  [sl1t,sl2t] = traj(0,T,px1,py1-dp,pa1,px2,py2,pa2)
  if sl1t < sl1 then 
   py1=py1-dp
   sl1=sl1t
   sl2=sl2t
  end
 end

 [sl1t,sl2t] = traj(0,T,px1,py1,pa1+dp,px2,py2,pa2)
 if sl1t < sl1 then 
  pa1=pa1+dp
  sl1=sl1t
  sl2=sl2t
 else
  [sl1t,sl2t] = traj(0,T,px1,py1,pa1-dp,px2,py2,pa2)
  if sl1t < sl1 then 
   pa1=pa1-dp
   sl1=sl1t
   sl2=sl2t
  end
 end

 [sl1t,sl2t] = traj(0,T,px1,py1,pa1,px2+dp,py2,pa2)
 if sl2t < sl2 then 
  px2=px2+dp
  sl1=sl1t
  sl2=sl2t
 else
  [sl1t,sl2t] = traj(0,T,px1,py1,pa1,px2-dp,py2,pa2)
  if sl2t < sl2 then 
   px2=px2-dp
   sl1=sl1t
   sl2=sl2t
  end
 end

 [sl1t,sl2t] = traj(0,T,px1,py1,pa1,px2,py2+dp,pa2)
 if sl2t < sl2 then 
  py2=py2+dp
  sl1=sl1t
  sl2=sl2t
 else
  [sl1t,sl2t] = traj(0,T,px1,py1-dp,pa1,px2,py2-dp,pa2)
  if sl2t < sl2 then 
   py2=py2-dp
   sl1=sl1t
   sl2=sl2t
  end
 end

 [sl1t,sl2t] = traj(0,T,px1,py1,pa1,px2,py2,pa2+dp)
 if sl2t < sl2 then 
  pa2=pa2+dp
  sl1=sl1t
  sl2=sl2t
 else
  [sl1t,sl2t] = traj(0,T,px1,py1,pa1,px2,py2,pa2-dp)
  if sl2t < sl2 then 
   pa2=pa2-dp
   sl1=sl1t
   sl2=sl2t
  end
 end

 if ((sl1==sl1p) & (sl2==sl2p)) then 
  dp=dp/2
 else
  dp=dp*2
 end

 if dp<eps then break end

end

traj(1,T,px1,py1,pa1,px2,py2,pa2)

r=0
endfunction
