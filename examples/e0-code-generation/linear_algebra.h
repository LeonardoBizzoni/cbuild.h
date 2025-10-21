#ifndef CB_LINEAR_ALGEBRA_H
#define CB_LINEAR_ALGEBRA_H

#define linagen_fn static inline

typedef struct Vec2Float {
  float values[2];
} Vec2Float;

typedef union {
  float values[3];
  struct {
    float x; float y; float z;
  };
} Vec3Float;

typedef union {
  float values[5];
  struct {
    float x; float y; float z; float _3;
    float _4;
  };
} Vec5Float;

typedef union Mat3Float {
  float values[3][3];
  Vec3Float cols[3];
  float arr[9];
} Mat3Float;

linagen_fn void vec2float_add(Vec2Float *res, const Vec2Float *v1, const Vec2Float *v2);
linagen_fn void vec2float_sub(Vec2Float *res, const Vec2Float *v1, const Vec2Float *v2);
linagen_fn void vec2float_hadamard_prod(Vec2Float *res, const Vec2Float *v1, const Vec2Float *v2);
linagen_fn void vec2float_hadamard_div(Vec2Float *res, const Vec2Float *v1, const Vec2Float *v2);
linagen_fn void vec2float_scale(Vec2Float *res, const Vec2Float *v1, float k);
linagen_fn bool vec2float_equal(const Vec2Float *restrict v1, const Vec2Float *restrict v2);
linagen_fn bool vec2float_near(const Vec2Float *restrict v1, const Vec2Float *restrict v2, float eps);
linagen_fn void vec2float_lerp(Vec2Float *restrict res, const Vec2Float *restrict start, const Vec2Float *restrict end, float t);
linagen_fn void vec2float_normalize(Vec2Float *res, const Vec2Float *v1);
linagen_fn float vec2float_dot(const Vec2Float *v1, const Vec2Float *v2);
linagen_fn float vec2float_magnitude(const Vec2Float *v1);
linagen_fn double vec2float_magnitude64(const Vec2Float *v1);
linagen_fn float vec2float_distance(const Vec2Float *restrict v1, const Vec2Float *restrict v2);
linagen_fn float vec2float_distance2(const Vec2Float *restrict v1, const Vec2Float *restrict v2);

linagen_fn void vec3float_add(Vec3Float *res, const Vec3Float *v1, const Vec3Float *v2);
linagen_fn void vec3float_sub(Vec3Float *res, const Vec3Float *v1, const Vec3Float *v2);
linagen_fn void vec3float_hadamard_prod(Vec3Float *res, const Vec3Float *v1, const Vec3Float *v2);
linagen_fn void vec3float_hadamard_div(Vec3Float *res, const Vec3Float *v1, const Vec3Float *v2);
linagen_fn void vec3float_scale(Vec3Float *res, const Vec3Float *v1, float k);
linagen_fn bool vec3float_equal(const Vec3Float *restrict v1, const Vec3Float *restrict v2);
linagen_fn bool vec3float_near(const Vec3Float *restrict v1, const Vec3Float *restrict v2, float eps);
linagen_fn void vec3float_lerp(Vec3Float *restrict res, const Vec3Float *restrict start, const Vec3Float *restrict end, float t);
linagen_fn void vec3float_normalize(Vec3Float *res, const Vec3Float *v1);
linagen_fn void vec3float_cross(Vec3Float *restrict res, const Vec3Float *restrict v1, const Vec3Float *restrict v2);
linagen_fn float vec3float_dot(const Vec3Float *v1, const Vec3Float *v2);
linagen_fn void vec3float_mulm(Vec3Float *restrict res, const Vec3Float *restrict v, const Mat3Float *restrict m);
linagen_fn float vec3float_magnitude(const Vec3Float *v1);
linagen_fn double vec3float_magnitude64(const Vec3Float *v1);
linagen_fn float vec3float_distance(const Vec3Float *restrict v1, const Vec3Float *restrict v2);
linagen_fn float vec3float_distance2(const Vec3Float *restrict v1, const Vec3Float *restrict v2);

