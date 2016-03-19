program fl (input, output);

label 1;

const 
	size = 2;
	maxmem = 5000;
	maxsymbols = 1000;
	presize = 2;
	plen = -1;
	ppc = 0;
	pret = 1;
	pstk = 2;
	penv = 3;
	pcode = 0;
	pval = 1;
        plenstr = 2;
	pstr = 3;
	
type 
	item = integer;
	expr = integer;
	exprsize = integer;
	exprstr = record case integer of
		1: (x: expr);
		2: (s: string[size]);
			end;

var
	freeptr: expr;
	mem : array [0..maxmem] of expr;
	
	nsymbols: integer;
	symbols: array [0..maxsymbols] of expr;
	
	instr_print_symbol,
	instr_quit: expr;

	ctx: expr;

        inbuf: string[1000];
        inptr: integer;
	
	a: array[0..30,0..120] of expr;
	n: array[0..30] of integer;

	buf: string[1000];

function get (x: expr; i: integer) : expr;
begin
	get := mem[x+i];
end;

procedure put (x: expr; i: integer; y: expr);
begin
        mem[x+i] := y;
end;

procedure init;
begin
	freeptr := presize;
	nsymbols := 0;
end;

function len (p: expr) : integer;
begin
	len := mem[p-1];
end;

procedure quit;
begin
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
	mem[x-1] := n;
        alloc := x;
end;

function cons (a: expr; b: expr) : expr;
var c: expr;
begin
	c := alloc(2);
	put (c, 0, a);
	put (c, 1, b);
        cons := c;
end;



function name (x: expr) : string;
var 
	s: string;
	l, i: integer;
	u: exprstr;

begin
        l := get (x, plenstr);
        s := '';
        for i:=0 to (l+1) div 2-1 do
	begin
                u.x := get (x, pstr+i);
                s := concat (s, u.s);
	end;
        name := s;
end;


procedure copyname (x: expr; s: string);
var
	l, i, j: integer;
	u: exprstr;
begin
        l := length(s);
        put (x, plenstr, l);
        for i:=0 to (l+1) div 2-1 do
	begin
                u.s := s[i*size]; 
                for j:=1 to size-1 do
			if j-i*size<l then
                                u.s := concat (u.s, s[j+i*size]);
	end;
	
	
end;

function symbol (s: string) : expr;
var 
	i: integer;
	found: boolean;
	x: expr;
begin
	found := false;
	i := 0;
        while (not found) and (i < nsymbols) do
	begin
                if s = name(symbols[i]) then found := true;
		if nsymbols >= maxsymbols then  
		begin
			error ('Symbol overflow');
			quit;
		end;
					
	end;
        x := alloc (3 + (length(s)+1) div size);
	copyname (x, s);
	symbols[nsymbols] := x;
	nsymbols := nsymbols + 1;
	symbol := x;
end;

procedure evol (ctx: expr);
var
	instr: expr;
	
begin
	instr := get (ctx, ppc);
	 
	if instr = instr_print_symbol then
                writeln(name(get(get(ctx,pstk),0)))
	else if instr = instr_quit then
                quit
	else put (ctx, pstk, cons(instr,get(ctx,pstk)));
end;

procedure init_prim;
begin
	instr_print_symbol := symbol('print_symbol');
	instr_quit := symbol('quit');
end;

function curchar : char;
begin
	curchar := inbuf[inptr];
end;

procedure nextchar;
begin
        inptr := inptr + 1;
end;

function readexpr : expr;
var
	i, j, v, l: integer;
	x: expr;
	cont1, cont: boolean;
begin
    cont1 := true;
    l := -1;
    while cont1 do begin
    	readexpr := 0;
	while curchar = ' ' do nextchar;
	if curchar = '[' then begin
		nextchar;
		l := l+1;
		n[l] := 0;
	end else if curchar = ']' then begin
		nextchar;
                x := alloc (n[l]);
                for j:=0 to n[l]-1 do
			put (x, j, a[l,j]);
		l := l-1;
	end else begin
		i := 0;
		buf := ' ';
		cont := true;
		while cont do begin
			if curchar = ' ' then begin
				nextchar;
				x := symbol(buf);
				cont := false;
			end else begin
                                if buf = ' ' then buf := curchar 
                                else buf := concat (buf, curchar);
				nextchar;
			end
			
				
		end;

	end;
	if not (x = 0) then begin
		if l < 0 then begin
			readexpr := x;
			cont := false;
		end else begin
			a[l,n[l]] := x;
			n[l] := n[l] + 1;
		end;
	end;
    end;

end;

function compil (s: string) : expr;
var x: expr;
begin
	x := readexpr;
        compil := x;
end;

begin
	init;
	init_prim;
	ctx := compil ('[[bonjour print_symbol quit ] nil nil nil ]');
	while true do evol(ctx);
	1:
	writeln ('Au revoir');
end.
