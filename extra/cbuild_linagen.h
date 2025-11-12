#ifndef CBUILD_EXTRA_LINAGEN_H
#define CBUILD_EXTRA_LINAGEN_H

#define CB_LINAGEN_VECN_COPY_CAP 4
#define CB_LINAGEN_MATNN_COPY_CAP 4

#define MIN(a, b) ((a) <= (b) ? (a) : (b))

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdarg.h>

internal void
cb_linagen_defun_vecn_element_wise(CB_Generator *gen, char *type,
                                   int32_t n, bool implementation,
                                   char *func_name, char op);

internal void
cb_linagen_defun_matnn_element_wise(CB_Generator *gen, char *type,
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
cb_linagen_defun_vecn(CB_Generator *gen, char *type,
                      int32_t n, bool implementation);
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
cb_linagen_defun_vecn_mulm(CB_Generator *gen, char *type,
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
cb_linagen_defun_vec3_cross(CB_Generator *gen, char *type, bool implementation);

static void
cb_linagen_defun_matnn_scale(CB_Generator *gen, char *type,
                             int32_t n, bool implementation);
static void
cb_linagen_defun_matnn_mulv(CB_Generator *gen, char *type,
                            int32_t n, bool implementation);
static void
cb_linagen_defun_matnn_identity(CB_Generator *gen, char *type,
                                int32_t n, bool implementation);

static void
cb_linagen_defun_mat4_transform_translate(CB_Generator *gen, char *type,
                                          bool implementation);
static void
cb_linagen_defun_mat4_transform_rotate(CB_Generator *gen, char *type,
                                       bool implementation);
static void
cb_linagen_defun_mat4_transform_scale(CB_Generator *gen, char *type,
                                      bool implementation);
static void
cb_linagen_defun_mat4_projection_orthographics(CB_Generator *gen, char *type,
                                               bool implementation);

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
cb_linagen_defun_vecn(CB_Generator *gen, char *type,
                      int32_t n, bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn Vec%d%s vec%d%s",
                                        n, suffix, n, type));
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push_func_arg(gen, cb_format("%s _%d", type, i));
  }
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, cb_format(" {" "\n  Vec%d%s res = {0};", n, suffix));
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  res.values[%d] = _%d;", i, i));
  }
  cb_gen_push(gen, "\n  return res;" "\n}\n\n");
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
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn bool vec%d%s_equal", n, type));
    cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *restrict v1", n, suffix));
    cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *restrict v2", n, suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, " {\n  return v1->values[0] == v2->values[0]");
  for (int32_t i = 1; i < n; ++i) {
    cb_gen_push(gen, cb_format(" &&\n         v1->values[%d] == v2->values[%d]",
                               i, i));
  }
  cb_gen_push(gen, ";\n}\n\n");
}

static inline void
cb_linagen_defun_vecn_near(CB_Generator *gen, char *type,
                            int32_t n, bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn bool vec%d%s_near",
                                        n, type));
    cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *restrict v1", n, suffix));
    cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *restrict v2", n, suffix));
    cb_gen_push_func_arg(gen, "float eps");
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, " {\n  return ((float)v1->values[0] + eps >= v2->values[0] && (float)v1->values[0] - eps <= v2->values[0])");
  for (int32_t i = 1; i < n; ++i) {
    cb_gen_push(gen, cb_format(" &&\n         ((float)v1->values[%d] + eps >= v2->values[%d] && (float)v1->values[%d] - eps <= v2->values[%d])",
                               i, i, i, i));
  }
  cb_gen_push(gen, ";\n}\n\n");
}

static inline void
cb_linagen_defun_vecn_lerp(CB_Generator *gen, char *type,
                           int32_t n, bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  if (n <= CB_LINAGEN_VECN_COPY_CAP) {
    cb_gen_push_func_begin(gen, cb_format("linagen_fn Vec%d%s vec%d%s_lerp", n, suffix, n, type));
      cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *restrict start", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *restrict end", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("%s t", type));
    cb_gen_push_func_end(gen, implementation);
    if (!implementation) { return; }
    cb_gen_push(gen, cb_format(" {" "\n  Vec%d%s res = {0};", n, suffix));
    for (int32_t i = 0; i < n; ++i) {
      cb_gen_push(gen, cb_format("\n  res.values[%d] = start->values[%d] * (1 - t) + end->values[%d] * t;", i, i, i));
    }
    cb_gen_push(gen, "\n  return res;" "\n}\n\n");
  } else {
    cb_gen_push_func_begin(gen, cb_format("linagen_fn void vec%d%s_lerp", n, type));
      cb_gen_push_func_arg(gen, cb_format("Vec%d%s *restrict res", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *restrict start", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *restrict end", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("%s t", type));
    cb_gen_push_func_end(gen, implementation);
    if (!implementation) { return; }
    cb_gen_push(gen, " {");
    for (int32_t i = 0; i < n; ++i) {
      cb_gen_push(gen, cb_format("\n  res->values[%d] = start->values[%d] * (1 - t) + end->values[%d] * t;", i, i, i));
    }
    cb_gen_push(gen, "\n}\n\n");
  }
}

