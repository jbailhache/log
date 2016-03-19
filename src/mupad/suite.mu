suite:=proc(u0,n)
local i,u;
begin
 u:=u0;
 for i from 1 to n do
  u:=2*u+5;
 end_for;
 u;
end_proc;
