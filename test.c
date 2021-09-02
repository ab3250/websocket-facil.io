/* Automatically generated by chibi-ffi; version: 0.4 */

#include <chibi/eval.h>

#include "chibi/eval.h"

#include "fio.h"

#include "fio_cli.h"

#include "fio_tls.h"

#include "http.h"

#include "redis_engine.h"
/*
types: (fio_str_info_s)
enums: ()
*/

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

sexp sexp_ws_write_stub (sexp ctx, sexp self, sexp_sint_t n, sexp arg0, sexp arg1, sexp arg2) {
  sexp res;
  if (! sexp_stringp(arg1))
    return sexp_type_exception(ctx, self, SEXP_STRING, arg1);
  if (! sexp_exact_integerp(arg2))
    return sexp_type_exception(ctx, self, SEXP_FIXNUM, arg2);
  res = ((ws_write(arg0, sexp_string_data(arg1), sexp_sint_value(arg2))), SEXP_VOID);
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
  op = sexp_define_foreign(ctx, env, "ws_write", 3, sexp_ws_write_stub);
  if (sexp_opcodep(op)) {
    sexp_opcode_return_type(op) = SEXP_VOID;
    sexp_opcode_arg2_type(op) = sexp_make_fixnum(SEXP_STRING);
    sexp_opcode_arg3_type(op) = sexp_make_fixnum(SEXP_FIXNUM);
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
  return SEXP_VOID;
}

