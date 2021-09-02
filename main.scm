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
  (wslib))

;(define deckstr (string-append "{\"type\": \"cards\"}," "{\"data\":" (json->string deck)(string #\})));

(define deckstr (jsonify "dots" deck))

(define gblWs -1)

(define (randomed retstr)
  (ws_write gblWs (jsonify "dots" (string->list3 retstr)) 1))
 
(define (onclose wsptr)
    (set! gblWs -1)
    (display (list 'closed ))
    (newline))

(define (onopen wsptr)
    (set! gblWs wsptr)
    (ws_write gblWs  (jsonify "greeting" "howdy") 1))
   

(define (ontimer1)    
   (unless (equal? gblWs -1)(ws_write gblWs (json->string(knuth-shuffle deck)) 1)))

(define (ontimer2)
   (unless (equal? gblWs -1) (ws_send_str deck2string)))

(define (onmessage msg)
    (cond 
      ((string=? msg "button1")(ws_write gblWs (jsonify "cards" deck) 1))
      ((string=? msg "button2")(ws_write gblWs (jsonify "cards" (knuth-shuffle deck)) 1 ))     
      ((string=? msg "button3")(ws_write gblWs (jsonify "dots" deck2) 1))
      ((string=? msg "button4")(ws_send_str deck2string))))      
    
(define (init-timers)

 ; (fio_run_every_wrap 6000 0 "timer1")
  (delay 3)
 ; (fio_run_every_wrap 6000 0 "timer2")
 )

(define (main args)  
  (ws_init))

