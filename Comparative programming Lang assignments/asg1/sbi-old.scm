#!/afs/cats.ucsc.edu/courses/cmps112-wm/usr/racket/bin/mzscheme -qr
;; $Id: sbi.scm,v 1.2 2015-09-23 17:11:09-07 - - $
;;
;; NAME
;;    sbi.scm - silly basic interpreter
;;
;; SYNOPSIS
;;    sbi.scm filename.sbir
;;
;; DESCRIPTION
;;    The file mentioned in argv[1] is read and assumed to be an
;;    SBIR program, which is the executed.  Currently it is only
;;    printed.
;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define *function-table* (make-hash))

(define *label-table* (make-hash))

(define *variable-table* (make-hash))

(define *goto-flag* #f)

(define (toggle-goto-flag)
        (if *goto-flag*
            (set! *goto-flag* #f)
            (set! *goto-flag* #t))
        *goto-flag*
)

(define (atom? x) (not (pair? x)))

(define (make-variable key val)
    (newline)
    (display "make-variable")
    (newline)
    (display key)
    (newline)
    (display val)
    (newline)
    (hash-set! *variable-table* key val)
)

(define (my-goto label)
        (printf "got into my-goto with label ~s~n" label)
        (when (hash-has-key? *label-table* label)
              (toggle-goto-flag))
        (if (hash-has-key? *label-table* label)
            (hash-ref! *label-table* label 0)
            (printf "ERROR: INVALID GOTO DETECTED~n"))
)

(define (my-print . args)
        (map display args)
        (newline)
)

(for-each
    (lambda (item) (hash-set! *function-table* (car item) (cadr item)))
    `((+ ,+)
      (- ,-)
      (* ,*)
      (/ ,(lambda (x y) (/ x (+ y 0.0))))
      (% ,modulo)
      (^ ,expt)
      (abs ,abs)
      (acos ,acos)
      (asin ,asin)
      (atan ,atan)
      (ceil ,ceiling)
      (cos ,cos)
      (exp ,exp)
      (floor ,floor)
      (log ,log)
      (log10 ,(lambda (x) (/ (log x) (log 10))))
      (log2 ,(lambda (x) (/ (log x) (log 2))))
      (round ,round)
      (sin ,sin)
      (sqrt ,sqrt)
      (tan ,tan)
      (trunc ,truncate)
      (dim ,+)
      (let ,make-variable)
      (goto ,my-goto)
      (if ,+)
      (print ,my-print)
      (input ,+)
      (= ,=)
      (< ,<)
      (> ,>)
      (<> ,(lambda (x y) (not (= x y))))
      (<= ,<=)
      (>= ,>=)
     )
)

(for-each
    (lambda (item) (hash-set! *variable-table* (car item) (cadr item)))
    `((e 2.718281828459)
      (pi 3.14159265359))
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define *stderr* (current-error-port))

(define *run-file*
    (let-values
        (((dirpath basepath root?)
            (split-path (find-system-path 'run-file))))
        (path->string basepath))
)

(define (die list)
    (for-each (lambda (item) (display item *stderr*)) list)
    (newline *stderr*)
    (exit 1)
)

(define (usage-exit)
    (die `("Usage: " ,*run-file* " filename"))
)

(define (readlist-from-inputfile filename)
    (let ((inputfile (open-input-file filename)))
         (if (not (input-port? inputfile))
             (die `(,*run-file* ": " ,filename ": open failed"))
             (let ((program (read inputfile)))
                  (close-input-port inputfile)
                         program))))

(define (handle-statement statement)
    (define (handle-arg argument)
            (cond ((number? argument) argument)
                  ((not (atom? argument)) (handle-statement argument))
                  ((and (hash-has-key? *variable-table* argument)
			(print (car statement))
			(string=? (car statement) "let")
		
			)
		   
		        (hash-ref! *variable-table* argument 0))
                  (else argument);;COME BACK AND FIX LATER
            )
    )
    (define asdf (map handle-arg (cdr statement)))

;; find the function
    (unless (hash-has-key? *function-table* (car statement)) (printf "complain: function not found~s~n" (car statement)) (exit 2));; check for atoms?

    (apply (hash-ref! *function-table* (car statement) printf) asdf)

)

(define (handle-line program)
    (define line (car program))
    (define next-line 0)
    (unless (null? (cdr line))
        (if (atom? (cadr line))
            (unless (null? (cddr line))
                    (set! next-line (handle-statement (caddr line))))
            (set! next-line (handle-statement (cadr line)))))
    (if *goto-flag* next-line (cdr program))
)

(define (handle-program program)
    (unless (null? program) 
            (when *goto-flag* (toggle-goto-flag))
            (handle-program (handle-line program)))
)

(define (get-label program)
    (define line (car program))
    (unless (null? (cdr line))
        (when (atom? (cadr line))
            (unless (hash-has-key? *label-table* (cadr line))
                    (hash-set! *label-table* (cadr line) program)
            )
        )
    )
)

(define (get-program-labels program)
    (unless (null? program) 
            (get-label program)
            (get-program-labels (cdr program)))
    
)

(define (write-program-by-line filename program)
    (printf "==================================================~n")
    (printf "~a: ~s~n" *run-file* filename)
    (printf "==================================================~n")
    ;;(get-program-labels program)
    ;;(handle-program program)
;;    (display program)
    (display (symbol->string (caadar program)))
    (display (equal? (caadar program) 'let))
    (newline)
    
)

(define (main arglist)
    (if (or (null? arglist) (not (null? (cdr arglist))))
        (usage-exit)
        (let* ((sbprogfile (car arglist))
               (program (readlist-from-inputfile sbprogfile)))
              (write-program-by-line sbprogfile program))))

(main (vector->list (current-command-line-arguments)))

