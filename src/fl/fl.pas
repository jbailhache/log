program fl;

label 1;

const 
 SYS_TRAP = $4E4F;
	size = 4;
	maxmem = 800;
	maxsymbols = 100;
          maxinx=120;
	presize = 2;
	plen = -1;
	ppc = 0;
	prtn = 1;
	pstk = 2;
	penv = 3;
	pcode = 0;
	pval = 1;
        plenstr = 2;
	pstr = 3;
	
type 
	expr=integer;
	expru = record case integer of
            1: (i: integer);
            2: (s: array [0..3] of char);
            3: (p: ^expr);
          end;
	exprsize = integer;
	exprstr = record case integer of
		1: (x: expr);
		2: (s: array[0..size-1]of char);
			end;
 aexpr=array [0..maxinx] of expr;
var
	freeptr: expr;
	mem : array [0..maxmem] of expr;
	
hmem: integer;
pmem: expru;

	nsymbols: integer;
	symbols: array [0..maxsymbols] of expr;
	
	instr_print_symbol,
instr_print_int,
instr_plus,
instr_times,
instr_at,
instr_set,
instr_alloc,
instr_jne,
instr_eqthen,
instr_eq,
instr_select,
instr_return,
instr_quote,
instr_eval,
instr_getctx,
instr_evol,
	instr_quit: expr;

	ctx: expr;

        inbuf: string;
        pad: array[0..300] of  integer;

        inptr: integer;
	
	a: array[0..30,0..120] of expr;
	n: array[0..30] of integer;

          inxbuf: aexpr;
          inxbegin, inxend: integer;
       

	buf: string;
 i: integer;
tracelevel: integer;
src: string;

(*
function name(x:expr):string; forward;
*)

procedure writemem; forward;


function get (x: expr; i: integer) : expr;
var u: expru;
begin
	get := mem[x+i];
(*
 u.i:=pmem.i;
 u.i := u.i+(x+i)*size;
 get:=u.p^;
*)
end;

procedure put (x: expr; i: integer; y: expr);
var u: expru;
begin
(*
trace(x,'put');
trace(i,' ');
trace(y,' ');
*)
writeln('put ',x,' ',i,' ',y);
        mem[x+i] := y;
(*
 u.i:=pmem.i;
 u.i := u.i+(x+i)*size;
 u.p^:=y;
 *)
end;


procedure trace (i: integer;s: string);
var buf1: string;
begin
if tracelevel>0 then begin
 writeln ('trace: ',i, ' ',s,' '
,symbols[6],' '(*,name(symbols[6])*)
(*
get(instr_print_symbol,4),' ',
get(instr_print_symbol,5),' ',
get(instr_print_symbol,6)
,instr_print_symbol 
,'<',inbuf,'>' *)
(*mem[instr_print_symbol+5] ,name(instr_print_symbol) *) );
 readln(buf1);
 if buf1[0]='q' then goto 1;
end;
end;

(*
procedure EvtGetEvent(var event:EventType;timeout:UInt32); inline(SYS_TRAP,$A11D);
*)
function MemHandleNew (n: integer): integer;
inline(SYS_TRAP,$A01E);
function MemHandleLock(h: integer): pointer;
inline(SYS_TRAP,$A021);

function atoi1(s:string):integer;
inline(SYS_TRAP,$A0CE);

function atoi(s:string):integer;
var i, x,sign: integer;
begin
 x:=0;
 sign:=1;
 for i:=1 to length(s) do begin
  if s[i]='-' then sign:=-sign
  else  
   x:=x*10+ord(s[i])-ord('0');
 
 end;
 atoi:=sign*x;
end;

procedure init;
var u: expru;
begin
writeln('new');
 hmem:=MemHandleNew(30000);
writeln('lock');
 pmem.p:=MemHandleLock(hmem);
u.i := pmem.i;
writeln('p=',u.i);
	freeptr := presize;
	nsymbols := 0;
end;

function len (p: expr) : integer;
begin
	len := get(p,-1);
end;

procedure quit;
begin
tracelevel:=1;
trace(0,'quit');
	goto 1;
end;

procedure error (s: string);
begin
	writeln (s);
