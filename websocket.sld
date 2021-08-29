(define-library (websocket)
  (import
    (scheme base))
  (export
    ws_on_message
    websocket_write
    ws_init
    ws_close_client
    ws_write
    ws_send_str
  )
(include-shared "websocket"))
