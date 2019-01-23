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

; 1. read book text which has join-command
; 2. formats of join-command
;  - $$src-name start-line end-line : print lines from start to end
;  - $$src-name ; print every line
; 3. read source files that are specified at command arguments
; 4. join book and source text and full source & describtion text is generated
; look example for detail: book.txt, src1.c, src2.c, src3.c

; args: (join_text.scm book.txt 1st 2nd 3rd ...)
(define (main args)
  (define (write-final-book book-text src-table)
    (if (not (null? book-text))
        (begin
          (trans-book-text (car book-text) src-table)
          (write-final-book (cdr book-text) src-table))))

  (check-args 3 (length args))

  ;; Read every source files and make table.
  ;; The table can be indexed by filename and returns text data in the file.
  (let ((book-text-table (make-book-file-table))
        (src-text-table (make-src-file-table))
        (book-file-name (get-book-file-name args))
        (src-file-names (get-src-file-names args)))
    (read-book-text book-file-name book-text-table)
    (read-many-books-text src-file-names src-text-table)
    (write-final-book (lookup book-file-name book-text-table) src-text-table))
  (quit 0))

  
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

(define (read-many-books-text filename-list book-text-table)
  (if (not (null? filename-list))
      (begin
        (read-book-text (car filename-list) book-text-table)
        (read-many-books-text (cdr filename-list) book-text-table))))

(define (read-book-text filename book-text-table)
  (let ((port (open-input-file filename)))
    (let ((text (read-file-lines port)))
      (insert! filename text book-text-table)
      (close-port port))))

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

(define (make-book-file-table)
  (list '*book-table*))
(define (make-src-file-table)
  (list '*src-table*))

(define (read-file-lines read-port)
  (define (read-lines lst)
    (let ((line (read-line read-port)))
      (if (eof-object? line)
          lst
          (read-lines (append lst (list line))))))
  (read-lines '()))


(define (write-list-from-to data-list from to)
  (define (write-list-from-to-zero-base from-zero-base to-zero-base)
    (if (<= from-zero-base to-zero-base)
        (begin
          (display (list-ref data-list from-zero-base)) (newline)
          (write-list-from-to-zero-base (+ from-zero-base 1)
                                        to-zero-base))))
  (if (and (null? from) (null? to))
      (write-list-from-to-zero-base 0 (- (length data-list) 1))
      (write-list-from-to-zero-base (- from 1) (- to 1))))

(define (trans-book-text desc-line src-table)
  (define (join-command? text-line)
    (let ((insert-indicator "$$"))
      (string-prefix? insert-indicator text-line)))
  (define (get-filename token-list)
    (substring (car token-list) 2))
  (define (get-start-num token-list)
    (if (null? (cdr token-list))
        '()
        (string->number (cadr token-list))))
  (define (get-end-num token-list)
    (if (null? (cdr token-list))
        '()
        (string->number (caddr token-list))))
  (define (analyze-join-command command-line)
    (let ((token-list (string-tokenize command-line)))
      (list (get-filename token-list)
            (get-start-num token-list)
            (get-end-num token-list))))
  (if (join-command? desc-line)
      (begin
        (let ((parsed-join-command (analyze-join-command desc-line)))
          (let ((src-list (lookup (car parsed-join-command) src-table)))
            (if src-list
                (write-list-from-to src-list
                                    (cadr parsed-join-command)
                                    (caddr parsed-join-command))
                (error "cannot find file " parsed-join-command)))))
      (begin
        (display desc-line)
        (newline))))

