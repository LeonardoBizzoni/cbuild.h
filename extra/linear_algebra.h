#ifndef CBUILD_EXTRA_LINEAR_ALGEBRA_H
#define CBUILD_EXTRA_LINEAR_ALGEBRA_H

#include <stdarg.h>

internal void
cb_linagen_defun_vecn_element_wise(CB_Generator *gen, char *type,
                                   int32_t n, bool implementation,
                                   char *func_name, char op);
internal void
cb_linagen_defun_vecn_element_wise_assign(CB_Generator *gen, char *type,
                                          int32_t n, bool implementation,
                                          char *func_name, char op);

internal char char_toupper(char ch);

static void
cb_linagen_typedef_vecn(CB_Generator *gen, char *type, int32_t n, ...);
static void
cb_linagen_typedef_vecn_unnamed(CB_Generator *gen, char *type, int32_t n);

static inline void
cb_linagen_defun_vecn_add(CB_Generator *gen, char *type,
                          int32_t n, bool implementation);
static inline void
cb_linagen_defun_vecn_sub(CB_Generator *gen, char *type,
                          int32_t n, bool implementation);
static inline void
cb_linagen_defun_vecn_hadamard_prod(CB_Generator *gen, char *type,
                                    int32_t n, bool implementation);
static inline void
cb_linagen_defun_vecn_hadamard_div(CB_Generator *gen, char *type,
                                   int32_t n, bool implementation);
static void
cb_linagen_defun_vecn_scale(CB_Generator *gen, char *type,
                            int32_t n, bool implementation);
static void
cb_linagen_defun_vecn_normalize(CB_Generator *gen, char *type,
                                int32_t n, bool implementation);

static void
cb_linagen_defun_vecn_dot(CB_Generator *gen, char *type,
                          int32_t n, bool implementation);
static void
cb_linagen_defun_vecn_magnitude(CB_Generator *gen, char *type,
                                int32_t n, bool implementation);
static void
cb_linagen_defun_vecn_magnitude64(CB_Generator *gen, char *type,
                                  int32_t n, bool implementation);

static inline void
cb_linagen_defun_vecn_add_assign(CB_Generator *gen, char *type,
                                 int32_t n, bool implementation);
static inline void
cb_linagen_defun_vecn_sub_assign(CB_Generator *gen, char *type,
                                 int32_t n, bool implementation);
static void
cb_linagen_defun_vecn_hadamard_prod_assign(CB_Generator *gen, char *type,
                                      int32_t n, bool implementation);
static void
cb_linagen_defun_vecn_hadamard_div_assign(CB_Generator *gen, char *type,
                                      int32_t n, bool implementation);
static void
cb_linagen_defun_vecn_scale_assign(CB_Generator *gen, char *type,
                                   int32_t n, bool implementation);
static void
cb_linagen_defun_vecn_normalize_assign(CB_Generator *gen, char *type,
                                       int32_t n, bool implementation);

// ======================================================================
// Implementations
static void
cb_linagen_typedef_vecn(CB_Generator *gen, char *type, int32_t n, ...) {
  va_list args = {0};
  va_start(args, n);
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push(gen, cb_format("typedef union {"
                             "\n  %s values[%d];"
                             "\n  struct {",
                             type, n));
  bool found_last_named_field = false;
  for (int32_t i = 0; i < n; ++i) {
    char *prefix = " ";
    if (i % 4 == 0) {
      prefix = "\n    ";
    }
    char *field;
    if (found_last_named_field) {
      field = cb_format("_%d", i);
    } else {
      field = va_arg(args, char*);
      if (!field) {
        field = cb_format("_%d", i);
        found_last_named_field = true;
      }
    }
    cb_gen_push(gen, cb_format("%s%s %s;", prefix, type, field));
  }
  cb_gen_push(gen, cb_format("\n  };"
                             "\n} Vec%d%s;\n\n",
                             n, suffix));
  va_end(args);
}

