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
;(define-c-struct ws_s)
(define-c-struct fio_str_info_s)
(define-c int ws_init())
(define-c int websocket_write(sexp (struct fio_str_info_s)  int))
(define-c int ws_close_client(int))
(define-c void ws_on_message(sexp (struct fio_str_info_s) int))
(define-c void ws_write(sexp string int int))
(define-c void ws_send_str(string))


; (define-c-struct fio_str_info_s
;   finalizer: freeaddrinfo
;   predicate: address-info?
;   (int              ai_family    address-info-family)
;   (int              ai_socktype  address-info-socket-type)
;   (int              ai_protocol  address-info-protocol)
;   ((link sockaddr)  ai_addr      address-info-address)
;   (size_t           ai_addrlen   address-info-address-length)
;   ((link addrinfo)  ai_next      address-info-next))


;   (register-simple-type <name-string> <parent> <field-names>)
;  => <type>    ; parent may be #f, field-names should be a list of symbols

; (make-type-predicate <opcode-name-string> <type>)
;  => <opcode>  ; takes 1 arg, returns #t iff that arg is of the type

; (make-constructor <constructor-name-string> <type>)
;  => <opcode>  ; takes 0 args, returns a newly allocated instance of type

; (make-getter <getter-name-string> <type> <field-index>)
;  => <opcode>  ; takes 1 args, retrieves the field located at the index

; (make-setter <setter-name-string> <type> <field-index>)
;  => <opcode>  ; takes 2 args, sets the field located at the index

; (type-slot-offset <type> <field-name>)
;  => <index>   ; returns the index of the field with the given name

;  (define-c-struct struct_name
;   [predicate: predicate-name]
;   [constructor: constructor-name]
;   [finalizer: c_finalizer_name]
;   (type c_field_name getter-name setter-name) ...)
