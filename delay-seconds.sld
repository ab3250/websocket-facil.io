(define-library (delay-seconds)
(import
    (scheme base)
    (scheme red)
    (websocket)
    (chibi time))
(export delay-seconds)
(begin
    (define (delay-seconds sec)
        (define start (current-seconds))
        (let timeloop ()    
            (if ( < (- (current-seconds) start) sec) (timeloop))))))
