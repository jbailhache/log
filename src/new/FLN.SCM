; fl.scm
 
; util

(define (trace x) (write x))

(define (mapkar l f)
 (if (not (pair? l)) l
  (cons (f (car l))
   (mapkar (cdr l) f))))

(define (mapk l f)
 (if (not (pair? l)) l
   (begin (f (car l))
    (mapk (cdr l) f)
)))

(define (contient l f)
 (and (pair? l)
 (or (f (car l))
  (contient (cdr l) f))))

(define (inter a b)
 (if (> a b) '()
  (cons a (inter (+ 1 a) b)) ) )

(define (append a b)
 (if (not (pair? a)) b
   (cons (car a) (append (cdr a) b))
))

; (define digits (string->list "0123456789"))
(define digits '(0 1 2 3 4 5 6 7 8 9))
(define (digit? c) 
 (contient digits (lambda (d) (eq? d c))))

(define (eqs1 a b)
 (and (string? a) (string? b)
  (string=? a b)))

(define (eqs a b) (equal? a b))

(define (sign x)
 (if (eq? x 0) 0
  (if (> x 0) 1 -1)))

(define (nthcdr n l)
 (if (eq? n 0) l
  (nthcdr (- n 1) (cdr l))))

(define (sublist a b)
 (if (eq? a b) '()
  (if (not (pair? a)) a
    (cons (car a) (sublist (cdr a) b)))))


; fl expr
      
(define maxmem 500)
; (define mem (make-vector 500 '()))
(define presize 1)
 (define freeptr 1)

(define (error s) (write s))

(define (at x) (car x))

(define (plus x y)
 (cond ((number? x)
  (cond ((number? y) (+ x y))
    ((pair? y) (nthcdr x y))
    (#t (list 'plus x y))))
  ((pair? x)
    (cond ((number? y) (nthcdr y x))
      (#t (list 'plus x y))))
  (#t (list 'plus x y))))

; (define (get x i) (vector-ref mem (+ x i)))
; (define (put x i y) (vector-set! mem (+ x i) y))

(define (get x i) (car (nthcdr i x)))

(define (put x i y) 
; (trace 'put) (trace x) (trace i) (trace y)
(set-car! (nthcdr i x) y)) 



(define (alloc1 n)
 (let ((x freeptr))
  (set! freeptr (+ freeptr n presize))
  (if (>= freeptr maxmem)
   (error "mem over") '())
  (put x -1 n)
  x))

(define (alloc n)
 (if (eq? n 0) '() 
  (cons '() (alloc (- n 1)))))

;(define (store a from n)
;(let ((x (alloc n)))
;  (mapk (inter 1 (- n 1)) (lambda (i)
;    (put x i (vector-ref a (+ from i)))
;)))

(define (flcons1 a b)
 (let ((c (alloc 2)))
  (put c 0 a)
  (put c 1 b)
  c))


(define (flcar1 a) (get a 0))
(define (flcdr1 a) (get a 1))

(define (flcons a b) (cons a b))
(define (flcar a) (car a))
(define (flcdr a) (cdr a))

(define (name x) (get x 3))
(define (copyname x s) (put x 3 s))

(define pcode 0)
(define pdef 1)
(define pimdf 2)

(define maxsymbols 100)
(define nsymbols 0)
(define symbols (make-vector 100 0))

(define (symbol s)
 (or (contient (inter 0 (- nsymbols 1))
      (lambda (i) (and (string=? s (name (vector-ref symbols i)))
                       (vector-ref symbols i))))
  (if (>= nsymbols maxsymbols) (error "sym over")
  (let ((x (alloc 4)))
   (copyname x s)
   (vector-set! symbols nsymbols x)
   (set! nsymbols (+ 1 nsymbols))
   x))))

 

; fl store

(define (store a from n)
(let ((x (alloc n)))
  (mapk (inter 0 (- n 1)) (lambda (i)
    (put x i (vector-ref a (+ from i)))
)) x ))

; fl readexpr

(define inptr 0)
(define inbuf "")
(define src 
"[[bonjour print_symbol quit ] nil nil nil ] ")

(define (curchar) (string-ref inbuf inptr))

(define (nextchar)
 (set! inptr (+ 1 inptr))
 ;(trace (curchar))
 (curchar))

(define maxinx 100)
(define inxbuf (make-vector 100 '()))
(define inxbegin 0)
(define inxend 0)


(define (readsym l)
 (if (eq? (curchar) '#\ )
;  (begin (nextchar) (symbol (list->string l)))
  (begin (nextchar) l)
  (let ((l1 (append l (list (curchar)))))
    (nextchar)
    (readsym l1))))

(define (readint s n)
 (write "readint") (write s) (write n)
 (if (eq? (curchar) #\ ) 
  (begin (nextchar) (* s n))
   (if (eq? (curchar) #\-) 
    (begin (nextchar)
     (readint (* -1 s) n))
    (let ((n1 (+ (* 10 n) (- (char->integer (curchar)) (char->integer #\0)))))
    (nextchar) (readint s n1)))))

  
; fl readexpr1

(define (readexpr1)
(trace inptr)
(trace inxbegin)
(trace inxend)
  (cond ((eq? (curchar) '#\[)
    (nextchar)
    (vector-set! inxbuf inxend inxbegin)
    (set! inxend (+ 1 inxend))
    (set! inxbegin inxend)
    #f)
   ((eq? (curchar) '#\])
    (nextchar)
    (let (( x (store inxbuf inxbegin (- inxend inxbegin))))
    (set! inxend (- inxbegin 1))
    (set! inxbegin (vector-ref inxbuf inxend))
    (trace "x(1):") (trace x) 
    x))
   ((or (digit? (curchar)) (eq? #\- (curchar))) (readint 1 0))
   (#t (readsym '()))
))

(define (readspaces)
 (if (eq? (curchar) '#\ )
  (begin (nextchar) (readspaces))
  ()))

(define (readexprloop)
(trace 't3)
 (readspaces)
 (trace 't4)
 (let ((x (readexpr1)))
  (trace "x(2):") (trace x)
  (if x
   (if (eq? inxend 0) 
    x
    (begin
 (trace "set")  (trace inxend) (trace x)
     (vector-set! inxbuf inxend x)
(trace 't1)
     (set! inxend (+ 1 inxend))
(trace 't2)
     (readexprloop)
    ))
   (readexprloop)))
)

(define (readexpr s)
 (set! inbuf s)
 (set! inptr 0)
 (set! inxbegin 0)
 (set! inxend 0)
 (readexprloop))

 

; evol defs 

(define running '#t)

(define (scar a) (flcar a))
(define (scdr a) (flcdr a))
(define (scons a b) (flcons a b))

(define (snth n l)
 (if (eq? n 0) (scar l)
  (snth (- 1 n) (scdr l))))

(define pph 0)
(define ppc 1)
(define prtn 2)
(define pstk 3)
(define penv 4)
(define pbuf 5)
(define porg 6)
(define pbgn 7)
(define pend 8)

(define run 1)
(define read 2)


; fl evolrun

(define (evolrun evol ctx)
 (let ((pc (get ctx ppc)))
  (let ((instr (get pc 0)))
  (let ((ni (name instr)))
 (trace ni)
   (set! pc (plus 1 pc))
   (put ctx ppc pc)
   (cond
    ((eqs "print_symbol" (name instr))
     (write (name (scar (get ctx pstk))))
     (put ctx pstk (scdr (get ctx pstk)))
    )
    ((eqs ni "print_int")
     (write (scar (get ctx pstk)))
     (put ctx pstk (scdr (get ctx pstk))))
    ((eqs ni "+")
      (put ctx pstk (scons (plus (scar (get ctx pstk)) (scar (scdr (get ctx pstk)))) (scdr (scdr (get ctx pstk))))))
  ((eqs ni "*")
      (put ctx pstk (scons (* (scar (get ctx pstk)) (scar (scdr (get ctx pstk)))) (scdr (scdr (get ctx pstk))))))
  ((eqs ni "sublist")
    (put ctx pstk (scons (sublist (scar (get ctx pstk)) (scar (scdr (get ctx pstk)))) (scdr (scdr (get ctx pstk))))))
 ((eqs ni "=")
     (put ctx pstk (scons (if (eq? (scar (get ctx pstk)) (scar (scdr (get ctx pstk)))) 1 0) (scdr (scdr (get ctx pstk))))))
    ((eqs ni "sign")
      (put ctx pstk (scons (sign (scar (get ctx pstk))) (scdr (get ctx pstk)))))
    ((eqs ni "@") 
      (put ctx pstk (scons (get (scar (get ctx pstk)) 0) (scdr (get ctx pstk)))))
    ((eqs ni "!")
     (put (scar (get ctx pstk)) 0
       (scar (scdr (get ctx pstk))))
     (put ctx pstk (scdr (scdr (get ctx pstk)))))
 ((eqs ni "get")
   (put ctx pstk (scons (get (scar (get ctx pstk)) (scar (scdr (get ctx pstk)))) (scdr (scdr (get ctx pstk))) )))
 ((eqs ni "put")
    (put (scar (get ctx pstk)) 
     (scar (scdr (get ctx pstk)))
     (scar (scdr (scdr (get ctx pstk))))
    )
    (put ctx pstk (scdr (scdr (scdr (get ctx pstk)))))
  )
   ((eqs ni "alloc")
     (put ctx pstk (scons (alloc (scar (get ctx pstk))) (scdr (get ctx pstk)))))
   ((eqs ni "'")
    (put ctx pstk (scons (get pc 0) (get ctx pstk)))
    (put ctx ppc (plus 1 pc)))
   ((eqs ni "eval")
     (put ctx prtn (scons pc (get ctx prtn))) 
     (put ctx ppc (scar (get ctx pstk)))
     (put ctx pstk (scdr (get ctx pstk))))
   ((eqs ni "then")
 (put ctx prtn (scons (scdr (scdr pc)) (get ctx prtn))) 
     (put ctx ppc (get pc (- 1 (scar (get ctx pstk)))))
     (put ctx pstk (scdr (get ctx pstk))))
   ((eqs ni "return")
     (put ctx ppc (scar (get ctx prtn)))
     (put ctx prtn (scdr (get ctx prtn))))
   ((eqs ni "getctx")
     (put ctx pstk (scons ctx (get ctx pstk))))
   ((eqs ni "evol")
     (apply evol (list (scar (get ctx pstk)))))
   ((eqs ni "<")
     (put ctx penv (scons (scar (get ctx pstk)) (get ctx penv)))
     (put ctx pstk (scdr (get ctx pstk))))
   ((eqs ni ">")
     (put ctx penv (scdr (get ctx penv))))
    ((eqs ni "%")
      (put ctx pstk (scons (snth (scar pc) (get ctx penv)) (get ctx pstk)))
      (put ctx ppc (plus 1 pc)))
    ((eqs ni "curchar")
      (put ctx pstk (scons (char->integer (curchar)) (get ctx pstk))))
    ((eqs ni "nextchar")
      (nextchar) ctx)
    ((eqs ni "read_symbol")
       (put ctx pstk (scons (readsym '()) (get ctx pstk))))
    ((eqs ni "read_expr1")
      (put ctx pstk (scons (readexprloop) (get ctx pstk))))
    ((eqs ni "read_expr")
       (put ctx pbgn (get ctx porg))
       (put ctx pend (get ctx porg))
       (put ctx pph read))
 ((eqs ni "nest1")
   (put ctx penv 
     (scons (get ctx porg)
      (scons (get ctx pbgn)
       (scons (get ctx pend) 
        (get ctx penv)))))
     (put ctx porg (get ctx pend))
 )
 ((eqs ni "endnest1")
  (put ctx porg (scar (get ctx penv)))
  (put ctx pbgn (scar (scdr (get ctx penv))))
  (put ctx pend (scar (scdr (scdr (get ctx penv)))))
  (put ctx penv (scdr (scdr (scdr (get ctx penv))))))
 ((eqs ni "nest")
   (put (get ctx pbuf) (get ctx pend) (get ctx porg))
   (put (get ctx pbuf) (+ 1 (get ctx pend))  (get ctx pbgn)) 
   (put ctx porg (+ 2 (get ctx pend))))
 ((eqs ni "endnest")
  (put ctx pend (- (get ctx porg) 2))
;  (put ctx porg (get (get ctx pend) 0))
  (put ctx porg (get (get ctx pbuf) (get ctx pend) ))
  (put ctx pbgn (get (get ctx pbuf) (+ 1 (get ctx pend)) )))

    ((eqs ni "place") (trace 'place)
; (put (get ctx pend) 0 (scar (get ctx pstk)))
      (put (get ctx pbuf) (get ctx pend)  (scar (get ctx pstk)))
      (put ctx pend (+ 1 (get ctx pend)))
      (put ctx pstk (scdr (get ctx pstk))))
    ((eqs ni "readon")
      (put ctx pph read))
    ((eqs "quit" (name instr))
     (set! running '#f))
    ((eq? '() (get instr pdef)) (put ctx pstk (flcons instr (get ctx pstk))))
 (#t (put ctx prtn (scons (get ctx ppc) (get ctx prtn)))
  (put ctx ppc (get instr pdef)))

)))))



; fl evolread

(define (retread ctx x)
 (trace 'retread)
 (trace ctx)
 (trace x)
 (let (
   (ixbuf (get ctx pbuf))
   (ixorg (get ctx porg))
   (ixbgn (get ctx pbgn))
   (ixend (get ctx pend)))
   (if (eq? ixend  ixorg)
     (begin
      (put ctx pstk (scons x (get ctx pstk)))
      (put ctx pph run)
     )
    (begin 
     (put ixbuf ixend x)
     (put ctx pend (+ 1 ixend))
     ))))

(define (evolread ctx) 
 (trace 'evolread)
 (trace ctx)
 (readspaces)
 (let ((ixorg (get ctx porg))
   (ixbgn (get ctx pbgn))
   (ixend (get ctx pend))
   (ixbuf (get ctx pbuf)))
 (trace 'curchar)
 (trace (curchar))
 (cond
  ((eq? (curchar) #\&)
    (nextchar)
    (put ctx prtn (scons (get ctx ppc) (get ctx prtn)))
   (put ctx ppc (get ixbuf (- ixend 1)))
;   (put ctx ppc (plus (- ixend 1) ixbuf))
    (put ctx pend (- ixend 1))
    (put ctx pph run)
   )
  ((eq? (curchar) #\[)
    (nextchar)
    (put ixbuf ixend  ixbgn)
    (put ctx pend (+ 1 ixend))
    (put ctx pbgn (+ 1 ixend)))
   ((eq? (curchar) #\])
     (nextchar)
;     (let ((x (store mem ixbgn (- ixend ixbgn))))
      (let ((x (sublist (plus ixbgn ixbuf) (plus  ixend ixbuf))))
      (put ctx pend (- ixbgn 1))
      (put ctx pbgn (get ixbuf (- ixbgn 1) ))
      (retread ctx x)))
     ((or (digit? (curchar)) (eq? #\- (curchar))) (retread (readint 1 0)))
   ;  (#t (trace 'symbol) (retread ctx (readsym '())))
 (#t (trace 'sym)
   (let ((x (readsym '())))
    (if (eq? '() (get x pimdf))
     (retread ctx x)
     (begin
       (put ctx prtn (scons (get ctx ppc) (get ctx prtn)))
    (put ctx ppc (get x pimdf))
;    (put ctx pend (- ixend 1))
    (put ctx pph run) ))))
 ))
)

(define (evol ctx)
(cond
 ((eq? run (get ctx pph))
   (evolrun evol ctx))
 ((eq? read (get ctx pph))
  (evolread ctx))
  (#t ctx)))
 
(define (ctxini prog)
 (let ((ctx (alloc 10))
 (inxorg (alloc 60)))
  (put ctx pph run)
  (put ctx ppc prog)
  (put ctx prtn 0)
  (put ctx pstk 0)
  (put ctx penv 0) 
  (put ctx pbuf inxorg)
  (put ctx porg 0)
  (put ctx pbgn 0)
  (put ctx pend 0)
  ctx
))

(define (toplevel ctx)
 (if running 
  (begin
   (evol ctx)
   (toplevel ctx))
  'bye))

(define (testfl)
 (let ((ctx (readexpr src)))
  (print ctx)
  (toplevel ctx)))






