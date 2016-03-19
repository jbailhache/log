
(define (f1 x) (cons x x))

(define (list-of-values v)
 (call-with-values 
   (lambda () v)
   (lambda l l)))

(define mp (lambda (x)
 (if (not (pair? x)) x
  (if (eq? ': (car x)) (list (mp (cdr x)))
   (cons (mp (car x)) (mp (cdr x)))))))

(eval (mp '(begin
 (define (f3 x) : cons x : cons x x)



)))