static inline void
cb_linagen_defun_vec3_cross(CB_Generator *gen, char *type,
                            bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn Vec3%s vec3%s_cross", suffix, type));
    cb_gen_push_func_arg(gen, cb_format("Vec3%s v1", suffix));
    cb_gen_push_func_arg(gen, cb_format("Vec3%s v2", suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, cb_format(" {" "\n  Vec3%s res = {0};", suffix));
  cb_gen_push(gen, "\n  res.values[0] = v1.values[1] * v2.values[2] - v1.values[2] * v2.values[1];");
  cb_gen_push(gen, "\n  res.values[1] = v1.values[2] * v2.values[0] - v1.values[0] * v2.values[2];");
  cb_gen_push(gen, "\n  res.values[2] = v1.values[0] * v2.values[1] - v1.values[1] * v2.values[0];");
  cb_gen_push(gen, "\n  return res;" "\n}\n\n");
}

static void
cb_linagen_defun_vecn_scale(CB_Generator *gen, char *type,
                            int32_t n, bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  if (n <= CB_LINAGEN_VECN_COPY_CAP) {
    cb_gen_push_func_begin(gen, cb_format("linagen_fn Vec%d%s vec%d%s_scale", n, suffix, n, type));
      cb_gen_push_func_arg(gen, cb_format("Vec%d%s v", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("%s k", type));
    cb_gen_push_func_end(gen, implementation);
    if (!implementation) { return; }
    cb_gen_push(gen, " {");
    for (int32_t i = 0; i < n; ++i) {
      cb_gen_push(gen, cb_format("\n  v.values[%d] *= k;", i, i));
    }
    cb_gen_push(gen, "\n  return v;" "\n}\n\n");
  } else {
    cb_gen_push_func_begin(gen, cb_format("linagen_fn void vec%d%s_scale", n, type));
      cb_gen_push_func_arg(gen, cb_format("Vec%d%s *res", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *v1", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("%s k", type));
    cb_gen_push_func_end(gen, implementation);
    if (!implementation) { return; }
    cb_gen_push(gen, " {");
    for (int32_t i = 0; i < n; ++i) {
      cb_gen_push(gen, cb_format("\n  res->values[%d] = v1->values[%d] * k;", i, i));
    }
    cb_gen_push(gen, "\n}\n\n");
  }
}

static void
cb_linagen_defun_vecn_normalize(CB_Generator *gen, char *type,
                                int32_t n, bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  if (n <= CB_LINAGEN_VECN_COPY_CAP) {
    cb_gen_push_func_begin(gen, cb_format("linagen_fn Vec%d%s vec%d%s_normalize", n, suffix, n, type));
      cb_gen_push_func_arg(gen, cb_format("Vec%d%s v", n, suffix));
    cb_gen_push_func_end(gen, implementation);
    if (!implementation) { return; }
    cb_gen_push(gen, cb_format(" {\n  %s dot = {0};", type));
    for (int32_t i = 0; i < n; ++i) {
      cb_gen_push(gen, cb_format("\n  dot += v.values[%d] * v.values[%d];", i, i));
    }
    cb_gen_push(gen, "\n  float magnitude = sqrtf((float)dot);"
                     "\n  if (magnitude == 0.f) { return v; }");
    for (int32_t i = 0; i < n; ++i) {
      cb_gen_push(gen, cb_format("\n  v.values[%d] /= magnitude;", i, type));
    }
    cb_gen_push(gen, "\n  return v;" "\n}\n\n");
  } else {
    cb_gen_push_func_begin(gen, cb_format("linagen_fn void vec%d%s_normalize", n, type));
      cb_gen_push_func_arg(gen, cb_format("Vec%d%s *res", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *v1", n, suffix));
    cb_gen_push_func_end(gen, implementation);
    if (!implementation) { return; }
    cb_gen_push(gen, cb_format(" {\n  %s dot = {0};", type));
    for (int32_t i = 0; i < n; ++i) {
      cb_gen_push(gen, cb_format("\n  dot += v1->values[%d] * v1->values[%d];", i, i, i));
    }
    cb_gen_push(gen, "\n  float magnitude = sqrtf((float)dot);"
                     "\n  if (magnitude == 0.f) { return; }");
    for (int32_t i = 0; i < n; ++i) {
      cb_gen_push(gen, cb_format("\n  res->values[%d] = v1->values[%d] / magnitude;",
                                 i, i, type));
    }
    cb_gen_push(gen, "\n}\n\n");
  }
}

static void
cb_linagen_defun_vecn_dot(CB_Generator *gen, char *type,
                          int32_t n, bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn %s vec%d%s_dot", type, n, type));
    cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *v1", n, suffix));
    cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *v2", n, suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, cb_format(" {" "\n  %s res = {0};", type));
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  res += v1->values[%d] * v2->values[%d];", i, i, i));
  }
  cb_gen_push(gen, "\n  return res;" "\n}\n\n");
}

static void
cb_linagen_defun_vecn_magnitude(CB_Generator *gen, char *type,
                                int32_t n, bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn float vec%d%s_magnitude",
                                        n, type));
    cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *v1", n, suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, " {\n  float dot = {0};");
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  dot += v1->values[%d] * v1->values[%d];",
                               i, i));
  }
  cb_gen_push(gen, "\n  float res = sqrtf(dot);"
                   "\n  return res;"
                   "\n}\n\n");
}

static void
cb_linagen_defun_vecn_magnitude64(CB_Generator *gen, char *type,
                                  int32_t n, bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn double vec%d%s_magnitude64",
                                        n, type));
    cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *v1", n, suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, " {\n  double dot = {0};");
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  dot += v1->values[%d] * v1->values[%d];",
                               i, i));
  }
  cb_gen_push(gen, "\n  double res = sqrt(dot);"
                   "\n  return res;"
                   "\n}\n\n");
}

