#ifndef CBUILD_EXTRA_LINEAR_ALGEBRA_H
#define CBUILD_EXTRA_LINEAR_ALGEBRA_H

static void cb_linagen_define_vecn(CB_Generator *gen, char *type,
                                   char *suffix, int32_t n) {
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

#endif
