#include <math.h>

#define Chunks 3
#define N_POINTS 50

static struct {float x, y;} waypoints[N_POINTS * Chunks] = {0};

static int32_t chunk = 0;
static void generate_arc_points(float center_x, float center_y, float radius,
                                float start_angle, float end_angle) {
  float start_radians = start_angle * M_PI / 180.f;
  float end_radians = end_angle * M_PI / 180.f;
  for (int32_t i = 0; i < N_POINTS; ++i) {
    float t = (float)i / (N_POINTS - 1);
    float radians = start_radians + t * (end_radians - start_radians);
    waypoints[i + chunk * N_POINTS].x = center_x + radius * cosf(radians);
    waypoints[i + chunk * N_POINTS].y = center_y + radius * sinf(radians);
  }
  chunk += 1;
}

static void generate_line_points(float start_x, float start_y, float length_mm, float angle) {
  float radians = angle * M_PI / 180.f;
  for (int32_t i = 0; i < N_POINTS; ++i) {
    float amount = i * (length_mm / N_POINTS);
    if (amount > length_mm) { amount = length_mm; }
    waypoints[i + chunk * N_POINTS].x = start_x + amount * cosf(radians);
    waypoints[i + chunk * N_POINTS].y = start_y + amount * sinf(radians);
  }
  chunk += 1;
}