static void
cb_linagen_defun_vecn_distance(CB_Generator *gen, char *type,
                               int32_t n, bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn float vec%d%s_distance",
                                        n, type));
    cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *restrict v1", n, suffix));
    cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *restrict v2", n, suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, cb_format(" {" "\n  Vec%d%s diff = {0};", n, suffix));
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  diff.values[%d] = v1->values[%d] - v2->values[%d];", i, i, i));
  }

  cb_gen_push(gen, cb_format("\n  %s dot = {0};", type));
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  dot += diff.values[%d] * diff.values[%d];",
                               i, i));
  }
  cb_gen_push(gen, "\n  float res = sqrtf(dot);"
                   "\n  return res;"
                   "\n}\n\n");
}

static void
cb_linagen_defun_vecn_distance2(CB_Generator *gen, char *type,
                                int32_t n, bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn float vec%d%s_distance2",
                                        n, type));
    cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *restrict v1", n, suffix));
    cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *restrict v2", n, suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, cb_format(" {" "\n  Vec%d%s diff = {0};", n, suffix));
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  diff.values[%d] = v1->values[%d] - v2->values[%d];", i, i, i));
  }

  cb_gen_push(gen, cb_format("\n  %s dot = {0};", type));
  for (int32_t i = 0; i < n; ++i) {
    cb_gen_push(gen, cb_format("\n  dot += diff.values[%d] * diff.values[%d];",
                               i, i));
  }
  cb_gen_push(gen, "\n  return dot;"
                   "\n}\n\n");
}