linagen_fn void vec5float_add(Vec5Float *res, const Vec5Float *v1, const Vec5Float *v2);
linagen_fn void vec5float_sub(Vec5Float *res, const Vec5Float *v1, const Vec5Float *v2);
linagen_fn void vec5float_hadamard_prod(Vec5Float *res, const Vec5Float *v1, const Vec5Float *v2);
linagen_fn void vec5float_hadamard_div(Vec5Float *res, const Vec5Float *v1, const Vec5Float *v2);
linagen_fn void vec5float_scale(Vec5Float *res, const Vec5Float *v1, float k);
linagen_fn bool vec5float_equal(const Vec5Float *restrict v1, const Vec5Float *restrict v2);
linagen_fn bool vec5float_near(const Vec5Float *restrict v1, const Vec5Float *restrict v2, float eps);
linagen_fn void vec5float_lerp(Vec5Float *restrict res, const Vec5Float *restrict start, const Vec5Float *restrict end, float t);
linagen_fn void vec5float_normalize(Vec5Float *res, const Vec5Float *v1);
linagen_fn float vec5float_dot(const Vec5Float *v1, const Vec5Float *v2);
linagen_fn float vec5float_magnitude(const Vec5Float *v1);
linagen_fn double vec5float_magnitude64(const Vec5Float *v1);
linagen_fn float vec5float_distance(const Vec5Float *restrict v1, const Vec5Float *restrict v2);
linagen_fn float vec5float_distance2(const Vec5Float *restrict v1, const Vec5Float *restrict v2);

linagen_fn void mat3float_add(Mat3Float *res, const Mat3Float *m1, const Mat3Float *m2);
linagen_fn void mat3float_sub(Mat3Float *res, const Mat3Float *m1, const Mat3Float *m2);
linagen_fn void mat3float_hadamard_prod(Mat3Float *res, const Mat3Float *m1, const Mat3Float *m2);
linagen_fn void mat3float_hadamard_div(Mat3Float *res, const Mat3Float *m1, const Mat3Float *m2);
linagen_fn void mat3float_scale(Mat3Float *res, const Mat3Float *m1, float k);
linagen_fn void mat3float_dot(Mat3Float *restrict res, const Mat3Float *m1, const Mat3Float *m2);
linagen_fn void mat3float_mulv(Vec3Float *restrict res, const Mat3Float *restrict m, const Vec3Float *restrict v);
linagen_fn void mat3float_identity(Mat3Float *res);

#ifndef CB_LINEAR_ALGEBRA_NO_IMPL
linagen_fn void vec2float_add(Vec2Float *res, const Vec2Float *v1, const Vec2Float *v2) {
  res->values[0] = v1->values[0] + v2->values[0];
  res->values[1] = v1->values[1] + v2->values[1];
}

linagen_fn void vec2float_sub(Vec2Float *res, const Vec2Float *v1, const Vec2Float *v2) {
  res->values[0] = v1->values[0] - v2->values[0];
  res->values[1] = v1->values[1] - v2->values[1];
}

linagen_fn void vec2float_hadamard_prod(Vec2Float *res, const Vec2Float *v1, const Vec2Float *v2) {
  res->values[0] = v1->values[0] * v2->values[0];
  res->values[1] = v1->values[1] * v2->values[1];
}

linagen_fn void vec2float_hadamard_div(Vec2Float *res, const Vec2Float *v1, const Vec2Float *v2) {
  res->values[0] = v1->values[0] / v2->values[0];
  res->values[1] = v1->values[1] / v2->values[1];
}

linagen_fn void vec2float_scale(Vec2Float *res, const Vec2Float *v1, float k) {
  res->values[0] = v1->values[0] * k;
  res->values[1] = v1->values[1] * k;
}

linagen_fn bool vec2float_equal(const Vec2Float *restrict v1, const Vec2Float *restrict v2) {
  return v1->values[0] == v2->values[0] &&
         v1->values[1] == v2->values[1];
}

linagen_fn bool vec2float_near(const Vec2Float *restrict v1, const Vec2Float *restrict v2, float eps) {
  return ((float)v1->values[0] + eps >= v2->values[0] && (float)v1->values[0] - eps <= v2->values[0]) &&
         ((float)v1->values[1] + eps >= v2->values[1] && (float)v1->values[1] - eps <= v2->values[1]);
}

linagen_fn void vec2float_lerp(Vec2Float *restrict res, const Vec2Float *restrict start, const Vec2Float *restrict end, float t) {
  res->values[0] = start->values[0] * (1 - t) + end->values[0] * t;
  res->values[1] = start->values[1] * (1 - t) + end->values[1] * t;
}

