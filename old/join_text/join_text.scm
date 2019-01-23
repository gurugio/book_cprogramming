#!/usr/bin/guile \
-e main -s
!#

;; How to run this program
;; $ guile join_text.scm test_source.c test_desc.txt

; for GUILE to use read-line, write-line
(use-modules (ice-9 rdelim))


;;-----------         WARNING!!              ----------
;; This is very initial version and there is very weak error handlier.
;; There is no warranty for error in join-command.
;; Be sure the format: $$src-name start-line end-line
;; The start-line and end-line must be valid number, not exceed maximum line.
;;---------------------------------------------------------------------------------

; - read book text which has join-command, and source files which has indicator.
; - source files and text file are specified by command arguments.
; - join book and source text and generate complete book file.
; look example for detail: book.txt, src1.c, src2.c, src3.c

; args: (join_text.scm book.txt 1st 2nd 3rd ...)
(define (main args)

  (check-args 3 (length args))
  
  ;; Read every source files and make table.
  ;; The table can be indexed by filename and returns text data in the file.
  (let ((book-file-name (get-book-file-name args))
        (src-file-names (get-src-file-names args)))
    (make-src-table src-file-names src-text-table)
    (make-final-book book-file-name src-text-table)
    );; end of let
;  ) ; end for REPL
  (quit 0)) ; end for shell

