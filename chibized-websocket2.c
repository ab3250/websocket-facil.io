#include "chibi/eval.h"
#include "fio.h"
#include "fio_cli.h"
#include "fio_tls.h"
#include "http.h"
#include "redis_engine.h"
#include "websockets.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


static sexp ctx2;
static void on_http_request(http_s *h);
static void on_http_upgrade(http_s *h, char *requested_protocol, size_t len);
static void initialize_cli(int argc, char const *argv[]);
static void initialize_redis(void);
static void ws_on_open(ws_s *ws);
static void ws_on_message(ws_s *ws, fio_str_info_s msg, uint8_t is_text);
static void ws_on_shutdown(ws_s *ws);
static void ws_on_close(intptr_t uuid, void *udata);
static void ws_write(sexp ws, char *msg, int len, int is_text);
static void ws_on_timer1(void);
static void ws_on_timer2(void);
static void ws_send_str(char* str);
static int rand_int(int n);
static void shuffle(char *array, int n);
static void initialize_timers(void);
 
static int rand_int(int n) {
  int limit = RAND_MAX - RAND_MAX % n;
  int rnd;
  do {
    rnd = rand();
  } 
  while (rnd >= limit);
  return rnd % n;
}
 
static void shuffle(char *array, int n) {
  int i, j, tmp; 
  for (i = n - 1; i > 0; i--) {
    j = rand_int(i + 1);
    tmp = array[j];
    array[j] = array[i];
    array[i] = tmp;
  }
}

static void ws_send_str(char* str) {
  shuffle(str,strlen(str));  
  sexp ctx = ctx2;
  sexp_gc_var3(cmd,arg_sym,arg_val); 
  sexp_gc_preserve3(ctx,cmd,arg_sym,arg_val);
  arg_sym=sexp_intern(ctx, "retstr", -1); 
  arg_val=sexp_c_string(ctx,str,-1);
  sexp_env_define(ctx, sexp_context_env(ctx), arg_sym, arg_val);
  sexp_eval_string(ctx, "(randomed retstr)", -1, NULL);
  sexp_gc_release3(ctx);
}

static void initialize_timers(void) {
  sexp ctx = ctx2;
  sexp_eval_string(ctx, "(init-timers)", -1, NULL);
}

static int fio_run_every_wrap(size_t interval, size_t repeat, char *func) {
  if (strcmp(func , "timer1") == 0){
    printf("%i",fio_run_every(interval, repeat, ws_on_timer1, NULL, NULL));
    printf(func);
  }
  else if (strcmp(func , "timer2") == 0){
    printf("%i",fio_run_every(interval, repeat, ws_on_timer2, NULL, NULL));
    printf(func);
  }
}

static void ws_on_timer1(void) {
 sexp ctx = ctx2; 
 sexp_eval_string(ctx, "(ontimer1)", -1, NULL); 
}

static void ws_on_timer2(void) {
 sexp ctx = ctx2; 
 sexp_eval_string(ctx, "(ontimer2)", -1, NULL); 
}

static void ws_write(sexp ws, char *msg, int len, int is_text) {  
  fio_str_info_s e;
  e.data = msg;  
  e.len = len;
  int ret = websocket_write((ws_s *)ws, e, is_text);
}

static int ws_init(void) {
  const char *port = "8080";
  const char *address = "127.0.0.1";
  const char *public_folder = "www";
  uint32_t threads = 1;
  uint32_t workers = 1;
  uint8_t print_log = 0; 
  setbuf(stdout, NULL);
  if (http_listen(port, address,
    .on_request = on_http_request,
    .on_upgrade = on_http_upgrade,
    .max_body_size = 1 * 1024 * 1024,
    .ws_max_msg_size = 1 * 1024,
    .public_folder = public_folder,
    .log = 0,
    .timeout = 10,
    .ws_timeout = 0) == -1) {
    /* listen failed ? */
     perror("ERROR: facil. io couldn't initialize HTTP service (already running?)");
     exit(1);
    }
  //fio_timer_clear_all(); 
  fio_run_every(500, 1, initialize_timers, NULL, NULL);
  //fio_defer(ws_on_timer1, NULL,NULL);
  fio_start(.threads = threads, .workers = workers);
  //fio_timer_clear_all();
  return(0);
 }

// /* *****************************************************************************
// HTTP Request / Response Handling
// ***************************************************************************** */
FIOBJ HTTP_HEADER_X_DATA;