end;

function alloc (n: integer) : expr;
var x: expr;
begin
	x := freeptr;
        freeptr := freeptr + n + presize;
	if freeptr >= maxmem then 
	begin
		error ('Memory overflow');
		quit;
	end;
	(*mem[x-1] := n;*)
 put(x,-1,n);
        alloc := x;
end;

function store (a: aexpr; from: integer; n: integer): expr;
var cont, eq:boolean;
 i, k: integer;
 x: expr;
begin
 writeln('store ',from,' ',n);
 store:=0;
 cont:=true;
 k:=presize;
 while cont and (k<freeptr) do begin
  (*trace(k,'k');*)
  eq:=true;
  i:=0;
  while eq and(i<n) do begin
   if not (a[from+i]=get(k,i)) then
    eq:=false;
   i:=i+1;
  end;
  if eq then begin
   trace(k,'eq');
   store:=k;
   cont:=false;
  end else
    k:=k+get(k, -1)+presize;
    (*trace(k,'neq');*)
   
 end;
 if cont then begin
  x:=alloc(n);
 (*trace(x,'alloc');*)
  for i:=0 to n-1 do
   put(x,i,a[from+i]);
 store:=x;
 end;
 
end;

function cons (a: expr; b: expr) : expr;
var c: expr;
begin
	c := alloc(2);
trace(c,'allocated by cons');
	put (c, 0, a);
	put (c, 1, b);
        cons := c;
end;

function cons1 (a: expr; b: expr): expr;
var c: expr;
 ae: aexpr;
begin
 ae[0]:= a;
 ae[1]:=b;
 c:=store(ae,0,2);
 cons1:= c;
 end;

function car (a:expr):expr;
begin
 car := get(a,0);
end;

function cdr (a:expr):expr;
begin
 cdr := get(a,1);
end;

function name  (x: expr) : string;
var 
	s: string;
	l, i, j: integer;
	u: exprstr;

begin
trace(x,'name');
 
        l := get (x, plenstr);
trace(l,'len');
        s := '';
if l>256 then name := '***'
else begin
        for i:=0 to l div size do
 
	begin

                u.x := get (x, pstr+i);
(*
trace(i,u.s[0]);
trace(u.x,u.s[3]);
*)
               (* s := concat (s, u.s); *)
               for j:=0 to size-1 do
begin
(*
trace(j,'j');
trace(i*size+j+1,u.s[j]);
if tracelevel>0 then
writeln('i=',i,' j=',j,' pos=',i*size+j+1,' car=',u.s[j],' ',ord(u.s[j]),' ', u.x, ' ',
get(x,pstr+i),'/',x,'=',instr_print_symbol,' ',pstr+i,'=5');
*)
                 s[i*size+j+1]:=u.s[j];
(*trace(i*size+j+1,s);*)
end;
	end;
s[l+1]:=chr(0);
trace(x,s);
        name := s;
end;
end;

 
procedure copyname (x: expr; s: string);
var
	l, i, j: integer;
	u: exprstr;
begin
        l := length(s);
trace(l,s);
        put (x, plenstr, l);
        for i:=0 to l div size do
	begin
(*trace(i,s);*)
                (*u.s := s[i*size]; *)
                for j:=0 to size-1 do
			if j+i*size<l then
                                (*u.s := concat (u.s, s[j+i*size]);*) 
begin
(*trace(j,'store');*)
                                 u.s[j]:=s[i*size+j+1];
(*trace(j,u.s[j]);*)
end else if i*size+j=l then begin
 u.s[j]:=chr(0);
end;
(*trace(u.x,'put');*)
 put(x,pstr+i,u.x);
	end;
trace(999,name(x));	
	
end;

function symbol (s: string) : expr;
var 
	i: integer;
	found: boolean;
	x: expr;
begin
(*tracelevel:=1;*)

	found := false;
	i := 0;
trace(0,s);
        while (not found) and (i < nsymbols) do
	begin
trace(i,name(symbols[i]));
                if s = name(symbols[i]) then 
begin
trace(i,'found');
x:=symbols[i];
found := true
end
 else begin
		if nsymbols >= maxsymbols then  
		begin
			error ('Symbol overflow');
			quit;
		end;
		end;		
 i:=i+1;	
	end;
