(import
  (chibi time)  
  (scheme base)
  (scheme red)  
  (websocket)
  (only (srfi 130) string-pad)
  (schemepunk json)
  (delay-seconds)
  (srfi 1)
  (srfi 27)
  )
   ; test if eq?
; (define ==
;   (lambda (x y)
;     (if (and (string? x) (string? y))
;       (== x y)
;       (if (or (string? x) (string? y))
;             (= 1 0) ;return false
;             (equal? x y)))))
; ; (define a "aString")
; (define l '("aString" "aOtherString"))
; (== (car l) a) ; true
; (== 1 1) ; true
; (== 1 0) ; false
; (== "a" 1) ; false diff. type
; (== "a" "b") ; false
; (== "a" "a") ; true
; (== '("a" "b") '("a" "b"))

; (define for-acc-list (lambda (start end func)
;   (let loop ((index start)
;              (acc '()))
;       (if (> index end)
;                       acc   
;                       (loop (+ index 1) (func index end acc))))))

; (define for-acc-str (lambda (start end func)
;   (let loop ((index start)
;              (acc ""))
;       (if (> index end)
;                       acc   
;                       (loop (+ index 1) (func index end acc))))))

(define for-accumulator (lambda (start end acc-initial func )
  (let loop ((index start)
             (acc acc-initial))
      (if (> index end)
                      acc   
                      (loop (+ index 1) (func index end acc))))))

(define (knuth-shuffle lst-org)  
  (let loop ((count (length lst-org)) (lst lst-org))      
    (if (zero? count)
    	lst
	(let*   ((j (random-integer count))
		 (new-count (- count 1))
	         (tmp (list-ref lst new-count))
	         (lst2 (list-set lst new-count (list-ref lst j)))
	         (lst3 (list-set lst2 j tmp)))	         
	         (loop new-count lst3)))))

(define (list-set lst idx val)
  (if (null? lst)
    lst
    (cons
      (if (zero? idx)
        val
        (car lst))
      (list-set (cdr lst) (- idx 1) val))))

(define padn (lambda(x y)
  (string-pad (number->string x) y #\0)))

(define (func-list x end acc)
  (cons (if (< (- x 1)(/ end 2)) "0" "1") acc))

(define (func-string index end acc)
  (if (< (- index 1)(/ end 2)) (string-append "0" acc) (string-append "1" acc) ))

;(define deck2string (for-acc-str 1 320000 func-string))
(define deck2string (for-accumulator 1 5000 "" func-string))

(define (string->list3 str)
  (map (lambda(x)(string x)) (string->list deck2string)))

(define deck2 (for-accumulator 1 5000 '() func-list))

(define deck (list "0000" "0001" "0002" "0003" "0004" "0005" "0006" "0007" "0008" "0009" "0010" "0011" "0012"
                   "0100" "0101" "0102" "0103" "0104" "0105" "0106" "0107" "0108" "0109" "0110" "0111" "0112"
                   "0200" "0201" "0202" "0203" "0204" "0205" "0206" "0207" "0208" "0209" "0210" "0211" "0212"
                   "0300" "0301" "0302" "0303" "0304" "0305" "0306" "0307" "0308" "0309" "0310" "0311" "0312"))

(define gblWs -1)

(define (delay sec)
    (define start (current-seconds))
    (let timeloop ()    
        (if ( < (- (current-seconds) start) sec) (timeloop))))

(define-syntax unless
  (syntax-rules ()
    ((unless test . body)
     (when (not test) . body))))

(define (randomed retstr)
  (ws_write gblWs (json->string (string->list3 retstr)) (string-length(json->string (string->list3 retstr)))   1))

(define (onclose wsptr)
  (set! gblWs -1)
    (display (list 'closed ))
    (newline))

(define (onopen wsptr)
    (set! gblWs wsptr)
    (ws_write gblWs  (json->string "{howdy}")  (string-length (json->string "{howdy}")) 1))
   

(define (ontimer1)
    (let ((len (string-length (json->string(knuth-shuffle deck)))))
   (unless (equal? gblWs -1)(ws_write gblWs (json->string(knuth-shuffle deck)) 5 1))))

(define (ontimer2)
   (unless (equal? gblWs -1) (ws_send_str deck2string)))

(define (onmessage msg)
    (cond 
      ((string=? msg "button1")(ws_write gblWs (json->string deck) (string-length (json->string deck)) 1))
      ((string=? msg "button2")(ws_write gblWs (json->string(knuth-shuffle deck))(string-length (json->string deck)) 1))
      ((string=? msg "button3")(ws_write gblWs (json->string deck2) (string-length (json->string deck2)) 1))
      ((string=? msg "button4")(ws_send_str deck2string))))      
    
(define (init-timers)
 ; (fio_run_every_wrap 6000 0 "timer1")
  (delay 3)
 ; (fio_run_every_wrap 6000 0 "timer2")
 )

(define (main args)  
  (ws_init))