static void on_http_request(http_s *h) {
  HTTP_HEADER_X_DATA = fiobj_str_new("X-Data", 6);
  /* set a response and send it (finnish vs. destroy). */
  http_set_header(h, HTTP_HEADER_CONTENT_TYPE,
                 http_mimetype_find("txt", 3));
  http_set_header(h, HTTP_HEADER_X_DATA, fiobj_str_new("my data", 7));
  http_send_body(h, "Hello World!", 12);
}

 /* *****************************************************************************
 HTTP Upgrade Handling
 ***************************************************************************** */

/* HTTP upgrade callback */
static void on_http_upgrade(http_s *h, char *requested_protocol, size_t len) {
 /* Upgrade to  WebSockets and set the request path as a nickname. */
  FIOBJ nickname;
  if (fiobj_obj2cstr(h->path).len > 1) {
    nickname = fiobj_str_new(fiobj_obj2cstr(h->path).data + 1,
                             fiobj_obj2cstr(h->path).len - 1);
  } else {
    nickname = fiobj_str_new("Guest", 5);
  }
 /* Test for upgrade protocol (websocket) */
  if (len == 9 && requested_protocol[1] == 'e') {
    if (fio_cli_get_bool("-v")) {
      fprintf(stderr, "* (%d) new WebSocket connection: %s.\n", getpid(),
              fiobj_obj2cstr(nickname).data);
    }
    http_upgrade2ws(h, .on_message = ws_on_message, .on_open = ws_on_open,
                    .on_shutdown = ws_on_shutdown, .on_close = ws_on_close,
                    .udata = (void *)nickname);
  } else {
    fprintf(stderr, "WARNING: unrecognized HTTP upgrade request: %s\n",
            requested_protocol);
    http_send_error(h, 400);
    fiobj_free(nickname); // we didn't use this
  }
}



///* *****************************************************************************
//WebSockets Callbacks
//***************************************************************************** */

static void ws_on_message(ws_s *ws, fio_str_info_s msg, uint8_t is_text) {
 sexp ctx = ctx2;
 msg.len--
 msg.data[len]=NULL;
 sexp_gc_var3(cmd,arg_sym,arg_val); 
 sexp_gc_preserve3(ctx,cmd,arg_sym,arg_val);
 //arg_sym=sexp_intern(ctx, "wsptr", -1); 
 //arg_val=ws;
 //sexp_env_define(ctx, sexp_context_env(ctx), arg_sym, arg_val);
 arg_sym=sexp_intern(ctx, "msg", -1); 
 arg_val=sexp_c_string(ctx,msg.data,-1);
 sexp_env_define(ctx, sexp_context_env(ctx), arg_sym, arg_val);
 cmd = sexp_eval_string(ctx, "(onmessage msg)", -1, NULL);
 sexp_gc_release3(ctx);
}
static void ws_on_open(ws_s *ws) 
{  
 sexp ctx = ctx2;
 sexp_gc_var3(cmd,arg_sym,arg_val); 
 sexp_gc_preserve3(ctx,cmd,arg_sym,arg_val);
 arg_sym=sexp_intern(ctx, "wsptr", -1); 
 arg_val=ws;
 sexp_env_define(ctx, sexp_context_env(ctx), arg_sym, arg_val);
 cmd = sexp_eval_string(ctx, "(onopen wsptr)", -1, NULL);
 sexp_gc_release3(ctx);
}
static void ws_on_shutdown(ws_s *ws) {
  websocket_write(
      ws, (fio_str_info_s){.data = "Server shutting down, goodbye.", .len = 30},
      1);
}

static void ws_on_close(intptr_t uuid, void *udata) {
  sexp ctx = ctx2;
  sexp_gc_var1(cmd); 
  sexp_gc_preserve1(ctx,cmd);
 // cmd = sexp_list2(ctx, sexp_intern(ctx, "onclose", -1),sexp_make_integer(ctx, fd));
 // sexp_eval(ctx, cmd, NULL);
  sexp_gc_release1(ctx); 
  /* Let everyone know we left the chat */
  //fiobj_str_write((FIOBJ)udata, " left the chat.", 15);
  //fio_publish(.channel = CHAT_CANNEL, .message = fiobj_obj2cstr((FIOBJ)udata));
  /* free the nickname */
  //fiobj_free((FIOBJ)udata);
//  (void)uuid; // we don't use the ID
}

