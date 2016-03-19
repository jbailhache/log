
(define make-vector (lambda (n x)
 (if (<= n 0) '()
  (cons x (make-vector (- n 1) x)))))

(define vector-ref (lambda (v i)
 (if (<= n 0) '(car v)
  (vector-ref (cdr v) (- i 1)))))

(define vector-set1 (lambda (v i x)
 (if (<= n 0) (set-car! v x)
  (vector-set1 (cdr v) (- i 1) x))))

(define vector-set! (lambda (v i x)
 (vector-set1 v i x)
 v))