linagen_fn void vec2float_normalize(Vec2Float *res, const Vec2Float *v1) {
  float dot = {0};
  dot += v1->values[0] * v1->values[0];
  dot += v1->values[1] * v1->values[1];
  float magnitude = sqrtf((float)dot);
  if (magnitude == 0.f) { return; }
  res->values[0] = v1->values[0] / magnitude;
  res->values[1] = v1->values[1] / magnitude;
}

linagen_fn float vec2float_dot(const Vec2Float *v1, const Vec2Float *v2) {
  float res = {0};
  res += v1->values[0] * v2->values[0];
  res += v1->values[1] * v2->values[1];
  return res;
}

linagen_fn float vec2float_magnitude(const Vec2Float *v1) {
  float dot = {0};
  dot += v1->values[0] * v1->values[0];
  dot += v1->values[1] * v1->values[1];
  float res = sqrtf(dot);
  return res;
}

linagen_fn double vec2float_magnitude64(const Vec2Float *v1) {
  double dot = {0};
  dot += v1->values[0] * v1->values[0];
  dot += v1->values[1] * v1->values[1];
  double res = sqrt(dot);
  return res;
}

linagen_fn float vec2float_distance(const Vec2Float *restrict v1, const Vec2Float *restrict v2) {
  Vec2Float diff = {0};
  diff.values[0] = v1->values[0] - v2->values[0];
  diff.values[1] = v1->values[1] - v2->values[1];
  float dot = {0};
  dot += diff.values[0] * diff.values[0];
  dot += diff.values[1] * diff.values[1];
  float res = sqrtf(dot);
  return res;
}

linagen_fn float vec2float_distance2(const Vec2Float *restrict v1, const Vec2Float *restrict v2) {
  Vec2Float diff = {0};
  diff.values[0] = v1->values[0] - v2->values[0];
  diff.values[1] = v1->values[1] - v2->values[1];
  float dot = {0};
  dot += diff.values[0] * diff.values[0];
  dot += diff.values[1] * diff.values[1];
  return dot;
}


linagen_fn void vec3float_add(Vec3Float *res, const Vec3Float *v1, const Vec3Float *v2) {
  res->values[0] = v1->values[0] + v2->values[0];
  res->values[1] = v1->values[1] + v2->values[1];
  res->values[2] = v1->values[2] + v2->values[2];
}

linagen_fn void vec3float_sub(Vec3Float *res, const Vec3Float *v1, const Vec3Float *v2) {
  res->values[0] = v1->values[0] - v2->values[0];
  res->values[1] = v1->values[1] - v2->values[1];
  res->values[2] = v1->values[2] - v2->values[2];
}

linagen_fn void vec3float_hadamard_prod(Vec3Float *res, const Vec3Float *v1, const Vec3Float *v2) {
  res->values[0] = v1->values[0] * v2->values[0];
  res->values[1] = v1->values[1] * v2->values[1];
  res->values[2] = v1->values[2] * v2->values[2];
}

linagen_fn void vec3float_hadamard_div(Vec3Float *res, const Vec3Float *v1, const Vec3Float *v2) {
  res->values[0] = v1->values[0] / v2->values[0];
  res->values[1] = v1->values[1] / v2->values[1];
  res->values[2] = v1->values[2] / v2->values[2];
}

linagen_fn void vec3float_scale(Vec3Float *res, const Vec3Float *v1, float k) {
  res->values[0] = v1->values[0] * k;
  res->values[1] = v1->values[1] * k;
  res->values[2] = v1->values[2] * k;
}

linagen_fn bool vec3float_equal(const Vec3Float *restrict v1, const Vec3Float *restrict v2) {
  return v1->values[0] == v2->values[0] &&
         v1->values[1] == v2->values[1] &&
         v1->values[2] == v2->values[2];
}

linagen_fn bool vec3float_near(const Vec3Float *restrict v1, const Vec3Float *restrict v2, float eps) {
  return ((float)v1->values[0] + eps >= v2->values[0] && (float)v1->values[0] - eps <= v2->values[0]) &&
         ((float)v1->values[1] + eps >= v2->values[1] && (float)v1->values[1] - eps <= v2->values[1]) &&
         ((float)v1->values[2] + eps >= v2->values[2] && (float)v1->values[2] - eps <= v2->values[2]);
}

linagen_fn void vec3float_lerp(Vec3Float *restrict res, const Vec3Float *restrict start, const Vec3Float *restrict end, float t) {
  res->values[0] = start->values[0] * (1 - t) + end->values[0] * t;
  res->values[1] = start->values[1] * (1 - t) + end->values[1] * t;
  res->values[2] = start->values[2] * (1 - t) + end->values[2] * t;
}