sexp sexp_fio_run_every_wrap_stub (sexp ctx, sexp self, sexp_sint_t n, sexp arg0, sexp arg1, sexp arg2) {
  sexp res;
  if (! sexp_exact_integerp(arg0))
    return sexp_type_exception(ctx, self, SEXP_FIXNUM, arg0);
  if (! sexp_exact_integerp(arg1))
    return sexp_type_exception(ctx, self, SEXP_FIXNUM, arg1);
  if (! sexp_stringp(arg2))
    return sexp_type_exception(ctx, self, SEXP_STRING, arg2);
  res = sexp_make_integer(ctx, fio_run_every_wrap(sexp_sint_value(arg0), sexp_sint_value(arg1), sexp_string_data(arg2)));
  return res;
}

sexp sexp_ws_send_str_stub (sexp ctx, sexp self, sexp_sint_t n, sexp arg0) {
  sexp res;
  if (! sexp_stringp(arg0))
    return sexp_type_exception(ctx, self, SEXP_STRING, arg0);
  res = ((ws_send_str(sexp_string_data(arg0))), SEXP_VOID);
  return res;
}

sexp sexp_ws_write_stub (sexp ctx, sexp self, sexp_sint_t n, sexp arg0, sexp arg1, sexp arg2, sexp arg3) {
  sexp res;
  if (! sexp_stringp(arg1))
    return sexp_type_exception(ctx, self, SEXP_STRING, arg1);
  if (! sexp_exact_integerp(arg2))
    return sexp_type_exception(ctx, self, SEXP_FIXNUM, arg2);
  if (! sexp_exact_integerp(arg3))
    return sexp_type_exception(ctx, self, SEXP_FIXNUM, arg3);
  res = ((ws_write(arg0, sexp_string_data(arg1), sexp_sint_value(arg2), sexp_sint_value(arg3))), SEXP_VOID);
  return res;
}

sexp sexp_ws_on_message_stub (sexp ctx, sexp self, sexp_sint_t n, sexp arg0, sexp arg1, sexp arg2) {
  sexp res;
  if (! (sexp_pointerp(arg1) && (sexp_pointer_tag(arg1) == sexp_unbox_fixnum(sexp_opcode_arg2_type(self)))))
    return sexp_type_exception(ctx, self, sexp_unbox_fixnum(sexp_opcode_arg2_type(self)), arg1);
  if (! sexp_exact_integerp(arg2))
    return sexp_type_exception(ctx, self, SEXP_FIXNUM, arg2);
  res = ((ws_on_message(arg0, *(struct fio_str_info_s*)sexp_cpointer_value(arg1), sexp_sint_value(arg2))), SEXP_VOID);
  return res;
}

sexp sexp_ws_close_client_stub (sexp ctx, sexp self, sexp_sint_t n, sexp arg0) {
  sexp res;
  if (! sexp_exact_integerp(arg0))
    return sexp_type_exception(ctx, self, SEXP_FIXNUM, arg0);
  res = sexp_make_integer(ctx, ws_close_client(sexp_sint_value(arg0)));
  return res;
}

sexp sexp_websocket_write_stub (sexp ctx, sexp self, sexp_sint_t n, sexp arg0, sexp arg1, sexp arg2) {
  sexp res;
  if (! (sexp_pointerp(arg1) && (sexp_pointer_tag(arg1) == sexp_unbox_fixnum(sexp_opcode_arg2_type(self)))))
    return sexp_type_exception(ctx, self, sexp_unbox_fixnum(sexp_opcode_arg2_type(self)), arg1);
  if (! sexp_exact_integerp(arg2))
    return sexp_type_exception(ctx, self, SEXP_FIXNUM, arg2);
  res = sexp_make_integer(ctx, websocket_write(arg0, *(struct fio_str_info_s*)sexp_cpointer_value(arg1), sexp_sint_value(arg2)));
  return res;
}

sexp sexp_ws_init_stub (sexp ctx, sexp self, sexp_sint_t n) {
  sexp res;
  res = sexp_make_integer(ctx, ws_init());
  return res;
}

