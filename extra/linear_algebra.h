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

static void
cb_linagen_typedef_matnn(CB_Generator *gen, char *type, int32_t n);

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
static inline void
cb_linagen_defun_vecn_equal(CB_Generator *gen, char *type,
                            int32_t n, bool implementation);
static inline void
cb_linagen_defun_vecn_near(CB_Generator *gen, char *type,
                            int32_t n, bool implementation);
static inline void
cb_linagen_defun_vecn_lerp(CB_Generator *gen, char *type,
                           int32_t n, bool implementation);
static inline void
cb_linagen_defun_vec3_cross(CB_Generator *gen, char *type, bool implementation);
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
static void
cb_linagen_defun_vecn_distance(CB_Generator *gen, char *type,
                               int32_t n, bool implementation);
static void
cb_linagen_defun_vecn_distance2(CB_Generator *gen, char *type,
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

static void
cb_linagen_defun_matnn_scale(CB_Generator *gen, char *type,
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

static void
cb_linagen_typedef_matnn(CB_Generator *gen, char *type, int32_t n) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push(gen, cb_format("typedef union Mat%d%s {"
                             "\n  %s values[%d][%d];"
                             "\n  Vec%d%s cols[%d];"
                             "\n  %s arr[%d];"
                             "\n} Mat%d%s;\n\n",
                             n, suffix,
                             type, n, n,
                             n, suffix, n,
                             type, n * n,
                             n, suffix));
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

static inline void
cb_linagen_defun_vecn_equal(CB_Generator *gen, char *type,
                            int32_t n, bool implementation) {
  char *vec_suffix = strdup(type);
  *vec_suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn bool vec%d%s_equal",
                                        n, type));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s v1", n, vec_suffix));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s v2", n, vec_suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, " {\n  return v1.values[0] == v2.values[0]");
  for (int32_t i = 1; i < n; ++i) {
    cb_gen_push(gen, cb_format(" &&\n         v1.values[%d] == v2.values[%d]",
                               i, i));
  }
  cb_gen_push(gen, ";\n}\n\n");
}

static inline void
cb_linagen_defun_vecn_near(CB_Generator *gen, char *type,
                            int32_t n, bool implementation) {
  char *vec_suffix = strdup(type);
  *vec_suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn bool vec%d%s_near",
                                        n, type));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s v1", n, vec_suffix));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s v2", n, vec_suffix));
    cb_gen_push_func_arg(gen, "float eps");
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, " {\n  return ((float)v1.values[0] + eps >= v2.values[0] && (float)v1.values[0] - eps <= v2.values[0])");
  for (int32_t i = 1; i < n; ++i) {
    cb_gen_push(gen, cb_format(" &&\n         ((float)v1.values[%d] + eps >= v2.values[%d] && (float)v1.values[%d] - eps <= v2.values[%d])",
                               i, i, i, i));
  }
  cb_gen_push(gen, ";\n}\n\n");
}

static inline void
cb_linagen_defun_vecn_lerp(CB_Generator *gen, char *type,
                           int32_t n, bool implementation) {
  char *vec_suffix = strdup(type);
  *vec_suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn Vec%d%s vec%d%s_lerp",
                                        n, vec_suffix, n, type));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s start", n, vec_suffix));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s end", n, vec_suffix));
    cb_gen_push_func_arg(gen, cb_format("%s t", type));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, cb_format(" {\n  Vec%d%s res = {0};", n, vec_suffix));
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  res.values[%d] = start.values[%d] * (1 - t) + end.values[%d] * t;", i, i, i));
  }
  cb_gen_push(gen, "\n  return res;"
                   "\n}\n\n");
}