if not found then begin
trace(i,'not found');
        x := alloc (3 + (length(s)) div size+1); 
trace(0,'t2');
	copyname (x, s);
trace(0,'t3');
	symbols[nsymbols] := x;
trace(0,'t4');
	nsymbols := nsymbols + 1;
trace(0,'t5');
end;
	symbol := x;
trace(x,s);
(*tracelevel:=0;*)
end;

procedure evol (ctx: expr);
var
	instr: expr;
	pc: expr;
ni:string;
begin
trace(ctx,'evol');
	pc := get (ctx, ppc);
trace(pc,'pc');
instr:=get(pc,0);
trace(instr,name(instr));	 
(*trace(instr_print_symbol,'print_symbol');*)
pc := pc + 1;
put(ctx,ppc,pc);
ni:=name(instr);
writeln('evol:',pc,' ',instr,' ',ni,' ',get(ctx,prtn),':',car(get(ctx,prtn)));
	if instr = instr_print_symbol then
                begin
trace(instr,'exec print_symbol');
trace(get(get(ctx,pstk),0),'top');
trace(0,name(get(get(ctx,pstk),0)));
writeln(name(get(get(ctx,pstk),0)));
end
 else if instr = instr_print_int then
 writeln(get(get(ctx,pstk),0))
else if instr = instr_plus then
 put(ctx,pstk,cons(car(get(ctx,pstk))+car(cdr(get(ctx,pstk))),cdr(cdr(get(ctx,pstk))))) 
else if instr = instr_times then
 put(ctx,pstk,cons(car(get(ctx,pstk))*car(cdr(get(ctx,pstk))),cdr(cdr(get(ctx,pstk))))) 
else if instr = instr_eq then begin
 if car(get(ctx,pstk)) = car(cdr(get(ctx,pstk))) then
  put(ctx,pstk,cons(1,cdr(cdr(get(ctx,pstk))))) 
 else
  put(ctx,pstk,cons(0,cdr(cdr(get(ctx,pstk))))) 
end else if instr = instr_at then begin
 trace(car(get(ctx,pstk)),'top');
 trace(car(car(get(ctx,pstk))),
  name(car(car(get(ctx,pstk)))));
 
put(ctx,pstk,cons(car(car(get(ctx,pstk))),cdr(get(ctx,pstk))));
end
else if instr = instr_set then begin
 put(car(get(ctx,pstk)),0,car(cdr(get(ctx,pstk))));
put(ctx,pstk,cdr(cdr(get(ctx,pstk))));
end
else if instr = instr_alloc then
 put(ctx,pstk,cons(alloc(car(get(ctx,pstk))),cdr(get(ctx,pstk))))
else if instr = instr_jne then begin
  if not (car(get(ctx,pstk)) = car(cdr(get(ctx,pstk)))) then 
   put(ctx,ppc,get(pc,0))
  else
   put(ctx,ppc,pc+1);
 end
else if instr = instr_eqthen then begin
  put(ctx,prtn,cons(pc,get(ctx,prtn)));
  if car(get(ctx,pstk)) = car(cdr(get(ctx,pstk))) then
   put(ctx,ppc,get(pc,0))
  else
   put(ctx,ppc,get(pc,1));
 end
else if instr = instr_select then begin
  put(ctx,prtn,cons(pc+1,get(ctx,prtn)));
  put(ctx,ppc,get(car(pc),car(get(ctx,pstk))));
  put(ctx,pstk,cdr(get(ctx,pstk)));
 end 
else if instr = instr_return then begin
 trace(car(get(ctx,prtn)),'ret adr');
  put(ctx,ppc,car(get(ctx,prtn)));
  put(ctx,prtn,cdr(get(ctx,prtn)));
 end
else if instr = instr_quote then begin
  put(ctx,pstk,cons(car(pc),get(ctx,pstk)));
  put(ctx,ppc,pc+1);
 end
else if instr = instr_eval then begin
  trace(pc,'pc ret');
