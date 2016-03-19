
(define mp (lambda (x)
 (if (not (pair? x)) x
  (if (eq? ': (car x)) (list (mp (cdr x)))
   (cons (mp (car x)) (mp (cdr x)))))))

(eval (mp '(begin

 (require (lib "srpersist.ss" "srpersist"))
 (define henv (alloc-env))
 (define hdbc (alloc-connect henv))
 (driver-connect hdbc "" 'sql-driver-prompt) 

 (define (fmserver)

  (printf "fmserver scheme - ")

  (printf "creating listener - ")
  (let ((listener (tcp-listen 6040)))

   (let loop ()
    (printf "accepting - ")
    (let ((client (call-with-values (lambda () (tcp-accept listener)) (lambda l l))))
     (printf "accepted - ")
 
     (server-loop client)
     (loop))
 )))
 
 (define login "")
 (define pass "")

 (define (read-line2 ch)
  (let ((str (read-line ch)))
   (substring str 0 (- (string-length str) 2))))

 (define read-line1 read-line)

 (define (lire-fichier dir file)
  (let ((path (string-append dir "\\" file)))
   (let ((f (open-input-file path)))
    (let ((str (read-line1 f)))
     str
    ))))

 (define (server-loop client) :
  let ((buf "") (cont 'yes)) 
   (printf "reading - ")
   (let loop ()
    (set! buf (read-line1 (car client)))
    (if (not (string? buf)) (loop)))
   (printf "received: ") (print buf) (printf " - ") 
   (if (string=? buf "BASEFAX") 
    (begin (printf "basefax - ") :
     let ((dirfax : read-line1  : car client))
     (printf "dirfax:") (printf dirfax) (printf " - ") :
     let ((language : read-line1 : car client))
      (printf "language:") (printf language) (printf " - ") :
      let ((basedroits (lire-fichier dirfax "baserights.txt"))
           (basefax    (lire-fichier dirfax "basefax.txt"))
           (faxdir     (lire-fichier dirfax "faxdir.txt")))
       (fprintf (cadr client)         
 "ID\tIdentifiant\t30\nCNX\tDate\t20\nTIME\tDurée\t5\nPGENB\tPages\t5\nHGN\tStatus\t5\nFILE\tFichier\t15\n\n") )
     :
   if (string=? buf "USER") 
    (set! login : read-line1 : car client) :
   if (string=? buf "PASS") 
    (begin (set! pass : read-line1 : car client)   
     (fprintf (cadr client) "LOGIN\n1\n")) :
   if (string=? buf "LISTREC")    
    (let ((hstmt (alloc-stmt hdbc))) 
     ; (prepare hstmt "USE FAXBT")
     ; (sql-execute hstmt)
     (prepare hstmt "SELECT * FROM rec")
     (sql-execute hstmt)
     : let ((id-buf (make-buffer '(sql-c-char 120))))
     : let ((id-ind (make-indicator)))
     (bind-col hstmt 1 id-buf id-ind)
   
     : let ((cnx-buf (make-buffer '(sql-c-char 120))))
     : let ((cnx-ind (make-indicator)))
     (bind-col hstmt 3 cnx-buf cnx-ind)

     : let ((time-buf (make-buffer 'sql-c-slong)))
     : let ((time-ind (make-indicator)))
     (bind-col hstmt 5 time-buf time-ind)

     : let ((pgenb-buf (make-buffer 'sql-c-slong)))
     : let ((pgenb-ind (make-indicator)))
     (bind-col hstmt 6 pgenb-buf pgenb-ind)

     : let ((hgn-buf (make-buffer 'sql-c-slong)))
     : let ((hgn-ind (make-indicator)))
     (bind-col hstmt 7 hgn-buf hgn-ind)

     : let ((file-buf (make-buffer '(sql-c-char 120))))
     : let ((file-ind (make-indicator)))
     (bind-col hstmt 2 file-buf file-ind)
  
     (with-handlers
      ([(lambda (exn) (exn-no-data? exn))
        (lambda (exn) (printf "End of data\n") (fprintf (cadr client) "\n"))])
      (let loop ()
        (fetch hstmt)
	(let ((buf (string-append 
		"<MESSAGE><BASE>FAXBT</BASE><ID>"
		(car (read-buffer id-buf))
		"</ID><CNX>"
		(car (read-buffer cnx-buf))
		"</CNX><TIME>"
		(number->string (car (read-buffer time-buf)))
		"</TIME><PGENB>"
		(number->string (car (read-buffer pgenb-buf)))
		"</PGENB><HGN>"
		(number->string (car (read-buffer hgn-buf)))
		"</HGN><FILE>"
		(car (read-buffer file-buf))
		"</FILE><EXT></EXT></MESSAGE>\n")))
	 ; (printf buf)
	 (fprintf (cadr client) buf)
	) 
        (loop))
      )     
   ) :
   if (string=? buf "END")
    (begin (printf "END - ") (set! cont 'no)) :
   begin
   (printf "Commande:") (printf buf) (printf " - ")       
   ) 
   (if (eq? cont 'yes) (server-loop client))
 )
 
)))



