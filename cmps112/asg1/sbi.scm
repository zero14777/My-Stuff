#!/afs/cats.ucsc.edu/courses/cmps112-wm/usr/racket/bin/mzscheme -qr
;; $Id: sbi.scm,v 1.2 2015-09-23 17:11:09-07 - - $
;; Jim Brower               jrbrower@ucsc.edu
;; Peter Greer-Berezovsky   pbgreerb@ucsc.eu
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

(define *function-table* (make-hash))

(define *label-table* (make-hash))

(define *variable-table* (make-hash))

(define (atom? x) (not (pair? x)))

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
   (log ,(lambda (x) (log (+ x 0.0))))
   (log10 ,(lambda (x) (/ (log (+ x 0.0)) (log 10))))
   (log2 ,(lambda (x) (/ (log (+ x 0.0)) (log 2))))
   (round ,round)
   (sin ,sin)
   (sqrt ,sqrt)
   (tan ,tan)
   (trunc ,truncate)
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

;; Reads in the lines of a .sbir file

(define (readlist-from-inputfile filename)
  (let ((inputfile (open-input-file filename)))
    (if (not (input-port? inputfile))
        (die `(,*run-file* ": " ,filename ": open failed"))
        (let ((program (read inputfile)))
          (close-input-port inputfile)
          program))))

;; Takes as input an expression in the form of a number,
;; variable or a list that represents an expression.
;;
;; Returns the evaluated expression
;; Returns #f if the expression is illegal

(define (handle-expression expression)
  (cond ((number? expression) expression)
        ((atom? expression) 
         (if (hash-has-key? *variable-table* expression)
             (hash-ref *variable-table* expression)
             (and 
              (printf "Error: uninitialized variable used~s~n" 
                expression) #f)))
        
        ((and (hash-has-key? *variable-table* (car expression))
              (vector? (hash-ref *variable-table* (car expression)))
              (>= (handle-expression (cadr expression)) 1)
              (<= (handle-expression (cadr expression))
                (vector-length (hash-ref *variable-table*
                  (car expression)))))
         (vector-ref (hash-ref *variable-table* (car expression))
           (- (handle-expression (cadr expression)) 1)))

        ((hash-has-key? *function-table* (car expression))
         (apply (hash-ref *function-table* (car expression))
           (map handle-expression (cdr expression))))
        
        (else (printf "Error: invalid expression ~s~n" expression) #f)))

;; Returns true if the argument accepted is in the
;; proper format to represent a piece of memory

(define (handle-memory args)
  (cond ((atom? args) #t)
        ((and (not (null? (cdr args))) (null? (cddr args)) 
              (handle-expression (cadr args))) #t)
        (else #f)))

;; Accepts a memory marker and length, then creates and stores
;; an array of the designated length in the *variable-table*
;; filled with 0s.

(define (handle-dim args)
  (if #t
      (hash-set! *variable-table* (caar args) (make-vector
        (handle-expression (cadar args)) 0))
      (printf "Error in handle-dim: failed to create new vector"))
  #f)

;; Accepts a list containing a memory marker and an expression.
;; Stores the evaluated expression in the *vector-table* slot
;; corresponding to the memory marker

(define (handle-let args)
  (if 
   (and (pair? args)
        (null? (cddr args))
        (handle-memory (car args))
        (handle-expression (cadr args)))
   (cond  ((atom? (car args)) (hash-set! *variable-table* (car args) 
                 (handle-expression (cadr args))))
          ((and (hash-has-key? *variable-table* (caar args))
                (vector? (hash-ref *variable-table* (caar args)))
                (>= (handle-expression (cadar args)) 1)
                (<= (handle-expression (cadar args))
                    (vector-length (hash-ref *variable-table*
                                             (caar args)))))
           (vector-set! (hash-ref *variable-table* (caar args))
                        (- (handle-expression (cadar args)) 1)
                        (handle-expression (cadr args))))
          (else 
           (printf "Error in handle-let: invalid array detected~n")))
   (printf "Error in handle-let: invalid arguments")) 
  #f)

;; Accepts a label and returns the statement that the label
;; refers to, which then gets used to direct the flow of the
;; program. If the label is not found a complaint is printed.

(define (handle-goto label)
  (if
   (and
    (atom? (car label))
    (hash-has-key? *label-table* (car label)))
   (hash-ref! *label-table* (car label) 0)
   (and (printf "Label not found~n") #f)))

;; Performs a comparison to two expressions then calls the
;; handle-goto function with the given label if the comparison
;; evaluates to true.

(define (handle-if args)
  (define condition (car args))
  (define label (cdr args))
  (if (or (equal? (car condition) '=)
          (equal? (car condition) '<)
          (equal? (car condition) '>)
          (equal? (car condition) '<>)
          (equal? (car condition) '>=)
          (equal? (car condition) '<=))
      (if (handle-expression condition)
          (handle-goto label)
          #f)
      (and (printf "Error if condition not valid~n") #f)))

;; accepts a variable list of expressions or strings and
;; prints them out in order to standard output. An error
;; is printed for every non-printable value recieved

(define (handle-print args)
  (define (print-printable printable)
    (cond ((string? printable) (display printable))
          ((handle-expression printable)
           (display (handle-expression printable)))
          (else (display "Error in handle-print: invalid printable"))))
  (map print-printable args)
  (newline)
  #f)

;; takes in list of arguments, each argument must be either a variable
;; initialized or uninitialized, or a valid element of an initialized 
;;array. 
(define (handle-input args)
  (define (read-input key inputcount)
    (unless (null? key) 
      (if (handle-memory (car args))
        (let* ((input (read)))
          (cond ((eof-object? input) (handle-let 
            (list 'inputcount -1)))
                ((real? input) (handle-let (list (car key) input))
                 (handle-let 
                  (list 'inputcount (+ inputcount 1)))
                 (read-input (cdr key) (+ inputcount 1)))      
                (else (printf "Invalid input, must be a number~n")
                      (read-input key inputcount))))
        (printf "bad key~n"))))
  (read-input args 0) #f)

;; Calls the proper function to evaluate the current statement and
;; passes in any arguments.

(define (handle-statement statement)
  (cond ((equal? (car statement) 'dim) (handle-dim (cdr statement)))
    ((equal? (car statement) 'let) (handle-let (cdr statement)))
    ((equal? (car statement) 'goto) (handle-goto (cdr statement)))
    ((equal? (car statement) 'if) (handle-if (cdr statement)))
    ((equal? (car statement) 'print) (handle-print (cdr statement)))
    ((equal? (car statement) 'input) (handle-input (cdr statement)))
    (else (printf "Error in handle-statement~n"))))

;; Splits up the line into line number, label and/or statement
;; then calls handle-statement. Check if a goto occured and
;; determines what the next line is.

(define (handle-line program)
  (define line (car program))
  (define (run-line)
    (if (not (null? (cdr line)))
        (if (atom? (cadr line))
            (if (not (null? (cddr line)))
                (handle-statement (caddr line)) #f)
            (handle-statement (cadr line))) #f))
  (let ((x (run-line)))
    (if (not x) (cdr program) x)))

;; Runs the program line by line until termination

(define (handle-program program)
  (unless (null? program) 
    (handle-program (handle-line program))))

;; Checks if the label isn't already in the *label-table* 
;; and then inserts them into the *label-table*

(define (get-label program)
  (define line (car program))
  (unless (null? (cdr line))
    (when (atom? (cadr line))
      (unless (hash-has-key? *label-table* (cadr line))
        (hash-set! *label-table* (cadr line) program)))))

;; Detects any labels in the program and calls get-label

(define (get-program-labels program)
  (unless (null? program) 
    (get-label program)
    (get-program-labels (cdr program))))

;; Reads in the program file, scans the program for labels to
;; insert into the *label-table* then runs the program.

(define (main arglist)
  (if (or (null? arglist) (not (null? (cdr arglist))))
    (usage-exit)
    (let* ((sbprogfile (car arglist))
           (program (readlist-from-inputfile sbprogfile)))
      (get-program-labels program)
      (handle-program program))))

(main (vector->list (current-command-line-arguments)))

