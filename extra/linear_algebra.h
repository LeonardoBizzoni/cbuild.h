#ifndef CBUILD_EXTRA_LINEAR_ALGEBRA_H
#define CBUILD_EXTRA_LINEAR_ALGEBRA_H

static char char_toupper(char ch) {
  if (ch >= 'a' && ch <= 'z') {
    return ch - ('a' - 'A');
  }
  return ch;
}

static void cb_linagen_define_vecn(CB_Generator *gen, char *type, int32_t n) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push(gen, cb_format("typedef union {"
                             "\n  %s values[%d];"
                             "\n  struct {",
                             type, n));
  for (int32_t i = 0; i < n; ++i) {
    char *prefix = " ";
    if (i % 4 == 0) {
      prefix = "\n    ";
    }
    cb_gen_push(gen, cb_format("%s%s _%d;", prefix, type, i));
  }
  cb_gen_push(gen, cb_format("\n  };"
                             "\n} Vec%d%s;\n\n",
                             n, suffix));
}

static void cb_linagen_defun_vecn_add(CB_Generator *gen, char *type,
                                      int32_t n, bool implementation) {
  char *vec_suffix = strdup(type);
  *vec_suffix = char_toupper(*type);
  cb_gen_sig_begin(gen, cb_format("linagen_fn Vec%d%s vec%d%s_add",
                                  n, vec_suffix, n, type));
    cb_gen_sig_arg(gen, cb_format("Vec%d%s v1", n, vec_suffix));
    cb_gen_sig_arg(gen, cb_format("Vec%d%s v2", n, vec_suffix));
  cb_gen_sig_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, cb_format(" {"
                             "\n  Vec%d%s res = {0};",
                             n, vec_suffix));
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  res._%d = v1._%d + v2._%d;", i, i, i));
  }
  cb_gen_push(gen, "\n  return res;"
                   "\n}\n\n");
}

static void cb_linagen_defun_vecn_add_assign(CB_Generator *gen, char *type,
                                             int32_t n, bool implementation) {
  char *vec_suffix = strdup(type);
  *vec_suffix = char_toupper(*type);
  cb_gen_sig_begin(gen, cb_format("linagen_fn void vec%d%s_add_assign",
                                  n, type));
    cb_gen_sig_arg(gen, cb_format("Vec%d%s *v1", n, vec_suffix));
    cb_gen_sig_arg(gen, cb_format("Vec%d%s v2", n, vec_suffix));
  cb_gen_sig_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, " {");
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  v1->_%d += v2._%d;", i, i));
  }
  cb_gen_push(gen, "\n}\n\n");
}

#endif
