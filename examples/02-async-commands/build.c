#include "../../cbuild.h"

int main(int argc, char **argv) {
  cb_rebuild_self(argc, argv);

  CB_Cmd cmd = {};
  CB_ProcessList procs = {};
#if OS_WINDOWS
  cb_cmd_append(&cmd, "cmd.exe", "/C", "dir");
  cb_proclist_push(&procs, cb_cmd_run(&cmd, .async = true));
  cb_cmd_append(&cmd, "cmd.exe", "/C", "dir", "C:\\");
  cb_proclist_push(&procs, cb_cmd_run(&cmd, .async = true));
#else
  cb_cmd_append(&cmd, "ls", "-lah", ".");
  cb_proclist_push(&procs, cb_cmd_run(&cmd, .async = true));
  cb_cmd_append(&cmd, "ls", "-lah", "/");
  cb_proclist_push(&procs, cb_cmd_run(&cmd, .async = true));
  cb_cmd_append(&cmd, "pwd");
  cb_proclist_push(&procs, cb_cmd_run(&cmd, .async = true));
#endif

  cb_proclist_wait(&procs);
}
