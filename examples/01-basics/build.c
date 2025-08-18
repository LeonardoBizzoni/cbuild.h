#include "../../cbuild.h"

int main(int argc, char **argv) {
  cb_rebuild_self(argc, argv);

  cb_cmd cmd = {};
#if OS_WINDOWS
  cb_cmd_append(&cmd, "cl.exe", "main.c", "/Femain.exe");
#else
  cb_cmd_append(&cmd, "cc", "main.c", "-o", "main");
#endif
  cb_run(&cmd);
}
