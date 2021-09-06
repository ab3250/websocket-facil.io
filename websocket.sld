(define-library (websocket)
  (import
    (scheme base))
  (export
    websocket_write
    ws_on_message   
    ws_close_client
    ws_send_str
    ;wrapper functions for c functions
    w_ws_init
    w_ws_write
    w_fio_run_every)
(include-shared "websocket"))
