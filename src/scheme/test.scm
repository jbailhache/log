
(define (intfrom n)
 (delay (cons n (intfrom (+ 1 n)))) )

