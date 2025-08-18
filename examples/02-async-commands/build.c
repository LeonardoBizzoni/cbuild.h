#include "../../cbuild.h"

int main(int argc, char **argv) {
  cb_rebuild_self(argc, argv);

  cb_cmd cmd = {};
  cb_procs procs = {};
  cb_cmd_append(&cmd, "ls", "-lah", ".");
  cb_procs_push(&procs, cb_run(&cmd, .async = true));
  cb_cmd_append(&cmd, "ls", "-lah", "/");
  cb_procs_push(&procs, cb_run(&cmd, .async = true));
  cb_cmd_append(&cmd, "pwd");
  cb_procs_push(&procs, cb_run(&cmd, .async = true));

  cb_procs_wait(&procs);
}
