#define CB_RECOMPILE_OPTIONS "-lm"
#include "../../cbuild.h"

#include "gen_arcs.c"

int main(int argc, char **argv) {
  cb_rebuild_self_with(argc, argv, "../../cbuild.h", "gen_arcs.c");

  CB_Handle trajectory = cb_handle_open("src/trajectory.h", CB_AccessFlag_Write);
  cb_handle_write(trajectory, "#ifndef GEN_TRAJECTORY_H\n#define GEN_TRAJECTORY_H\n");
  cb_handle_write(trajectory, cb_format("#define N_POINTS %d\n", N_POINTS * Chunks));

  cb_handle_write(trajectory, "typedef struct { float x, y; } Points;\n");

  generate_arc_points(0, 900, 900, -90.f, 0.f);
  generate_arc_points(1800, 900, 900, 180.f, 90.f);
  generate_line_points(1800, 1800, 900, 0.f);

  cb_handle_write(trajectory, "static Points waypoints[N_POINTS] = {");
  for (int32_t i = 0; i < N_POINTS * Chunks; ++i) {
    cb_handle_write(trajectory, cb_format("{%f,%f}", waypoints[i].x, waypoints[i].y));
    if (i < (N_POINTS * Chunks) - 1) { cb_handle_write(trajectory, ","); }
  }
  cb_handle_write(trajectory, "};\n#endif\n");
  cb_handle_close(trajectory);

  CB_Cmd cmd = {};
  cb_cmd_append(&cmd, "cc", "-o", "main", "src/main.c");
  cb_cmd_run(&cmd);
}