static inline void
cb_linagen_defun_vec3_cross(CB_Generator *gen, char *type,
                            bool implementation) {
  char *vec_suffix = strdup(type);
  *vec_suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn Vec3%s vec3%s_cross",
                                        vec_suffix, type));
    cb_gen_push_func_arg(gen, cb_format("Vec3%s v1", vec_suffix));
    cb_gen_push_func_arg(gen, cb_format("Vec3%s v2", vec_suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, cb_format(" {" "\n  Vec3%s res = {0};", vec_suffix));
  cb_gen_push(gen, "\n  res.values[0] = v1.values[1] * v2.values[2] - v1.values[2] * v2.values[1];");
  cb_gen_push(gen, "\n  res.values[1] = v1.values[2] * v2.values[0] - v1.values[0] * v2.values[2];");
  cb_gen_push(gen, "\n  res.values[2] = v1.values[0] * v2.values[1] - v1.values[1] * v2.values[0];");
  cb_gen_push(gen, "\n  return res;"
                   "\n}\n\n");
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

  cb_gen_push(gen, cb_format(" {\n  %s dot = {0};", type));
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  dot += v1.values[%d] * v1.values[%d];", i, i, i));
  }

  cb_gen_push(gen, cb_format("\n  float magnitude = sqrtf((float)dot);" ,
                             n, type));

  cb_gen_push(gen, cb_format("\n  Vec%d%s res = {0};"
                             "\n  if (magnitude == 0.f) { return res; }",
                             n, vec_suffix));
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  res.values[%d] = v1.values[%d] / magnitude;",
                               i, i, type));
  }

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

  cb_gen_push(gen, " {\n  float dot = {0};");
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  dot += v1.values[%d] * v1.values[%d];",
                               i, i));
  }
  cb_gen_push(gen, "\n  float res = sqrtf(dot);"
                   "\n  return res;"
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

  cb_gen_push(gen, " {\n  double dot = {0};");
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  dot += v1.values[%d] * v1.values[%d];",
                               i, i));
  }
  cb_gen_push(gen, "\n  double res = sqrt(dot);"
                   "\n  return res;"
                   "\n}\n\n");
}

static void
cb_linagen_defun_vecn_distance(CB_Generator *gen, char *type,
                               int32_t n, bool implementation) {
  char *vec_suffix = strdup(type);
  *vec_suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn float vec%d%s_distance",
                                        n, type));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s v1", n, vec_suffix));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s v2", n, vec_suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, " {");
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  v1.values[%d] -= v2.values[%d];", i, i));
  }

  cb_gen_push(gen, cb_format("\n  %s dot = {0};", type));
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  dot += v1.values[%d] * v1.values[%d];",
                               i, i));
  }
  cb_gen_push(gen, "\n  float res = sqrtf(dot);"
                   "\n  return res;"
                   "\n}\n\n");
}

static void
cb_linagen_defun_vecn_distance2(CB_Generator *gen, char *type,
                                int32_t n, bool implementation) {
  char *vec_suffix = strdup(type);
  *vec_suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn float vec%d%s_distance2",
                                        n, type));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s v1", n, vec_suffix));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s v2", n, vec_suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, " {");
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  v1.values[%d] -= v2.values[%d];", i, i));
  }

  cb_gen_push(gen, cb_format("\n  %s res = {0};", type));
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  res += v1.values[%d] * v1.values[%d];",
                               i, i));
  }
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
  cb_gen_push_func_begin(gen, cb_format("linagen_fn void vec%d%s_normalize_assign",
                                        n, type));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s *v1", n, vec_suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, cb_format(" {\n  %s dot = {0};", type));
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  dot += v1->values[%d] * v1->values[%d];",
                               i, i));
  }
  cb_gen_push(gen, "\n  float magnitude = sqrtf((float)dot);"
                   "\n  if (magnitude == 0.f) { return; }");

  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  v1->values[%d] /= magnitude;",
                               i, type));
  }
  cb_gen_push(gen, "\n}\n\n");
}

static void
cb_linagen_defun_matnn_scale(CB_Generator *gen, char *type,
                             int32_t n, bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn Mat%d%s mat%d%s_scale",
                                        n, suffix, n, type));
    cb_gen_push_func_arg(gen, cb_format("Mat%d%s m1", n, suffix));
    cb_gen_push_func_arg(gen, cb_format("%s k", type));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, cb_format(" {"
                             "\n  Mat%d%s res = {0};",
                             n, suffix));
  for (int32_t i = 0; i < n; ++i) {
    for (int32_t j = 0; j < n; ++j) {
      cb_gen_push(gen, cb_format("\n  res.values[%d][%d] = m1.values[%d][%d] * k;",
                                 i, j, i, j));
    }
  }
  cb_gen_push(gen, "\n  return res;"
                   "\n}\n\n");
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