static void
cb_linagen_typedef_vecn_unnamed(CB_Generator *gen, char *type, int32_t n) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push(gen, cb_format("typedef struct Vec%d%s {"
                             "\n  %s values[%d];"
                             "\n} Vec%d%s;\n\n",
                             n, suffix, type, n, n, suffix));
}


static inline void
cb_linagen_defun_vecn_add(CB_Generator *gen, char *type,
                          int32_t n, bool implementation) {
  cb_linagen_defun_vecn_element_wise(gen, type, n, implementation, "add", '+');
}

static inline void
cb_linagen_defun_vecn_sub(CB_Generator *gen, char *type,
                          int32_t n, bool implementation) {
  cb_linagen_defun_vecn_element_wise(gen, type, n, implementation, "sub", '-');
}

static inline void
cb_linagen_defun_vecn_hadamard_prod(CB_Generator *gen, char *type,
                                    int32_t n, bool implementation) {
  cb_linagen_defun_vecn_element_wise(gen, type, n, implementation,
                                     "hadamard_prod", '*');
}

static inline void
cb_linagen_defun_vecn_hadamard_div(CB_Generator *gen, char *type,
                                   int32_t n, bool implementation) {
  cb_linagen_defun_vecn_element_wise(gen, type, n, implementation,
                                     "hadamard_div", '/');
}

static void
cb_linagen_defun_vecn_scale(CB_Generator *gen, char *type,
                            int32_t n, bool implementation) {
  char *vec_suffix = strdup(type);
  *vec_suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn Vec%d%s vec%d%s_scale",
                                        n, vec_suffix, n, type));
  cb_gen_push_func_arg(gen, cb_format("Vec%d%s v1", n, vec_suffix));
  cb_gen_push_func_arg(gen, cb_format("%s k", type));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, cb_format(" {"
                             "\n  Vec%d%s res = {0};",
                             n, vec_suffix));
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  res.values[%d] = v1.values[%d] * k;", i, i));
  }
  cb_gen_push(gen, "\n  return res;"
                   "\n}\n\n");
}

static void
cb_linagen_defun_vecn_normalize(CB_Generator *gen, char *type,
                                int32_t n, bool implementation) {
  char *vec_suffix = strdup(type);
  *vec_suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn Vec%d%s vec%d%s_normalize",
                                        n, vec_suffix, n, type));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s v1", n, vec_suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  // NOTE(lb): should generated function use other generated functions,
  //           or should they completely independent from each other?
  cb_gen_push(gen, cb_format(" {"
                             "\n  Vec%d%s res = vec%d%s_scale(v1, ((%s)1) / vec%d%s_magnitude(v1));",
                             n, vec_suffix, n, type, type, n, type));
  cb_gen_push(gen, "\n  return res;"
                   "\n}\n\n");
}

static void
cb_linagen_defun_vecn_dot(CB_Generator *gen, char *type,
                          int32_t n, bool implementation) {
  char *vec_suffix = strdup(type);
  *vec_suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn %s vec%d%s_dot",
                                        type, n, type));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s v1", n, vec_suffix));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s v2", n, vec_suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, cb_format(" {" "\n  %s res = {0};", type));
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  res += v1.values[%d] * v2.values[%d];", i, i, i));
  }
  cb_gen_push(gen, "\n  return res;"
                   "\n}\n\n");
}

static void
cb_linagen_defun_vecn_magnitude(CB_Generator *gen, char *type,
                                int32_t n, bool implementation) {
  char *vec_suffix = strdup(type);
  *vec_suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn float vec%d%s_magnitude",
                                        n, type));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s v1", n, vec_suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, cb_format(" {"
                             "\n  float res = sqrtf((float)vec%d%s_dot(v1, v1));",
                             n, type));
  cb_gen_push(gen, "\n  return res;"
                   "\n}\n\n");
}

static void
cb_linagen_defun_vecn_magnitude64(CB_Generator *gen, char *type,
                                  int32_t n, bool implementation) {
  char *vec_suffix = strdup(type);
  *vec_suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn double vec%d%s_magnitude64",
                                        n, type));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s v1", n, vec_suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, cb_format(" {"
                             "\n  double res = sqrt((double)vec%d%s_dot(v1, v1));",
                             n, type));
  cb_gen_push(gen, "\n  return res;"
                   "\n}\n\n");
}

