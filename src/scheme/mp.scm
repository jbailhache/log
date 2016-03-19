
(define (mp x) 
 (if (not (pair? x)) x
  (if (eq? (car x) ':) (list (mp (cdr x)))
   (cons (mp (car x)) (mp (cdr x))) ) ) )

(eval (mp '(begin

(define (snd x) : car : cdr x)

)) user-initial-environment)
