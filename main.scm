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
  (wslib)
  )

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