put(ctx,prtn,cons(pc,get(ctx,prtn)));
 trace(car(get(ctx,prtn)),'ret adr 1');
 put(ctx,ppc,car(get(ctx,pstk)));
trace(prtn,'prtn');
trace(get(ctx,prtn),'rtn');
trace(car(get(ctx,prtn)),'ret adr 2');
put(ctx,pstk,cdr(get(ctx,pstk)));
trace(car(get(ctx,prtn)),'ret adr 3');
 end
else if instr = instr_getctx then 
  put(ctx,pstk,cons(ctx,get(ctx,pstk)))
else if instr = instr_evol then
  evol(car(get(ctx,pstk)))

	else if instr = instr_quit then
                quit
	else put (ctx, pstk, cons(instr,get(ctx,pstk)));

end;

procedure init_prim;
begin
	instr_print_symbol := symbol('print_symbol');
instr_print_int:=symbol('print_int');
instr_plus:=symbol('+');
instr_times:=symbol('*');
instr_at:=symbol('@');
instr_set:=symbol('!');
instr_alloc:=symbol('alloc');
instr_jne:=symbol('jne');
instr_eqthen:=symbol('eqthen');
instr_eq := symbol('=');
instr_select:=symbol('select');
instr_return:=symbol('return');
instr_quote:=symbol('''');
instr_eval:=symbol('eval');
instr_getctx:=symbol('getctx');
instr_evol:=symbol('evol');
	instr_quit := symbol('quit');
end;

function curchar : char;
begin
(*trace(inptr,inbuf);*)
	curchar := inbuf[inptr];
(*trace(ord(curchar),curchar);*)
end;

procedure nextchar;
begin
        inptr := inptr + 1;
trace(0,inbuf[inptr]);
end;

procedure writeinx;
var i: integer;
begin
 for i:=0 to inxend do
   write(i,':',inxbuf[i],';');
end;

function readexpr : expr;
var 
 cont1, cont: boolean;
 x: expr;
 i,k: integer;
begin
trace(4,inbuf);
 inxbegin:=0;
 inxend:=0;
 cont1:=true;
 while cont1 do begin
tracelevel:=1;
(*trace(5,inbuf);*)
trace(ord(curchar),curchar);
  writeinx;
tracelevel:=0;
  x:=0;
  while curchar = ' ' do nextchar;
  if curchar = '[' then begin
   nextchar;
   inxbuf[inxend]:=inxbegin;
   inxend:=inxend+1;
   inxbegin:=inxend;
  end else if curchar = ']' then begin
    nextchar;
    (*x:= alloc(inxend-inxbegin);
    for i:=0 to inxend-inxbegin do
      put(x,i,inxbuf[inxbegin+i]);*)
 x:=store(inxbuf,inxbegin,inxend-inxbegin);

     inxend:= inxbegin-1;
     inxbegin:=inxbuf[inxend];
   end else begin

	i := 0;
		buf := ' ';
                    k:=1;
		cont := true;
(*tracelevel:=1;*)
trace(0,'read sym');
		while cont do begin
trace(inxend,'inxend');
trace(k,curchar);
trace(ord(curchar),'curchar');
			if curchar = ' ' then begin
trace(inxend,'inxend end sym');
  				(*buf[0]:=chr(k-1);
trace(inxend,'inxend 8');*)
				buf[k]:=chr(0);
trace(inxend,'inxend null added');
				nextchar;
trace(inxend,'inxend next read');
(*tracelevel:=1;*)
trace(0,buf);
if (buf[1]='-') or ((ord(buf[1])>=ord('0')) and (ord(buf[1])<=ord('9'))) then
 x:=atoi(buf)
 else begin
(*tracelevel:=0;*)
				x := symbol(buf);
(*tracelevel:=1;*)
trace(x,buf);
(*tracelevel:=0; *)
end;
trace(x,buf);
(*tracelevel:=0;*)
trace(inxend,'inxend 6');
trace(x,buf);
				cont := false;
trace(inxend,'inxend 2');
			end else begin
                                (*if buf = ' ' then buf := curchar 
                                else buf := concat (buf, curchar);*) 
                                 buf[k]:=curchar;
			   k:=k+1;
trace(k,'on continue');
				nextchar;
     end;
    end;

   end;
trace(inxend,'inxend 3');
trace(x,'t12');
	if not (x = 0) then begin
trace(0,'t14');
		if inxend=0 then begin
tracelevel:=1;
			readexpr := x;
			cont1 := false;
		end else begin
trace(inxend,'inxend 4');
			inxbuf[inxend]:=x;
			inxend:=inxend+1;
		end;
trace(x,'t15');
	end;
trace(x,'t16');


 end;
trace(x,'t17');
writemem;
while true do evol(x);
end;
 
function readexpr1 : expr;
var
	i, j, k, v, l: integer;
	x: expr;
	cont1, cont: boolean;
begin
    cont1 := true;
    l := -1;
    while cont1 do begin
x:=0;
    	readexpr1 := 0;
	while curchar = ' ' do nextchar;
	if curchar = '[' then begin
		nextchar;
		l := l+1;
trace(l,'level +');
		n[l] := 0;
	end else if curchar = ']' then begin
		nextchar;
                x := alloc (n[l]);
(*tracelevel:=1;*)
                for j:=0 to n[l]-1 do
			put (x, j, a[l,j]);
tracelevel:=0;
		l := l-1;
trace(l,'level -');
	end else begin
		i := 0;
		buf := ' ';
                    k:=1;
		cont := true;
trace(l,'read sym');
		while cont do begin
trace(k,curchar);
trace(ord(curchar),'curchar');
			if curchar = ' ' then begin
  				buf[0]:=chr(k-1);
				buf[k]:=chr(0);
				nextchar;
				x := symbol(buf);
trace(x,buf);
				cont := false;
			end else begin
                                (*if buf = ' ' then buf := curchar 
                                else buf := concat (buf, curchar);*) 
                                 buf[k]:=curchar;
			   k:=k+1;
trace(k,'on continue');
				nextchar;
			end;
trace(0,'t10');			
				
		end;
trace(0,'t11');

	end;
trace(x,'t12');
	if not (x = 0) then begin
trace(l,'t14');
		if l < 0 then begin
			readexpr1 := x;
			cont1 := false;
		end else begin
			a[l,n[l]] := x;
			n[l] := n[l] + 1;
		end;
trace(x,'t15');
	end;
trace(x,'t16');
    end;

end;

function compil (s: string) : expr;
var x: expr;
begin
trace(2,inbuf);
        (*inbuf:=s;*)
        inptr:=1;
trace(25,inbuf);
(*tracelevel:=1;*)
trace(3,inbuf);
	x := readexpr;
(*tracelevel:=1;*)
trace(x,'ret by readexpr');
        compil := x;
trace(x,'end compil');
end;

procedure writemem;
var i,j,k: integer;
begin
  for i:=0 to freeptr div 10 do
   begin

   for j:=0 to 9 do 
begin
k:=i*10+j;
   write(k,':', get(0,k),';');
end;
   readln(buf);
   end;

 writeln;
end;

begin
tracelevel:=0;
          writeln('bonjour');
          readln(buf);
trace(0,'test');        
	init;
trace(0,'init prim');
	init_prim;
 for i:=0 to nsymbols-1 do
begin
  writeln ('symbol ',i,' : ',symbols[i],' '
,name(symbols[i]));
readln(buf);
end;
          writeln('compil');
(*tracelevel:=1;*)
	inbuf:='[[1 select [nil [salut print_symbol return ]] deretour print_symbol hello '' [print_symbol return ] eval bonjour print_symbol quit ] nil nil nil ]';
(* 134 print_int '' [rebonjour ] @ print_symbol salut 400 ! 400 @ print_symbol  
3 alloc print_int 5 5 = print_int 5 6 = print_int hello '' [print_symbol return ] eval getctx @ @ print_int 1 select nil [hi! print_symbol return ] bye print_symbol quit ] nil nil nil ]'; *)
trace(1,inbuf);
(*tracelevel:=1;*)
ctx:=compil(inbuf);
tracelevel:=1;
trace(ctx,'ctx');
 (*        writeln('interp');*)
writemem;
tracelevel:=1;
	while true do evol(ctx);
	1:
	writeln ('Au revoir');
end.
