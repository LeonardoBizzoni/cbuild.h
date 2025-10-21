#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "linear_algebra.h"

int main(void) {
  Vec3Float v1 = { 12, 2 };
  Vec3Float v2 = { 8, 3, 6 };
  vec3float_add(&v1, &v1, &v2);
  for (int i = 0; i < 3; ++i) {
    printf("%.2f, ", v1.values[i]);
  }
  printf("\b\b\n\n");

  Mat3Float m1 = {
    2, 3, 5, // col 0
    1, 4, 2, // col 1
    3, 1, 3, // col 2
  };
  Mat3Float m2 = {
    1, 4, 3, // col 0
    2, 1, 2, // col 1
    0, 2, 1, // col 2
  };
  Mat3Float m3 = {0};
  mat3float_dot(&m3, &m1, &m2);
  printf("M1 ⋅ M2:\n");
  for (int i = 0; i < 3; ++i) {
    printf("\tCol %d: ", i);
    for (int j = 0; j < 3; ++j) {
      printf("%.2f ", m3.values[i][j]);
    }
    printf("\n");
  }
}