static inline void
cb_linagen_defun_vecn_add_assign(CB_Generator *gen, char *type,
                                 int32_t n, bool implementation) {
  cb_linagen_defun_vecn_element_wise_assign(gen, type, n, implementation,
                                            "add", '+');
}

static inline void
cb_linagen_defun_vecn_sub_assign(CB_Generator *gen, char *type,
                                 int32_t n, bool implementation) {
  cb_linagen_defun_vecn_element_wise_assign(gen, type, n, implementation,
                                            "sub", '-');
}

static void
cb_linagen_defun_vecn_hadamard_prod_assign(CB_Generator *gen, char *type,
                                      int32_t n, bool implementation) {
  cb_linagen_defun_vecn_element_wise_assign(gen, type, n, implementation,
                                            "hadamard_prod", '*');
}

static void
cb_linagen_defun_vecn_hadamard_div_assign(CB_Generator *gen, char *type,
                                      int32_t n, bool implementation) {
  cb_linagen_defun_vecn_element_wise_assign(gen, type, n, implementation,
                                            "hadamard_div", '/');
}

static void
cb_linagen_defun_vecn_scale_assign(CB_Generator *gen, char *type,
                                   int32_t n, bool implementation) {
  char *vec_suffix = strdup(type);
  *vec_suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen,
                         cb_format("linagen_fn void vec%d%s_scale_assign",
                                   n, type));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s *v1", n, vec_suffix));
    cb_gen_push_func_arg(gen, cb_format("%s k", type));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, " {");
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  v1->values[%d] *= k;", i));
  }
  cb_gen_push(gen, "\n}\n\n");
}

static void
cb_linagen_defun_vecn_normalize_assign(CB_Generator *gen, char *type,
                                       int32_t n, bool implementation) {
  char *vec_suffix = strdup(type);
  *vec_suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn Vec%d%s vec%d%s_normalize_assign",
                                        n, vec_suffix, n, type));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s *v1", n, vec_suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, cb_format(" {"
                             "\n  vec%d%s_scale_assign(v1, ((%s)1) / vec%d%s_magnitude(*v1));"
                              "\n}\n\n",
                             n, type, type, n, type));
}


internal void
cb_linagen_defun_vecn_element_wise(CB_Generator *gen, char *type,
                                   int32_t n, bool implementation,
                                   char *func_name, char op) {
  char *vec_suffix = strdup(type);
  *vec_suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn Vec%d%s vec%d%s_%s",
                                        n, vec_suffix, n, type, func_name));
  cb_gen_push_func_arg(gen, cb_format("Vec%d%s v1", n, vec_suffix));
  cb_gen_push_func_arg(gen, cb_format("Vec%d%s v2", n, vec_suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, cb_format(" {"
                             "\n  Vec%d%s res = {0};",
                             n, vec_suffix));
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  res.values[%d] = v1.values[%d] %c v2.values[%d];",
                               i, i, op, i));
  }
  cb_gen_push(gen, "\n  return res;"
                   "\n}\n\n");
}

internal void
cb_linagen_defun_vecn_element_wise_assign(CB_Generator *gen, char *type,
                                          int32_t n, bool implementation,
                                          char *func_name, char op) {
  char *vec_suffix = strdup(type);
  *vec_suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn void vec%d%s_%s_assign",
                                        n, type, func_name));
  cb_gen_push_func_arg(gen, cb_format("Vec%d%s *v1", n, vec_suffix));
  cb_gen_push_func_arg(gen, cb_format("Vec%d%s v2", n, vec_suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, " {");
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  v1->values[%d] %c= v2.values[%d];", i, op, i));
  }
  cb_gen_push(gen, "\n}\n\n");
}

internal char char_toupper(char ch) {
  if (ch >= 'a' && ch <= 'z') {
    return ch - ('a' - 'A');
  }
  return ch;
}

#endif