linagen_fn void vec3float_normalize(Vec3Float *res, const Vec3Float *v1) {
  float dot = {0};
  dot += v1->values[0] * v1->values[0];
  dot += v1->values[1] * v1->values[1];
  dot += v1->values[2] * v1->values[2];
  float magnitude = sqrtf((float)dot);
  if (magnitude == 0.f) { return; }
  res->values[0] = v1->values[0] / magnitude;
  res->values[1] = v1->values[1] / magnitude;
  res->values[2] = v1->values[2] / magnitude;
}

linagen_fn void vec3float_cross(Vec3Float *restrict res, const Vec3Float *restrict v1, const Vec3Float *restrict v2) {
  res->values[0] = v1->values[1] * v2->values[2] - v1->values[2] * v2->values[1];
  res->values[1] = v1->values[2] * v2->values[0] - v1->values[0] * v2->values[2];
  res->values[2] = v1->values[0] * v2->values[1] - v1->values[1] * v2->values[0];
}

linagen_fn float vec3float_dot(const Vec3Float *v1, const Vec3Float *v2) {
  float res = {0};
  res += v1->values[0] * v2->values[0];
  res += v1->values[1] * v2->values[1];
  res += v1->values[2] * v2->values[2];
  return res;
}

linagen_fn void vec3float_mulm(Vec3Float *restrict res, const Vec3Float *restrict v, const Mat3Float *restrict m) {
  res->values[0] = (v->values[0] * m->values[0][0]) + (v->values[1] * m->values[0][1]) + (v->values[2] * m->values[0][2]);
  res->values[1] = (v->values[0] * m->values[1][0]) + (v->values[1] * m->values[1][1]) + (v->values[2] * m->values[1][2]);
  res->values[2] = (v->values[0] * m->values[2][0]) + (v->values[1] * m->values[2][1]) + (v->values[2] * m->values[2][2]);
}

linagen_fn float vec3float_magnitude(const Vec3Float *v1) {
  float dot = {0};
  dot += v1->values[0] * v1->values[0];
  dot += v1->values[1] * v1->values[1];
  dot += v1->values[2] * v1->values[2];
  float res = sqrtf(dot);
  return res;
}

linagen_fn double vec3float_magnitude64(const Vec3Float *v1) {
  double dot = {0};
  dot += v1->values[0] * v1->values[0];
  dot += v1->values[1] * v1->values[1];
  dot += v1->values[2] * v1->values[2];
  double res = sqrt(dot);
  return res;
}

linagen_fn float vec3float_distance(const Vec3Float *restrict v1, const Vec3Float *restrict v2) {
  Vec3Float diff = {0};
  diff.values[0] = v1->values[0] - v2->values[0];
  diff.values[1] = v1->values[1] - v2->values[1];
  diff.values[2] = v1->values[2] - v2->values[2];
  float dot = {0};
  dot += diff.values[0] * diff.values[0];
  dot += diff.values[1] * diff.values[1];
  dot += diff.values[2] * diff.values[2];
  float res = sqrtf(dot);
  return res;
}

linagen_fn float vec3float_distance2(const Vec3Float *restrict v1, const Vec3Float *restrict v2) {
  Vec3Float diff = {0};
  diff.values[0] = v1->values[0] - v2->values[0];
  diff.values[1] = v1->values[1] - v2->values[1];
  diff.values[2] = v1->values[2] - v2->values[2];
  float dot = {0};
  dot += diff.values[0] * diff.values[0];
  dot += diff.values[1] * diff.values[1];
  dot += diff.values[2] * diff.values[2];
  return dot;
}


linagen_fn void vec5float_add(Vec5Float *res, const Vec5Float *v1, const Vec5Float *v2) {
  res->values[0] = v1->values[0] + v2->values[0];
  res->values[1] = v1->values[1] + v2->values[1];
  res->values[2] = v1->values[2] + v2->values[2];
  res->values[3] = v1->values[3] + v2->values[3];
  res->values[4] = v1->values[4] + v2->values[4];
}

linagen_fn void vec5float_sub(Vec5Float *res, const Vec5Float *v1, const Vec5Float *v2) {
  res->values[0] = v1->values[0] - v2->values[0];
  res->values[1] = v1->values[1] - v2->values[1];
  res->values[2] = v1->values[2] - v2->values[2];
  res->values[3] = v1->values[3] - v2->values[3];
  res->values[4] = v1->values[4] - v2->values[4];
}

