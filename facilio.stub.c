(c-include "chibi/eval.h")
(c-include "fio.h")
(c-include "fio_cli.h")
(c-include "fio_tls.h")
(c-include "http.h")
(c-include "redis_engine.h")

;void *websocket_udata_get(ws_s *ws);

;void *websocket_udata_set(ws_s *ws, void *udata);

;intptr_t websocket_uuid(ws_s *ws);

;uint8_t websocket_is_client(ws_s *ws);

;int websocket_write(ws_s *ws, fio_str_info_s msg, uint8_t is_text);
;void websocket_close(ws_s *ws);

;void (*on_message)(ws_s *ws, fio_str_info_s channel, fio_str_info_s msg,void *udata);
;void (*on_unsubscribe)(void *udata);

 ;static void on_http_request(http_s *h);
;static void on_http_upgrade(http_s *h, char *requested_protocol, size_t len);
;static void initialize_cli(int argc, char const *argv[]);
;static void initialize_redis(void);


;static void ws_on_open(ws_s *ws);

;static void ws_on_message(ws_s *ws, fio_str_info_s msg, uint8_t is_text);
;static void ws_on_shutdown(ws_s *ws);
;static void ws_on_close(intptr_t uuid, void *udata);
;static void ws_init(void);
;fio_run_every(9000, 0, ws_on_timer1, NULL, NULL);
;int fio_run_every(size_t milliseconds, size_t repetitions, void (*task)(void *),
                  ;void *arg, void (*on_finish)(void *))
;(define-c-struct ws_s)
(define-c-struct fio_str_info_s)
(define-c int ws_init())
(define-c int websocket_write(sexp (struct fio_str_info_s)  int))
(define-c int ws_close_client(int))
(define-c void ws_on_message(sexp (struct fio_str_info_s) int))
(define-c void ws_write(sexp string int int))
(define-c int fio_run_every_wrap(size_t size_t string))