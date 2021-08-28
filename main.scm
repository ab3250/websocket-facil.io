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
   

(define for-acc (lambda (start end func)
  (let loop ((index start)
             (acc '()))
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

(define (func x end acc)
  (cons (if (< (- x 1)(/ end 2)) "0" "1" ) acc))

(define deck2 (for-acc 1 20000 func))

(define deck (list "0000" "0001" "0002" "0003" "0004" "0005" "0006" "0007" "0008" "0009" "0010" "0011" "0012"
                   "0100" "0101" "0102" "0103" "0104" "0105" "0106" "0107" "0108" "0109" "0110" "0111" "0112"
                   "0200" "0201" "0202" "0203" "0204" "0205" "0206" "0207" "0208" "0209" "0210" "0211" "0212"
                   "0300" "0301" "0302" "0303" "0304" "0305" "0306" "0307" "0308" "0309" "0310" "0311" "0312"))

; ;;;;

;;;;



(define gblWs -1)
(define port 8080)
(define count 1)

(define (delay sec)
    (define start (current-seconds))
    (let timeloop ()    
        (if ( < (- (current-seconds) start) sec) (timeloop))))

(define-syntax unless
  (syntax-rules ()
    ((unless test . body)
     (when (not test) . body))))

(define (onclose wsptr)
    (display (list 'closed wsptr))
    (newline)
    (set! gblFd -1))

(define (onopen wsptr)
    (set! gblWs wsptr)
    (ws_write gblWs  "howdy2"  6  1))

(define (set-timer)
(fio_run_every_wrap 9000 0 "ws_on_timer1" )
)

(define (ontimer1)
    (let ((len2 (string-length (json->string(knuth-shuffle deck2))))(len (string-length (json->string(knuth-shuffle deck)))))
    (ws_write gblWs (json->string(knuth-shuffle deck2)) len2   1) 
    (ws_write gblWs (json->string(knuth-shuffle deck)) len   1) 
    (set! count (+ count 1))))

(define (onmessage msg)    
    (display (list 'frame 'recieved msg))
     (ws_write gblWs  msg  7  1)     
    (newline)   
    ) ;echo

(define (main args)      
    (ws_init))