
(define call/cc call-with-current-continuation)

(define lctxs '())

(define-syntax alt 
 (syntax-rules ()
  ((_ a b) (if (alt1 '#t '#f) a b)) ))

(define (alt1 a b)
 (call/cc (lambda (k)
  (set! lctxs (cons (lambda () (k b)) lctxs))
  (k a))))

(define (end)
 (if (not (pair? lctxs)) '()
  (let ((ctx (car lctxs)))
   (set! lctxs (cdr lctxs))
   (ctx))))
 