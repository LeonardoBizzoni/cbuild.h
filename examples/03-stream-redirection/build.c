#include "../../cbuild.h"

int main(int argc, char **argv) {
  cb_rebuild_self(argc, argv);

  CB_Handle file = cb_handle_open("ls-curdir-log", CB_AccessFlag_Read |
                                                   CB_AccessFlag_Write);

  CB_Cmd cmd = {};
  cb_cmd_append(&cmd, "ls", "-lah", ".");
  cb_run(&cmd, .stdout = file);

  cb_cmd_append(&cmd, "cat");
  cb_run(&cmd, .stdin = file);

  cb_handle_close(file);
}
