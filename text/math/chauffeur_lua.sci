
pi=3.1415926
dt=0.02
s= 1
c=s* 1.5
c1=s*0.2
c2=s*0.1
a0=pi

v1=5
v2=3
L=2
T=5

w=150
h=150
xmin=-20
xmax=20
ymin=20
ymax=-20

fsl1=pfield(1,10,10)
fsl2=pfield(1,10,10)

function signe(x)
if x<0 then return -1
elseif x>0 then return 1
else return 0
end
end

// function plot(x,y,c)
// i=w*(x-xmin)/(xmax-xmin)
// j=h*(y-ymin)/(ymax-ymin)
// pset(i,j,c)
// end

function h1(x,y,a,u1,u2,px1,py1,pa1)
return u1^2+x^2+y^2+px1*(-v1*cos(a/pi*180)+v2*cos(u2/pi*180))+py1*(-v1*sin(a/pi*180)+v2*sin(u2/pi*180))+pa1 *v1/L *u1
end

function h2(x,y,a,u1,u2,px2,py2,pa2)
return -x^2-y^2+px2*(-v1*cos(a/pi*180)+v2*cos(u2/pi*180))+py2*(-v1*sin(a/pi*180)+v2*sin(u2/pi*180))+pa2 *v1/L *u1
end

--function traj(x,y,px,py)
function traj()
plot(0,0,14)
sl1=0
sl2=0

x1=0
y1=0
x2=-3
y2=1
x=x2-x1
y=y2-y1
a=0

px1=0
py1=0
pa1=0
px2= 1
py2=1
pa2=1

px1=-1.5
py1=-1.5
pa1=-1.5
px2=-5
py2=-0.5
pa2=1

--local x,y,px,py
--x=0
--y=0
--px=s*sin(a/pi*180)
--py=s*cos(a /pi*180)
  --for t=0,10,dt do
t =0
--palert("cond="..((x*x+y*y)) )
--while ((x*x+y*y>0.0001 )and (t<20)) do 
--palert  ("t="..t)
--t=t+dt
for t =0,T ,dt do
--palert(x)
 
--u=-py/(2*c)

u1=-0.5*pa1*v1/L
u2=atan(py2/px2)/180*pi
if x <0 then u2=u2+pi end

e=0.1
--h1z =h1 (x,y,a,u1,u2,px1,py1,pa1)
--h1p=h1 (x,y,a,u1+e,u2,px1,py1,pa1)
--h1m=h1 (x,y,a,u1-e,u2,px1,py1,pa1)
--palert(h1m .." " ..h1z.." " ..h1p.." u1=" ..u1.." u2=" ..u2)

--palert("x="..x.." y="..y.." a="..a .." px1="..px1.." py1=" ..py1.." pa1="..pa1.." px2="..px2.." py2="..py2.." pa2="..pa2     )

--plot(x1,y1,12)
--plot(x2,y2,10)

--palert ("x="..x.." y=".. y.." px="..px .." py=" ..py.." u=" ..u)

 x1=x1 +dt*v1*cos(a/pi*180)
 y1=y1 +dt*v1*sin(a/pi*180)

x2=x2 +dt*v2*cos(u2/pi*180)
y2=y2+dt*v2*sin(u2/pi*180)

--x=x+dt* (-v1*cos(a/pi*180)+v2*cos(u2/pi*180))
 --y=y+dt*(-v1*sin(a/pi*180)+v2*sin(u2/pi*180))

x=x2-x1
y=y2-y1

  --plot (x,y,12)
--plot (xx,yy,8)
plot(x1,y1,12)
plot(x2,y2,8)

for r=1,4 do
plot(r*cos(a/pi *180),r*sin(a/pi*180),3)
end
a=a+dt *v1/L*u1
for r=1,4 do
plot(r*cos(a/pi *180),r*sin(a/pi*180),14)
end

px1 =px1+dt* (-2*x)
py1=py1+dt*(-2*y)
pa1=pa1+dt*(v1*(-px1*sin(a/pi *180)+py1*cos(a/pi*180)))

px2 =px2+dt*(2*x)
py2=py2+dt*(2*y)
pa2=pa2+dt*(v1*(-px2*sin(a/pi *180)+py2*cos(a/pi*180)))

sl1=sl1 +dt *(u1^2+x^2 +y^2)
sl2=sl2+dt*(-x^2-y^2)
psettext(fsl1,sl1)
psettext(fsl2,sl2)

--py=py-dt*(px-c2*y)
-- px=px+dt*((py)* (1+6*x^2)-c1*x)

end
end

--angle(1 +a0)
--angle(2.5+a0)

--for a=0,2*pi-pi/4,pi/4 do
--angle(a)
--end
--traj(0,6, -1,1)
traj()

while 1 do pevent() end