linagen_fn void vec5float_hadamard_prod(Vec5Float *res, const Vec5Float *v1, const Vec5Float *v2) {
  res->values[0] = v1->values[0] * v2->values[0];
  res->values[1] = v1->values[1] * v2->values[1];
  res->values[2] = v1->values[2] * v2->values[2];
  res->values[3] = v1->values[3] * v2->values[3];
  res->values[4] = v1->values[4] * v2->values[4];
}

linagen_fn void vec5float_hadamard_div(Vec5Float *res, const Vec5Float *v1, const Vec5Float *v2) {
  res->values[0] = v1->values[0] / v2->values[0];
  res->values[1] = v1->values[1] / v2->values[1];
  res->values[2] = v1->values[2] / v2->values[2];
  res->values[3] = v1->values[3] / v2->values[3];
  res->values[4] = v1->values[4] / v2->values[4];
}

linagen_fn void vec5float_scale(Vec5Float *res, const Vec5Float *v1, float k) {
  res->values[0] = v1->values[0] * k;
  res->values[1] = v1->values[1] * k;
  res->values[2] = v1->values[2] * k;
  res->values[3] = v1->values[3] * k;
  res->values[4] = v1->values[4] * k;
}

linagen_fn bool vec5float_equal(const Vec5Float *restrict v1, const Vec5Float *restrict v2) {
  return v1->values[0] == v2->values[0] &&
         v1->values[1] == v2->values[1] &&
         v1->values[2] == v2->values[2] &&
         v1->values[3] == v2->values[3] &&
         v1->values[4] == v2->values[4];
}

linagen_fn bool vec5float_near(const Vec5Float *restrict v1, const Vec5Float *restrict v2, float eps) {
  return ((float)v1->values[0] + eps >= v2->values[0] && (float)v1->values[0] - eps <= v2->values[0]) &&
         ((float)v1->values[1] + eps >= v2->values[1] && (float)v1->values[1] - eps <= v2->values[1]) &&
         ((float)v1->values[2] + eps >= v2->values[2] && (float)v1->values[2] - eps <= v2->values[2]) &&
         ((float)v1->values[3] + eps >= v2->values[3] && (float)v1->values[3] - eps <= v2->values[3]) &&
         ((float)v1->values[4] + eps >= v2->values[4] && (float)v1->values[4] - eps <= v2->values[4]);
}

linagen_fn void vec5float_lerp(Vec5Float *restrict res, const Vec5Float *restrict start, const Vec5Float *restrict end, float t) {
  res->values[0] = start->values[0] * (1 - t) + end->values[0] * t;
  res->values[1] = start->values[1] * (1 - t) + end->values[1] * t;
  res->values[2] = start->values[2] * (1 - t) + end->values[2] * t;
  res->values[3] = start->values[3] * (1 - t) + end->values[3] * t;
  res->values[4] = start->values[4] * (1 - t) + end->values[4] * t;
}

linagen_fn void vec5float_normalize(Vec5Float *res, const Vec5Float *v1) {
  float dot = {0};
  dot += v1->values[0] * v1->values[0];
  dot += v1->values[1] * v1->values[1];
  dot += v1->values[2] * v1->values[2];
  dot += v1->values[3] * v1->values[3];
  dot += v1->values[4] * v1->values[4];
  float magnitude = sqrtf((float)dot);
  if (magnitude == 0.f) { return; }
  res->values[0] = v1->values[0] / magnitude;
  res->values[1] = v1->values[1] / magnitude;
  res->values[2] = v1->values[2] / magnitude;
  res->values[3] = v1->values[3] / magnitude;
  res->values[4] = v1->values[4] / magnitude;
}

linagen_fn float vec5float_dot(const Vec5Float *v1, const Vec5Float *v2) {
  float res = {0};
  res += v1->values[0] * v2->values[0];
  res += v1->values[1] * v2->values[1];
  res += v1->values[2] * v2->values[2];
  res += v1->values[3] * v2->values[3];
  res += v1->values[4] * v2->values[4];
  return res;
}

linagen_fn float vec5float_magnitude(const Vec5Float *v1) {
  float dot = {0};
  dot += v1->values[0] * v1->values[0];
  dot += v1->values[1] * v1->values[1];
  dot += v1->values[2] * v1->values[2];
  dot += v1->values[3] * v1->values[3];
  dot += v1->values[4] * v1->values[4];
  float res = sqrtf(dot);
  return res;
}