sexp sexp_init_library (sexp ctx, sexp self, sexp_sint_t n, sexp env, const char* version, const sexp_abi_identifier_t abi) {
  sexp sexp_fio_str_info_s_type_obj;
  sexp_gc_var3(name, tmp, op);
  if (!(sexp_version_compatible(ctx, version, sexp_version)
        && sexp_abi_compatible(ctx, abi, SEXP_ABI_IDENTIFIER)))
    return SEXP_ABI_ERROR;
  sexp_gc_preserve3(ctx, name, tmp, op);
  name = sexp_c_string(ctx, "fio_str_info_s", -1);
  sexp_fio_str_info_s_type_obj = sexp_register_c_type(ctx, name, sexp_finalize_c_type);
  tmp = sexp_string_to_symbol(ctx, name);
  sexp_env_define(ctx, env, tmp, sexp_fio_str_info_s_type_obj);
  op = sexp_define_foreign(ctx, env, "fio_run_every_wrap", 3, sexp_fio_run_every_wrap_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = sexp_make_fixnum(SEXP_FIXNUM);
    sexp_opcode_arg1_type(op) = sexp_make_fixnum(SEXP_FIXNUM);
    sexp_opcode_arg2_type(op) = sexp_make_fixnum(SEXP_FIXNUM);
    sexp_opcode_arg3_type(op) = sexp_make_fixnum(SEXP_STRING);
  }
   op = sexp_define_foreign(ctx, env, "ws_send_str", 1, sexp_ws_send_str_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = SEXP_VOID;
    sexp_opcode_arg1_type(op) = sexp_make_fixnum(SEXP_STRING);
  }
  op = sexp_define_foreign(ctx, env, "ws_write", 4, sexp_ws_write_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = SEXP_VOID;
    sexp_opcode_arg2_type(op) = sexp_make_fixnum(SEXP_STRING);
    sexp_opcode_arg3_type(op) = sexp_make_fixnum(SEXP_FIXNUM);
    sexp_opcode_argn_type(op) = sexp_make_vector(ctx, SEXP_ONE, sexp_make_fixnum(SEXP_OBJECT));
    sexp_vector_set(sexp_opcode_argn_type(op), SEXP_ZERO, sexp_make_fixnum(SEXP_FIXNUM));
  }
  op = sexp_define_foreign(ctx, env, "ws_on_message", 3, sexp_ws_on_message_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = SEXP_VOID;
    sexp_opcode_arg2_type(op) = sexp_make_fixnum(sexp_type_tag(sexp_fio_str_info_s_type_obj));
    sexp_opcode_arg3_type(op) = sexp_make_fixnum(SEXP_FIXNUM);
  }
  op = sexp_define_foreign(ctx, env, "ws_close_client", 1, sexp_ws_close_client_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = sexp_make_fixnum(SEXP_FIXNUM);
    sexp_opcode_arg1_type(op) = sexp_make_fixnum(SEXP_FIXNUM);
  }
  op = sexp_define_foreign(ctx, env, "websocket_write", 3, sexp_websocket_write_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = sexp_make_fixnum(SEXP_FIXNUM);
    sexp_opcode_arg2_type(op) = sexp_make_fixnum(sexp_type_tag(sexp_fio_str_info_s_type_obj));
    sexp_opcode_arg3_type(op) = sexp_make_fixnum(SEXP_FIXNUM);
  }
  op = sexp_define_foreign(ctx, env, "ws_init", 0, sexp_ws_init_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = sexp_make_fixnum(SEXP_FIXNUM);
  }
  sexp_gc_release3(ctx);
  ctx2 = ctx;  
  sexp_preserve_object(ctx, ctx2);
  return SEXP_VOID;
}

/**
// /* struct ws_s {
//   /** The Websocket protocol */
//   fio_protocol_s protocol;
//   /** connection data */
//   intptr_t fd;
//   /** callbacks */
//   void (*on_message)(ws_s *ws, fio_str_info_s msg, uint8_t is_text);
//   void (*on_shutdown)(ws_s *ws);
//   void (*on_ready)(ws_s *ws);
//   void (*on_open)(ws_s *ws);
//   void (*on_close)(intptr_t uuid, void *udata);
//   /** Opaque user data. */
//   void *udata;
//   /** The maximum websocket message size */
//   size_t max_msg_size;
//   /** active pub/sub subscriptions */
//   fio_ls_s subscriptions;
//   fio_lock_i sub_lock;
//   /** socket buffer. */
//   struct buffer_s buffer;
//   /** data length (how much of the buffer actually used). */
//   size_t length;
//   /** message buffer. */
//   FIOBJ msg;
//   /** latest text state. */
//   uint8_t is_text;
//   /** websocket connection type. */
//   uint8_t is_client;
// };
//  */
// typedef struct fio_str_info_s {
//   size_t capa; /* Buffer capacity, if the string is writable. */
//   size_t len;  /* String length. */
//   char *data;  /* String's first byte. */
// } fio_str_info_s;