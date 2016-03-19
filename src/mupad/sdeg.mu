sdeg := proc(a,b,c)
local sol,d,x1,x2;
begin
d:=b^2-4*a*c;
if d>0 then
 x1:=(-b+sqrt(d))/(2*a);
 x2:=(-b-sqrt(d))/(2*a);
 sol:={x1,x2};
elif d=0 then
 x1:=-b/(2*a);
 sol:={x1};
else
 sol:={};
end_if;
 sol;
end_proc;