linagen_fn double vec5float_magnitude64(const Vec5Float *v1) {
  double dot = {0};
  dot += v1->values[0] * v1->values[0];
  dot += v1->values[1] * v1->values[1];
  dot += v1->values[2] * v1->values[2];
  dot += v1->values[3] * v1->values[3];
  dot += v1->values[4] * v1->values[4];
  double res = sqrt(dot);
  return res;
}

linagen_fn float vec5float_distance(const Vec5Float *restrict v1, const Vec5Float *restrict v2) {
  Vec5Float diff = {0};
  diff.values[0] = v1->values[0] - v2->values[0];
  diff.values[1] = v1->values[1] - v2->values[1];
  diff.values[2] = v1->values[2] - v2->values[2];
  diff.values[3] = v1->values[3] - v2->values[3];
  diff.values[4] = v1->values[4] - v2->values[4];
  float dot = {0};
  dot += diff.values[0] * diff.values[0];
  dot += diff.values[1] * diff.values[1];
  dot += diff.values[2] * diff.values[2];
  dot += diff.values[3] * diff.values[3];
  dot += diff.values[4] * diff.values[4];
  float res = sqrtf(dot);
  return res;
}

linagen_fn float vec5float_distance2(const Vec5Float *restrict v1, const Vec5Float *restrict v2) {
  Vec5Float diff = {0};
  diff.values[0] = v1->values[0] - v2->values[0];
  diff.values[1] = v1->values[1] - v2->values[1];
  diff.values[2] = v1->values[2] - v2->values[2];
  diff.values[3] = v1->values[3] - v2->values[3];
  diff.values[4] = v1->values[4] - v2->values[4];
  float dot = {0};
  dot += diff.values[0] * diff.values[0];
  dot += diff.values[1] * diff.values[1];
  dot += diff.values[2] * diff.values[2];
  dot += diff.values[3] * diff.values[3];
  dot += diff.values[4] * diff.values[4];
  return dot;
}


linagen_fn void mat3float_add(Mat3Float *res, const Mat3Float *m1, const Mat3Float *m2) {
  res->values[0][0] = m1->values[0][0] + m2->values[0][0];
  res->values[0][1] = m1->values[0][1] + m2->values[0][1];
  res->values[0][2] = m1->values[0][2] + m2->values[0][2];
  res->values[1][0] = m1->values[1][0] + m2->values[1][0];
  res->values[1][1] = m1->values[1][1] + m2->values[1][1];
  res->values[1][2] = m1->values[1][2] + m2->values[1][2];
  res->values[2][0] = m1->values[2][0] + m2->values[2][0];
  res->values[2][1] = m1->values[2][1] + m2->values[2][1];
  res->values[2][2] = m1->values[2][2] + m2->values[2][2];
}

linagen_fn void mat3float_sub(Mat3Float *res, const Mat3Float *m1, const Mat3Float *m2) {
  res->values[0][0] = m1->values[0][0] - m2->values[0][0];
  res->values[0][1] = m1->values[0][1] - m2->values[0][1];
  res->values[0][2] = m1->values[0][2] - m2->values[0][2];
  res->values[1][0] = m1->values[1][0] - m2->values[1][0];
  res->values[1][1] = m1->values[1][1] - m2->values[1][1];
  res->values[1][2] = m1->values[1][2] - m2->values[1][2];
  res->values[2][0] = m1->values[2][0] - m2->values[2][0];
  res->values[2][1] = m1->values[2][1] - m2->values[2][1];
  res->values[2][2] = m1->values[2][2] - m2->values[2][2];
}

linagen_fn void mat3float_hadamard_prod(Mat3Float *res, const Mat3Float *m1, const Mat3Float *m2) {
  res->values[0][0] = m1->values[0][0] * m2->values[0][0];
  res->values[0][1] = m1->values[0][1] * m2->values[0][1];
  res->values[0][2] = m1->values[0][2] * m2->values[0][2];
  res->values[1][0] = m1->values[1][0] * m2->values[1][0];
  res->values[1][1] = m1->values[1][1] * m2->values[1][1];
  res->values[1][2] = m1->values[1][2] * m2->values[1][2];
  res->values[2][0] = m1->values[2][0] * m2->values[2][0];
  res->values[2][1] = m1->values[2][1] * m2->values[2][1];
  res->values[2][2] = m1->values[2][2] * m2->values[2][2];
}