(define (init-src-file-table)
  (list '*src-table*))

(define src-text-table (init-src-file-table))

(define (check-args min count)
  (if (> min count)
      (begin
        (display "USAGE: $ join_text.scm book-text-file 1st-src 2st-src ...")
        (newline)
        (quit 1))))
(define (get-book-file-name args)
  (cadr args))
(define (get-src-file-names args)
  (cddr args))

; lookup and insert! are from SICP
(define (lookup key table)
  (let ((record (assoc key (cdr table))))
    (if record
        (cdr record)
        #f)))

(define (insert! key value table)
  (let ((record (assoc key (cdr table))))
    (if record
        (set-cdr! record value)
        (set-cdr! table
                  (cons (cons key value) (cdr table)))))
  'ok)

; read a file port and return a list of text lines
(define (read-file read-port)
  (define (read-lines lst)
    (let ((line (read-line read-port)))
      (if (eof-object? line)
          lst
          (read-lines (append lst (list line))))))
  (read-lines '()))


(define (get-command line)
  ;; (display (list "get-command:" line))
  (if (not (null? line))
      (car line)))
(define (get-indicator line)
  (cadr line))

(define start-command "/*$$start")
(define end-command "/*$$end")

; get the list of block which are indicated by special command
(define (get-blocks lines result flag)
  (if (not (null? lines))
      (let ((line-token (string-tokenize (car lines))))
        (begin
          ;; (display (list "processing:" (car lines) ":")) (display line-token) (newline)
          (cond
           ((equal? (car lines) "")
<<<<<<< .mine
            ;; ignore empty line in or out of block
            ;; (display "empty-line") (newline)
            (get-blocks (cdr lines) result flag))
           ((and (equal? (get-command line-token) start-command) ; must use equal?
=======
            (display (list "empty line" result)) (newline)
            ;; add empty line
            (get-blocks (cdr lines)
                        (cons (append (car result) (list ""))
                              (cdr result))
                        flag))
           ((null? line-token)
            ;; (display "This line has only spaces") (newline)
            ;; add empty line
            (get-blocks (cdr lines)
                        (cons (append (car result) (list ""))
                              (cdr result))
                        flag))
            ((and (equal? (get-command line-token) start-command) ; must use equal?
>>>>>>> .r669
                 (eq? flag 'out-block))
            ;; start to extract one block
            ;; (display (list "start result:" (list (get-indicator line-token)))) (newline)
            (get-blocks (cdr lines)
                        (cons (list (get-indicator line-token)) result)
                        'in-block))
           ((and (equal? (get-command line-token) end-command)
                 (eq? flag 'in-block))
            ;; end to extract one block
            ;; (display (list "end")) (newline)
            (get-blocks (cdr lines)
                        result
                        'out-block))
           ((eq? flag 'in-block)
            ;; insert lines into block
            ;; (display (list "add:" (append (car result) (list (car lines))))) (newline)
            (get-blocks (cdr lines)
                        (cons (append (car result) (list (car lines)))
                              (cdr result))
                        'in-block))
           (else
            ;; not included in special block
            (get-blocks (cdr lines) result flag)))))
      result)) ; return result


;; make one sub-table for one source file
(define (make-one-src-table filename)
  (let ((src-table (list filename))
        (lines (read-file (open-input-file filename))))
    ;; The second argument of get-blocks must be '(()), not '(),
    ;; because get-blocks has code like (cdr result).
    ;; '() will generate error at (cdr result)
    ;; (cdr '(())) is the same with '(), so it's ok.
    (get-blocks lines '(()) 'out-block)))


;; test
;; (make-one-src-table "src1.c")
;; (make-one-src-table "src2.c")
;;(make-one-src-table "src3.c") ;; I don't make error handling yet. Is it need?


(define (make-src-table src-files src-table)
  ;; read indicators of one file and generate table
  ;; combine tables of each files and generate greate table of total source files
  (if (not (null? src-files))
      (begin
        (insert! (car src-files) (make-one-src-table (car src-files)) src-table)
        (make-src-table (cdr src-files) src-table))))

;; test 
;; (define test-table (list '*src-table*))
;; (make-src-files (list "src1.c" "src2.c") test-table)

(define (join-command? text-line)
  (let ((insert-indicator "$$"))
    (string-prefix? insert-indicator text-line)))
(define (get-filename token-list)
  (substring (car token-list) 2))
(define (get-indicator token-list)
  (cadr token-list))

;;test
;; (define test-line "$$src1.c ex1")
;; (join-command? test-line)
;; (get-filename (string-tokenize test-line))
;; (get-indicator (string-tokenize test-line))


(define (find-src-block filename indicator src-table)
  (let ((file-table (lookup filename src-table)))
    (assoc indicator file-table)))
    

(define (make-final-book book-filename src-table)
  (let ((book-text-lines (read-file (open-input-file book-filename))))
    (print-lines-with-command-result book-text-lines src-table)))

(define (print-file-except-command filename)
  (let ((lines (read-file (open-input-file filename))))
    (define (print-lines-except-command ls)
      (if (not (null? ls))
          (if (not (or (string-prefix? start-command (car ls))
                       (string-prefix? end-command (car ls))))
              (begin
                (display (car ls)) (newline)
                (print-lines-except-command (cdr ls)))
              (print-lines-except-command (cdr ls)))))
    (print-lines-except-command lines)))


(define (get-source-lines block-from-table)
  (cdr block-from-table))
(define (print-block-lines lines)
  (if (not (null? lines))
      (begin
        (display (car lines)) (newline)
        (print-block-lines (cdr lines)))))

(define (print-lines-with-command-result lines src-table)
  (if (not (null? lines))
      (begin
        (cond 
         ((join-command? (car lines))
          ;; print source lines
          (let ((line-token (string-tokenize (car lines))))
            (if (null? (cdr line-token))
                (print-file-except-command (get-filename line-token))
                (print-block-lines (get-source-lines (find-src-block (get-filename line-token)
                                                                     (get-indicator line-token)
                                                                     src-table)))))
          (newline))
         (else
          ;; normal text
          (display (car lines))
          (newline)))
        (print-lines-with-command-result (cdr lines) src-table))))

;;test
;; (define test-table (list '*src-table*))
;; (make-src-table (list "src1.c" "src2.c") test-table)
;; (find-src-block "src2.c" "ex2" test-table)
;; (find-src-block "src2.c" "ex1" test-table)
;; (find-src-block "src2.c" "ex3" test-table)
;; (find-src-block "src2.c" "ex4" test-table) ;; fail that ex4 has no line
;; (find-src-block "src1.c" "ex2" test-table)
;; (find-src-block "src1.c" "ex1" test-table)
;; (find-src-block "src1.c" "ex3" test-table)
;; (find-src-block "src1.c" "ex4" test-table) ;; fail that ex4 has no line
;; (find-src-block "src1.c" "ex5" test-table) ;; not exist indicator
;; (find-src-block "src5.c" "ex4" test-table) ;; fail that ex4 has no line
;; (make-final-book "book.txt" test-table)




;; final test
;;(main (list "./join_text.scm" "book.txt" "src1.c" "src2.c"))
