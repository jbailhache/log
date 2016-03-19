
(define consp pair?)
(define eq eq?)
; (define progn begin)
(define print write)

; (define extdp (lambda (x)
;       (if (consp x)
;               (if (eq (car x) ':) (list (extdp (cdr x)))
;                       (cons (extdp (car x)) (extdp (cdr x))) )
;               x)))

; (define mp (lambda (x)
;       (extdp x) ))

(define mp (lambda (x)
	(if (consp x)
		(if (eq (car x) ':) (list (mp (cdr x)))
			(cons (mp (car x)) (mp (cdr x))) )
		(if (eq x '^) 'lambda
		 (if (eq x 'def) 'define
		  (if (eq x 'progn) 'begin
		   x ) ) ) ) ))

(define mapkar1 (lambda (l f)
	(if (not (consp l)) l
		(cons (f (car l))
			(mapkar1 (cdr l) f) ) ) ))

(eval (mp '(begin

	(define mapkar : lambda (l f) :
		if (not : consp l) l :
		cons (f : car l) :
		mapkar (cdr l) f)

	(def mapkan : ^ (l f) :
		if (not : consp l) l :
		append (f : car l) :
		mapkan (cdr l) f)

	(def mapk : ^ (l f) :
		if (not : consp l) '() :
		begin (f : car l) :
		mapk (cdr l) f)

	(def == : ^ (x f) : f x)

	(def contient : ^ (l f) :
		if (not : consp l) '#f :
		or (f : car l) :
		contient (cdr l) f)

	(def extraire : ^ (l p) :
		if (not : consp l) l :
		if (p : car l) (cons (car l) : extraire (cdr l) p) :
		extraire (cdr l) p)

	(def trans : ^ (l x f) :
		if (not : consp l) x :
		f (car l) : trans (cdr l) x f)

	(def parcours : ^ (c l f) :
		and (consp l) :
		c (f : car l) :
		parcours c (cdr l) f)

	(def ensemble : ^ (egal l) :
		union egal l '())

	(def union : ^ (egal l e) :
		trans l e : ^ (x r) :
		if (contien r : ^ (y) : egal y x) r :
		cons x r)

	(def vide : ^ (x) : not : consp x)

	(def trier : ^ (ordre l) :
		interclasser ordre l '())

	(def interclasser : ^ (ordre lnt lt) :
		trans lnt lt : ^ (x l) :
		inserer ordre x l)

	(def inserer : ^ (ordre x l) :
		if (vide l) (list x) :
		if (ordre x : car l) (cons x l) :
		cons (car l) : inserer ordre x : cdr l)

	(def fusionner : ^ (ordre lt1 lt2) :
		if (vide lt1) lt2 :
		if (vide lt2) lt1 :
		if (ordre (car lt1) (car lt2))
			(cons (car lt1) : fusionner ordre (cdr lt1) lt2)
			(cons (car lt2) : fusionner ordre lt1 (cdr lt2)) )

	(def atom : ^(x) : not : consp x)

	(def equal : ^(x y) : 
		if (atom x) (eq? x y) :
		and (equal (car x) (car y))
		    (equal (cdr x) (cdr y)))
)))

(define newtop (lambda ()
	(print '>)
;       (print (eval (mp (read))))
	(define topread (mp (read)))
	(if (eq topread 'exit)
		(begin
			(print 'bye)
			(newline) )
		(begin
			(define result (eval topread))
			(print '=)
			(print result)
			(newline)
			(newtop) ) )
))

(newtop)

