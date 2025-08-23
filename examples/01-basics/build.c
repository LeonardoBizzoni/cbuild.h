#include "../../cbuild.h"

int main(int argc, char **argv) {
  cb_rebuild_self(argc, argv);

  CB_Cmd cmd = {};
#if OS_WINDOWS
  cb_cmd_append(&cmd, "cl.exe", "main.c", "/Fe:main");
#else
  cb_cmd_append(&cmd, "cc", "main.c", "-o", "main");
#endif
  cb_run(&cmd);
}
