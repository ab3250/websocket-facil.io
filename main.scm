(import
    (scheme base)
    (scheme red)
    (chibi time)    
    (websocket))

(define gblWs -1)
(define port 8080)
(define nonblocking 1)

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
   ;  (let loop ((count 0))   
   ;      (delay 2)     
         (ws_write gblWs  "howdy2"  6  1)               
  ;  (loop (+ count 1)))
   )

(define (closeconn fd)
    ws_close_client(fd))

(define (onmessage msg)    
    (display (list 'frame 'recieved msg))
     (ws_write gblWs  msg  7  1)     
    (newline)   
    ) ;echo

(define (main args)      
    (ws_init))