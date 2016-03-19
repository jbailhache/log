
(define (call/cc f) (call-with-current-continuation f))

(define (message x) (display x))

(define (alt sc x y)
 (eval 
  (call/cc (lambda (k) (sc (list 'alt k x y))))
  user-initial-environment
   ))

(define (end sc)
 (sc (list 'end)))

(define (scheduler prog)
 (let ((ctxs '())
   (r (call/cc prog)))
    (cond ((eq? (car r) 'alt)
     (set! ctxs (cons (list (cadr r) (cadr (cddr r))) ctxs))
     (display ctxs)
     ((cadr r) (caddr r)))
    ((eq? (car r) 'end)
     (display ctxs)
     (if (pair? ctxs)
     (let ((ctx (car ctxs)))
      (set! ctxs (cdr ctxs))
      ((car ctx) (cadr ctx))
     ) 'exhausted)
))))

(define (prog1 sc)
 (let ((x (alt sc ''aaa ''bbb)))
  (message x)
  (end sc)))