static void
cb_linagen_defun_vecn_mulm(CB_Generator *gen, char *type,
                           int32_t n, bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  if (n < MIN(CB_LINAGEN_VECN_COPY_CAP, CB_LINAGEN_MATNN_COPY_CAP)) {
    cb_gen_push_func_begin(gen, cb_format("linagen_fn Vec%d%s vec%d%s_mulm", n, suffix, n, type));
      cb_gen_push_func_arg(gen, cb_format("Vec%d%s v", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("Mat%d%s m", n, suffix));
    cb_gen_push_func_end(gen, implementation);
    if (!implementation) { return; }
    cb_gen_push(gen, cb_format(" {" "\n  Vec%d%s res = {0};", n, suffix));
    for (int32_t i = 0; i < n; ++i) {
      char *prefix = "";
      cb_gen_push(gen, cb_format("\n  res.values[%d] = ", i));
      for (int32_t j = 0; j < n; ++j) {
        cb_gen_push(gen, cb_format("%s(v.values[%d] * m.values[%d][%d])",
                                   prefix, j, i, j));
        prefix = " + ";
      }
      cb_gen_push(gen, ";");
    }
    cb_gen_push(gen, "\n  return res;" "\n}\n\n");
  } else {
    cb_gen_push_func_begin(gen, cb_format("linagen_fn void vec%d%s_mulm", n, type));
      cb_gen_push_func_arg(gen, cb_format("Vec%d%s *restrict res", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *restrict v", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("const Mat%d%s *restrict m", n, suffix));
    cb_gen_push_func_end(gen, implementation);
    if (!implementation) { return; }

    cb_gen_push(gen, " {");
    for (int32_t i = 0; i < n; ++i) {
      char *prefix = "";
      cb_gen_push(gen, cb_format("\n  res->values[%d] = ", i));
      for (int32_t j = 0; j < n; ++j) {
        cb_gen_push(gen, cb_format("%s(v->values[%d] * m->values[%d][%d])",
                                   prefix, j, i, j));
        prefix = " + ";
      }
      cb_gen_push(gen, ";");
    }
    cb_gen_push(gen, "\n}\n\n");
  }
}

static void
cb_linagen_defun_matnn_scale(CB_Generator *gen, char *type,
                             int32_t n, bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  if (n < CB_LINAGEN_MATNN_COPY_CAP) {
    cb_gen_push_func_begin(gen, cb_format("linagen_fn Mat%d%s mat%d%s_scale", n, suffix, n, type));
      cb_gen_push_func_arg(gen, cb_format("Mat%d%s m1", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("%s k", type));
    cb_gen_push_func_end(gen, implementation);
    if (!implementation) { return; }

    cb_gen_push(gen, " {");
    for (int32_t i = 0; i < n; ++i) {
      for (int32_t j = 0; j < n; ++j) {
        cb_gen_push(gen, cb_format("\n  m1.values[%d][%d] *= k;", i, j));
      }
    }
    cb_gen_push(gen, "\n  return m1;" "\n}\n\n");
  } else {
    cb_gen_push_func_begin(gen, cb_format("linagen_fn void mat%d%s_scale", n, type));
      cb_gen_push_func_arg(gen, cb_format("Mat%d%s *res", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("const Mat%d%s *m1", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("%s k", type));
    cb_gen_push_func_end(gen, implementation);
    if (!implementation) { return; }

    cb_gen_push(gen, " {");
    for (int32_t i = 0; i < n; ++i) {
      for (int32_t j = 0; j < n; ++j) {
        cb_gen_push(gen, cb_format("\n  res->values[%d][%d] = m1->values[%d][%d] * k;",
                                   i, j, i, j));
      }
    }
    cb_gen_push(gen, "\n}\n\n");
  }
}

static inline void
cb_linagen_defun_matnn_add(CB_Generator *gen, char *type,
                           int32_t n, bool implementation) {
  cb_linagen_defun_matnn_element_wise(gen, type, n, implementation, "add", '+');
}

static inline void
cb_linagen_defun_matnn_sub(CB_Generator *gen, char *type,
                           int32_t n, bool implementation) {
  cb_linagen_defun_matnn_element_wise(gen, type, n, implementation, "sub", '-');
}

static inline void
cb_linagen_defun_matnn_hadamard_prod(CB_Generator *gen, char *type,
                                     int32_t n, bool implementation) {
  cb_linagen_defun_matnn_element_wise(gen, type, n, implementation,
                                      "hadamard_prod", '*');
}

static inline void
cb_linagen_defun_matnn_hadamard_div(CB_Generator *gen, char *type,
                                    int32_t n, bool implementation) {
  cb_linagen_defun_matnn_element_wise(gen, type, n, implementation,
                                      "hadamard_div", '/');
}

static void
cb_linagen_defun_matnn_dot(CB_Generator *gen, char *type,
                           int32_t n, bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  if (n <= CB_LINAGEN_MATNN_COPY_CAP) {
    cb_gen_push_func_begin(gen, cb_format("linagen_fn Mat%d%s mat%d%s_dot", n, suffix, n, type));
      cb_gen_push_func_arg(gen, cb_format("Mat%d%s m1", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("Mat%d%s m2", n, suffix));
    cb_gen_push_func_end(gen, implementation);
    if (!implementation) { return; }

    cb_gen_push(gen, cb_format(" {" "\n  Mat%d%s res = {0};", n, suffix));
    for (int32_t col = 0; col < n; ++col) {
      for (int32_t row = 0; row < n; ++row) {
        cb_gen_push(gen, cb_format("\n  res.values[%d][%d] = ", col, row));
        char *prefix = "";
        for (int32_t k = 0; k < n; ++k) {
          cb_gen_push(gen, cb_format("%s(m1.values[%d][%d] * m2.values[%d][%d])",
                                     prefix, k, row, col, k));
          prefix = " + ";
        }
        cb_gen_push(gen, ";");
      }
    }
    cb_gen_push(gen, "\n  return res;" "\n}\n\n");
  } else {
    cb_gen_push_func_begin(gen, cb_format("linagen_fn void mat%d%s_dot", n, type));
      cb_gen_push_func_arg(gen, cb_format("Mat%d%s *restrict res", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("const Mat%d%s *m1", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("const Mat%d%s *m2", n, suffix));
    cb_gen_push_func_end(gen, implementation);
    if (!implementation) { return; }

    cb_gen_push(gen, " {");
    for (int32_t col = 0; col < n; ++col) {
      for (int32_t row = 0; row < n; ++row) {
        cb_gen_push(gen, cb_format("\n  res->values[%d][%d] = ", col, row));
        char *prefix = "";
        for (int32_t k = 0; k < n; ++k) {
          cb_gen_push(gen, cb_format("%s(m1->values[%d][%d] * m2->values[%d][%d])",
                                     prefix, k, row, col, k));
          prefix = " + ";
        }
        cb_gen_push(gen, ";");
      }
    }
    cb_gen_push(gen, "\n}\n\n");
  }
}

static void
cb_linagen_defun_matnn_mulv(CB_Generator *gen, char *type,
                            int32_t n, bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  if (n <= CB_LINAGEN_MATNN_COPY_CAP) {
    cb_gen_push_func_begin(gen, cb_format("linagen_fn Vec%d%s mat%d%s_mulv", n, suffix, n, type));
      cb_gen_push_func_arg(gen, cb_format("Mat%d%s m", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("Vec%d%s v", n, suffix));
    cb_gen_push_func_end(gen, implementation);
    if (!implementation) { return; }

    cb_gen_push(gen, cb_format(" {" "\n  Vec%d%s res = {0};", n, suffix));
    for (int32_t i = 0; i < n; ++i) {
      char *prefix = "";
      cb_gen_push(gen, cb_format("\n  res.values[%d] = ", i));
      for (int32_t j = 0; j < n; ++j) {
        cb_gen_push(gen, cb_format("%s(m.values[%d][%d] * v.values[%d])",
                                   prefix, j, i, j));
        prefix = " + ";
      }
      cb_gen_push(gen, ";");
    }
    cb_gen_push(gen, "\n  return res;" "\n}\n\n");
  } else {
    cb_gen_push_func_begin(gen, cb_format("linagen_fn void mat%d%s_mulv", n, type));
      cb_gen_push_func_arg(gen, cb_format("Vec%d%s *restrict res", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("const Mat%d%s *restrict m", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *restrict v", n, suffix));
    cb_gen_push_func_end(gen, implementation);
    if (!implementation) { return; }

    cb_gen_push(gen, " {");
    for (int32_t i = 0; i < n; ++i) {
      char *prefix = "";
      cb_gen_push(gen, cb_format("\n  res->values[%d] = ", i));
      for (int32_t j = 0; j < n; ++j) {
        cb_gen_push(gen, cb_format("%s(m->values[%d][%d] * v->values[%d])",
                                   prefix, j, i, j));
        prefix = " + ";
      }
      cb_gen_push(gen, ";");
    }
    cb_gen_push(gen, "\n}\n\n");
  }
}

static void
cb_linagen_defun_matnn_identity(CB_Generator *gen, char *type,
                                int32_t n, bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  if (n <= CB_LINAGEN_MATNN_COPY_CAP) {
    cb_gen_push_func_begin(gen, cb_format("linagen_fn Mat%d%s mat%d%s_identity", n,suffix, n, type));
    cb_gen_push_func_end(gen, implementation);
    if (!implementation) { return; }

    cb_gen_push(gen, cb_format(" {" "\n  Mat%d%s res = {0};", n, suffix));
    for (int32_t i = 0; i < n; ++i) {
      cb_gen_push(gen, cb_format("\n  res.values[%d][%d] = 1;", i, i));
    }
    cb_gen_push(gen, "\n  return res;" "\n}\n\n");
  } else {
    cb_gen_push_func_begin(gen, cb_format("linagen_fn void mat%d%s_identity", n, type));
      cb_gen_push_func_arg(gen, cb_format("Mat%d%s *res", n, suffix));
    cb_gen_push_func_end(gen, implementation);
    if (!implementation) { return; }

    cb_gen_push(gen, " {");
    cb_gen_push(gen, "\n  memset(res, 0, sizeof *res);");
    for (int32_t i = 0; i < n; ++i) {
      cb_gen_push(gen, cb_format("\n  res->values[%d][%d] = 1;", i, i));
    }
    cb_gen_push(gen, "\n}\n\n");
  }
}

static void
cb_linagen_defun_mat4_transform_translate_by(CB_Generator *gen, char *type, bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn Mat4%s mat4%s_transform_translate_by", suffix, type));
    cb_gen_push_func_arg(gen, cb_format("Mat4%s m", suffix));
    cb_gen_push_func_arg(gen, cb_format("Vec3%s by", suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, " {");
  for (int32_t i = 0; i < 3; ++i) {
    cb_gen_push(gen, cb_format("\n  m.values[3][%d] += by.values[%d];", i, i));
  }
  cb_gen_push(gen, "\n  return m;" "\n}\n\n");
}

static void
cb_linagen_defun_mat4_transform_translate_to(CB_Generator *gen, char *type, bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn Mat4%s mat4%s_transform_translate_to", suffix, type));
    cb_gen_push_func_arg(gen, cb_format("Mat4%s m", suffix));
    cb_gen_push_func_arg(gen, cb_format("Vec3%s to", suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, " {");
  for (int32_t i = 0; i < 3; ++i) {
    cb_gen_push(gen, cb_format("\n  m.values[3][%d] = to.values[%d];", i, i));
  }
  cb_gen_push(gen, "\n  return m;" "\n}\n\n");
}

static void
cb_linagen_defun_mat4_transform_rotate(CB_Generator *gen, char *type, bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn Mat4%s mat4%s_transform_rotate", suffix, type));
    cb_gen_push_func_arg(gen, cb_format("Mat4%s m", suffix));
    cb_gen_push_func_arg(gen, "float degrees");
    cb_gen_push_func_arg(gen, cb_format("Vec3%s axis", suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, cb_format(" {"
                             "\n  Mat4%s res = {0};"
                             "\n  float radians = degrees * %.16ff / 180.f;", suffix, (float)M_PI));
  cb_gen_push(gen, cb_format("\n  Mat4%s rot = { .values = {"
                             "\n    { cosf(radians) + axis.values[0]*axis.values[0]*(1 - cosf(radians)), axis.values[0]*axis.values[1]*(1 - cosf(radians)) + axis.values[2]*sinf(radians), axis.values[0]*axis.values[2]*(1 - cosf(radians)) - axis.values[1]*sinf(radians), 0 },"
                             "\n    { axis.values[1]*axis.values[0]*(1 - cosf(radians)) - axis.values[2]*sinf(radians), cosf(radians) + axis.values[1]*axis.values[1]*(1 - cosf(radians)), axis.values[1]*axis.values[2]*(1 - cosf(radians)) + axis.values[0]*sinf(radians), 0 },"
                             "\n    { axis.values[2]*axis.values[0]*(1 - cosf(radians)) + axis.values[1]*sinf(radians), axis.values[2]*axis.values[1]*(1 - cosf(radians)) - axis.values[0]*sinf(radians), cosf(radians) + axis.values[2]*axis.values[2]*(1 - cosf(radians)), 0 },"
                             "\n    { 0, 0, 0, 1 }"
                             "\n  }};" , suffix));
  for (int32_t i = 0; i < 4; ++i) {
    for (int32_t j = 0; j < 4; ++j) {
      cb_gen_push(gen, cb_format("\n  res.values[%d][%d] = ", i, j));
      char *prefix = "";
      for (int32_t k = 0; k < 4; ++k) {
        cb_gen_push(gen, cb_format("%s(rot.values[%d][%d]) * m.values[%d][%d]",
                                   prefix, k, j, i, k));
        prefix = " + ";
      }
      cb_gen_push(gen, ";");
    }
  }
  cb_gen_push(gen, "\n  return res;" "\n}\n\n");
}

static void
cb_linagen_defun_mat4_transform_rotate_rad(CB_Generator *gen, char *type, bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn Mat4%s mat4%s_transform_rotate_rad", suffix, type));
    cb_gen_push_func_arg(gen, cb_format("Mat4%s m", suffix));
    cb_gen_push_func_arg(gen, "float radians");
    cb_gen_push_func_arg(gen, cb_format("Vec3%s axis", suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, cb_format(" {" "\n  Mat4%s res = {0};", suffix));
  cb_gen_push(gen, cb_format("\n  Mat4%s rot = { .values = {"
                             "\n    { cosf(radians) + axis.values[0]*axis.values[0]*(1 - cosf(radians)), axis.values[0]*axis.values[1]*(1 - cosf(radians)) + axis.values[2]*sinf(radians), axis.values[0]*axis.values[2]*(1 - cosf(radians)) - axis.values[1]*sinf(radians), 0 },"
                             "\n    { axis.values[1]*axis.values[0]*(1 - cosf(radians)) - axis.values[2]*sinf(radians), cosf(radians) + axis.values[1]*axis.values[1]*(1 - cosf(radians)), axis.values[1]*axis.values[2]*(1 - cosf(radians)) + axis.values[0]*sinf(radians), 0 },"
                             "\n    { axis.values[2]*axis.values[0]*(1 - cosf(radians)) + axis.values[1]*sinf(radians), axis.values[2]*axis.values[1]*(1 - cosf(radians)) - axis.values[0]*sinf(radians), cosf(radians) + axis.values[2]*axis.values[2]*(1 - cosf(radians)), 0 },"
                             "\n    { 0, 0, 0, 1 }"
                             "\n  }};" , suffix));
  for (int32_t i = 0; i < 4; ++i) {
    for (int32_t j = 0; j < 4; ++j) {
      cb_gen_push(gen, cb_format("\n  res.values[%d][%d] = ", i, j));
      char *prefix = "";
      for (int32_t k = 0; k < 4; ++k) {
        cb_gen_push(gen, cb_format("%s(rot.values[%d][%d]) * m.values[%d][%d]",
                                   prefix, k, j, i, k));
        prefix = " + ";
      }
      cb_gen_push(gen, ";");
    }
  }
  cb_gen_push(gen, "\n  return res;" "\n}\n\n");
}

static void
cb_linagen_defun_mat4_transform_scale(CB_Generator *gen, char *type,
                                      bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn Mat4%s mat4%s_transform_scale", suffix, type));
    cb_gen_push_func_arg(gen, cb_format("Mat4%s m", suffix));
    cb_gen_push_func_arg(gen, cb_format("Vec3%s v", suffix));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, " {");
  cb_gen_push(gen, cb_format("\n  Mat4%s res = {0};"
                             "\n  Mat4%s scale = {0};"
                             "\n  scale.values[0][0] = v.values[0];"
                             "\n  scale.values[1][1] = v.values[1];"
                             "\n  scale.values[2][2] = v.values[2];"
                             "\n  scale.values[3][3] = 1;",
                             suffix, suffix));
  for (int32_t i = 0; i < 4; ++i) {
    for (int32_t j = 0; j < 4; ++j) {
      cb_gen_push(gen, cb_format("\n  res.values[%d][%d] = ", i, j));
      char *prefix = "";
      for (int32_t k = 0; k < 4; ++k) {
        cb_gen_push(gen, cb_format("%s(scale.values[%d][%d]) * m.values[%d][%d]",
                                   prefix, k, j, i, k));
        prefix = " + ";
      }
      cb_gen_push(gen, ";");
    }
  }
  cb_gen_push(gen, "\n  return res;" "\n}\n\n");
}

static void
cb_linagen_defun_mat4_projection_orthographic(CB_Generator *gen, char *type,
                                              bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn Mat4%s mat4%s_projection_orthographic", suffix, type));
    cb_gen_push_func_arg(gen, cb_format("%s left, %s right, %s bottom, %s top, %s near, %s far",
                                        type, type, type, type, type, type));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, " {");
  cb_gen_push(gen, cb_format("\n  Mat4%s res = {0};"
                             "\n  res.values[0][0] =  2 / (right - left);"
                             "\n  res.values[1][1] =  2 / (top - bottom);"
                             "\n  res.values[2][2] = -2 / (far - near);"
                             "\n  res.values[3][0] =  -(right + left) / (right - left);"
                             "\n  res.values[3][1] =  -(top + bottom) / (top - bottom);"
                             "\n  res.values[3][2] =  -(far + near) / (far - near);"
                             "\n  res.values[3][3] =  1;",
                             suffix, suffix));
  cb_gen_push(gen, "\n  return res;" "\n}\n\n");
}

static void
cb_linagen_defun_mat4_projection_perspective(CB_Generator *gen, char *type,
                                             bool implementation) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  cb_gen_push_func_begin(gen, cb_format("linagen_fn Mat4%s mat4%s_projection_perspective", suffix, type));
    cb_gen_push_func_arg(gen, cb_format("float yfov_radians, float aspect_ratio, float near, float far"));
  cb_gen_push_func_end(gen, implementation);
  if (!implementation) { return; }

  cb_gen_push(gen, " {");
  cb_gen_push(gen, cb_format("\n  %s top = near * tanf(yfov_radians/2);"
                             "\n  %s right = top * aspect_ratio;"
                             "\n  Mat4%s res = {0};"
                             "\n  res.arr[0] =  near / right;"
                             "\n  res.arr[5] =  near / top;"
                             "\n  res.arr[10] =  -(far + near) / (far - near);"
                             "\n  res.arr[11] =  -1;"
                             "\n  res.arr[14] =  -(2 * far * near) / (far - near);",
                             type, type, suffix, suffix));
  cb_gen_push(gen, "\n  return res;" "\n}\n\n");
}


internal void
cb_linagen_defun_vecn_element_wise(CB_Generator *gen, char *type,
                                   int32_t n, bool implementation,
                                   char *func_name, char op) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  if (n <= CB_LINAGEN_VECN_COPY_CAP) {
    cb_gen_push_func_begin(gen, cb_format("linagen_fn Vec%d%s vec%d%s_%s",
                                          n, suffix, n, type, func_name));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s v1", n, suffix));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s v2", n, suffix));
    cb_gen_push_func_end(gen, implementation);
    if (!implementation) { return; }

    cb_gen_push(gen, " {");
    for (int32_t i = 0; i < n; ++i) {
      cb_gen_push(gen, cb_format("\n  v1.values[%d] %c= v2.values[%d];",
                                 i, op, i));
    }
    cb_gen_push(gen, "\n  return v1;" "\n}\n\n");
  } else {
    cb_gen_push_func_begin(gen, cb_format("linagen_fn void vec%d%s_%s",
                                          n, type, func_name));
    cb_gen_push_func_arg(gen, cb_format("Vec%d%s *res", n, suffix));
    cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *v1", n, suffix));
    cb_gen_push_func_arg(gen, cb_format("const Vec%d%s *v2", n, suffix));
    cb_gen_push_func_end(gen, implementation);
    if (!implementation) { return; }

    cb_gen_push(gen, " {");
    for (int32_t i = 0; i < n; ++i) {
      cb_gen_push(gen, cb_format("\n  res->values[%d] = v1->values[%d] %c v2->values[%d];",
                                 i, i, op, i));
    }
    cb_gen_push(gen, "\n}\n\n");
  }
}

internal void
cb_linagen_defun_matnn_element_wise(CB_Generator *gen, char *type,
                                    int32_t n, bool implementation,
                                    char *func_name, char op) {
  char *suffix = strdup(type);
  *suffix = char_toupper(*type);
  if (n < CB_LINAGEN_MATNN_COPY_CAP) {
    cb_gen_push_func_begin(gen, cb_format("linagen_fn Mat%d%s mat%d%s_%s",
                                          n, suffix, n, type, func_name));
      cb_gen_push_func_arg(gen, cb_format("Mat%d%s m1", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("Mat%d%s m2", n, suffix));
    cb_gen_push_func_end(gen, implementation);
    if (!implementation) { return; }

    cb_gen_push(gen, cb_format(" {" "\n  Mat%d%s res = {0};", n, suffix));
    for (int32_t i = 0; i < n; ++i) {
      for (int32_t j = 0; j < n; ++j) {
        cb_gen_push(gen, cb_format("\n  res.values[%d][%d] = m1->values[%d][%d] %c m2->values[%d][%d];",
                                   i, j, i, j, op, i, j));
      }
    }
    cb_gen_push(gen, "\n  return res;" "\n}\n\n");
  } else {
    cb_gen_push_func_begin(gen, cb_format("linagen_fn void mat%d%s_%s",
                                          n, type, func_name));
      cb_gen_push_func_arg(gen, cb_format("Mat%d%s *res", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("const Mat%d%s *m1", n, suffix));
      cb_gen_push_func_arg(gen, cb_format("const Mat%d%s *m2", n, suffix));
    cb_gen_push_func_end(gen, implementation);
    if (!implementation) { return; }

    cb_gen_push(gen, " {");
    for (int32_t i = 0; i < n; ++i) {
      for (int32_t j = 0; j < n; ++j) {
        cb_gen_push(gen, cb_format("\n  res->values[%d][%d] = m1->values[%d][%d] %c m2->values[%d][%d];",
                                   i, j, i, j, op, i, j));
      }
    }
    cb_gen_push(gen, "\n}\n\n");
  }
}

internal char char_toupper(char ch) {
  if (ch >= 'a' && ch <= 'z') {
    return ch - ('a' - 'A');
  }
  return ch;
}

#undef MIN

#endif