linagen_fn void mat3float_hadamard_div(Mat3Float *res, const Mat3Float *m1, const Mat3Float *m2) {
  res->values[0][0] = m1->values[0][0] / m2->values[0][0];
  res->values[0][1] = m1->values[0][1] / m2->values[0][1];
  res->values[0][2] = m1->values[0][2] / m2->values[0][2];
  res->values[1][0] = m1->values[1][0] / m2->values[1][0];
  res->values[1][1] = m1->values[1][1] / m2->values[1][1];
  res->values[1][2] = m1->values[1][2] / m2->values[1][2];
  res->values[2][0] = m1->values[2][0] / m2->values[2][0];
  res->values[2][1] = m1->values[2][1] / m2->values[2][1];
  res->values[2][2] = m1->values[2][2] / m2->values[2][2];
}

linagen_fn void mat3float_scale(Mat3Float *res, const Mat3Float *m1, float k) {
  res->values[0][0] = m1->values[0][0] * k;
  res->values[0][1] = m1->values[0][1] * k;
  res->values[0][2] = m1->values[0][2] * k;
  res->values[1][0] = m1->values[1][0] * k;
  res->values[1][1] = m1->values[1][1] * k;
  res->values[1][2] = m1->values[1][2] * k;
  res->values[2][0] = m1->values[2][0] * k;
  res->values[2][1] = m1->values[2][1] * k;
  res->values[2][2] = m1->values[2][2] * k;
}

linagen_fn void mat3float_dot(Mat3Float *restrict res, const Mat3Float *m1, const Mat3Float *m2) {
  res->values[0][0] = (m1->values[0][0] * m2->values[0][0]) + (m1->values[1][0] * m2->values[0][1]) + (m1->values[2][0] * m2->values[0][2]);
  res->values[0][1] = (m1->values[0][1] * m2->values[0][0]) + (m1->values[1][1] * m2->values[0][1]) + (m1->values[2][1] * m2->values[0][2]);
  res->values[0][2] = (m1->values[0][2] * m2->values[0][0]) + (m1->values[1][2] * m2->values[0][1]) + (m1->values[2][2] * m2->values[0][2]);
  res->values[1][0] = (m1->values[0][0] * m2->values[1][0]) + (m1->values[1][0] * m2->values[1][1]) + (m1->values[2][0] * m2->values[1][2]);
  res->values[1][1] = (m1->values[0][1] * m2->values[1][0]) + (m1->values[1][1] * m2->values[1][1]) + (m1->values[2][1] * m2->values[1][2]);
  res->values[1][2] = (m1->values[0][2] * m2->values[1][0]) + (m1->values[1][2] * m2->values[1][1]) + (m1->values[2][2] * m2->values[1][2]);
  res->values[2][0] = (m1->values[0][0] * m2->values[2][0]) + (m1->values[1][0] * m2->values[2][1]) + (m1->values[2][0] * m2->values[2][2]);
  res->values[2][1] = (m1->values[0][1] * m2->values[2][0]) + (m1->values[1][1] * m2->values[2][1]) + (m1->values[2][1] * m2->values[2][2]);
  res->values[2][2] = (m1->values[0][2] * m2->values[2][0]) + (m1->values[1][2] * m2->values[2][1]) + (m1->values[2][2] * m2->values[2][2]);
}

linagen_fn void mat3float_mulv(Vec3Float *restrict res, const Mat3Float *restrict m, const Vec3Float *restrict v) {
  res->values[0] = (m->values[0][0] * v->values[0]) + (m->values[1][0] * v->values[1]) + (m->values[2][0] * v->values[2]);
  res->values[1] = (m->values[0][1] * v->values[0]) + (m->values[1][1] * v->values[1]) + (m->values[2][1] * v->values[2]);
  res->values[2] = (m->values[0][2] * v->values[0]) + (m->values[1][2] * v->values[1]) + (m->values[2][2] * v->values[2]);
}

linagen_fn void mat3float_identity(Mat3Float *res) {
  memset(res, 0, sizeof *res);
  res->values[0][0] = 1;
  res->values[1][1] = 1;
  res->values[2][2] = 1;
}

#endif // CB_LINEAR_ALGEBRA_NO_IMPL
#endif // CB_LINEAR_ALGEBRA_H
