#include "../../cbuild.h"

int main(int argc, char **argv) {
  cb_rebuild_self(argc, argv);

  CB_Cmd cmd = {};
  CB_ProcessList procs = {};
  cb_cmd_append(&cmd, "ls", "-lah", ".");
  cb_proclist_push(&procs, cb_run(&cmd, .async = true));
  cb_cmd_append(&cmd, "ls", "-lah", "/");
  cb_proclist_push(&procs, cb_run(&cmd, .async = true));
  cb_cmd_append(&cmd, "pwd");
  cb_proclist_push(&procs, cb_run(&cmd, .async = true));

  cb_proclist_wait(&procs);
}
