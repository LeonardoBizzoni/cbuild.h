#include <stdio.h>

#include "trajectory.h"

int main(void) {
  for (int i = 0; i < N_POINTS; ++i) {
    printf("(%f, %f)\n", waypoints[i].x, waypoints[i].y);
  }
}
