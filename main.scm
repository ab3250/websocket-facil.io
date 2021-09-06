(import
  (chibi time)  
  (scheme base)
  (scheme red)  
  (websocket)
  (schemepunk json)  
  (delay-seconds)
  (srfi 1)
  (wslib))

(define deckstr (jsonify "dots" deck))

(define gblWs -1)

(define (randomed retstr)
  (w_ws_write gblWs (jsonify "dots" (string->list3 retstr)) 1))
 
(define (scm-onclose wsptr)
    (set! gblWs -1)
    (display (list 'closed ))
    (newline))

(define (scm-onopen wsptr)
    (set! gblWs wsptr)
    (w_ws_write gblWs  (jsonify "greeting" "howdy") 1))

(define (scm-ontimer1)
   (unless (equal? gblWs -1)(w_ws_write gblWs (jsonify "cards" (knuth-shuffle deck)) 1)))

(define (scm-ontimer2)
       (unless (equal? gblWs -1) (ws_send_str deck2string)))

(define (scm-onmessage msg)
    (cond 
      ((string=? msg "button1")(w_ws_write gblWs (jsonify "cards" deck) 1))
      ((string=? msg "button2")(w_ws_write gblWs (jsonify "cards" (knuth-shuffle deck)) 1 ))     
      ((string=? msg "button3")(w_ws_write gblWs (jsonify "dots" deck2) 1))
      ((string=? msg "button4")(ws_send_str deck2string))
      ((string=? msg "close")((display "closed")))))     
    
(define (main args) 
  (w_fio_run_every 6000 0 "set-timer1")
  (w_fio_run_every 6000 0 "set-timer2")
  (w_ws_init))
